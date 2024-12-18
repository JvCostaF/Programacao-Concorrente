# Trabalho Final - Programação Concorrete
Alunos: 
- João Víctor da Costa Farias - 120014876
- Thalita de Freitas Silva - 120048493

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

### O Gerador de Grafos
Um dos nossos problemas era encontrar um grafo, relativamente grande, para realizar os testes e coletar as métricas que vão nos permitir validar se houve ganho de performance ou não.

Pensando nisso, implementamos um programa em **Python** que gera grafos randomizados utilizando as libs Numpy e NetworkX. Estamos utilizando **fast_gnp_random_graph(n, p)**, onde passamos o número fixo de vértices **n** e um valor de probabilidade **p**, esse valor representa a probabilidade de existir uma aresta entre dois vértices do grafo gerado. Além disso, atribuímos pesos nas arestas de maneira que esses pesos não sejam negativos, pois isso faria o algoritmo falhar. E salvamos a matriz de adjacências em um arquivo (esse arquivo será utilizado como parâmetro de entrada nos programas: sequencia e concorrente). 

Para rodar o programa localmente é necessário:
- Ter o Python instalado e configurado na máquina;
- Ter o Pip instalado e configurado na máquina; 
- Ter as libs Numpy e NetworkX instaladas via pip;

O download do python pode ser feito na página oficial da linguagem.

Para instalar o pip, salve esse link: https://bootstrap.pypa.io/get-pip.py 

Após isso, vá até o diretório onde está salvo o link, clique com o botão direito no mesmo e copie o seu path (caminho).

Por fim, no terminal, execute:
```
python <caminho_do_link>
```

Para instalar as libs basta executar no terminal:
```
pip install numpy
pip install networkx
```

### O Algoritmo Sequencial
O arquivo **dijkstra.c** é o nosso algoritmo sequencial. Para executar voce deve fazer a compilacao do codigo (Recomendo fortemente utilizar o gcc), para funcionar o codigo espera que sejam passados: A dimensao da matriz de adjacencia, o arquivo que contenha a matriz (Um arquivo binario ou texto), o vertice raiz e o vertice destino. 

Nosso codigo foi feito para encontrar o caminho minimo da raiz para qualquer vertice, mas pedimos que seja passado um vertice de destino para facilitar a impressao dos resultados do algoritmo pelo terminal.

Para compilar:
```
gcc dijkstra.c
```

Para executar:
```
.\<nome do executavel> <dimensao> <arquivo de entrada> <raiz> <destino>
.\a 10 matriz10x10.bin 0 6 (exemplo)
```

### O Algoritmo Concorrente
O arquivo **dijkstraConc.c** é o nosso algoritmo concorrente. Para executar voce deve fazer a compilacao do codigo (Recomendo fortemente utilizar o gcc), para funcionar o codigo espera que sejam passados: A dimensao da matriz de adjacencia, o arquivo que contenha a matriz (Um arquivo binario ou texto), o vertice raiz, o vertice destino e a quantidade de threads. 

Nosso codigo foi feito para encontrar o caminho minimo da raiz para qualquer vertice, mas pedimos que seja passado um vertice de destino para facilitar a impressao dos resultados do algoritmo pelo terminal.

Para compilar:
```
gcc dijkstraConc.c
```

Para executar:
```
.\<nome do executavel> <dimensao> <arquivo de entrada> <raiz> <destino> <qtd de threads>
.\a 10 matriz10x10.bin 0 6 2(exemplo)
```
