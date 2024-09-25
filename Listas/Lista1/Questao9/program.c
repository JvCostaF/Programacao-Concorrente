#include <stdio.h>
#include <pthread.h>

/*
    Objetivo: implementar um programa concorrente que imprima as seguintes frases, seguindo essa ordem:
    Thread1: olá, você está acessando a variável ’aux’ agora?
    Thread2: oi, não, não estou
    Thread1: certo, então vou alterá-la, tá?
    Thread2: tudo bem
    Thread1: terminei a alteração da variável ’aux’
    Thread2: perfeito, recebido!

    Ao total teremos duas threads, mas elas só irão executar tudo o que devem em partes. 
*/

int aux =0;

pthread_mutex_t lock;
pthread_cond_t condT1, condT2;

void *primeiraThread(void *arg) {
    pthread_mutex_lock(&lock);

    printf("Thread 1: ola, voce esta acessando a variavel 'aux' agora?\n");

    pthread_cond_signal(&condT1);

    pthread_cond_wait(&condT2, &lock);

    printf("Thread 1: certo, entao vou altera-la, ta?\n");

    aux = 171;
    
    pthread_cond_signal(&condT1);

    pthread_cond_wait(&condT2, &lock);

    printf("Thread 1: terminei a alteracao da variavel 'aux'\n");

    pthread_cond_signal(&condT1);

    pthread_mutex_unlock(&lock);

    return NULL;
}

void *segundaThread(void *arg) {
    pthread_mutex_lock(&lock);
    pthread_cond_wait(&condT1, &lock);

    printf("Thread 2: oi, nao estou\n");

    pthread_cond_signal(&condT2);

    pthread_cond_wait(&condT1, &lock);

    printf("Thread 2: tudo bem\n");

    pthread_cond_signal(&condT2);

    pthread_cond_wait(&condT1, &lock);

    printf("Thread 2: perfeito, recebido\n");

    pthread_mutex_unlock(&lock);

    return NULL;
}


int main(int argc, char *argv[]){
    pthread_mutex_init(&lock, NULL);

    pthread_cond_init(&condT1, NULL);
    pthread_cond_init(&condT2, NULL);

    pthread_t T1, T2;

    pthread_create(&T1, NULL, primeiraThread, NULL);
    pthread_create(&T2, NULL, segundaThread, NULL);

    pthread_mutex_destroy(&lock);

    pthread_cond_destroy(&condT1);
    pthread_cond_destroy(&condT2);

    return 0;
}