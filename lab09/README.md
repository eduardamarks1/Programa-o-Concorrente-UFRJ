# Explorando Goroutines e Canais em Go

## Channels

Este código demonstra o uso de goroutines e canais para comunicação e sincronização entre uma goroutine e a função principal (`main`). O código cria uma goroutine que se comunica com a função principal através de um canal de mensagens, permitindo que ambas enviem e recebam mensagens de forma sincronizada.

### Funcionamento

1. **Canais**: 
   - O código utiliza dois canais: `str` (um canal de string para enviar mensagens) e `fim` (um canal booleano para sinalizar o término da goroutine).
  
2. **Goroutine**:
   - A função `tarefa` é executada como uma goroutine. Ela espera receber mensagens do canal `str`, processa essas mensagens, e envia respostas de volta para a função principal. 

3. **Sincronização**:
   - O uso do canal `fim` permite que a goroutine informe à função principal quando terminou sua execução. A função `main` espera essa confirmação antes de finalizar.

### Sequência de Operações

- A função principal (`main`) envia uma mensagem inicial para a goroutine.
- A goroutine lê essa mensagem, imprime na tela e envia uma resposta de volta.
- A função principal lê a resposta, imprime, e envia outra mensagem para a goroutine.
- A goroutine processa a nova mensagem e responde novamente.
- Por fim, a função principal envia um sinal de término, e a goroutine finaliza sua execução e retorna um sinal de conclusão.

## Como Executar

Para executar o código, escreva no terminal:

   ```bash
   go run channels.go
   ```

### Saída 


```
Main envia para Goroutine: Olá, Goroutine, bom dia! (impresso pela Goroutine)
Goroutine envia para Main: Oi Main, bom dia, tudo bem? (impresso pela Main)
Main envia para Goroutine: Tudo bem! Vou terminar tá? (impresso pela Goroutine)
Goroutine envia para Main: Certo, entendido. (impresso pela Main)
Goroutine imprime: finalizando
Main imprime: finalizando
```

## Primos

Este código demonstra a verificação de primalidade de números utilizando duas abordagens: sequencial e concorrente. O código gera uma sequência de números aleatórios, verifica se eles são primos e compara os resultados obtidos pelas duas abordagens. A abordagem concorrente utiliza goroutines para processar os números de forma simultânea, enquanto a abordagem sequencial faz a verificação em um único fluxo.

### Funcionamento

1. **Geração de Números**:
   - O código gera uma sequência de `N` números aleatórios dentro de um intervalo definido (0 a 100) e armazena esses números em um array, que será utilizado posteriormente para fazer a comparação.

2. **Abordagem Sequencial**:
   - A função `ehPrimoSequencial` percorre a sequência de números e verifica a primalidade de cada número. A quantidade de primos encontrados é contabilizada na variável `countPrimosSequencial`.

3. **Abordagem Concorrente**:
   - A função `processarNumeros` é chamada em `M` goroutines, que lêem os números de um canal e verificam se cada número é primo. O resultado é contabilizado em `countPrimosConcorrente`.

4. **Sincronização**:
   - O programa utiliza um `sync.WaitGroup` para garantir que a função principal (`main`) aguarde a conclusão de todas as goroutines antes de prosseguir. O canal é fechado após o envio de todos os números, sinalizando que não haverá mais dados a serem processados.

5. **Comparação dos Resultados**:
   - Após a execução das duas abordagens, o programa compara o número de primos encontrados nas duas abordagens e imprime os resultados.

## Como Executar

Para executar o código, escreva no terminal:

   ```bash
   go run primos.go
   ```

### Saída


```
50 não é primo
73 é primo
23 é primo
40 não é primo
...
As soluções sequencial e concorrente encontraram a mesma quantidade de primos :)
Quantidade de primos encontrados na abordagem sequencial: 5
Quantidade de primos encontrados na abordagem concorrente: 5
```

