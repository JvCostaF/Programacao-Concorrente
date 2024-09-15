#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

long int soma = 0; // variável compartilhada entre as threads
int primeiros = 0; // contador de múltiplos de 10
pthread_mutex_t mutex; // variável de lock para exclusão mútua
pthread_cond_t cond_imprimir, cond_prosseguir;

// Função executada pelas threads
void *ExecutaTarefa(void *arg) {
    long int id = (long int)arg;
    printf("Thread %ld está executando...\n", id);

    for (int i = 0; i < 100000; i++) {
        pthread_mutex_lock(&mutex); // Entrada na seção crítica
        soma++; // Incrementa a variável compartilhada

        // Se for múltiplo de 10 e ainda não imprimimos os 20 primeiros múltiplos
        if (soma % 10 == 0 && primeiros < 20) {
            pthread_cond_signal(&cond_imprimir); // Sinaliza para a thread 'extra'
            pthread_cond_wait(&cond_prosseguir, &mutex); // Aguarda a thread 'extra'
        }

        pthread_mutex_unlock(&mutex); // Saída da seção crítica
    }

    printf("Thread %ld terminou!\n", id);
    pthread_exit(NULL);
}

// Função executada pela thread de log
void *extra(void *args) {
    printf("Extra: está executando...\n");

    pthread_mutex_lock(&mutex); // Garante que a thread de log comece corretamente

    while (primeiros < 20) {
        pthread_cond_wait(&cond_imprimir, &mutex); // Aguarda o sinal de que 'soma' é múltiplo de 10

        if (soma % 10 == 0) {
            printf("soma = %ld\n", soma); // Imprime o valor de 'soma'
            primeiros++;
        }

        pthread_cond_broadcast(&cond_prosseguir); // Sinaliza para as threads continuarem
    }

    pthread_mutex_unlock(&mutex);
    printf("Extra: terminou!\n");
    pthread_exit(NULL);
}

// Fluxo principal
int main(int argc, char *argv[]) {
    pthread_t *tid; // Identificadores das threads no sistema
    int nthreads;   // Quantidade de threads (passada por linha de comando)

    // Lê e avalia os parâmetros de entrada
    if (argc < 2) {
        printf("Digite: %s <número de threads>\n", argv[0]);
        return 1;
    }
    nthreads = atoi(argv[1]);

    // Aloca as estruturas
    tid = (pthread_t*) malloc(sizeof(pthread_t) * (nthreads + 1));
    if (tid == NULL) {
        puts("ERRO--malloc");
        return 2;
    }

    // Inicializa o mutex e as variáveis de condição
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_imprimir, NULL);
    pthread_cond_init(&cond_prosseguir, NULL);

    // Cria as threads de execução
    for (long int t = 0; t < nthreads; t++) {
        if (pthread_create(&tid[t], NULL, ExecutaTarefa, (void *)t)) {
            printf("--ERRO: pthread_create()\n");
            exit(-1);
        }
    }

    // Cria a thread de log
    if (pthread_create(&tid[nthreads], NULL, extra, NULL)) {
        printf("--ERRO: pthread_create()\n");
        exit(-1);
    }

    // Espera todas as threads terminarem
    for (int t = 0; t < nthreads + 1; t++) {
        if (pthread_join(tid[t], NULL)) {
            printf("--ERRO: pthread_join()\n");
            exit(-1);
        }
    }

    // Finaliza o mutex e as variáveis de condição
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_imprimir);
    pthread_cond_destroy(&cond_prosseguir);

    printf("Valor final de 'soma' = %ld\n", soma);

    return 0;
}
