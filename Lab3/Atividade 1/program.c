#include <stdio.h>
#include <stdlib.h>

/*
Programa sequencial que ira ler um arquivo binario com as dimensoes (linhas e colunas)de duas matrizes e
as duas matrizes respectivamente e ira calcular o produto entre elas.
*/

// Definindo uma struct para representar uma matriz
typedef struct {
    int linhas;
    int colunas;
    float *dados;
} Matriz;

/**
 * @brief Lê dados binários de um arquivo e armazena em variáveis fornecidas.
 *
 * Esta função lê as dimensões e os dados de duas matrizes de um arquivo binário e armazena nas structs fornecidas.
 *
 * @param arquivo Ponteiro para o arquivo binário a ser lido. O arquivo deve estar previamente aberto.
 * @param matriz1 Ponteiro para a struct onde a primeira matriz será armazenada.
 * @param matriz2 Ponteiro para a struct onde a segunda matriz será armazenada.
 *
 * @note Esta função encerra o programa com `exit(1)` em caso de erro ao ler os dados ou ao alocar memória.
 */
void leDadosBinario(FILE *arquivo, Matriz *matriz1, Matriz *matriz2) {
    size_t ret;

    // Ler o número de Linhas
    ret = fread(&matriz1->linhas, sizeof(int), 1, arquivo);
    if (ret != 1) {
        fprintf(stderr, "Erro ao ler o numero de linhas\n");
        exit(1);
    }

    // Ler o número de Colunas
    ret = fread(&matriz1->colunas, sizeof(int), 1, arquivo);
    if (ret != 1) {
        fprintf(stderr, "Erro ao ler o numero de colunas\n");
        exit(1);
    }

    // Ler o número de Linhas
    ret = fread(&matriz2->linhas, sizeof(int), 1, arquivo);
    if (ret != 1) {
        fprintf(stderr, "Erro ao ler o numero de linhas\n");
        exit(1);
    }

    // Ler o número de Colunas
    ret = fread(&matriz2->colunas, sizeof(int), 1, arquivo);
    if (ret != 1) {
        fprintf(stderr, "Erro ao ler o numero de colunas\n");
        exit(1);
    }


    int dimensaoMat1 = matriz1->linhas * matriz1->colunas;
    int dimensaoMat2 = matriz2->linhas * matriz2->colunas;

    // Alocar memória para os dados das matrizes
    matriz1->dados = (float*)malloc(dimensaoMat1 * sizeof(float));
    matriz2->dados = (float*)malloc(dimensaoMat2 * sizeof(float));

    if (matriz1->dados == NULL || matriz2->dados == NULL) {
        fprintf(stderr, "Erro na alocacao de memoria\n");
        exit(1);
    }

    // Ler os elementos da matriz1
    ret = fread(matriz1->dados, sizeof(float), dimensaoMat1, arquivo);
    if (ret != dimensaoMat1) {
        fprintf(stderr, "Erro ao ler a matriz1 do arquivo\n");
        exit(1);
    }

    // Ler os elementos da matriz2
    ret = fread(matriz2->dados, sizeof(float), dimensaoMat2, arquivo);
    if (ret != dimensaoMat2) {
        fprintf(stderr, "Erro ao ler a matriz2 do arquivo\n");
        exit(1);
    }
}

/**
 * @brief Imprime os dados de duas matrizes.
 *
 * Esta função imprime as dimensões e os dados das duas matrizes fornecidas.
 *
 * @param matriz A matriz a ser impressa.
 */
void imprimeDados(Matriz matriz) {
    printf("Dimensoes: %dx%d\n", matriz.linhas, matriz.colunas);

    printf("Matriz:\n");
    for (int i = 0; i < matriz.linhas; i++) {
        for (int j = 0; j < matriz.colunas; j++) {
            printf("%f ", matriz.dados[i * matriz.colunas + j]);
        }
        printf("\n");
    }
}

/**
 * @brief Multiplica duas matrizes e retorna a matriz resultante.
 *
 * Esta função realiza a multiplicação de duas matrizes, garantindo que o número de colunas da primeira matriz
 * seja igual ao número de linhas da segunda matriz. Caso contrário, a função retorna NULL e exibe uma mensagem
 * de erro.
 *
 * @param matriz1 A primeira matriz a ser multiplicada.
 * @param matriz2 A segunda matriz a ser multiplicada.
 * @return Matriz* A matriz resultante da multiplicação, ou NULL se a multiplicação não for possível.
 */
Matriz* multiplicaMatrizes(const Matriz *matriz1, const Matriz *matriz2) {
    if (matriz1->colunas != matriz2->linhas) {
        fprintf(stderr, "Erro: O numero de colunas da Matriz 1 deve ser igual ao numero de linhas da Matriz 2.\n");
        return NULL;
    }

    // Alocar memória para a matriz resultante
    Matriz *resultado = (Matriz*)malloc(sizeof(Matriz));
    resultado->linhas = matriz1->linhas;
    resultado->colunas = matriz2->colunas;
    resultado->dados = (float*)malloc(resultado->linhas * resultado->colunas * sizeof(float));

    if (resultado->dados == NULL) {
        fprintf(stderr, "Erro na alocacao de memoria para a matriz resultante\n");
        free(resultado);
        return NULL;
    }

    // Multiplicar as matrizes
    for (int i = 0; i < resultado->linhas; i++) {
        for (int j = 0; j < resultado->colunas; j++) {
            resultado->dados[i * resultado->colunas + j] = 0.0;
            for (int k = 0; k < matriz1->colunas; k++) {
                resultado->dados[i * resultado->colunas + j] +=
                    matriz1->dados[i * matriz1->colunas + k] * matriz2->dados[k * matriz2->colunas + j];
            }
        }
    }

    return resultado;
}

int main(int argc, char *argv[]) {

    FILE * descritorArquivo; //descritor do arquivo de saida
    size_t ret; //retorno da funcao de escrita no arquivo de saida

    if (argc < 2) {
        fprintf(stderr, "Uso: %s <arquivo binario>\n", argv[0]);
        return 1;
    }

    FILE *arq = fopen(argv[1], "rb");
    if (arq == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo\n");
        return 2;
    }

    Matriz matriz1, matriz2;
    leDadosBinario(arq, &matriz1, &matriz2);
    fclose(arq);

    int dimensaoMat1 = matriz1.linhas * matriz1.colunas;
    int dimensaoMat2 = matriz2.linhas * matriz2.colunas;

    Matriz *resultado = multiplicaMatrizes(&matriz1, &matriz2);
    if (resultado != NULL) {
        imprimeDados(*resultado); // Imprime a matriz resultante
        free(resultado->dados);   // Liberar a memória da matriz resultante
        free(resultado);          // Liberar a struct matriz resultante
    }

    descritorArquivo = fopen(argv[2], "wb");
    if (!descritorArquivo) {
        fprintf(stderr, "Erro de abertura do arquivo\n");
        return 3;
    }

    ret = fwrite(matriz1.dados, sizeof(float), dimensaoMat1, descritorArquivo);
    if (ret != dimensaoMat1) {
        fprintf(stderr, "Erro ao escrever a primeira matriz no arquivo\n");
        return 5;
    }

    ret = fwrite(matriz2.dados, sizeof(float), dimensaoMat2, descritorArquivo);
    if (ret != dimensaoMat2) {
        fprintf(stderr, "Erro ao escrever a primeira matriz no arquivo\n");
        return 5;
    }

    int dimensaoResultado = matriz1.linhas * matriz2.colunas;

    ret = fwrite(resultado, sizeof(float), dimensaoResultado, descritorArquivo);
    if (ret != dimensaoResultado) {
        fprintf(stderr, "Erro ao escrever a primeira matriz no arquivo\n");
        return 5;
    }

    // TODO : Implementar escrita no arquivo.bin de saída (que contera as matrizes e a matriz resultante) 

    // Liberar a memória das outras matrizes
    free(matriz1.dados);
    free(matriz2.dados);
    free(resultado);

    return 0;
}
