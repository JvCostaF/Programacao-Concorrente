package main

import (
	"fmt"
	"math"
	"math/rand"
	"sync"
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

	var wg sync.WaitGroup //Sincronizacao para esperar todas as goroutines terminarem
	var mutex sync.Mutex  //Sincronizacao para exclusao mutua da variavel qtdPrimos

	var qtdRoutines int //Quantidade de goroutines
	var seqSize int     //Tamanho da sequencia de numeros inteiros
	qtdPrimos := 0

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
		Adicionei cada numero aleatorio da sequencia no canal
	*/
	go func() {
		for i := 0; i < seqSize; i++ {
			sequencia[i] = randRange(0, 100)
			channel <- sequencia[i]
		}
		close(channel)
	}()

	for r := 0; r < qtdRoutines; r++ {
		wg.Add(1)
		go func() {
			defer wg.Done()
			for n := range channel {
				if isPrimo(int64(n)) {
					mutex.Lock()
					qtdPrimos++
					mutex.Unlock()
				}
			}
		}()
	}

	wg.Wait() //Precisamos esperar todas as goroutine terminarem

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
