# Lab03 - Programação Concorrente


### Análise do Desempenho: Multiplicação de Matrizes (Multithreading)

Este projeto tem como objetivo implementar uma solução concorrente para o problema de multiplicação de duas matrizes, coletar informações sobre o seu tempote de execução, e calcular o ganho obtido.


## Estrutura do Projeto

- `mat_seq.c`: Implementa a multiplicação de duas matrizes de forma sequencial.
- `mat_conc.c`: Implementa a multiplicação de duas matrizes utilizando múltiplas threads para realizar o cálculo de forma concorrente.
- `geraMatrizBinario.c`: Gera matrizes aleatórias e salva em arquivos binários, que são utilizados como entrada pelos programas de multiplicação.

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
