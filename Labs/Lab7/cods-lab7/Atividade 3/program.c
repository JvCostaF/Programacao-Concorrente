#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS 3

sem_t estado1, estado2;

int caracter;

/*
Criei uma struct para facilitar a minha vida na hora de passar um arquivo para uma thread.
*/
typedef struct {
    FILE *file;
    int thread_id;
} thread_data_t;

typedef struct {
    int *Buffer;
    int bufferSize;
} buffer;

/*
Thread 1 irá armazenar os caracteres do arquivo de entrada no Buffer 1. Ela será executada antes das
outras duas.
*/
void *Thread1(void *arg) {

    thread_data_t *data = (thread_data_t *) arg;

    buffer b;

    // Lê o tamanho do arquivo. 
    // Retirei isso do seguinte link: https://stackoverflow.com/questions/238603/how-can-i-get-a-files-size-in-c
    fseek(data->file, 0L, SEEK_END);
    b.bufferSize = ftell(data->file); // O tamanho do arquivo vai ser o tamanho do Buffer.

    fseek(data->file, 0L, SEEK_SET);

    b.Buffer = (int *) malloc(b.bufferSize * sizeof(int)); // Aloca espaço na memória para o Buffer 1.

    // Armazena os caracteres do arquivo de entrada no Buffer 1.
    for (int i = 0; i < b.bufferSize; i++) {
            caracter = fgetc(data->file); // Lê cada caractere do arquivo.
            if (caracter == EOF) { // Verifica se chegou ao final do arquivo.
                break;
            }
            b.Buffer[i] = caracter; // Armazena o caractere no Buffer1.
    }

    ImprimeBuffer(&b);
    free(b.Buffer);

    pthread_exit(NULL);

}

void *Thread2(void *arg) {

    buffer *Buffer1 = (buffer *)arg;

    buffer *Buffer2;

    Buffer2->Buffer = (int *) malloc(Buffer1->bufferSize * sizeof(int)); // Aloca espaço na memória para o Buffer 2.

    Buffer2->bufferSize = Buffer1->bufferSize; // Define tamanho do Buffer 2 igual ao do Buffer 1.

    int n = 0; // Representa um indice no Buffer 2.

    int caracteresLidos = 0;

    int indice = 2*n + 1;

    while(n < 10) {
        for(int i = 0; i < Buffer1->bufferSize; i++) {
            caracteresLidos++;
            if(caracteresLidos == indice) {
                Buffer2->Buffer[indice] = Buffer1->Buffer[i] + '\n';
            }
            n++;
        }
    }


}

/*
Fiz uma funcao para imprimir o buffer para facilitar no debug do meu codigo. Ela tambem sera
utilizada para cumprir os objetivos do Lab 7.
*/
int ImprimeBuffer(buffer *b) {

    printf("Conteúdo do Buffer:\n");
    printf("[");
    for (int i = 0; i < b->bufferSize; i++) {
        printf("%c", b->Buffer[i]);  // Imprime cada caractere do buffer
        if (i < b->bufferSize - 1) {
            printf(", ");  // Adiciona uma vírgula e espaço, exceto no último elemento
        }
    }
    printf("]");
    printf("\n");

    return 0;

}

int main(int argc, char *argv[]) {

    pthread_t tid[NTHREADS];
    thread_data_t thread_data;

    thread_data.file = fopen(argv[1], "r");
    if(thread_data.file == NULL) {
        perror("Erro ao abrir o arquivo");
        return EXIT_FAILURE;
    }

    if(argc < 2) {
        printf("Digite o %s <nome do arquivo de entrada>\n", argv[0]);
    }

    thread_data.thread_id = 1;

    // Criando as threads.
    // Thread 1
    if(pthread_create(&tid[0], NULL, Thread1, (void *)&thread_data) != 0) {
        perror("Erro ao criar thread");
        return EXIT_FAILURE;
    }

    // Thread 2
    if(pthread_create(&tid[1], NULL, Thread2, ) {
        perror("Erro ao criar thread");
        return EXIT_FAILURE;
    }

    // Thread 3
    if(pthread_create(&tid[2], NULL, Thread3, ) {
        perror("Erro ao criar thread");
    return EXIT_FAILURE;
    }

    // Lê e imprime o conteúdo do arquivo
    // char linha[256];
    // while (fgets(linha, sizeof(linha), thread_data.file)) {
    //     printf("%s", linha);
    // }
    // printf("\n");

    //--espera todas as threads terminarem
    for (int t=0; t<NTHREADS; t++) {
        if (pthread_join(tid[t], NULL)) {
            printf("--ERRO: pthread_join() \n"); exit(-1); 
        } 
    } 

    // Fecha o arquivo
    fclose(thread_data.file);
    return EXIT_SUCCESS;

}