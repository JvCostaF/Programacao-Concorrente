#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS 3

sem_t estado1, estado2;

int *Buffer1;
int *Buffer2;

int caracter;

int main(int argc, char *argv[]) {

    pthread_t *tid;
    int nThreads;

    FILE *arquivoEntrada = fopen(argv[2], "r");


    if(argc < 3) {
        printf("Digite %s <número de threads e  %s <nome do arquivo>\n", argv[1], argv[2]);
    }

    // Lê e imprime o conteúdo do arquivo
    char linha[256];
    while (fgets(linha, sizeof(linha), arquivoEntrada)) {
        printf("%s", linha);
    }
    printf("\n");

    // Lê o tamanho do arquivo.
    fseek(arquivoEntrada, 0L, SEEK_END);
    long fileSize = ftell(arquivoEntrada);
    printf("Tamanho total do arquivo de entrada: %ld\n", fileSize);

    fseek(arquivoEntrada, 0L, SEEK_SET);

    Buffer1 = (int *) malloc(fileSize * sizeof(int));
    for (int i = 0; i < fileSize; i++) {
            caracter = fgetc(arquivoEntrada); // Lê cada caractere do arquivo
            if (caracter == EOF) { // Verifica se chegou ao final do arquivo
                break;
            }
            Buffer1[i] = caracter; // Armazena o caractere no Buffer1
    }

    printf("Conteúdo do Buffer 1:\n");
    printf("[");
    for (int i = 0; i < fileSize; i++) {
        printf("%c", Buffer1[i]);  // Imprime cada caractere do buffer
        if (i < fileSize - 1) {
            printf(", ");  // Adiciona uma vírgula e espaço, exceto no último elemento
        }
    }
    printf("]");
    printf("\n");

    // Fecha o arquivo
    fclose(arquivoEntrada);
    free(Buffer1);
    return 0;

}