/* Disciplina: Programacao Concorrente */
/* Codigo: O objetivo deste Laboratório é introduzir o mecanismo de sincronização por condição
usando variaveis de condição da biblioteca Pthread. */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>


long int qt_mul = 0;
int impresso = 0;
long int soma = 0;
pthread_mutex_t mutex; 
pthread_cond_t cond;

//funcao executada pelas threads
void *ExecutaTarefa (void *arg) {
  long int id = (long int) arg;
  printf("Thread : %ld esta executando...\n", id);

  for (int i=0; i<100000; i++) {

     pthread_mutex_lock(&mutex);
     soma++;  // incrementa o valor de soma
     if(soma%10 == 0){ // ao encontrar um múltiplo de 10
       impresso = 0; // marca que ainda não foi impresso
       pthread_cond_signal(&cond);  // avisa p/  thread extra que ela pode imprimir
       while(impresso == 0){ // aguarda até que seja impresso
         pthread_cond_wait(&cond, &mutex);
       }
       impresso = 0;
     }
     pthread_mutex_unlock(&mutex);

  }

  printf("Thread : %ld terminou!\n", id);
  pthread_exit(NULL);
}

//funcao executada pela thread de log
void *extra (void *args) {
  printf("Extra : esta executando...\n");
  pthread_mutex_lock(&mutex);
  for (int i=0; i<10000; i++) {
    if (qt_mul < 20){
      printf("soma = %ld \n", soma);
      qt_mul++;
      impresso = 1;
      pthread_cond_signal(&cond);
    } 
    pthread_mutex_unlock(&mutex);   
      
  }

  printf("Extra : terminou!\n");
  pthread_exit(NULL);
}

//fluxo principal
int main(int argc, char *argv[]) {
   pthread_t *tid; 
   int nthreads; 

   //--le e avalia os parametros de entrada
   if(argc<2) {
      printf("Digite: %s <numero de threads>\n", argv[0]);
      return 1;
   }
   nthreads = atoi(argv[1]);

   //--aloca as estruturas
   tid = (pthread_t*) malloc(sizeof(pthread_t)*(nthreads+1));
   if(tid==NULL) {puts("ERRO--malloc"); return 2;}

   //--inicilaiza o mutex (lock de exclusao mutua) e a variável de condição
   pthread_mutex_init(&mutex, NULL);
   pthread_cond_init(&cond, NULL);

   //--cria as threads
   for(long int t=0; t<nthreads; t++) {
     if (pthread_create(&tid[t], NULL, ExecutaTarefa, (void *)t)) {
       printf("--ERRO: pthread_create()\n"); exit(-1);
     }
   }

   //--cria thread de print
   if (pthread_create(&tid[nthreads], NULL, extra, NULL)) {
      printf("--ERRO: pthread_create()\n"); exit(-1);
   }

   //--espera todas as threads terminarem
   for (int t=0; t<nthreads+1; t++) {
     if (pthread_join(tid[t], NULL)) {
         printf("--ERRO: pthread_join() \n"); exit(-1); 
     } 
   } 

   //--finaliza o mutex e a cond
   pthread_mutex_destroy(&mutex);
   pthread_cond_destroy(&cond);
   
   printf("Valor de 'soma' = %ld\n", soma);

   return 0;
}
