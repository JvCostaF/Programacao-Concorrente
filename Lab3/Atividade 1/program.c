#include <stdio.h>
#include <stdlib.h>
#include <time.h>

float* geradorMatriz(int dimensaoMatriz) {
    float *matriz;
    matriz = (float*) malloc(sizeof(float) * dimensaoMatriz); // Aloca espaço na memória para a matriz

    if (!matriz) {
        fprintf(stderr, "Erro de alocação da memória da matriz\n");
        return NULL;
    }

    for (int i = 0; i < dimensaoMatriz; i++) {
        matriz[i] = (rand() % 1000) * 0.3;
    }

    return matriz;
}

void printMatriz(float *matriz, int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            printf("%.6f ", matriz[i * colunas + j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main(int argc, char* argv[]) {
    int linhas, colunas; // dimensões da matriz
    long long int tam;   // qtde de elementos na matriz
    FILE *descritorArquivo; // descritor do arquivo de saída
    size_t ret;          // retorno da função de escrita no arquivo de saída
   
    // recebe os argumentos de entrada
    if (argc < 4) {
        fprintf(stderr, "Digite: %s <linhas> <colunas> <arquivo saída>\n", argv[0]);
        return 1;
    }

    linhas = atoi(argv[1]); 
    colunas = atoi(argv[2]);
    tam = linhas * colunas;

    srand(time(NULL));

    float *matriz1 = geradorMatriz(tam);
    float *matriz2 = geradorMatriz(tam);

    if (!matriz1 || !matriz2) {
        return 2;
    }

    // Imprime as matrizes geradas
    printf("Matriz 1 (%dx%d):\n", linhas, colunas);
    printMatriz(matriz1, linhas, colunas);

    printf("Matriz 2 (%dx%d):\n", linhas, colunas);
    printMatriz(matriz2, linhas, colunas);

    // abre o arquivo para escrita binária
    descritorArquivo = fopen(argv[3], "wb");
    if (!descritorArquivo) {
        fprintf(stderr, "Erro de abertura do arquivo\n");
        return 3;
    }
  
    ret = fwrite(&linhas, sizeof(int), 1, descritorArquivo);
    if (ret != 1) {
        fprintf(stderr, "Erro ao escrever as dimensões no arquivo\n");
        return 4;
    }

    ret = fwrite(&colunas, sizeof(int), 1, descritorArquivo);
    if (ret != 1) {
        fprintf(stderr, "Erro ao escrever as dimensões no arquivo\n");
        return 4;
    }

    ret = fwrite(matriz1, sizeof(float), tam, descritorArquivo);
    if (ret != tam) {
        fprintf(stderr, "Erro ao escrever a primeira matriz no arquivo\n");
        return 5;
    }

    ret = fwrite(matriz2, sizeof(float), tam, descritorArquivo);
    if (ret != tam) {
        fprintf(stderr, "Erro ao escrever a segunda matriz no arquivo\n");
        return 6;
    }

    fclose(descritorArquivo);
    free(matriz1);
    free(matriz2);
    return 0;
}
