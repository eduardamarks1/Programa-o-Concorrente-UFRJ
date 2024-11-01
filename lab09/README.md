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
   go run main.go
   ```

### Saída 


```
Main envia para Goroutine: Olá, Goroutine, bom dia! (impresso pela Goroutine)
Goroutine envia para Main: Oi Main, bom dia, tudo bem? (impresso pela Main)
Goroutine envia para Main: Certo, entendido. (impresso pela Main)
Goroutine imprime: finalizando
Main imprime: finalizando
```

## Primos
