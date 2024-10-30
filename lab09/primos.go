package main

import (
	"fmt"
	"math"
	"math/rand"
	"sync"
	"time"
)

const N = 10 // quantidade de números na sequência a ser gerada
const M = 3  // número de goroutines

// variáveis para contar primos encontrados em cada abordagem
var countPrimosSequencial int
var countPrimosConcorrente int

// função utilizada para verificar a primalidade de um número
func ehPrimo(n int) int {
	if n <= 1 {
		return 0
	}
	if n == 2 {
		return 1
	}
	if n%2 == 0 {
		return 0
	}
	for i := 3; i <= int(math.Sqrt(float64(n)))+1; i += 2 {
		if n%i == 0 {
			return 0
		}
	}
	return 1
}

// checa quais números no array são primos de forma sequencial
func ehPrimoSequencial(array []int) {
	for _, num := range array {
		if ehPrimo(num) == 1 {
			fmt.Printf("%d é primo\n", num)
			countPrimosSequencial++
		} else {
			fmt.Printf("%d não é primo\n", num)
		}
	}
}

// função utilizada para verificar a primalidade dos números recebidos pelo canal (abordagem concorrente)
func processarNumeros(numeros <-chan int, wg *sync.WaitGroup) {
	defer wg.Done() // decrementa o WaitGroup quando a goroutine termina
	for num := range numeros {
		if ehPrimo(num) == 1 {
			fmt.Printf("%d é primo\n", num)
			countPrimosConcorrente++
		} else {
			fmt.Printf("%d não é primo\n", num)
		}
	}
}

func main() {
	rand.Seed(time.Now().UnixNano())

	min, max := 0, 100          // intervalo de valores que podem ser gerados
	numeros := make(chan int)   // canal para enviar os números
	sequencia := make([]int, N) // array para armazenar os números gerados

	var wg sync.WaitGroup

	// inicia M goroutines para processar os números
	for i := 0; i < M; i++ {
		wg.Add(1)
		go processarNumeros(numeros, &wg)
	}

	// gera os números da sequência e envia para o canal
	for i := 0; i < N; i++ {
		num := rand.Intn(max-min) + min
		sequencia[i] = num // guarda os números gerados em um array
		numeros <- num     // envia número para o canal
	}

	close(numeros) // fecha o canal após enviar todos os números

	wg.Wait() // espera todas as goroutines terminarem

	ehPrimoSequencial(sequencia)

	fmt.Print(sequencia)

	if countPrimosSequencial != countPrimosConcorrente {
		println("As soluções sequencial e concorrente erraram a primalidade de pelo menos um número :(")
	} else {
		println("As soluções sequencial e concorrente encontraram a mesma quantidade de primos :)")
	}

	fmt.Printf("Quantidade de primos encontrados na abordagem sequencial: %d\n", countPrimosSequencial)
	fmt.Printf("Quantidade de primos encontrados na abordagem concorrente: %d\n", countPrimosConcorrente)

}
