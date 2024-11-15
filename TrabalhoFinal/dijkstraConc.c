#include <stdio.h>

typedef struct {
    int linhas;
    int colunas;
    int tamanho;
    float *dados;
} MatrizDeAdjacencias;

typedef struct {
    int distRaiz;
    char antecessor;
    int jaVisitado;
    char id;
} Vertice;

