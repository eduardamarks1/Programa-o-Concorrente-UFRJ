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
