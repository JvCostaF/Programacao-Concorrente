#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "timer.h"

#define F_OK 0

/*
Programa concorrente que ira ler a quantidade de Threads e um arquivo binario com as dimensoes (linhas e colunas)de duas matrizes e
as duas matrizes respectivamente e ira calcular o produto entre elas.

Minha ideia foi colocara concorrencia do programa no produto interno entre as linhas e colunas das matrizes, para reaproveitar o maximo possivel
do codigo do ultimo Lab
*/

int nThreads;

// Definindo uma struct para representar uma matriz
typedef struct {
    int linhas;
    int colunas;
    float *dados;
} Matriz;

typedef struct {
    const Matriz *matriz1;
    const Matriz *matriz2;
    Matriz *resultado;
    int linha;
} ThreadData;

void checaAlocacao(void* ptr) {
    if (ptr == NULL) {
        fprintf(stderr, "Erro na alocacao de memoria\n");
        exit(1);
    }
}

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

    // Define as dimensoes das matrizes.
    int dimensaoMat1 = matriz1->linhas * matriz1->colunas;
    int dimensaoMat2 = matriz2->linhas * matriz2->colunas;

    // Aloca memória para os dados das matrizes
    matriz1->dados = (float*)malloc(dimensaoMat1 * sizeof(float));
    matriz2->dados = (float*)malloc(dimensaoMat2 * sizeof(float));

    if (matriz1->dados == NULL || matriz2->dados == NULL) {
        fprintf(stderr, "Erro na alocacao de memoria\n");
        exit(1);
    }

    // Le os elementos da matriz1
    ret = fread(matriz1->dados, sizeof(float), dimensaoMat1, arquivo);
    if (ret != dimensaoMat1) {
        fprintf(stderr, "Erro ao ler a matriz1 do arquivo\n");
        exit(1);
    }

    // Le os elementos da matriz2
    ret = fread(matriz2->dados, sizeof(float), dimensaoMat2, arquivo);
    if (ret != dimensaoMat2) {
        fprintf(stderr, "Erro ao ler a matriz2 do arquivo\n");
        exit(1);
    }
}

void* produtoInterno(void* arg) {
    ThreadData* data = (ThreadData*) arg;

    int i = data->linha;

    for(int j =0; j < data->matriz2->colunas; j++) {
        data->resultado->dados[i * data->resultado->colunas + j] = 0.0;
        for(int k = 0; k < data->matriz1->colunas; k++) {
            data->resultado->dados[i * data->resultado->colunas + j] +=
                data->matriz1->dados[i * data->matriz1->colunas + k] * 
                data->matriz2->dados[k * data->matriz2->colunas + j];
        }
    }

    pthread_exit(NULL);

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

Matriz* multiplicaMatrizes(const Matriz *matriz1, const Matriz *matriz2) {
    if (matriz1->colunas != matriz2->linhas) {
        fprintf(stderr, "Erro: O numero de colunas da Matriz 1 deve ser igual ao numero de linhas da Matriz 2.\n");
        return NULL;
    }

    Matriz *resultado = (Matriz*)malloc(sizeof(Matriz));
    checaAlocacao(resultado);
    
    resultado->linhas = matriz1->linhas;
    resultado->colunas = matriz2->colunas;
    resultado->dados = (float*)malloc(resultado->linhas * resultado->colunas * sizeof(float));
    checaAlocacao(resultado->dados);

    pthread_t *threads = (pthread_t*)malloc(resultado->linhas * sizeof(pthread_t));
    ThreadData *threadData = (ThreadData*)malloc(resultado->linhas * sizeof(ThreadData));
    
    for (int i = 0; i < resultado->linhas; i++) {
        threadData[i].matriz1 = matriz1;
        threadData[i].matriz2 = matriz2;
        threadData[i].resultado = resultado;
        threadData[i].linha = i;

        pthread_create(&threads[i], NULL, produtoInterno, (void*)&threadData[i]);
    }

    for (int i = 0; i < resultado->linhas; i++) {
        pthread_join(threads[i], NULL);
    }

    free(threads);
    free(threadData);
    
    return resultado;
}

int checkFile(char *filename) {
    if (access(filename, F_OK) == 0) {
        return 1;
    }

    return 0;
}

// Escreve os resultados no CSV
void writeToCSV(char *filename, char tipo, int numThreads, int rows, int cols, double init, double process, double end) {

    char *header = "dimensao,tipo,threads,inicialização,processamento,finalização";

    // Verifica se o arquivo existe
    int fileExists = checkFile(filename);

    // Prepara arquivo para escrita no de append
    FILE *file;
    file = fopen(filename,"a");

    // Caso o arquivo não exista ele adiciona o header
    if (fileExists == 0) {
        fprintf(file, "%s\n", header);
    }

    // Adiciona nova linha
    fprintf(file, "%dx%d,%c,%d,%lf,%lf,%lf\n", rows, cols, tipo, numThreads, init, process, end);
    fclose(file);
}

int main(int argc, char *argv[]) {

    FILE * descritorArquivo; //descritor do arquivo de saida
    size_t ret; //retorno da funcao de escrita no arquivo de saida

    if (argc < 3) {
        fprintf(stderr, "Uso: %s <arquivo binario> <arquivo resultado> <nThreads>\n", argv[0]);
        return 1;
    }

    int nThreads = atoi(argv[3]);
    if (nThreads <= 0) {
        fprintf(stderr, "Erro: O numero de threads deve ser um numero positivo.\n");
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

    double inicio, fim, tempoTotal;

    descritorArquivo = fopen(argv[2], "wb");
    if (!descritorArquivo) {
        fprintf(stderr, "Erro de abertura do arquivo\n");
        return 3;
    }

    ret = fwrite(&matriz1.linhas, sizeof(int), 1, descritorArquivo);
    if (ret != 1) {
        fprintf(stderr, "Erro ao escrever as dimensões no arquivo\n");
        return 4;
    }

    ret = fwrite(&matriz1.colunas, sizeof(int), 1, descritorArquivo);
    if (ret != 1) {
        fprintf(stderr, "Erro ao escrever as dimensões no arquivo\n");
        return 4;
    }

    ret = fwrite(&matriz2.linhas, sizeof(int), 1, descritorArquivo);
    if (ret != 1) {
        fprintf(stderr, "Erro ao escrever as dimensões no arquivo\n");
        return 4;
    }

    ret = fwrite(&matriz2.colunas, sizeof(int), 1, descritorArquivo);
    if (ret != 1) {
        fprintf(stderr, "Erro ao escrever as dimensões no arquivo\n");
        return 4;
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

    GET_TIME(inicio);

    Matriz *resultado = multiplicaMatrizes(&matriz1, &matriz2);
    if (resultado != NULL) {
        //imprimeDados(*resultado); // Imprime a matriz resultante

        int dimensaoResultado = matriz1.linhas * matriz2.colunas;

        ret = fwrite(resultado->dados, sizeof(float), dimensaoResultado, descritorArquivo);
        if (ret != dimensaoResultado) {
            fprintf(stderr, "Erro ao escrever a primeira matriz no arquivo\n");
            return 5;
        } 

        free(resultado->dados);   // Liberar a memória da matriz resultante
        free(resultado);  
    }

    GET_TIME(fim);
    tempoTotal = fim - inicio;

    printf("Tempo total foi: %.14f\n", tempoTotal);

    // Liberar a memória das outras matrizes
    free(matriz1.dados);
    free(matriz2.dados);

    fclose(descritorArquivo);

    writeToCSV("medidasTempo.csv", 'C', nThreads, matriz1.linhas, matriz2.colunas, inicio, tempoTotal, fim);

    return 0;
}
