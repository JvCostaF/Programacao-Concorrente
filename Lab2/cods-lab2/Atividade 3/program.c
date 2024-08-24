/* Disciplina: Programacao Concorrente */
/* Profa.: Silvana Rossetto */
/* Aluno: Joao Victor da Costa Farias - 120014876 */
/* Laboratório: 2 - Atividade 3 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define MAX 10000

float* criaVetor(int tamanho) {

    float* array = (float*)malloc(tamanho * sizeof(float));

    if(array == NULL) {
        printf("Problemas com a alocação de memória");
        exit(1);
    }

    for(long int i = 0; i < tamanho; i++) {
        array[i] = ( rand() % MAX ) / 5.0;
    }

    return array;
}

float calculaProdutoInterno(float* A, float* B, int tamanho) {

    float soma = 0.0;

    for(long int i = 0; i < tamanho; i++) {
        soma += A[i] * B[i];
    }

    return soma;
}

int main(int argc, char*argv[]){

    long int n;

    FILE * descritorArquivo; //descritor do arquivo de saida
    size_t ret; //retorno da funcao de escrita no arquivo de saida

    if(argc < 3){
      fprintf(stderr, "Digite: %s <dimensao> <arquivo saida>\n", argv[0]);
      return 1;
    }

    n = atoi(argv[1]);

    float* vetor1 = criaVetor(n);

    float* vetor2 = criaVetor(n);

    // for(int i = 0; i < n; i++){
    //     printf("%f ", vetor1[i]);
    // }

    // printf("\n");

    //     for(int i = 0; i < n; i++){
    //     printf("%f ", vetor2[i]);
    // }

    // printf("\n");

    float produtoInterno = calculaProdutoInterno(vetor1, vetor2, n);

    // printf("%f\n", produtoInterno);

    //escreve o vetor no arquivo
    //abre o arquivo para escrita binaria
    descritorArquivo = fopen(argv[2], "wb");
    if(!descritorArquivo) {
        fprintf(stderr, "Erro de abertura do arquivo\n");
        free(vetor1);
        free(vetor2);
        return 3;
    }

    // Escrever a dimensão
    ret = fwrite(&n, sizeof(int), 1, descritorArquivo);

    // Escrever os elementos do vetor1
    ret = fwrite(vetor1, sizeof(float), n, descritorArquivo);

    // Escrever os elementos do vetor2
    ret = fwrite(vetor2, sizeof(float), n, descritorArquivo);

    // Escrever o produto interno
    ret = fwrite(&produtoInterno, sizeof(float), 1, descritorArquivo);

    //finaliza o uso das variaveis
    fclose(descritorArquivo);
    free(vetor1);
    free(vetor2);
    return 0;

}