# **Observações**

O arquivo binário de entrada vai conter 4 inteiros. Resolvi fazer dessa forma pois as posições representam _**qtdLinhasMatriz1 qtdColunasMatriz1 qtdLinhasMatriz2 qtdColunasMatriz2**_ para que o programa aceite matrizes com dimensões diferentes, ou seja, sem ser apenas matrizes quadradas.

# Sobre os arquivos do repositório

- **buildBinFile.c**: Programa com a finalidade de gerar um _arquivo.bin_ contendo as dimensões e as duas matrizes.  
- **readBinFile.c**: Programa com a finalidade de ler um _arquivo.bin_ e printar no console as informações desse _.bin_, mais para minha validação durante a implementação do Lab. Não deve ser levado em consideração do que foi pedido.
- **program.c**: Programa que lê um _arquivo.bin_, calcula o produto entre as duas matrizes de maneira sequêncial e entrega outro _arquivo.bin_ com o mesmo formato do de entrada, mas com o acréscimo da matriz resultante da multiplicação.
- **concorrentProgram**: Programa que lê um _arquivo.bin_, calcula o produto entre as duas matrizes usando **CONCORRÊNCIA** e entrega outro _arquivo.bin_ com o mesmo formato do de entrada, mas com o acréscimo da matriz resultante da multiplicação.

# Como executar os programas

1) Compilar os programas com o gcc ou outro compilador C

    `gcc -o nome_do_executavel programa.c -Wall`


2) Para o **buildBinFile.c**: 

    `./nome_do_executavel qtdLinhasMatriz1 qtdColunasMatriz1 qtdLinhasMatriz2 qtdColunasMatriz2 arquivo_de_saida.bin`

3) Para o **program.c**:

    `./nome_do_executavel arquivo_de_entrada.bin`

4) Para o **concorrentProgram.c**:

    `./nome_do_executavel qtdThreads arquivo_de_entrada.bin`