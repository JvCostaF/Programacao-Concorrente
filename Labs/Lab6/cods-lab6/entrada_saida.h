#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//lock de exclusao mutua
pthread_mutex_t mutex;
//variaveis condicionais para controlar a ordem que os leitores e escritores executam
pthread_cond_t cond_ler, cond_escrever;
//variaceis globais para controlar a quantidade de leitores e escritores
int leitores = 0; int escritores = 0; int querendoEscrever = 0; int leitoresAguardando = 0;

void EntraLeitura() {
    pthread_mutex_lock(&mutex);

    leitoresAguardando++;

    //verifica se temos escritores escrevendo ou querendo escrever
    while(escritores > 0 || querendoEscrever > 0) { //alguem ja escrevendo ou alguem querendo escrever
        printf("Leitor: Existem escritores trabalhando e ou querendo escrever, aguarde!\n");
        printf("Leitores aguardando: %d\n", leitoresAguardando);
        pthread_cond_wait(&cond_ler, &mutex);
    }

    leitoresAguardando--;

    leitores++;
    printf("Chegou um leitor\n");
    pthread_mutex_unlock(&mutex);
}

void SaiLeitura() {
    pthread_mutex_lock(&mutex);

    printf("Leitor saindo.\n");
    printf("Leitores aguardando: %d\n", leitoresAguardando);
    leitores--;
    
    pthread_cond_broadcast(&cond_escrever);
    pthread_cond_broadcast(&cond_ler);
    pthread_mutex_unlock(&mutex);
}

void EntraEscrita() {
    pthread_mutex_lock(&mutex);

    querendoEscrever++;

    while(leitores > 0 || escritores > 0) {
        printf("Escritor: Existem %d escritor(es) ou %d leitor(es) trabalhando e %d escritores esperando, aguarde!\n", escritores, leitores, querendoEscrever);
        pthread_cond_wait(&cond_escrever, &mutex);
    }

    querendoEscrever--;

    escritores++;
    printf("Chegou um escritor\n");
    pthread_mutex_unlock(&mutex);
}

void SaiEscrita() {
    pthread_mutex_lock(&mutex);

    printf("Escritor saindo.\n");
    printf("Escritores aguardando: %d\n", querendoEscrever);
    escritores--;

    pthread_cond_broadcast(&cond_ler);
    pthread_cond_broadcast(&cond_escrever);
    pthread_mutex_unlock(&mutex);
}