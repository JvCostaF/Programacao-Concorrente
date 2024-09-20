#include <stdio.h> 
#include <stdlib.h> 
#include <math.h> 
#include <pthread.h>

/*
    Objetivo: implementar uma versão com concorrência do programa piBBP.c

    A ideia é dividir a série em intervalos iguais entre as threads!
*/ 

#define nThreads 4 //Optei por deixar o número de Threads fixo, mas porderíamos solicitar isso como entrada padrão.

pthread_mutex_t mutex; //Inicia nossa variável de lock e unlock.

long double pi = 0.0; //Inicia pi

typedef struct {
    int inicio;
    int fim;
} Intervalo;

void* piBBP(void* arg){

    Intervalo *intervalo = (Intervalo *) arg;

    int inicio = intervalo->inicio;
    int fim = intervalo->fim;

    long double soma = 0.0;

    for (int i = inicio; i < fim; i++) {
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
    Intervalo intervalos[nThreads];

    pthread_mutex_init(&mutex, NULL);

    int termosPorThread = n / nThreads;
    int resto = n % nThreads;

    for (int i = 0; i < nThreads; i++) {
        intervalos[i].inicio = i * termosPorThread;
        intervalos[i].fim = (i + 1) * termosPorThread;

        if (i == nThreads - 1) {
            intervalos[i].fim += resto;
        }
        pthread_create(&threads[i], NULL, piBBP, &intervalos[i]); 
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