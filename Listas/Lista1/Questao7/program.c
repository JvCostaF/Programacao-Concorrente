/*
Aqui precisamos implementar parte de uma solução com produtores e consumidores, pois precisamos apenas implementar as funções INSERE, 
RETIRA e os parâmetros globais.

Algumas regras para o nosso código:

1) Cada execução do produtor deve preencher o buffer inteiro, e não apenas um único item (para isso ele deve esperar o buffer ficar
totalmente vazio);
2) Os consumidores seguem a lógica convencional, só vão inserir um item de cada vez.
*/

#include <stdio.h>
#include <pthread.h>

#define N //Tamanho do Buffer

int Buffer[N]; //Buffer com tamanho N
int count = 0; //Variável que vai validar se o Buffer está cheio ou não
int in, out; //Variáveis para lidar com a inserção dos produtores e remoção dos consumidores

pthread_mutex_t mutex;
pthread_cond_t cond_inse, cond_reti;

void *Insere(tElemento elem){
    pthread_mutex_lock(&mutex);

    while(count <= N){
        pthread_cond_wait(&cond_inse, &mutex);
    }
    count++;
    Buffer[in] = elem;
    in = (in + 1) % N;
    pthread_cond_signal(&cond_reti);
    pthread_mutex_unlock(&mutex);   
}

tElemento Retira(void){
    pthread_mutex_lock(&mutex);
    while(count == 0){
        pthread_cond_wait(&cond_reti, &mutex);
    }
    count--;
    elem = Buffer[out];
    out = (out - 1) % N;
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&cond_inse);
    return elem;
}