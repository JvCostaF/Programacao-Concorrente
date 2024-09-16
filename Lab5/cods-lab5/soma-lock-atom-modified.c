#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

long int soma = 0; // variável compartilhada entre as threads
int primeiros = 0; // contador de múltiplos de 10
int pode_imprimir = 0; // flag para controle de impressão
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
            pode_imprimir = 1; // Permite que a thread extra imprima
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
        if (pode_imprimir) { // Só imprime se a flag permitir
            printf("soma = %ld\n", soma); // Imprime o valor de 'soma'
            primeiros++;
            pode_imprimir = 0; // Reseta a flag de impressão
            pthread_cond_signal(&cond_prosseguir); // Sinaliza para as threads continuarem
        } else {
            pthread_cond_wait(&cond_imprimir, &mutex); // Aguarda o sinal de que 'soma' é múltiplo de 10
        }
    }

    pthread_mutex_unlock(&mutex);
    printf("Extra: terminou!\n");
    pthread_exit(NULL);
}

// Fluxo principal
int main(int argc, char *argv[]) {
    pthread_t tid[2]; // Array para armazenar os dois identificadores de thread

    // Inicializa o mutex e as variáveis de condição
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_imprimir, NULL);
    pthread_cond_init(&cond_prosseguir, NULL);

    // Cria a única thread de execução
    if (pthread_create(&tid[0], NULL, ExecutaTarefa, (void *)0)) {
        printf("--ERRO: pthread_create()\n");
        exit(-1);
    }

    // Cria a única thread de log (extra)
    if (pthread_create(&tid[1], NULL, extra, NULL)) {
        printf("--ERRO: pthread_create()\n");
        exit(-1);
    }

    // Espera as duas threads terminarem
    for (int t = 0; t < 2; t++) {
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
