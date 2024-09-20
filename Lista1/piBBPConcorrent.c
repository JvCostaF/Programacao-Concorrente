#include <stdio.h> 
#include <stdlib.h> 
#include <math.h> 
#include <pthread.h>

/*
    Objetivo: implementar uma versão com concorrência do programa piBBP.c

    A ideia é dividir a série em intervalos iguais entre as threads!
*/ 

#define nThreads 3

pthread_mutex_t mutex; //Inicia nossa variável de lock e unlock.

long double pi = 0.0; //Inicia pi

void* piBBP(void* arg){

    int inicio = *(int *) arg;   
    int n = *(int *) arg + (n/nThreads);

    long double soma = 0.0;

    for (int i = 0; i < n; i++) {
        long double termo = (4.0 / (8*i + 1)) -
                        (2.0 / (8*i + 4)) -
                        (1.0 / (8*i + 5)) -
                        (1.0 / (8*i + 6));
        soma += termo / powl(16, i);
    }

    //Início Seção Crítica
    pthread_mutex_lock(&mutex);
    pi += soma;
    pthread_mutex_unlock(&mutex);
    //Fim Seção Crítica

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

    if(argc < 2) {
        printf("Digite o numero de termos.\n");
        return 1;
    }

    int n = atoi(argv[1]);

    pthread_t threads[nThreads];
    int tarefas[nThreads];

    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < nThreads; i++) {
        tarefas[i] = i * (n/nThreads);
        pthread_create(&threads[i], NULL, piBBP, &tarefas[i]); 
    }

    for(int i = 0; i < nThreads; i++) {
            if (pthread_join(threads[i], NULL)) {
            printf("--ERRO: pthread_join()\n");
            exit(-1);
        }
    }

    printf("Valor aproximado de pi com %d termos: %.26Lf\n", n, pi);

    pthread_mutex_destroy(&mutex);

    return 0;
}