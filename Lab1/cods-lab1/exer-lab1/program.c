

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
Função que cria um vetor de tamanho N, com base no valor escolhido
*/
int* makeArray(int size){

    int* array = (int*)malloc(size * sizeof(int)); //Alocando espaco para o meu vetor.

    if(array == NULL){
        printf("Problemas com a alocacao de memoria");
        exit(1);
    }

    srand(time(NULL)); //Maneira existente em C para utilizar numeros randomicos.

    for(int i = 0; i < size; i++){
        array[i] = rand() % 100;
    }

    return array;
}

/*
Função que adiciona uma unidade em cada elemento de um vetor passado como argumento.
*/
int* addOneForEachArrayElement(int* array, int size){
    int* modifiedArray = (int*)malloc(size * sizeof(int)); //Alocando espaco para o vetor alterado.

    modifiedArray = array; //Atribuindo ao vetor alterado os valores do vetor original

    if(array == NULL){
        printf("Problemas com a alocacao de memoria");
        exit(1);
    }

    for(int i = 0; i < size; i++){
        modifiedArray[i] += 1; //Somando 1 em todos os elementos do vetor alterado
    }

    return modifiedArray;
}

int main(void){

    int size;
    printf("Digite o tamanho do vetor: \n");
    scanf("%d", &size);
    
    int* array = makeArray(size);

    printf("Vetor de %d posições:\n", size);
    printf("[ ");
    for(int i = 0; i < size; i++){
        printf("%d ", array[i]);
    }
    printf("]");

    printf("\n");

    int* modifiedArray = addOneForEachArrayElement(array, size);

    printf("Vetor alterado:\n");
    printf("[ ");
    for(int i = 0; i < size; i++){
        printf("%d ", modifiedArray[i]);
    }
    printf("]");

    printf("\n");

    free(array);

    return 0;
}