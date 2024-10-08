/* Disciplina: Programacao Concorrente */
/* Profa.: Silvana Rossetto */
/* Aluno: Joao Victor da Costa Farias - 120014876 */
/* Laboratório: 1 - Atividade 5 */

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
int* criaVetor(int tamanho) {
    int* array = (int*)malloc(tamanho * sizeof(int)); //Alocando espaço para o vetor.

    if(array == NULL) {
        printf("Problemas com a alocação de memória");
        exit(1);
    }

    srand(time(NULL)); //Usando números aleatórios

    for(int i = 0; i < tamanho; i++) {
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

    int tamanho, qtdThreads;
    printf("Digite o tamanho do vetor: \n");
    scanf("%d", &tamanho);

    printf("Digite o número de threads: \n");
    scanf("%d", &qtdThreads);

    int* array = criaVetor(tamanho);

    printf("Vetor de %d posições:\n", tamanho);
    printf("[ ");
    for(int i = 0; i < tamanho; i++) {
        printf("%d ", array[i]);
    }
    printf("]\n");

    pthread_t threads[qtdThreads];
    DadosThread dadosThread[qtdThreads];
    int parte = tamanho / qtdThreads;

    // Criando threads para processar o vetor
    for(int i = 0; i < qtdThreads; i++) {
        dadosThread[i].array = array;
        dadosThread[i].inicio = i * parte;
        dadosThread[i].fim = (i == qtdThreads - 1) ? tamanho : (i + 1) * parte;
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

    printf("Vetor de %d posições alterado:\n", tamanho);
    printf("[ ");
    for(int i = 0; i < tamanho; i++) {
        printf("%d ", array[i]);
    }
    printf("]\n");

    free(array);

    return 0;
}
