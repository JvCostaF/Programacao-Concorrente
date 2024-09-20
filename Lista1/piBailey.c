#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
    Não perca tempo compilando no Windows, tive sérios problemas por lá.
    Para compilar no linux, a lib math.h não é linkada com outras básicas,
    então precisamos dizer que ela será usada no processo de build com o 
    gcc da seguinte maneira

    $ gcc -o pi piBailey.c -lm
*/

long double piBBP(int n){

    long double pi = 0.0;

    for (int i = 0; i < n; i++) {
        long double termo = (4.0 / (8*i + 1)) -
                        (2.0 / (8*i + 4)) -
                        (1.0 / (8*i + 5)) -
                        (1.0 / (8*i + 6));
        pi += termo / powl(16, i);
    }

    return pi;
}

int main(int argc, char *argv[]) {

    if(argc < 2) {
        printf("Digite o numero de termos.");
        return 1;
    }

    int n = atoi(argv[1]);

    long double pi = piBBP(n);
    printf("Valor aproximado de pi com %d termos: %.14Lf\n", n, pi);

    return 0;
}