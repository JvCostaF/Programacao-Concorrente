/* Disciplina: Programacao Concorrente */
/* Profa.: Silvana Rossetto */
/* Aluno: Joao Victor da Costa Farias - 120014876 */
/* Laboratório: 2 - Atividade 3 */
/* 
    Código que recebe um arquivo binário com a dimensão, dois vetores de N elementos e o produto interno desses vetores.
    Após ler os dados do arquivo binário o produto interno é calculado novamente, dessa vez com concorrência. Onde ao final
    o valor do produto intenor do arquivo e o produto interno calculado com concorrência são printados no terminal.

    OBS: A doc string dos métodos foi gerada com uso do ChatGPT.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> 

int n, nThreads;
float *vetor1, *vetor2;
float produtoInternoArquivo;

/**
 * @brief Lê dados binários de um arquivo e armazena em variáveis fornecidas.
 *
 * Esta função lê a dimensão de um vetor, dois vetores de números reais e um valor de produto interno
 * de um arquivo binário. A memória para os vetores é alocada dinamicamente com base na dimensão lida.
 *
 * @param arquivo Ponteiro para o arquivo binário a ser lido. O arquivo deve estar previamente aberto.
 * @param n Ponteiro para um inteiro onde a dimensão do vetor será armazenada.
 * @param vetor1 Ponteiro para um ponteiro de float onde o primeiro vetor será armazenado. A memória
 *               é alocada dentro da função.
 * @param vetor2 Ponteiro para um ponteiro de float onde o segundo vetor será armazenado. A memória
 *               é alocada dentro da função.
 * @param produtoInterno Ponteiro para um float onde o valor do produto interno será armazenado.
 *
 * @note Esta função encerra o programa com `exit(1)` em caso de erro ao ler os dados ou ao alocar memória.
 *
 * @warning Certifique-se de liberar a memória alocada para `vetor1` e `vetor2` após o uso para evitar vazamentos de memória.
 */
void leDadosBinario(FILE *arquivo, int *n, float **vetor1, float **vetor2, float *produtoInterno) {

    size_t ret = fread(n, sizeof(int), 1, arquivo);
    if (ret != 1) {
        fprintf(stderr, "Erro ao ler a dimensão do arquivo\n");
        exit(1);
    }

    // Alocar memória para os vetores com base na dimensão lida
    *vetor1 = (float*)malloc(*n * sizeof(float));
    *vetor2 = (float*)malloc(*n * sizeof(float));
    if (*vetor1 == NULL || *vetor2 == NULL) {
        fprintf(stderr, "Erro na alocação de memória\n");
        exit(1);
    }

    // Ler os elementos do vetor1
    ret = fread(*vetor1, sizeof(float), *n, arquivo);
    if (ret != *n) {
        fprintf(stderr, "Erro ao ler o vetor1 do arquivo\n");
        exit(1);
    }

    // Ler os elementos do vetor2
    ret = fread(*vetor2, sizeof(float), *n, arquivo);
    if (ret != *n) {
        fprintf(stderr, "Erro ao ler o vetor2 do arquivo\n");
        exit(1);
    }

    // Ler o produto interno
    ret = fread(produtoInterno, sizeof(float), 1, arquivo);
    if (ret != 1) {
        fprintf(stderr, "Erro ao ler o produto interno do arquivo\n");
        exit(1);
    }
}

/**
 * @brief Função de thread para calcular uma parte do produto interno de dois vetores.
 *
 * Esta função é executada por uma thread e calcula uma parte do produto interno entre dois vetores
 * globais `vetor1` e `vetor2`. Cada thread calcula o produto interno para um segmento específico dos vetores,
 * determinado pelo seu ID. O resultado parcial é retornado como um ponteiro para `float` alocado dinamicamente.
 *
 * @param idThread Identificador da thread, que deve ser passado como um ponteiro para `long int`. Este ID
 *                 é usado para calcular os índices de início e fim do segmento a ser processado.
 * 
 * @return void* Retorna um ponteiro para `float` contendo o valor do produto interno calculado pela thread.
 *               O valor retornado deve ser liberado com `free()` após o uso.
 *
 * @note A função encerra a thread com `pthread_exit()` e retorna o resultado parcial. Em caso de erro na alocação
 *       de memória, um erro é impresso e `NULL` é retornado.
 *
 * @warning Certifique-se de liberar a memória alocada para o resultado parcial após o uso para evitar vazamentos de memória.
 */

void* calculaProdInternoConc(void* idThread) {
    long int id = (long int) idThread;
    int inicio, fim, bloco;
    float produtoInterno = 0, *ret;

    bloco = n/nThreads;
    inicio = id*bloco;
    fim = inicio + bloco;

    if(id == (nThreads-1)) {
        fim = n;
    }

    //calcula o produto interno
    for(int i = inicio; i < fim; i++) {
        produtoInterno += vetor1[i] * vetor2[i];
    }

    ret = malloc(sizeof(float));
    if(ret != NULL) {
        *ret = produtoInterno;
    } else {
        printf("--ERRO: malloc() thread\n");
    }
    pthread_exit((void*) ret);
}

// void imprimeDados(int n, float *vetor1, float *vetor2, float produtoInterno) {
//     printf("Dimensão: %d\n", n);
//     printf("Vetor 1:\n");
//     for (int i = 0; i < n; i++) {
//         printf("%f\n", vetor1[i]);
//     }
//     printf("Vetor 2:\n");
//     for (int i = 0; i < n; i++) {
//         printf("%f\n", vetor2[i]);
//     }
//     printf("Produto Interno: %f\n", produtoInterno);
// }

int main(int argc, char *argv[]) {

    float produtoInternoGlobal = 0; //auxiliar para a soma de todos os valores das threads
    float *produtoInternoThreads; //auxiliar para o retorno das threads

    pthread_t *tid_sistema;

    if (argc < 3) {
        fprintf(stderr, "Uso: %s <arquivo binário> <número de threads>\n", argv[0]);
        return 1;
    }

    FILE *arq = fopen(argv[1], "rb");
    if (arq == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo\n");
        return 2;
    }

    leDadosBinario(arq, &n, &vetor1, &vetor2, &produtoInternoArquivo);
    fclose(arq);

    nThreads = atoi(argv[2]);
    printf("Número de Threads: %d\n", nThreads);

    if(nThreads > n) {
        nThreads = n;
    }

    tid_sistema = (pthread_t *) malloc(sizeof(pthread_t) * nThreads);
    if(tid_sistema == NULL) { 
        printf("--ERRO: malloc()\n"); exit(-1); 
    }

    for(long int i = 0; i < nThreads; i++) {
        if(pthread_create(&tid_sistema[i], NULL, calculaProdInternoConc, (void*) i)) {
            printf("--ERRO: pthread_create()\n");
        }
    }

    for(int i = 0; i < nThreads; i++) {
        if(pthread_join(tid_sistema[i], (void*) &produtoInternoThreads)) {
            printf("--ERRO: pthread_join()\n");
            exit(-1);
        }
        produtoInternoGlobal += *produtoInternoThreads;
        free(produtoInternoThreads);
    }

    float varRelativa = ((produtoInternoArquivo - produtoInternoGlobal) / produtoInternoArquivo) * 100;

    // imprimeDados(n, vetor1, vetor2, produtoInterno);

    printf("Produto Interno Arquivo: %.26f\n", produtoInternoArquivo);

    printf("Produto Interno Concorrente: %.26f\n", produtoInternoGlobal);

    printf("Variância Relativa: %.14f\n", varRelativa);

    free(vetor1);
    free(vetor2);

    return 0;
}
