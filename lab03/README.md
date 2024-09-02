# Lab03 - Programação Concorrente


### Análise do Desempenho: Multiplicação de Matrizes (Multithreading)

Este projeto tem como objetivo implementar uma solução concorrente para o problema de multiplicação de duas matrizes, coletar informações sobre o seu tempote de execução, e calcular o ganho obtido.


## Estrutura do Projeto

- `mat_seq.c`: Implementa a multiplicação de duas matrizes de forma sequencial.
- `mat_conc.c`: Implementa a multiplicação de duas matrizes utilizando múltiplas threads para realizar o cálculo de forma concorrente.
- `geraMatrizBinario.c`: Gera matrizes aleatórias e salva em arquivos binários, que são utilizados como entrada pelos programas de multiplicação.

## Detalhes de Implementação
Aqui está um exemplo de um arquivo README para o código de multiplicação de matrizes usando threads:

### 1. Divisão do Trabalho entre as Threads

A divisão do trabalho é feita com base nas linhas da primeira matriz (`mat1`):

- O número total de linhas de `mat1` é dividido pelo número de threads (`nthreads`).
- Cada thread é atribuída a um bloco de linhas para processar. O número básico de linhas por thread é calculado como:
  ```c
  blocos = mat1->linha / nthreads;
  ```
- Se houver linhas restantes após a divisão, essas linhas são distribuídas entre as primeiras threads. Isso é controlado pela variável `resto`:
  ```c
  resto = mat1->linha % nthreads;
  ```

### 2. Distribuição e Execução das Threads

- Cada thread recebe um intervalo de linhas para processar, definido por `args[i].inicio` e `args[i].fim`.
- As primeiras `resto` threads recebem uma linha adicional para balancear a carga:
  ```c
  args[i].fim = inicioLinha + blocos + (i < resto ? 1 : 0);
  ```
- As threads são criadas usando a função `pthread_create` e executam a função `multiplicacaoMatrizes`, que realiza a multiplicação de suas linhas atribuídas com todas as colunas da segunda matriz (`mat2`).

## Como Executar

1. Compile o programa utilizando um compilador compatível com POSIX Threads (exemplo: GCC):
   ```bash
   gcc -o mult_matrix mult_matrix.c -lpthread
   ```

2. Execute o programa passando os nomes dos arquivos das duas matrizes de entrada, o arquivo de saída e o número de threads:
   ```bash
   ./mult_matrix matriz1.bin matriz2.bin matriz_saida.bin 4
   ```

   Neste exemplo, `4` é o número de threads que serão utilizadas para a multiplicação.

## Considerações

- As dimensões das matrizes de entrada devem ser compatíveis para multiplicação (o número de colunas da primeira matriz deve ser igual ao número de linhas da segunda matriz).
- O programa lida com a alocação dinâmica de memória e a leitura/escrita de arquivos binários para eficiência.
- O uso de múltiplas threads melhora o desempenho em sistemas de múltiplos núcleos, dividindo a carga de trabalho e aproveitando a concorrência.

## Licença

Este projeto está licenciado sob a [MIT License](LICENSE).

---

Esse README fornece uma visão geral do projeto, como ele funciona e como pode ser executado. Se tiver mais alguma dúvida ou precisar de ajustes adicionais, sinta-se à vontade para perguntar!

## Como Compilar e Executar

### Compilação

Use o `gcc` para compilar os arquivos C. Abaixo estão os comandos de exemplo para compilar cada um dos programas:

```bash
gcc -o mat_seq mat_seq.c -lpthread
gcc -o mat_conc mat_conc.c -lpthread
gcc -o geraMatrizBinario geraMatrizBinario.c
```

### Execução

1. **Gerar Matrizes Aleatórias**:

   Para gerar duas matrizes de dimensões especificadas e salvá-las em arquivos binários:

   ```bash
   ./geraMatrizBinario <arquivo_matriz1> <linhas1> <colunas1> <arquivo_matriz2> <linhas2> <colunas2>
   ```

   Exemplo:

   ```bash
   ./geraMatrizBinario matriz1.bin 100 200 matriz2.bin 200 150
   ```

2. **Multiplicação Sequencial**:

   Para executar a multiplicação de matrizes sequencial:

   ```bash
   ./mat_seq <arquivo_matriz1> <arquivo_matriz2> <arquivo_matriz_saida>
   ```

   Exemplo:

   ```bash
   ./mat_seq matriz1.bin matriz2.bin resultado_seq.bin
   ```

3. **Multiplicação Concorrente**:

   Para executar a multiplicação de matrizes usando threads:

   ```bash
   ./mat_conc <arquivo_matriz1> <arquivo_matriz2> <arquivo_matriz_saida> <numero_de_threads>
   ```

   Exemplo:

   ```bash
   ./mat_conc matriz1.bin matriz2.bin resultado_conc.bin 4
   ```

## Observações
