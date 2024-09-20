#include <stdio.h>
#include <stdlib.h>

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
void leDadosBinario(FILE *arquivo, Matriz *matriz) {
    size_t ret;

    Matriz * matriz = malloc(sizeof(Matriz));

    if(!arquivo) {
        fprinf(stderr, "Erro na abertura do arquivo\n");
        return NULL;
    }

    int dimensao = matriz->linhas * matriz->colunas;

    ret = fread(matriz->dados, sizeof(float), dimensao, arquivo);
    if(ret != dimensao) {
        fprintf(stderr, "Erro ao ler a matriz do arquivo\n");
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

    leDadosBinario(arq, &matriz1);
    leDadosBinario(arq, &matriz2);
    leDadosBinario(arq, &matrizResultado);
    fclose(arq);

    imprimeDados(matriz1);
    imprimeDados(matriz2);
    imprimeDados(matrizResultado);

    free(matriz1.dados);
    free(matriz2.dados);
    free(matrizResultado.dados);

    return 0;
}