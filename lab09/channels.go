package main

import (
	"fmt"
)

// goroutine
func tarefa(str chan string, fim chan bool) {
	// goroutine lê o canal
	msg := <-str
	fmt.Println("Main envia para Goroutine:", msg, "(impresso pela Goroutine)") // impresso pela Goroutine

	// envia a mensagem de resposta pelo canal para a main ler
	str <- "Oi Main, bom dia, tudo bem?"

	// goroutine lê o canal novamente
	msg = <-str
	fmt.Println("Main envia para Goroutine:", msg, "(impresso pela Goroutine)") // impresso pela Goroutine

	// goroutine escreve no canal
	str <- "Certo, entendido."

	// recebe sinal de término da main
	<-fim
	fmt.Println("Goroutine imprime: finalizando")

	// envia sinal de conclusão para main
	fim <- true
}

func main() {
	str := make(chan string) // cria canal de mensagens
	fim := make(chan bool)   // cria canal de sinalização de término

	// cria uma goroutine para a função 'tarefa'
	go tarefa(str, fim)

	// envia primeira mensagem para a goroutine
	str <- "Olá, Goroutine, bom dia!"

	msg := <-str                                                           // lê o canal
	fmt.Println("Goroutine envia para Main:", msg, "(impresso pela Main)") // imprime a mensagem enviada pela Goroutine

	// escreve uma mensagem no canal para a goroutine ler
	str <- "Tudo bem! Vou terminar tá?"

	msg = <-str // lê o canal
	fmt.Println("Goroutine envia para Main:", msg, "(impresso pela Main)")

	// envia sinal de término para a goroutine
	fim <- true
	<-fim // aguarda o sinal de término da goroutine p/ depois terminar

	fmt.Println("Main imprime: finalizando")
}
