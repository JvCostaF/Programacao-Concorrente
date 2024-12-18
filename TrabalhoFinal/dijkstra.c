#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "timer.h"

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

void imprimeVertices(MatrizDeAdjacencias *grafo, Vertice *vertices) {
    int qtdVertices = grafo->dimensao;
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
        if(!vertices[v].jaVisitado && vertices[v].distRaiz <= minimo) { // O(n)
            minimo = vertices[v].distRaiz;
            indVerticeMenorPeso = v;
        }
    }

    return indVerticeMenorPeso;
}


void dijkstra(MatrizDeAdjacencias *grafo, Vertice *vertices, Vertice raiz, int verticeFinal) 
{

    int qtdVertices = grafo->dimensao;
    
    //Inicialmente, todos os vertices precisam ter distRaiz = INFINITO, antecessor = NULL, jaVisitado = 0 e o seu proprio id
    for(int i = 0; i < qtdVertices; i++){
        if(i == raiz.id){
            vertices[i] = raiz;
            continue;
        }
        vertices[i].distRaiz = INT_MAX; //Define como o maior valor inteiro possivel
        vertices[i].antecessor = '\0';
        vertices[i].jaVisitado = 0;
        vertices[i].id = i;
    }

    //imprimeVertices(grafo, vertices);

    // Executa o algoritmo de Dijkstra
    for (int count = 0; count < qtdVertices - 1; count++) {
        // Chama a função para obter o índice do vértice com a menor distância
        int minIndex = menorDistancia(vertices, qtdVertices);
        
        // Marca o vértice como visitado
        vertices[minIndex].jaVisitado = 1;

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
    }

    // Caso seja um grafo desconexo
    // Se a gente definir uma boa probabilidade na hora de gerar os grafos com o gerador teremmos poucos casos de grafos desconexos, mas vale testar mesmo!
    if (vertices[verticeFinal].distRaiz==INT_MAX) {
            printf("Nao ha caminho minimo entre os vertices fornecidos!\n");
            return;
    }

    // Reconstruindo o caminho mínimo
    int caminho[qtdVertices];
    int indiceCaminho = 0;
    int v = verticeFinal;

    while (v != raiz.id) {
        caminho[indiceCaminho++] = v;
        v = vertices[v].antecessor;
    }
    caminho[indiceCaminho++] = raiz.id;

    //imprimeVertices(grafo, vertices);

    // Imprimindo o caminho mínimo
    printf("-----Resultados da execucao do algoritmo-----\n");
    printf("Caminho Minimo: ");
    for (int i = indiceCaminho - 1; i >= 0; i--) {
        printf("%d ", caminho[i]);
    }
    printf("\n");
    printf("Tamanho do caminho: %d\n", vertices[verticeFinal].distRaiz);

}

int main(int argc, char* argv[]) {

    double time_inicio, time_fim, time_delta;

    // Nome do arquivo CSV para armazenar os resultados
    const char *csvFilename = "resultados.csv";
    FILE *csvFile;

    GET_TIME(time_inicio);

    if (argc < 5) {
        fprintf(stderr, "Digite a dimensao do grafo, o arquivo de entrada, o indice do vertice raiz e o indice do vertice final.\n");
        return 1;
    }

    int dimensao = atoi(argv[1]);
    int indiceRaiz = atoi(argv[3]);
    int indiceFinal = atoi(argv[4]);

    if (indiceRaiz<0 || indiceRaiz>=dimensao) {
        fprintf(stderr, "Indice do vertice raiz invalido!\n");
    }

    if (indiceFinal<0 || indiceFinal>=dimensao) {
        fprintf(stderr, "Indice do vertice final invalido!\n");
    }
    

    FILE *file = fopen(argv[2], "rb");
    if (file == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo.\n");
        return 2;
    }

    //Criando o grafo 
    MatrizDeAdjacencias grafo;
    grafo.dimensao = dimensao;
    grafo.dados = (float *)malloc(dimensao * dimensao * sizeof(float));

    if (grafo.dados == NULL) {
        fprintf(stderr, "Erro ao alocar memória para a matriz.\n");
        fclose(file);
        return 1;
    }

    //Lendo os dados da matriz do arquivo
    fread(grafo.dados, sizeof(float), dimensao * dimensao, file);

    fclose(file);

    // printf("Matriz de Adjacencias:\n");
    // for (int i = 0; i < grafo.dimensao; i++) {
    //     for (int j = 0; j < grafo.dimensao; j++) {
    //         printf("%.1f ", grafo.dados[i * grafo.dimensao + j]);
    //     }
    //     printf("\n");
    // }

    // Criando o vetor de vértices
    Vertice *vertices = malloc(dimensao * sizeof(Vertice));
    if (vertices == NULL) {
        fprintf(stderr, "Erro ao alocar memória para os vértices.\n");
        return 1;
    }
    
    //Criando o vértice raiz
    Vertice raiz;
    raiz.id = indiceRaiz; 
    raiz.distRaiz = 0; // Inicializa a distância da raiz como 0
    raiz.antecessor = '\0'; // Sem antecessor inicialmente
    raiz.jaVisitado = 0; // Marca como não visitado

    dijkstra(&grafo, vertices, raiz, indiceFinal);

    free(grafo.dados);
    free(vertices);

    GET_TIME(time_fim);
    time_delta = time_fim - time_inicio;
    printf("Tempo total sequencial:%lf\n", time_delta);

    // Abre o arquivo CSV para adicionar resultados
    csvFile = fopen(csvFilename, "a");
    if (csvFile == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo CSV.\n");
        return 3;
    }

    // Se o arquivo CSV estiver vazio, escreve o cabeçalho
    if (ftell(csvFile) == 0) {
        fprintf(csvFile, "Forma, Dimensao, NumThreads, TempoTotal\n");
    }

    // Grava os resultados no arquivo CSV
    fprintf(csvFile, "%s, %d, %d, %lf\n", "Sequencial", dimensao, 0, time_delta);

    fclose(csvFile);

    return 0;
}