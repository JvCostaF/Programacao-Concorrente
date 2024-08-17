#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

typedef struct {
    int* array;
    int inicio;
    int fim;
    int id; // Adiciona um ID para a thread
} ThreadData;

/*
Função que cria um vetor de tamanho N, com base no valor escolhido
*/
int* makeArray(int size) {
    int* array = (int*)malloc(size * sizeof(int)); // Alocando espaço para o vetor.

    if(array == NULL) {
        printf("Problemas com a alocação de memória");
        exit(1);
    }

    srand(time(NULL)); // Usando números aleatórios

    for(int i = 0; i < size; i++) {
        array[i] = rand() % 100;
    }

    return array;
}

/*
Função que será executada por cada thread, somando 1 em cada elemento da sua parte do vetor.
*/
void* addOneToEachElement(void* arg) {
    ThreadData* data = (ThreadData*)arg;

    for(int i = data->inicio; i < data->fim; i++) {
        data->array[i] += 1;
    }

    // Indica que a thread terminou seu processamento
    printf("Thread %d terminou de processar elementos de %d a %d.\n", data->id, data->inicio, data->fim - 1);
    return NULL;
}

int main(void) {

    int size, num_threads;
    printf("Digite o tamanho do vetor: \n");
    scanf("%d", &size);

    printf("Digite o número de threads: \n");
    scanf("%d", &num_threads);

    int* array = makeArray(size);

    printf("Vetor de %d posições:\n", size);
    printf("[ ");
    for(int i = 0; i < size; i++) {
        printf("%d ", array[i]);
    }
    printf("]\n");

    pthread_t threads[num_threads];
    ThreadData thread_data[num_threads];
    int parte = size / num_threads;

    // Criando threads para processar o vetor
    for(int i = 0; i < num_threads; i++) {
        thread_data[i].array = array;
        thread_data[i].inicio = i * parte;
        thread_data[i].fim = (i == num_threads - 1) ? size : (i + 1) * parte;
        thread_data[i].id = i; // Define o ID da thread

        if(pthread_create(&threads[i], NULL, addOneToEachElement, &thread_data[i]) != 0) {
            printf("Erro ao criar a thread %d.\n", i);
            return 1;
        }
    }

    // Espera todas as threads terminarem
    for(int i = 0; i < num_threads; i++) {
        if(pthread_join(threads[i], NULL)){
            printf("--ERRO: pthread_join() da thread %d\n", i); 
        }
    }

    printf("Vetor de %d posições alterado:\n", size);
    printf("[ ");
    for(int i = 0; i < size; i++) {
        printf("%d ", array[i]);
    }
    printf("]\n");

    free(array);

    return 0;
}
