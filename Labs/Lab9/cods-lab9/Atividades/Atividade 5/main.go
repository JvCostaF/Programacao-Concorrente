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

/*
Gera um numero aleatorio definido dentro do intervalo [min, max]
*/
func randRange(min, max int) int {
	return rand.Intn(max-min) + min
}

func main() {

	channel := make(chan int)

	done := make(chan int)

	var qtdRoutines int //Quantidade de goroutines
	var seqSize int     //Tamanho da sequencia de numeros inteiros

	fmt.Println("Digite o tamanho da sequencia de numeros inteiros: ")
	if _, erro := fmt.Scan(&seqSize); erro != nil {
		fmt.Println("Erro ao ler entrada!", erro)
		return
	}

	fmt.Println("Digite a quantidade de Routines: ")
	if _, erro := fmt.Scan(&qtdRoutines); erro != nil {
		fmt.Println("Erro ao ler entrada!", erro)
		return
	}

	sequencia := make([]int, seqSize)

	/*
		Adicionei cada numero aleatorio da sequencia no canal, veja que numeros aleatorios podem ser
		gerados, mas nao se preocupe, se for um numero primo duplicado a contagem sera feita corretamente.
	*/
	go func() {
		for i := 0; i < seqSize; i++ {
			sequencia[i] = randRange(0, 100)
			channel <- sequencia[i]
		}
		close(channel)
	}()

	for r := 0; r < qtdRoutines; r++ {
		go func() {
			contPrimosGoRoutine := 0
			for n := range channel {
				if isPrimo(int64(n)) {
					contPrimosGoRoutine++
				}
			}
			done <- contPrimosGoRoutine
		}()
	}

	qtdPrimos := 0
	for i := 0; i < qtdRoutines; i++ {
		qtdPrimos += <-done
	}

	fmt.Println(sequencia)
	fmt.Println("Quantidade de numeros primos: ", qtdPrimos)

}

/* Links Utilizados
https://golang.cafe/blog/golang-random-number-generator.html
https://stackoverflow.com/questions/23577091/generating-random-numbers-over-a-range-in-go
https://stackoverflow.com/questions/24703943/passing-a-slice-into-a-channel
https://victoriametrics.com/blog/go-sync-mutex/
https://go.dev/tour/concurrency/9
*/
