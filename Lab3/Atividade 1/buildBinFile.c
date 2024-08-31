#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
Programa que gera duas matrizes e um arquivo binario com a dimensao (linhas e colunas) de duas matizes,
armazenando tambem as duas matrizes. 
*/

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
    int linhasMat1, colunasMat1; // dimensões da matriz1
    int linhasMat2, colunasMat2;
    long long int tamMat1;
    long long int tamMat2;   // qtde de elementos na matriz
    FILE *descritorArquivo; // descritor do arquivo de saída
    size_t ret;          // retorno da função de escrita no arquivo de saída
   
    // recebe os argumentos de entrada
    if (argc < 4) {
        fprintf(stderr, "Digite: %s <linhas> <colunas> <linhas> <colunas> <arquivo saída>\n", argv[0]);
        return 1;
    }

    linhasMat1 = atoi(argv[1]); 
    colunasMat1 = atoi(argv[2]);
    tamMat1 = linhasMat1 * colunasMat1;

    linhasMat2 = atoi(argv[3]);
    colunasMat2 = atoi(argv[4]);
    tamMat2 = linhasMat2 * colunasMat2;

    srand(time(NULL));

    float *matriz1 = geradorMatriz(tamMat1);
    float *matriz2 = geradorMatriz(tamMat2);

    if (!matriz1 || !matriz2) {
        return 2;
    }

    // Imprime as matrizes geradas
    printf("Matriz 1 (%dx%d):\n", linhasMat1, colunasMat1);
    printMatriz(matriz1, linhasMat1, colunasMat1);

    printf("Matriz 2 (%dx%d):\n", linhasMat2, colunasMat2);
    printMatriz(matriz2, linhasMat2, colunasMat2);

    // abre o arquivo para escrita binária
    descritorArquivo = fopen(argv[5], "wb");
    if (!descritorArquivo) {
        fprintf(stderr, "Erro de abertura do arquivo\n");
        return 3;
    }
  
    ret = fwrite(&linhasMat1, sizeof(int), 1, descritorArquivo);
    if (ret != 1) {
        fprintf(stderr, "Erro ao escrever as dimensões no arquivo\n");
        return 4;
    }

    ret = fwrite(&colunasMat1, sizeof(int), 1, descritorArquivo);
    if (ret != 1) {
        fprintf(stderr, "Erro ao escrever as dimensões no arquivo\n");
        return 4;
    }

    ret = fwrite(&linhasMat2, sizeof(int), 1, descritorArquivo);
    if (ret != 1) {
        fprintf(stderr, "Erro ao escrever as dimensões no arquivo\n");
        return 4;
    }

    ret = fwrite(&colunasMat2, sizeof(int), 1, descritorArquivo);
    if (ret != 1) {
        fprintf(stderr, "Erro ao escrever as dimensões no arquivo\n");
        return 4;
    }

    ret = fwrite(matriz1, sizeof(float), tamMat1, descritorArquivo);
    if (ret != tamMat1) {
        fprintf(stderr, "Erro ao escrever a primeira matriz no arquivo\n");
        return 5;
    }

    ret = fwrite(matriz2, sizeof(float), tamMat2, descritorArquivo);
    if (ret != tamMat2) {
        fprintf(stderr, "Erro ao escrever a segunda matriz no arquivo\n");
        return 6;
    }

    fclose(descritorArquivo);
    free(matriz1);
    free(matriz2);
    return 0;
}
