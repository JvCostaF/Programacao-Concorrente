#include <stdio.h>
#include <limits.h>

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

int menorDistancia(Vertice *vertices, int qtdVertices) {
    int minimo = INT_MAX;
    int indVerticeMenorPeso;

    for (int v = 0; v < qtdVertices; v++) {
        if (!vertices[v].jaVisitado && vertices[v].distRaiz <= minimo) {
            minimo = vertices[v].distRaiz;
            indVerticeMenorPeso = v;
        }
    }

    return indVerticeMenorPeso;
}

// funcao executada pelas threads
void dijkstra(MatrizDeAdjacencias *grafo, Vertice *vertices, Vertice raiz, int verticeFinal){

    int qtdVertices = grafo->linhas;

    for (int i = 0; i < qtdVertices; i++) {
        if (i == raiz.id - 'A') {
            vertices[i] = raiz;
            continue;
        }
        vertices[i].distRaiz = INT_MAX;
        vertices[i].antecessor = '\0';
        vertices[i].jaVisitado = 0;
        vertices[i].id = 'A' + i;
    }

    // Executando o algoritmo de Dijkstra de forma concorrente

    

    // faz um monte de coisa
    

    if (vertices[verticeFinal].distRaiz) {
        printf("Nao ha caminho minimo entre os vertices fornecidos!\n");
        return;
    }

    // Reconstruindo o caminho minimo
    int caminho[qtdVertices];
    int indiceCaminho = 0;
    int v = verticeFinal;
    int pesoTotal = 0;
    while (v != raiz.id - 'A') {
        caminho[indiceCaminho] = v;
        indiceCaminho++;
        v = vertices[v].antecessor - 'A';
    }
    caminho[indiceCaminho++] = raiz.id - 'A';

    for (int j = 0; j < qtdVertices; j++) {
        if (vertices[j].id == vertices[verticeFinal].id) {
            pesoTotal = vertices[verticeFinal].distRaiz;
        }  
    }

    // Imprimindo o caminho minimo
    printf("-----Resultados da execucao do algoritmo-----\n");
    printf("Caminho Minimo: ");
    for (int i = indiceCaminho - 1; i >= 0; i--) {
        printf("%c ", 'A' + caminho[i]);
    }
    printf("\n");
    printf("Tamanho do caminho: %d\n", pesoTotal);
    
    

    

}

int main(int argc, char *argv[]){
    if (argc < 5) {
        fprintf(stderr, "Digite a dimensao do grafo, o arquivo de entrada, o indice do vertice raiz e o indice do vertice final.\n", argv[0]);
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

    FILE *file = fopen(argv[2], "r");
    if (file == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo.\n");
        return 2;
    }

    // Criando o grafo
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

    // Lendo os dados da matriz do arquivo
    for (int i = 0; i < grafo.tamanho; i++) {
        for (int j = 0; j < grafo.tamanho; j++) {
            fscanf(file, "%f", &grafo.dados[i * grafo.tamanho + j]);
        }   
    }

    fclose(file);

    printf("Matriz de Adjacencias:\n");
    for (int i = 0; i < grafo.tamanho; i++) {
        for (int j = 0; j < grafo.tamanho; j++) {
            printf("%.1f ", grafo.dados[i * grafo.tamanho + j]);
        }
        printf("\n");
    }

    // Criando o vetor de vértices
    Vertice *vertices = malloc(dimensao * sizeof(Vertice));
    if (vertices == NULL) {
        fprintf(stderr, "Erro ao alocar memória para os vértices.\n");
        return 1;
    }
    
    // Criando o vértice raiz
    Vertice raiz;
    raiz.id = 'A' + indiceRaiz; 
    raiz.distRaiz = 0; // Inicializa a distância da raiz como 0
    raiz.antecessor = '\0'; // Sem antecessor inicialmente
    raiz.jaVisitado = 0; // Marca como não visitado
    
    
    
    
    
}