# Trabalho Final - Programação Concorrete
Alunos: 
- João Víctor da Costa Farias - 120014876
- Thalita Freitas Silva - 

## Proposta do Trabalho
Implementar o algoritmo de **Dijkstra** com uso de concorrência na linguagem C. 

### O Algoritmo de Dijkstra
O algoritmo de Dijkstra é utilizado para encontrar o caminho mínimo de um vértice origem e todos os outros vértices em um grafo ponderado, produzindo uma árvore de custo mínimo.

Por sua natureza o algoritmo é sequencial, e ele funciona da seguinte maneira:

- O algoritmo é iniciado em um vértice (source), definindo a distância dele como 0 e a distância para todos os outros como infinito;
- Para cada vértice visitado, o algoritmo atualiza as distância dos seus vizinhos. De maneira que ele mantenha o registo da distância mais curta atualmente conhecida de um vértic;
- Uma vez que o menor caminho entre o source e outro vértice, esse vértice é marcado como visitado e adicionado no caminho (na árvore);
- O processo se repete até que todos os vértices tenham sido visitados.

Para funcionar, o algoritmo de Dijkstra possue alguns requisitos:
- Não podem existir arestas com peso negativo no grafo;

A complexidade do algoritmo de Dijkstra é O(|V| + |E|) * log |V|, onde V é a quantidade de vértices e E a quantidade de arestas.