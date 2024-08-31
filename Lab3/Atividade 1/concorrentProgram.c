#include <stdio.h>
#include <stdlib.h>

int linhas, colunas;
float *matriz1, *matriz2;

/**
 * @brief Lê dados binários de um arquivo e armazena em variáveis fornecidas.
 *
 * Esta função lê a dimensão de um vetor, dois vetores de números reais e um valor de produto interno
 * de um arquivo binário. A memória para os vetores é alocada dinamicamente com base na dimensão lida.
 *
 * @param arquivo Ponteiro para o arquivo binário a ser lido. O arquivo deve estar previamente aberto.
 * @param linhas Ponteiro para um inteiro onde a qtd de linhas da matriz será armazenada.
 * @param colunas Ponteiro para um inteiro onde a qtd de linhas da matriz será armazenada.
 * @param matriz1 Ponteiro para um ponteiro de float onde o primeiro vetor será armazenado. A memória
 *               é alocada dentro da função.
 * @param matriz2 Ponteiro para um ponteiro de float onde o segundo vetor será armazenado. A memória
 *               é alocada dentro da função.
 *
 * @note Esta função encerra o programa com `exit(1)` em caso de erro ao ler os dados ou ao alocar memória.
 *
 * @warning Certifique-se de liberar a memória alocada para `vetor1` e `vetor2` após o uso para evitar vazamentos de memória.
 */
void leDadosBinario(FILE *arquivo, int *linhas, int *colunas, float **matriz1, float **matriz2) {

    size_t ret;

    // Le o numero de Linhas
    ret = fread(linhas, sizeof(int), 1, arquivo);
    if (ret != 1) {
        fprintf(stderr, "Erro ao ler o numero de linhas\n");
        exit(1);
    }

    // Le o numero de Colunas
    ret = fread(colunas, sizeof(int), 1, arquivo);
    if (ret != 1) {
        fprintf(stderr, "Erro ao ler o numero de colunas\n");
        exit(1);
    }

    int dimensao = (*linhas) * (*colunas);

    // Alocar memória para os vetores com base na dimensão lida
    *matriz1 = (float*)malloc(dimensao * sizeof(float));
    *matriz2 = (float*)malloc(dimensao * sizeof(float));
    if (*matriz1 == NULL || *matriz2 == NULL) {
        fprintf(stderr, "Erro na alocacao de memoria\n");
        exit(1);
    }

    // Ler os elementos da matriz1
    ret = fread(*matriz1, sizeof(float), dimensao, arquivo);
    if (ret != dimensao) {
        fprintf(stderr, "Erro ao ler a matriz1 do arquivo\n");
        exit(1);
    }

    // Ler os elementos da matriz2
    ret = fread(*matriz2, sizeof(float), dimensao, arquivo);
    if (ret != dimensao) {
        fprintf(stderr, "Erro ao ler a matriz2 do arquivo\n");
        exit(1);
    }

}

void imprimeDados(int linhas, int colunas, float *matriz1, float *matriz2) {
    printf("Dimensoes: %dx%d\n", linhas, colunas);

    printf("Matriz 1:\n");
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            printf("%f ", matriz1[i * colunas + j]);
        }
        printf("\n");
    }

    printf("Matriz 2:\n");
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            printf("%f ", matriz2[i * colunas + j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        fprintf(stderr, "Uso: %s <arquivo binario>\n", argv[0]);
        return 1;
    }

    FILE *arq = fopen(argv[1], "rb");
    if (arq == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo\n");
        return 2;
    }

    leDadosBinario(arq, &linhas, &colunas, &matriz1, &matriz2);
    fclose(arq);

    imprimeDados(linhas, colunas, matriz1, matriz2);

    free(matriz1);
    free(matriz2);

    return 0;

}