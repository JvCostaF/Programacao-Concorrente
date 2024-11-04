import numpy as np
import networkx as nx
import matplotlib.pyplot as plt

def gerar_grafo(n, p, peso_maximo=10):
    # Gera um grafo aleatório G(n, p)
    G = nx.gnp_random_graph(n, p)
    
    # Cria uma matriz de adjacência com pesos
    matriz_adjacencia = np.zeros((n, n), dtype=int)
    
    for (u, v) in G.edges():
        peso = np.random.randint(1, peso_maximo + 1)  # Peso aleatório entre 1 e peso_maximo
        matriz_adjacencia[u][v] = peso
        matriz_adjacencia[v][u] = peso  # Para grafo não direcionado
    
    return G, matriz_adjacencia

def salvar_matriz_em_txt(matriz, nome_arquivo):
    np.savetxt(nome_arquivo, matriz, fmt='%d')

# Parâmetros do grafo
n = 10 # Número de vértices
p = 0.5  # Probabilidade de arestas

# Gera o grafo e a matriz de adjacência
G, matriz = gerar_grafo(n, p)

# Salva a matriz em um arquivo .txt
salvar_matriz_em_txt(matriz, 'grafoPequenoTeste.txt')

# Printa o grafo usando o pypltu
nx.draw(G, with_labels=True, node_color='lightblue', edge_color='gray')
plt.show()

# Print the adjacency matrix in the desired format
for row in matriz:
    print('  '.join(map(str, row))) # Convert each element to string and join with double spaces
