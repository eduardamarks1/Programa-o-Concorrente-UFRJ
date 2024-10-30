package main

import (
	"fmt"
	"math"
	"math/rand"
	"time"
)

const N = 10 // quantidade de números na sequência a ser gerada

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

// checa quais números no array são primos de forma sequencial para fazer um sanity check
func ehPrimoSequencial(array []int) {
	for _, num := range array {
		if ehPrimo(num) == 1 {
			fmt.Printf("%d é primo\n", num)
		} else {
			fmt.Printf("%d não é primo\n", num)
		}
	}
}

func main() {
	rand.Seed(time.Now().UnixNano())

	min, max := 0, 100 // intervalo de valores que podem ser gerados

	array := make([]int, N) // cria o slice que vai guardar os números da sequência

	// gera os números da sequência e guarda no slice
	for i := 0; i < N; i++ {
		array[i] = rand.Intn(max-min) + min
	}

	fmt.Println("Sequência de números aleatórios:", array)

	// chama a função que verifica quais números da sequência são primos
	ehPrimoSequencial(array)
}
