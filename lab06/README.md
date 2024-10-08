# Priority RWLock

Foi criada uma biblioteca chamada `priority_rwlock` para implementar uma versão própria das funções `rwlock` da `pthread.h`, que adiciona **prioridade para operações de escrita**. Isso significa que, sempre que uma operação de escrita for solicitada, **nenhuma nova leitura poderá ser iniciada** até que a escrita seja concluída. 

## Funcionamento Básico

O controle da prioridade é feito por meio da variável `escrEsperando`. Quando uma operação de escrita é solicitada, essa variável é incrementada. Se houver leitores ou escritores ativos, o escritor entra em espera. Enquanto houver escritores esperando, nenhuma nova operação de leitura será permitida. Isso garante que assim que os leitores atuais terminem suas operações, o próximo escritor na fila será atendido imediatamente.

## Funções

A `priority_rwlock` tem as seguintes funções:

- **rwlock_priority_init()**  
  Inicializa o lock de leitura eescrita com prioridade. Além de inicializar o arquivo de log.

- **rwlock_priority_destroy()**  
  Destrói as variáveis de condição e fecha o arquivo de log.

- **rwlock_priority_read_lock(pthread_mutex_t* mutex)**  
  Solicita o acesso de leitura. Se houver escritores ativos ou aguardando, o leitor é colocado em espera (bloqueado).

- **rwlock_priority_read_unlock(pthread_mutex_t* mutex)**  
  Libera o lock de leitura. Se nenhum leitor estiver ativo, o próximo escritor entra na prioridade.

- **rwlock_priority_write_lock(pthread_mutex_t* mutex)**  
  Solicita o acesso de escrita. Escritores têm prioridade sobre novas leituras, e aguardam até que não haja leitores ou outros escritores ativos.

- **rwlock_priority_write_unlock(pthread_mutex_t* mutex)**  
  Libera o lock de escrita. Se houver outros escritores esperando, um escritor é acordado. Caso contrário, todos os leitores na fila são acordados.

## Exemplo 

O `main_rwlock.c` é um exemplo de implementação da biblioteca que realiza operações de consulta (98%), inserção (1%) e remoção (1%), conforme os parâmetros padrão do código fornecido pela professora da disciplina. No entanto, para melhorar a visualização das operações no log, os parâmetros foram alterados para 50% de consulta, 15% de inserção e 35% de remoção (apenas para o log salvo em `LOG.txt`). Além disso, a quantidade total de operações foi reduzida de 100000 para 1000 Isso foi feito para evitar que o log se tornasse absurdamente grande. Nada impede, porém, que o programa seja executado com os parâmetros originais, apenas resultando em um log muito mais extenso.

```c
#include <pthread.h>
#include "priority_rwlock.h"

pthread_mutex_t mutex;

int main() {
    rwlock_priority_init();

    // Exemplo de uso de leitura e escrita
    rwlock_priority_read_lock(&mutex);
    // ... Leitura ...
    rwlock_priority_read_unlock(&mutex);

    rwlock_priority_write_lock(&mutex);
    // ... Escrita ...
    rwlock_priority_write_unlock(&mutex);

    rwlock_priority_destroy();
    return 0;
}
```

## LOG

Foi implementada uma bibliteca `log` que salva as mensagens em um arquivo chamado `LOG.txt`. Isso foi bastante útil para analisar o comportamento do programa e da implementação das prioridades.

## RESULTADOS

O trecho a seguir do `LOG.txt` ilustra a corretude da implementação. Veja:

```C
[LOG] Escritor comecou a escrever
[LOG] Escritor terminou de escrever
[LOG] Escritor tenta escrever
[LOG] Escritor comecou a escrever
[LOG] Escritor terminou de escrever
[LOG] Escritor tenta escrever
[LOG] Escritor comecou a escrever
[LOG] Leitor tenta ler
[LOG] Leitor esperando, porque tem escritor ativo ou esperando
[LOG] Escritor terminou de escrever
[LOG] Leitor tenta ler
[LOG] Leitor comecou a ler
[LOG] Leitor comecou a ler
[LOG] Leitor terminou de ler
[LOG] Escritor tenta escrever
[LOG] Escritor esperando, porque tem leitores ou escritores ativos
[LOG] Leitor terminou de ler
[LOG] Leitor tenta ler
[LOG] Leitor esperando, porque tem escritor ativo ou esperando
[LOG] Escritor comecou a escrever
[LOG] Escritor terminou de escrever
[LOG] Leitor tenta ler
[LOG] Leitor comecou a ler
[LOG] Leitor terminou de ler

```

Repare que além das propriedades clássicas de programas do tipo produtor/consumitor serem respeitadas, a prioridade do escritor é evidenciada em:

```C
[LOG] Escritor tenta escrever
[LOG] Escritor esperando, porque tem leitores ou escritores ativos
[LOG] Leitor terminou de ler
[LOG] Leitor tenta ler
[LOG] Leitor esperando, porque tem escritor ativo ou esperando
[LOG] Escritor comecou a escrever
[LOG] Escritor terminou de escrever
```
O escritor tenta escrever, mas é interrompido e entre em espera. Enquanto isso, um leitor tenta ler e entra em espera, pois cede prioridade para o escritor que já estava esperando.

## Compilação

Para compilar o código, digite na linha de comando:

```C
 gcc -o main_rwlock .\list_int.c .\priority_rwlock.c .\log.c  .\main_rwlock.c -lpthread
```

Para rodar, digite:

```C
 ./main_rwlock <número de threads>
```
