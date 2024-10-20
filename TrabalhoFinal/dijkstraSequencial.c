#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct {
    int qtdLinhas, qtdColunas;
    int dimensão;
    float *dados;
} MatrizDeAdjacencias;

typedef struct {
    int distanciaRaiz;
    int jaVisitado;
    char verticeAnterior;
} Vertice;

/*
Funcao com o objetivo de decidir qual o proximo vertice sera processado no algoritmo. Esse vértice é 
aquele que tem a menor distância conhecida em relação ao vértice inicial e ainda não foi visitado.
*/
int distanciaMinima(Vertice *vertices, int qtdVertices) {

    int minimo = INT_MAX, minIndex; // Iniciando a variavel minimo com o maior valor possivel.
                                    // minIndex armazena o indece do vertice com a menor distancia da raiz.
    
    for(int v = 0; v < qtdVertices ; v++) {
        if(!vertices[v].jaVisitado && vertices[v].distanciaRaiz <= minimo) {
            minimo = vertices[v].distanciaRaiz;
            minIndex = v;
        }
    }

    return minIndex;
}

void dijkstra(MatrizDeAdjacencias *matriz, int raiz) {
    
    int qtdVertices = matriz->dimensão;

    Vertice *vertices = malloc(qtdVertices*sizeof(Vertice));

    // Precisamos iniciar todos os vertices seguindo as necessidades de Dijkstra.
    for(int i = 0; i < qtdVertices; i++) {
        vertices[i].distanciaRaiz = INT_MAX; // Infinito.
        vertices[i].jaVisitado = 0; // De inicio, nenhum vertice foi visitado ainda.
        vertices[i].verticeAnterior = -1; // De inicio, nao temos vertice anterior.
    }

    // O vertice raiz precisa ter a distancia para ele mesmo igual a 0.
    vertices[raiz].distanciaRaiz = 0;

    for(int c = 0; c < qtdVertices-1; c++) {
        int verticeEscolhido = distanciaMinima(vertices, qtdVertices); // Vamos escolher o vertice com a menor distancia para a raiz.
        vertices[verticeEscolhido].jaVisitado = 1; // Apos escolher o vertice, marcamos ele como visitado.

        // Agora, vamos atualizar as distancias com base no vertice escolhido.
        for(int u = 0; u < qtdVertices; u++){
            if(!vertices[verticeEscolhido].jaVisitado && matriz->dados[verticeEscolhido*qtdVertices + u] > 0 // Se o vertive nao estiver sido visitado e exista uma aresta entre o verticeEscolhido e o vertice u.
            && vertices[verticeEscolhido].distanciaRaiz != INT_MAX // Se a distancia do verticeEscolhido para a raiz for diferente de infinito.
            && vertices[verticeEscolhido].distanciaRaiz + matriz->dados[verticeEscolhido * qtdVertices + u] < vertices[u].distanciaRaiz) // Se a distancia da raiz mais o peso da aresta for menor que a distancia atual.
            { // Atualiza a distancia do vertice e o vertice anterios.
                vertices[u].distanciaRaiz = vertices[verticeEscolhido].distanciaRaiz + matriz->dados[verticeEscolhido*qtdVertices + u];
                vertices[u].verticeAnterior = verticeEscolhido;
            }
        }
    }

    // Imprimindo as distancias finais.
    // printf("Vertice\t\tDistancia da Raiz\n");
    // for(int i = 0; i < qtdVertices; i++) {
    //     printf("%d\t\t%d\n", i, vertices[i].distanciaRaiz);
    // }

    free(vertices);
}

int main() {
    // Implementar main.
}