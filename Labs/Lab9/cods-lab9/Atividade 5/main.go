package main

import (
	"fmt"
	"math"
	"math/rand"
)

/*
	int ehPrimo(long long int n) {
		if (n<=1) return 0;
		if (n==2) return 1;
		if (n%2==0) return 0;
		for (int i=3; i<sqrt(n)+1; i+=2)
			if(n%i==0) return 0;
		return 1;

Descobre se o numero passado como parametro e primo ou nao
*/
func isPrimo(n int64) bool {
	if n <= 1 {
		return false
	}
	if n == 2 {
		return true
	}
	if n%2 == 0 {
		return false
	}
	for i := int64(3); i <= int64(math.Sqrt(float64(n))); i += 2 {
		if n%i == 0 {
			return false
		}
	}
	return true
}

// func routine(id int, ch <- chan int) bool{
// 	for n := range ch {
// 		return isPrimo(n)
// 	}
// }

/*
Gera um numero aleatorio definido dentro do intervalo [min, max]
*/
func randRange(min, max int) int {
	return rand.Intn(max-min) + min
}

func main() {

	// Testando funcao isPrimo
	// numero := int64(40)

	// channel := make(chan []int)

	var qtdRoutines int
	var seqSize int
	qtdPrimos := 0

	fmt.Println("Digite o tamanho da sequencia de numeros inteiros: ")
	if _, erro := fmt.Scanf("%d", &seqSize); erro != nil {
		fmt.Println("Erro ao ler entrada!", erro)
		return
	}

	fmt.Println("Digite a quantidade de Routines: ")
	if _, erro := fmt.Scanf("%d", &qtdRoutines); erro != nil {
		fmt.Println("Erro ao ler entrada!", erro)
		return
	}

	sequencia := make([]int, seqSize)

	for i := 0; i < seqSize; i++ {
		sequencia[i] = randRange(0, 100)

		if isPrimo(int64(sequencia[i])) {
			qtdPrimos++
		}

	}
	fmt.Println(sequencia)
	fmt.Println("Quantidade de numeros primos: ", qtdPrimos)

}
