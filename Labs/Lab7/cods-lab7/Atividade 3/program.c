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
} buffer_t;

/*
Thread 1 irá armazenar os caracteres do arquivo de entrada no Buffer 1. Ela será executada antes das
outras duas.
*/
void *Thread1(void *arg) {

    printf("Cheguei, sou eu a Thread 1\n");

    thread_data_t *data = (thread_data_t *) arg;

    buffer_t *b = (buffer_t *) malloc(sizeof(buffer_t));

    // Lê o tamanho do arquivo. 
    // Retirei isso do seguinte link: https://stackoverflow.com/questions/238603/how-can-i-get-a-files-size-in-c
    fseek(data->file, 0L, SEEK_END);
    b->bufferSize = ftell(data->file); // O tamanho do arquivo vai ser o tamanho do Buffer.

    fseek(data->file, 0L, SEEK_SET);

    b->Buffer = (int *) malloc(b->bufferSize * sizeof(int)); // Aloca espaço na memória para o Buffer 1.

    // Armazena os caracteres do arquivo de entrada no Buffer 1.
    for (int i = 0; i < b->bufferSize; i++) {
            caracter = fgetc(data->file); // Lê cada caractere do arquivo.
            if (caracter == EOF) { // Verifica se chegou ao final do arquivo.
                break;
            }
            b->Buffer[i] = caracter; // Armazena o caractere no Buffer1.
    }

    sem_post(&estado1);

    //ImprimeBuffer(b);
    printf("Thread 1 terminou\n");

    pthread_exit((void *) b);

}

void *Thread2(void *arg) {

    sem_wait(&estado1);

    printf("Cheguei, sou eu a Thread 2\n");

    buffer_t *Buffer1 = (buffer_t *)arg;

    int qtdDeQuebraDeLinhas = 10 + ((Buffer1->bufferSize - (2 * 10 + 1)) / 10); // Seguindo a logica do enunciado.

    buffer_t *Buffer2 = (buffer_t *) malloc(sizeof(buffer_t));

    Buffer2->Buffer = (int *) malloc((Buffer1->bufferSize+qtdDeQuebraDeLinhas) * sizeof(int)); // Aloca espaço na memória para o Buffer 2.

    Buffer2->bufferSize = Buffer1->bufferSize+qtdDeQuebraDeLinhas; // Define tamanho do Buffer 2 igual ao do Buffer 1 + qtd de quebra de linhas.

    int n = 0;

    int caracteresLidos = 0;

    int indice = 2*n + 1;

    // Primeiro processo, insere \n após 2n+1 caracteres
    for (int i = 0; i < Buffer1->bufferSize; i++) {
        Buffer2->Buffer[caracteresLidos++] = Buffer1->Buffer[i];

        if (caracteresLidos == indice && caracteresLidos < Buffer2->bufferSize) {  // Se atingiu o número correto de caracteres.
            Buffer2->Buffer[caracteresLidos++] = '\n'; // Adiciona a quebra de linha.
            n++;
            if (n <= 10) {
                indice = caracteresLidos + (2 * n + 1); // Atualiza o próximo ponto de quebra de linha.
            } else {
                indice = caracteresLidos + 10;
                // Sai do loop quando n > 10.
            }
        }
    }

    // Depois de n atingir 10, insere \n a cada 10 caracteres
    while (caracteresLidos < Buffer1->bufferSize) {
        for (int i = 0; i < 10 && caracteresLidos < Buffer1->bufferSize; i++) {
            Buffer2->Buffer[caracteresLidos++] = Buffer1->Buffer[caracteresLidos];
        }
        if (caracteresLidos < Buffer1->bufferSize) {
            Buffer2->Buffer[caracteresLidos++] = '\n';  // Adiciona nova linha após 10 caracteres.
        }
    }

    // Garante que a última linha sempre termine com '\n' se o último grupo de 10 caracteres não fez isso.
    if (Buffer2->Buffer[caracteresLidos - 1] != '\n') {
        Buffer2->Buffer[caracteresLidos++] = '\n';
    }

    sem_post(&estado2);

    //ImprimeBuffer(Buffer2);

    printf("Thread 2 terminou\n");

    // free(Buffer2->Buffer);
    // free(Buffer2);

    pthread_exit((void *) Buffer2); // Passando o Buffer 2 para a Thread 3 imprimir.

}

void *Thread3(void *arg) {

    sem_wait(&estado2);

    printf("Cheguei, sou eu a Thread 3\n");

    buffer_t *Buffer = (buffer_t *) arg;



    printf("Conteúdo do Buffer 2:\n");
    printf("\n");
    //printf("[");
    for (int i = 0; i < Buffer->bufferSize; i++) {
        printf("%c", Buffer->Buffer[i]);  // Imprime cada caractere do buffer
        // if (i < b->bufferSize - 1) {
        //     printf(", ");  // Adiciona uma vírgula e espaço, exceto no último elemento
        // }
    }
    //printf("]");
    printf("\n");

    printf("Thread 3 terminou\n");

    pthread_exit(NULL);
}

/*
Fiz uma funcao para imprimir o buffer para facilitar no debug do meu codigo. Ela tambem sera
utilizada para cumprir os objetivos do Lab 7.
*/
int ImprimeBuffer(buffer_t *b) {

    printf("Conteúdo do Buffer:\n");
    printf("[");
    for (int i = 0; i < b->bufferSize; i++) {
        printf("%c", b->Buffer[i]);  // Imprime cada caractere do buffer
        // if (i < b->bufferSize - 1) {
        //     printf(", ");  // Adiciona uma vírgula e espaço, exceto no último elemento
        // }
    }
    printf("]");
    printf("\n");

    return 0;

}

int main(int argc, char *argv[]) {

    pthread_t tid[NTHREADS];
    thread_data_t thread_data;

    sem_init(&estado1, 0, 0);
    sem_init(&estado2, 0, 0);

    thread_data.file = fopen(argv[1], "r");
    if(thread_data.file == NULL) {
        perror("Erro ao abrir o arquivo");
        return EXIT_FAILURE;
    }

    if(argc < 2) {
        printf("Digite o %s <nome do arquivo de entrada>\n", argv[0]);
    }

    // Criando as threads.
    // Thread 1
    buffer_t *Buffer1;
    if(pthread_create(&tid[0], NULL, Thread1, (void *)&thread_data) != 0) {
        perror("Erro ao criar thread");
        return EXIT_FAILURE;
    }

    pthread_join(tid[0], (void **)&Buffer1);

    // Thread 2
    buffer_t *Buffer2;
    if(pthread_create(&tid[1], NULL, Thread2, (void *)Buffer1) != 0) {
        perror("Erro ao criar thread");
        return EXIT_FAILURE;
    }

    pthread_join(tid[1], (void **)&Buffer2);

    // Thread 3
    if(pthread_create(&tid[2], NULL, Thread3, (void *)Buffer2) != 0) {
        perror("Erro ao criar thread");
        return EXIT_FAILURE;
    }

    pthread_join(tid[2], NULL);

    // Lê e imprime o conteúdo do arquivo
    // char linha[256];
    // while (fgets(linha, sizeof(linha), thread_data.file)) {
    //     printf("%s", linha);
    // }
    // printf("\n");

    // Liberando Buffer 1 e Buffer 2

    free(Buffer1->Buffer);
    free(Buffer1);

    free(Buffer2->Buffer);
    free(Buffer2);

    // Fecha o arquivo
    fclose(thread_data.file);
    return EXIT_SUCCESS;

}