#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

long int qt_mul = 0; // contador para a quantidade de múltiplos
long int soma = 0;
int impresso = 0;
pthread_mutex_t mutex;
pthread_cond_t cond;
pthread_cond_t condExtra;

// funcao executada pelas threads
void *ExecutaTarefa (void *arg) {
    long int id = (long int) arg;
    printf("Thread : %ld esta executando...\n", id);

    for (int i = 0; i < 100000; i++) {
        pthread_mutex_lock(&mutex);  
        
        if(soma % 10 == 0 && qt_mul < 20) { 
          pthread_cond_signal(&condExtra);  
          while(impresso == 0){
              pthread_cond_wait(&cond, &mutex);   
          }    
          impresso = 0;  // resetar a variável após imprimir
        }
        soma++;
        pthread_mutex_unlock(&mutex);
    }

    printf("Thread : %ld terminou!\n", id);
    pthread_exit(NULL);
}

// funcao executada pela thread de log
void *extra (void *args) {
    printf("Extra : esta executando...\n");

    for(int i = 0; i < 10000; i++){
        pthread_mutex_lock(&mutex);
        if(qt_mul < 20) {
            while(soma % 10 != 0) { // espera por múltiplo de 10
                pthread_cond_wait(&condExtra, &mutex); 
            }
            printf("soma = %ld \n", soma);
            impresso = 1;
            qt_mul++;
            pthread_cond_signal(&cond); 
        }
        pthread_mutex_unlock(&mutex);
    }

    printf("Extra : terminou!\n");
    pthread_exit(NULL);
}

// fluxo principal
int main(int argc, char *argv[]) {
    pthread_t *tid;
    int nthreads;

    if(argc < 2) {
        printf("Digite: %s <numero de threads>\n", argv[0]);
        return 1;
    }
    nthreads = atoi(argv[1]);

    tid = (pthread_t*) malloc(sizeof(pthread_t)*(nthreads+1));
    if(tid == NULL) {
        puts("ERRO--malloc");
        return 2;
    }

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&condExtra, NULL);
    pthread_cond_init(&cond, NULL);

    // cria as threads
    for(long int t = 0; t < nthreads; t++) {
        if(pthread_create(&tid[t], NULL, ExecutaTarefa, (void *)t)) {
            printf("--ERRO: pthread_create()\n");
            exit(-1);
        }
    }

    // cria thread de print
    if(pthread_create(&tid[nthreads], NULL, extra, NULL)) {
        printf("--ERRO: pthread_create()\n");
        exit(-1);
    }

    // espera todas as threads terminarem
    for(int t = 0; t < nthreads + 1; t++) {
        if(pthread_join(tid[t], NULL)) {
            printf("--ERRO: pthread_join() \n");
            exit(-1);
        }
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    pthread_cond_destroy(&condExtra);

    printf("Valor de 'soma' = %ld\n", soma);
    return 0;
}
