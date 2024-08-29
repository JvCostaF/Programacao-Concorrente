#include <stdio.h>
#include <stdlib.h>
#include <time.h>

float* geradorMatriz(int dimensaoMatriz) {

   float *matriz;
   matriz = (float*) malloc(sizeof(float) * dimensaoMatriz); //Aloca espaco na memoria para a matriz

    if(!matriz) {
        fprintf(stderr, "Erro de alocao da memoria da matriz\n");
        return 2;
   }

   srand(time(NULL));
   for(int i=0; i<dimensaoMatriz; i++){
        *(matriz+i) = (rand() % 1000) * 0.3;
   }

   return matriz;
}

int main(int argc, char*argv[]){

   int linhas, colunas; //dimensoes da matriz
   long long int tam; //qtde de elementos na matriz
   FILE * descritorArquivo; //descritor do arquivo de saida
   size_t ret; //retorno da funcao de escrita no arquivo de saida
   
   //recebe os argumentos de entrada
   if(argc < 4) {
      fprintf(stderr, "Digite: %s <linhas> <colunas> <arquivo saida>\n", argv[0]);
      return 1;
   }
   linhas = atoi(argv[1]); 
   colunas = atoi(argv[2]);
   tam = linhas * colunas;

   float *matriz1 = geradorMatriz(tam);

   float *matriz2 = geradorMatriz(tam);

   //escreve a matriz no arquivo
   //abre o arquivo para escrita binaria
   descritorArquivo = fopen(argv[3], "wb");
   if(!descritorArquivo) {
      fprintf(stderr, "Erro de abertura do arquivo\n");
      return 3;
   }
   //escreve numero de linhas e de colunas
   ret = fwrite(&linhas, sizeof(int), 1, descritorArquivo);
   ret = fwrite(&colunas, sizeof(int), 1, descritorArquivo);
   //escreve os elementos da matriz
   ret = fwrite(matriz1, sizeof(float), tam, descritorArquivo);
   ret = fwrite(matriz2, sizeof(float), tam, descritorArquivo);
   if(ret < tam) {
      fprintf(stderr, "Erro de escrita no  arquivo\n");
      return 4;
   }

   //finaliza o uso das variaveis
   fclose(descritorArquivo);
   free(matriz1);
   return 0;

}