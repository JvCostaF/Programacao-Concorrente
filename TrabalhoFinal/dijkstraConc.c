#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>

#define NUM_THREADS 5

typedef struct {
    int linhas;
    int colunas;
    int tamanho;
    float *dados;
} MatrizDeAdjacencias;

typedef struct {
    int distRaiz;
    int antecessor;
    int jaVisitado;
    int id;
} Vertice;

typedef struct {
    int minIndex;
    Vertice *vetices;
    MatrizDeAdjacencias *grafo;
} ThreadData;

pthread_t threads[NUM_THREADS];
pthread_mutex_t mutex;

void imprimeVertices(MatrizDeAdjacencias *grafo, Vertice *vertices) {
    int qtdVertices = grafo->linhas;
    for(int j = 0; j < qtdVertices; j++){
        printf("Vertice: %d\n", vertices[j].id);
        printf("Distancia para a Raiz: %d\n", vertices[j].distRaiz);
        printf("Antecessor: %d\n", vertices[j].antecessor);
        printf("\n");
    }
}

int menorDistancia(Vertice *vertices, int qtdVertices) {
    int minimo = INT_MAX;
    int indVerticeMenorPeso;
    
    for(int v = 0; v < qtdVertices ; v++) {
        if(!vertices[v].jaVisitado && vertices[v].distRaiz <= minimo) {
            minimo = vertices[v].distRaiz;
            indVerticeMenorPeso = v;
        }
    }

    return indVerticeMenorPeso;
}

void* atualizarDistancias(void *args) {
    ThreadData *data = (ThreadData*) args;
    int minIndex = data->minIndex;
    Vertice *vertices = data->vetices;
    MatrizDeAdjacencias *grafo = data->grafo;
    int qtdVertices = grafo->linhas;

    // Protegendo a atualização de distâncias com mutex
    pthread_mutex_lock(&mutex);

    // Atualiza as distâncias para os vizinhos do vértice atual
    for (int i = 0; i < qtdVertices; i++) {
        if (!vertices[i].jaVisitado && grafo->dados[minIndex * qtdVertices + i] != 0) {
            int nova_distancia = vertices[minIndex].distRaiz + grafo->dados[minIndex * qtdVertices + i];
            if (nova_distancia < vertices[i].distRaiz) {
                vertices[i].distRaiz = nova_distancia;
                vertices[i].antecessor = vertices[minIndex].id;
            }
        }
    }

    pthread_mutex_unlock(&mutex); // Liberando o mutex após a atualização

    return NULL;
}


void dijkstra(MatrizDeAdjacencias *grafo, Vertice *vertices, Vertice raiz, int verticeFinal) {
    int qtdVertices = grafo->linhas;
    
    // Inicialização dos vértices
    for(int i = 0; i < qtdVertices; i++){
        if(i == raiz.id){
            vertices[i] = raiz;
            continue;
        }
        vertices[i].distRaiz = INT_MAX;
        vertices[i].antecessor = -1;
        vertices[i].jaVisitado = 0;
        vertices[i].id = i;
    }

    // Execução do algoritmo Dijkstra
    for (int count = 0; count < qtdVertices - 1; count++) {
        int minIndex = menorDistancia(vertices, qtdVertices); 
        vertices[minIndex].jaVisitado = 1;

        ThreadData threadData;
        threadData.minIndex = minIndex;
        threadData.vetices = vertices;
        threadData.grafo = grafo;

        // Criar ou reutilizar uma thread do pool para processar
        pthread_create(&threads[count % NUM_THREADS], NULL, atualizarDistancias, &threadData);
        
        // Esperar todas as threads completarem antes de seguir para a próxima iteração
        if ((count + 1) % NUM_THREADS == 0 || count == qtdVertices - 2) {
            for (int i = 0; i < NUM_THREADS; i++) {
                pthread_join(threads[i], NULL);
            }
        }
    }

    // Reconstruir o caminho mínimo (como antes)
    int caminho[qtdVertices];
    int indiceCaminho = 0;
    int v = verticeFinal;
    int pesoTotal = 0;

    while (v != raiz.id) {
        caminho[indiceCaminho] = v;
        indiceCaminho++;
        v = vertices[v].antecessor;
    }
    caminho[indiceCaminho++] = raiz.id;

    for (int j = 0; j < qtdVertices; j++) {
        if (vertices[j].id == vertices[verticeFinal].id) {
            pesoTotal = vertices[verticeFinal].distRaiz;
        }
    }

    printf("-----Resultados da execução do algoritmo-----\n");
    printf("Caminho Mínimo: ");
    for (int i = indiceCaminho - 1; i >= 0; i--) {
        printf("%d ", caminho[i]);
    }
    printf("\n");
    printf("Tamanho do caminho: %d\n", pesoTotal);
}

int main(int argc, char* argv[]) {
    if (argc < 5) {
        fprintf(stderr, "Digite a dimensao do grafo, o arquivo de entrada, o indice do vertice raiz e o indice do vertice final.\n", argv[0]);
        return 1;
    }

    int dimensao = atoi(argv[1]);
    int indiceRaiz = atoi(argv[3]);
    int indiceFinal = atoi(argv[4]);

    if (indiceRaiz < 0 || indiceRaiz >= dimensao) {
        fprintf(stderr, "Indice do vertice raiz invalido!\n");
    }

    if (indiceFinal < 0 || indiceFinal >= dimensao) {
        fprintf(stderr, "Indice do vertice final invalido!\n");
    }

    FILE *file = fopen(argv[2], "r");
    if (file == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo.\n");
        return 2;
    }

    MatrizDeAdjacencias grafo;
    grafo.linhas = dimensao;
    grafo.colunas = dimensao;
    grafo.tamanho = dimensao;
    grafo.dados = (float *)malloc(dimensao * dimensao * sizeof(float));

    if (grafo.dados == NULL) {
        fprintf(stderr, "Erro ao alocar memória para a matriz.\n");
        fclose(file);
        return 1;
    }

    for (int i = 0; i < grafo.tamanho; i++) {
        for (int j = 0; j < grafo.tamanho; j++) {
            fscanf(file, "%f", &grafo.dados[i * grafo.tamanho + j]);
        }
    }

    fclose(file);

    Vertice *vertices = malloc(dimensao * sizeof(Vertice));
    if (vertices == NULL) {
        fprintf(stderr, "Erro ao alocar memória para os vértices.\n");
        return 1;
    }

    Vertice raiz;
    raiz.id = indiceRaiz;
    raiz.distRaiz = 0;
    raiz.antecessor = '\0';
    raiz.jaVisitado = 0;

    pthread_mutex_init(&mutex, NULL);

    dijkstra(&grafo, vertices, raiz, indiceFinal);

    free(grafo.dados);
    free(vertices);
    pthread_mutex_destroy(&mutex);

    return 0;
}
