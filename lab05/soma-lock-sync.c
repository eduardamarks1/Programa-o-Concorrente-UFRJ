#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define QT_MULTIPLOS 20 // quantos múltiplos queremos imprimir

long int qt_mul = 0; // conta a quantidade de múltiplos de 10 já impressos
long int soma = 0;
int impresso = 0; // booleano (0 ou 1) para identificar quando um múltiplo já foi impresso ou não
pthread_mutex_t mutex;
pthread_cond_t condExtra;
pthread_cond_t condExecuta;

// função executada pelas threads
void *ExecutaTarefa(void *arg) {
    long int id = (long int)arg;
    printf("Thread %ld: está executando...\n", id);

    while (qt_mul < QT_MULTIPLOS) { // enquanto houver múltiplos a serem impressos
        pthread_mutex_lock(&mutex);

        // se a soma for múltiplo de 10, aguarda a thread extra imprimir
        while (soma % 10 == 0 && impresso == 0) {
            pthread_cond_wait(&condExecuta, &mutex);
        }

        if (qt_mul >= QT_MULTIPLOS) {
            pthread_mutex_unlock(&mutex);
            break; // sai do loop se já imprimimos todos os múltiplos
        }

        soma++; 
        impresso = 0; 

        pthread_cond_signal(&condExtra); // Acorda a thread extra para verificar a impressão
        pthread_mutex_unlock(&mutex);
    }

    printf("Thread %ld: terminou!\n", id);
    pthread_exit(NULL);
}

// função executada pela thread de log (extra)
void *extra(void *args) {
    printf("Extra: está executando...\n");

    while (qt_mul < QT_MULTIPLOS) {
        pthread_mutex_lock(&mutex);

        // se a soma for múltiplo de 10 e não foi impresso
        if (soma % 10 == 0 && impresso == 0) {
            printf("(%ld) - múltiplo de 10 encontrado! soma = %ld\n", (qt_mul + 1),soma);
            impresso = 1; 
            qt_mul++; 
            pthread_cond_broadcast(&condExecuta); // acorda todas as threads de ExecutaTarefa
        }

        pthread_cond_signal(&condExtra); 
        pthread_mutex_unlock(&mutex);
    }

    printf("Extra: terminou!\n");
    pthread_exit(NULL);
}

// fluxo principal
int main(int argc, char *argv[]) {
    pthread_t *tid;
    int nthreads;

    if (argc < 2) {
        printf("Digite: %s <numero de threads>\n", argv[0]);
        return 1;
    }
    nthreads = atoi(argv[1]);

    tid = (pthread_t *)malloc(sizeof(pthread_t) * (nthreads + 1));
    if (tid == NULL) {
        puts("ERRO--malloc");
        return 2;
    }

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&condExtra, NULL);
    pthread_cond_init(&condExecuta, NULL);

    // Cria as threads de tarefa
    for (long int t = 0; t < nthreads; t++) {
        if (pthread_create(&tid[t], NULL, ExecutaTarefa, (void *)t)) {
            printf("--ERRO: pthread_create()\n");
            exit(-1);
        }
    }

    // Cria a thread de log (extra)
    if (pthread_create(&tid[nthreads], NULL, extra, NULL)) {
        printf("--ERRO: pthread_create()\n");
        exit(-1);
    }

    // Espera todas as threads terminarem
    for (int t = 0; t < nthreads + 1; t++) {
        if (pthread_join(tid[t], NULL)) {
            printf("--ERRO: pthread_join()\n");
            exit(-1);
        }
    }

    // Limpeza
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condExtra);
    pthread_cond_destroy(&condExecuta);

    printf("Valor final de 'soma' = %ld\n", soma);
    return 0;
}
