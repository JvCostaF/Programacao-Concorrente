#include <stdio.h>
#include <stdlib.h>
#include "list_int.h"
#include <pthread.h>
#include "timer.h"

#define QTDE_OPS 100 //quantidade de operacoes sobre a lista (insercao, remocao, consulta)
#define QTDE_INI 100 //quantidade de insercoes iniciais na lista
#define MAX_VALUE 100 //valor maximo a ser inserido

//lista compartilhada iniciada 
struct list_node_s* head_p = NULL; 
//qtde de threads no programa
int nthreads;
//lock de exclusao mutua
pthread_mutex_t mutex;
//variaveis condicionais para controlar a ordem que os leitores e escritores executam
pthread_cond_t cond_ler, cond_escrever;
//variaceis globais para controlar a quantidade de leitores e escritores
int leitores = 0; int escritores = 0; int querendoEscrever = 0;

void EntraLeitura() {
    pthread_mutex_lock(&mutex);

    //verifica se temos escritores
    while(escritores > 0 || querendoEscrever>0) { //alguem ja escrevendo ou alguem querendo escrever
        printf("Existem escritores trabalhando, aguarde!\n");
        pthread_cond_wait(&cond_ler, &mutex);
    }

    leitores++;
    printf("Existem %d leitores.\n", leitores);
    pthread_mutex_unlock(&mutex);
}

void SaiLeitura() {
    pthread_mutex_lock(&mutex);

    printf("Leitor saindo.\n");
    leitores--;
    pthread_cond_broadcast(&cond_escrever);
    pthread_mutex_unlock(&mutex);
}

void EntraEscrita() {
    pthread_mutex_lock(&mutex);

    querendoEscrever++;
    while(leitores > 0 || escritores > 0) {
        printf("Existem %d escritores ou %d leitores trabalhando, agurade!\n", escritores, leitores);
        pthread_cond_wait(&cond_escrever, &mutex);
    }

    escritores++;
    querendoEscrever--;
    printf("Existem %d escritores.\n", escritores);
    pthread_mutex_unlock(&mutex);
}

void SaiEscrita() {
    pthread_mutex_lock(&mutex);

    printf("Escritor saindo.\n");
    escritores--;

    pthread_cond_broadcast(&cond_ler);
    pthread_mutex_unlock(&mutex);
}

//tarefa das threads
void* tarefa(void* arg) {
   long int id = (long int) arg;
   int op;
   int in, out, read; 
   in=out=read = 0; 

   //realiza operacoes de consulta (98%), insercao (1%) e remocao (1%)
   for(long int i=id; i<QTDE_OPS; i+=nthreads) {
      op = rand() % 100;
      //printf("%d\n", op);
      if(op<98) {
	      EntraLeitura();    
          Member(i%MAX_VALUE, head_p);   /* Ignore return value */
	      SaiLeitura();
	      read++;
      } else if(98<=op && op<99) {
	      EntraEscrita();     
          Insert(i%MAX_VALUE, &head_p);  /* Ignore return value */
	      SaiEscrita();    
	      in++;
      } else if(99<=op) {
	      EntraEscrita();     
          Delete(i%MAX_VALUE, &head_p);  /* Ignore return value */
	      SaiEscrita();    
	      out++;
      }
   }
   //registra a qtde de operacoes realizadas por tipo
   printf("Thread %ld: in=%d out=%d read=%d\n", id, in, out, read);
   pthread_exit(NULL);
}

/*-----------------------------------------------------------------*/
int main(int argc, char* argv[]) {
   pthread_t *tid;
   double ini, fim, delta;
   
   //verifica se o numero de threads foi passado na linha de comando
   if(argc<2) {
      printf("Digite: %s <numero de threads>\n", argv[0]); return 1;
   }
   nthreads = atoi(argv[1]);

   //insere os primeiros elementos na lista
   for(int i=0; i<QTDE_INI; i++)
      Insert(i%MAX_VALUE, &head_p);  /* Ignore return value */
   
   //aloca espaco de memoria para o vetor de identificadores de threads no sistema
   tid = malloc(sizeof(pthread_t)*nthreads);
   if(tid==NULL) {  
      printf("--ERRO: malloc()\n"); return 2;
   }

   //inicializa a variavel mutex
   pthread_mutex_init(&mutex, NULL);
   
   //tomada de tempo inicial
   GET_TIME(ini);
   //cria as threads
   for(long int i=0; i<nthreads; i++) {
      if(pthread_create(tid+i, NULL, tarefa, (void*) i)) {
         printf("--ERRO: pthread_create()\n"); return 3;
      }
   }
   
   //aguarda as threads terminarem
   for(int i=0; i<nthreads; i++) {
      if(pthread_join(*(tid+i), NULL)) {
         printf("--ERRO: pthread_join()\n"); return 4;
      }
   }

   //tomada de tempo final
   GET_TIME(fim);
   delta = fim-ini;
   printf("Tempo: %lf\n", delta);

   //libera o mutex
   pthread_mutex_destroy(&mutex);
   //libera o espaco de memoria do vetor de threads
   free(tid);
   //libera o espaco de memoria da lista
   Free_list(&head_p);

   return 0;
}  /* main */