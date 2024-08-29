#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    float *matriz;      // matriz que será carregada do arquivo
    int linhas, colunas; // dimensões da matriz
    long long int tam;  // qtde de elementos na matriz
    FILE *descritorArquivo; // descritor do arquivo de entrada
    size_t ret;         // retorno da função de leitura no arquivo de entrada

    // recebe os argumentos de entrada
    if (argc < 2) {
        fprintf(stderr, "Digite: %s <arquivo entrada>\n", argv[0]);
        return 1;
    }

    // abre o arquivo para leitura binária
    descritorArquivo = fopen(argv[1], "rb");
    if (!descritorArquivo) {
        fprintf(stderr, "Erro de abertura do arquivo\n");
        return 2;
    }

    // loop para ler todas as matrizes no arquivo
    while (1) {
        // lê as dimensões da matriz
        ret = fread(&linhas, sizeof(int), 1, descritorArquivo);
        if (ret != 1) {
            // Verifica se atingiu o fim do arquivo
            if (feof(descritorArquivo)) {
                break;
            } else {
                fprintf(stderr, "Erro de leitura das dimensões da matriz no arquivo\n");
                return 3;
            }
        }
        ret = fread(&colunas, sizeof(int), 1, descritorArquivo);
        if (ret != 1) {
            fprintf(stderr, "Erro de leitura das dimensões da matriz no arquivo\n");
            return 3;
        }

        tam = linhas * colunas; // calcula a qtde de elementos da matriz

        // aloca memória para a matriz
        matriz = (float *)malloc(sizeof(float) * tam);
        if (!matriz) {
            fprintf(stderr, "Erro de alocação da memória da matriz\n"); //TODO : Algum problema para ler a segunda matriz!
            return 3;
        }

        // carrega a matriz de elementos do tipo float do arquivo
        ret = fread(matriz, sizeof(float), tam, descritorArquivo);
        if (ret < tam) {
            fprintf(stderr, "Erro de leitura dos elementos da matriz\n");
            free(matriz);
            return 4;
        }

        // imprime a matriz na saída padrão
        printf("Matriz %dx%d:\n", linhas, colunas);
        for (int i = 0; i < linhas; i++) {
            for (int j = 0; j < colunas; j++)
                fprintf(stdout, "%.6f ", matriz[i * colunas + j]);
            fprintf(stdout, "\n");
        }
        fprintf(stdout, "\n");

        // libera a memória da matriz para a próxima iteração
        free(matriz);
    }

    // finaliza o uso das variáveis
    fclose(descritorArquivo);
    return 0;
}
