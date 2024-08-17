#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

typedef struct {
    int* array;
    int inicio;
    int fim;
    int id; //ID para a thread
} DadosThread;

/*
Função que cria um vetor de tamanho N, com base no valor escolhido
*/
int* criaVetor(int size) {
    int* array = (int*)malloc(size * sizeof(int)); //Alocando espaço para o vetor.

    if(array == NULL) {
        printf("Problemas com a alocação de memória");
        exit(1);
    }

    srand(time(NULL)); //Usando números aleatórios

    for(int i = 0; i < size; i++) {
        array[i] = rand() % 100;
    }

    return array;
}

/*
Função que será executada por cada thread, somando 1 em cada elemento da sua parte do vetor.
*/
void* addUmParaCadaElemento(void* arg) { //A assinatura do metodo precisou ser alterada por causa do pthread_create.
    DadosThread* data = (DadosThread*)arg;

    for(int i = data->inicio; i < data->fim; i++) {
        data->array[i] += 1;
    }

    //Mensagem para dizer que a Thread i terminou seu processamento.
    printf("Thread %d terminou de processar elementos de %d a %d.\n", data->id, data->inicio, data->fim - 1);
    return NULL;
}

int main(void) {

    int size, qtdThreads;
    printf("Digite o tamanho do vetor: \n");
    scanf("%d", &size);

    printf("Digite o número de threads: \n");
    scanf("%d", &qtdThreads);

    int* array = criaVetor(size);

    printf("Vetor de %d posições:\n", size);
    printf("[ ");
    for(int i = 0; i < size; i++) {
        printf("%d ", array[i]);
    }
    printf("]\n");

    pthread_t threads[qtdThreads];
    DadosThread dadosThread[qtdThreads];
    int parte = size / qtdThreads;

    // Criando threads para processar o vetor
    for(int i = 0; i < qtdThreads; i++) {
        dadosThread[i].array = array;
        dadosThread[i].inicio = i * parte;
        dadosThread[i].fim = (i == qtdThreads - 1) ? size : (i + 1) * parte;
        dadosThread[i].id = i; // Define o ID da thread

        if(pthread_create(&threads[i], NULL, addUmParaCadaElemento, &dadosThread[i]) != 0) {
            printf("Erro ao criar a thread %d.\n", i);
            return 1;
        }
    }

    // Espera todas as threads terminarem
    for(int i = 0; i < qtdThreads; i++) {
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
