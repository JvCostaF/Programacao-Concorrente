#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>

typedef struct {
    int dimensao;
    float *dados;
} MatrizDeAdjacencias;

typedef struct {
    int distRaiz;
    int antecessor;
    int jaVisitado;
    int id;
} Vertice;

typedef struct {
    int thread_id;
    int num_threads;
    MatrizDeAdjacencias *grafo;
    Vertice *vertices;
    int minIndex;
    pthread_mutex_t *mutex;
    pthread_barrier_t *barreira;
} ThreadData;

// Função para encontrar o menor índice da distância (realizada por uma thread principal)
int menorDistancia(Vertice *vertices, int qtdVertices) {
    int minimo = INT_MAX;
    int indVerticeMenorPeso = -1;

    for (int v = 0; v < qtdVertices; v++) {
        if (!vertices[v].jaVisitado && vertices[v].distRaiz < minimo) {
            minimo = vertices[v].distRaiz;
            indVerticeMenorPeso = v;
        }
    }

    return indVerticeMenorPeso;
}

// Função executada por cada thread
void *atualizarDistancias(void *arg) {
    ThreadData *data = (ThreadData *)arg;

    int qtdVertices = data->grafo->dimensao;
    int start = (qtdVertices / data->num_threads) * data->thread_id;
    int end = (data->thread_id == data->num_threads - 1) ? qtdVertices : start + (qtdVertices / data->num_threads);

    while (1) {
        // Esperar pela thread principal selecionar o próximo vértice
        pthread_barrier_wait(data->barreira);

        if (data->minIndex == -1) break; // Nenhum vértice restante

        // Atualizar os vizinhos
        for (int i = start; i < end; i++) {
            if (!data->vertices[i].jaVisitado && data->grafo->dados[data->minIndex * qtdVertices + i] != 0) {
                int nova_distancia = data->vertices[data->minIndex].distRaiz +
                                     data->grafo->dados[data->minIndex * qtdVertices + i];

                pthread_mutex_lock(data->mutex);
                if (nova_distancia < data->vertices[i].distRaiz) {
                    data->vertices[i].distRaiz = nova_distancia;
                    data->vertices[i].antecessor = data->minIndex;
                }
                pthread_mutex_unlock(data->mutex);
            }
        }

        // Esperar todas as threads terminarem antes de avançar
        pthread_barrier_wait(data->barreira);
    }

    return NULL;
}

void dijkstra(MatrizDeAdjacencias *grafo, Vertice *vertices, Vertice raiz, int verticeFinal, int num_threads) {
    int qtdVertices = grafo->dimensao;

    // Inicialização dos vértices
    for (int i = 0; i < qtdVertices; i++) {
        if (i == raiz.id) {
            vertices[i] = raiz;
            continue;
        }
        vertices[i].distRaiz = INT_MAX;
        vertices[i].antecessor = -1;
        vertices[i].jaVisitado = 0;
        vertices[i].id = i;
    }

    pthread_t threads[num_threads];
    ThreadData thread_data[num_threads];
    pthread_mutex_t mutex; //Mutex unico para garantir thread safe nas operacoes de atualizacao de valores de distancias.
    pthread_barrier_t barreira;

    pthread_mutex_init(&mutex, NULL);
    pthread_barrier_init(&barreira, NULL, num_threads + 1); //A quantidade de threads mais a thread main

    // Criar threads
    for (int t = 0; t < num_threads; t++) {
        thread_data[t].thread_id = t;
        thread_data[t].num_threads = num_threads;
        thread_data[t].grafo = grafo;
        thread_data[t].vertices = vertices;
        thread_data[t].minIndex = -1;
        thread_data[t].mutex = &mutex;
        thread_data[t].barreira = &barreira;
        pthread_create(&threads[t], NULL, atualizarDistancias, &thread_data[t]);
    }

    // Executar o algoritmo de Dijkstra
    for (int count = 0; count < qtdVertices - 1; count++) {
        int minIndex = menorDistancia(vertices, qtdVertices);
        if (minIndex == -1) break; // Grafo desconexo

        vertices[minIndex].jaVisitado = 1;

        // Atualizar o índice mínimo para as threads
        for (int t = 0; t < num_threads; t++) {
            thread_data[t].minIndex = minIndex;
        }

        // Permitir que as threads processem
        pthread_barrier_wait(&barreira);

        // Esperar as threads finalizarem
        pthread_barrier_wait(&barreira);
    }

    // Finalizar as threads
    for (int t = 0; t < num_threads; t++) {
        thread_data[t].minIndex = -1; // Sinalizar término
    }
    pthread_barrier_wait(&barreira); // Permitir que as threads saiam

    for (int t = 0; t < num_threads; t++) {
        pthread_join(threads[t], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_barrier_destroy(&barreira);

    // Reconstrução do caminho mínimo
    if (vertices[verticeFinal].distRaiz == INT_MAX) {
        printf("Nao ha caminho minimo entre os vertices fornecidos!\n");
        return;
    }

    int caminho[qtdVertices];
    int indiceCaminho = 0;
    int v = verticeFinal;

    while (v != raiz.id) {
        caminho[indiceCaminho++] = v;
        v = vertices[v].antecessor;
    }
    caminho[indiceCaminho++] = raiz.id;

    printf("-----Resultados da execucao do algoritmo-----\n");
    printf("Caminho Minimo: ");
    for (int i = indiceCaminho - 1; i >= 0; i--) {
        printf("%d ", caminho[i]);
    }
    printf("\n");
    printf("Tamanho do caminho: %d\n", vertices[verticeFinal].distRaiz);
}

int main(int argc, char *argv[]) {
    if (argc < 6) {
        fprintf(stderr, "Digite a dimensao do grafo, o arquivo de entrada, o indice do vertice raiz, o indice do vertice final e o numero de threads.\n");
        return 1;
    }

    int dimensao = atoi(argv[1]);
    int indiceRaiz = atoi(argv[3]);
    int indiceFinal = atoi(argv[4]);
    int num_threads = atoi(argv[5]);

    FILE *file = fopen(argv[2], "rb");
    if (file == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo.\n");
        return 2;
    }

    MatrizDeAdjacencias grafo;
    grafo.dimensao = dimensao;
    grafo.dados = (float *)malloc(dimensao * dimensao * sizeof(float));

    // Lendo os dados da matriz do arquivo binário
    fread(grafo.dados, sizeof(float), dimensao * dimensao, file);

    fclose(file);

    Vertice *vertices = malloc(dimensao * sizeof(Vertice));
    Vertice raiz = { .id = indiceRaiz, .distRaiz = 0, .antecessor = -1, .jaVisitado = 0 };

    dijkstra(&grafo, vertices, raiz, indiceFinal, num_threads);

    free(grafo.dados);
    free(vertices);

    return 0;
}
