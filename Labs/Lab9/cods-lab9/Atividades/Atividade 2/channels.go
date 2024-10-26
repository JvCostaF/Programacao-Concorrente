package main

import "fmt"

func routine(str chan string) {
	var msg = ""
	msg = <-str
	fmt.Println("Main: ", msg)

	str <- "Oi Main, bom dia, tudo bem?"

	msg = <-str
	fmt.Println("Main: ", msg)

	str <- "Certo, entendido"

	msg = <-str
	fmt.Println("Main: ", msg)

	str <- "finalizando"

}

func main() {

	//cria um canal de comunicacao nao-bufferizado
	str := make(chan string) //alocando memoria

	go routine(str)

	str <- "Ola, Goroutine, bom dia!"

	msg := <-str

	fmt.Println("Goroutine: ", msg)

	str <- "Tudo bem! Vou terminar tá?"

	msg = <-str

	fmt.Println("Goroutine: ", msg)

	str <- "finalizando"

	msg = <-str

	fmt.Println("Goroutine: ", msg)

}
