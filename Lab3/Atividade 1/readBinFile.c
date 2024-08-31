#include <stdio.h>

/*
Programa que recebe um arquivo binario com duas matrizes e a matriz resultante da multiplicao entre as
duas matrizes.
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
void leDadosBinario(FILE *arquivo, Matriz *matriz1, Matriz *matriz2, Matriz *matrizResultado) {
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
    int dimensaoMatResultado = matriz1->linhas * matriz2->colunas;

    // Alocar memória para os dados das matrizes
    matriz1->dados = (float*)malloc(dimensaoMat1 * sizeof(float));
    matriz2->dados = (float*)malloc(dimensaoMat2 * sizeof(float));

    matrizResultado->dados = (float*)malloc(dimensaoMatResultado * sizeof(float));

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

    // Ler os elementos da matriz2
    ret = fread(matrizResultado->dados, sizeof(float), dimensaoMatResultado, arquivo);
    if (ret != dimensaoMatResultado) {
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

int main(int argc, char* argv[]) {

    if (argc < 2) {
        fprintf(stderr, "Uso: %s <arquivo binario>\n", argv[0]);
        return 1;
    }

    FILE *arq = fopen(argv[1], "rb");
    if (arq == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo\n");
        return 2;
    }

    Matriz matriz1, matriz2, matrizResultado;

    leDadosBinario(arq, &matriz1, &matriz2, &matrizResultado);
    fclose(arq);

    imprimeDados(matriz1);
    imprimeDados(matriz2);
    imprimeDados(matrizResultado);

    free(matriz1.dados);
    free(matriz2.dados);
    free(matrizResultado.dados);

    return 0;
}