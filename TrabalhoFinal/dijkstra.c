#include <stdio.h>
#include <stdlib.h>
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
    
    for(int v = 0; v < qtdVertices ; v++) {
        if(!vertices[v].jaVisitado && vertices[v].distRaiz <= minimo) {
            minimo = vertices[v].distRaiz;
            indVerticeMenorPeso = v;
        }
    }

    return indVerticeMenorPeso;
}

void dijkstra(MatrizDeAdjacencias *grafo, Vertice raiz) {

    int qtdVertices = grafo->linhas;
    Vertice *vertices = malloc(qtdVertices * sizeof(Vertice));
    
    //Inicialmente, todos os vertices precisam ter distRaiz = INFINITO, antecessor = NULL, jaVisitado = 0 e o seu proprio id
    for(int i = 0; i < qtdVertices; i++){
        if(i == 0){
            vertices[i] = raiz;
            continue;
        }
        vertices[i].distRaiz = INT_MAX; //Define como o maior valor inteiro possivel
        vertices[i].antecessor = '\0';
        vertices[i].jaVisitado = 0;
        vertices[i].id = 'A' + i;
    }

    //Para validar os valores iniciais
    printf("Antes de rodar o algoritmo!\n");
    for(int j = 0; j < qtdVertices; j++){
        printf("Vertice: %c\n", vertices[j].id);
        printf("Distancia para a Raiz: %d\n", vertices[j].distRaiz);
        printf("Antecessor: %c\n", vertices[j].antecessor == '\0' ? ' ' : vertices[j].antecessor);
        printf("\n");
    }

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

    printf("Depois de rodar o algoritmo!\n");
    // Imprime o estado final dos vértices
    for (int j = 0; j < qtdVertices; j++) {
        printf("Vertice: %c\n", vertices[j].id);
        printf("Distancia da raiz: %d\n", vertices[j].distRaiz);
        printf("Antecessor: %c\n", vertices[j].antecessor == '\0' ? ' ' : vertices[j].antecessor);
        printf("\n");
    }

    free(vertices);

}


int main(int argc, char* argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Digite a dimensao do grafo e o arquivo de entrada\n", argv[0]);
        return 1;
    }

    int dimensao = atoi(argv[1]);

    FILE *file = fopen(argv[2], "r");
    if (file == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo.\n");
        return 2;
    }

    //Criando o grafo 
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

    //Lendo os dados da matriz do arquivo
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
    
    //Criando o vértice raiz
    Vertice raiz;
    raiz.id = 'A'; 
    raiz.distRaiz = 0; // Inicializa a distância da raiz como 0
    raiz.antecessor = '\0'; // Sem antecessor inicialmente
    raiz.jaVisitado = 0; // Marca como não visitado

    dijkstra(&grafo, raiz);

    free(grafo.dados);

    return 0;
}