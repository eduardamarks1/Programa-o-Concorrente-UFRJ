/* Multiplicacao de duas amtrizes de forma sequencial */
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"

float *mat1; //matriz de entrada 1
float *mat2; //matriz de entrada 1
float *matRet; // matriz de saída
int nthreads; // número de threads

typedef struct{
   int id; //identificador do elemento que a thread ira processar
   int linha; 
   int coluna;
} tArgs;

typedef struct{
    float *matriz;
    int linha;
    int coluna;
    int tam;
} Matriz;

// funcao que as threads executarao (main das threads)
// void * tarefa(void *arg) {
//    tArgs *args = (tArgs*) arg;
//    for(int i=args->id; i<args->dim; i+=nthreads) //separando em linhas
//       for(int j=0; j<args->dim; j++) 
//          saida[i] += mat[i*(args->dim) + j] * vet[j];

//    pthread_exit(NULL);
// }

void * tarefa(void *arg) {
   tArgs *args = (tArgs*) arg;
   for(int i=args->id; i<args->linha; i+=nthreads) //separando em linhas
      for(int j=0; j<args->coluna; j++) 
         matRet[i*(arg->dim)+j] += mat1[i*(args->dim) + j] * mat2[i*(args->dim) + j];

   pthread_exit(NULL);
}

Matriz* leMatrizBinario(){
   Matriz* matriz;

   FILE * arq; //descritor do arquivo de entrada
   size_t ret; //retorno da funcao de leitura no arquivo de entrada

   //recebe os argumentos de entrada
   if(argc < 2) {
      fprintf(stderr, "Digite: %s <arquivo entrada>\n", argv[0]);
      return 1;
   }
   
   //abre o arquivo para leitura binaria
   arq = fopen(argv[1], "rb");
   if(!arq) {
      fprintf(stderr, "Erro de abertura do arquivo\n");
      return 2;
   }

   //le as dimensoes da matriz
   ret = fread(&matriz->linha, sizeof(int), 1, arq);
   if(!ret) {
      fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo \n");
      return 3;
   }
   ret = fread(&matriz->coluna, sizeof(int), 1, arq);
   if(!ret) {
      fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo \n");
      return 3;
   }
   matriz->tam = matriz->linha * matriz->coluna; //calcula a qtde de elementos da matriz

   //aloca memoria para a matriz
   matriz = (float*) malloc(sizeof(float) * matriz->tam);

   if(!matriz) {
      fprintf(stderr, "Erro de alocao da memoria da matriz\n");
      return 3;
   }

   //carrega a matriz de elementos do tipo float do arquivo
   ret = fread(matriz, sizeof(float), tam, arq);
   if(ret < matriz->tam) {
      fprintf(stderr, "Erro de leitura dos elementos da matriz\n");
      return 4;
   }

   //imprime a matriz na saida padrao
   for(int i=0; i<matriz->linha; i++) { 
      for(int j=0; j<matriz->coluna; j++)
        fprintf(stdout, "%.6f ", matriz[i*matriz->coluna+j]);
      fprintf(stdout, "\n");
   }

   //finaliza o uso das variaveis
   fclose(arq);
   return matriz;
   //free(matriz);
}

//fluxo principal
int main(int argc, char* argv[]) {
   Matriz *mat1;
   Matriz *mat2;
   Matriz *matRet;
   long long int qtElementos;
   pthread_t *tid; //identificadores das threads no sistema
   tArgs *args; //identificadores locais das threads e dimensao
   double inicio, fim, delta;

   //alocacao de memoria para as estruturas de dados
*-
   
   GET_TIME(inicio);
   //leitura e avaliacao dos parametros de entrada
   if(argc<4) {
      printf("Digite: %s <arq mat 1> <arq mat 2> <arq saida> <numero de threads>\n", argv[0]);
      return 1;
   }

   // lê arquivos das matrizes pegando linha1, coluna1, l2, c2 e calculando qt de elementos
   mat1 = leMatrizBinario();
   // confere se pd fazer a multiplicacao delas
    qtElementos = coluna1 * linha2;
    if (nthreads > qtElementos) nthreads = qtElementos;



   //inicializacao das estruturas de dados de entrada e saida
   for(int i=0; i<dim; i++) {
      for(int j=0; j<dim; j++)
         mat[i*dim+j] = 1;    //equivalente mat[i][j]
      vet[i] = 1; 
      saida[i] = 0;
   }

   GET_TIME(fim);
   delta = fim - inicio;
   printf("Tempo inicializacao:%lf\n", delta);

   //multiplicacao da matriz pelo vetor
   GET_TIME(inicio);
   //alocacao das estruturas
   tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
   if(tid==NULL) {puts("ERRO--malloc"); return 2;}
   args = (tArgs*) malloc(sizeof(tArgs)*nthreads);
   if(args==NULL) {puts("ERRO--malloc"); return 2;}
   //criacao das threads
   for(int i=0; i<nthreads; i++) {
      (args+i)->id = i;
      (args+i)->dim = dim;
      if(pthread_create(tid+i, NULL, tarefa, (void*) (args+i))){
         puts("ERRO--pthread_create"); return 3;
      }
   } 
   //espera pelo termino da threads
   for(int i=0; i<nthreads; i++) {
      pthread_join(*(tid+i), NULL);
   }
   GET_TIME(fim)   
   delta = fim - inicio;
   printf("Tempo multiplicacao (dimensao %d) (nthreads %d): %lf\n", dim, nthreads, delta);

   //liberacao da memoria
   GET_TIME(inicio);
   free(mat);
   free(vet);
   free(saida);
   free(args);
   free(tid);
   GET_TIME(fim)   
   delta = fim - inicio;
   printf("Tempo finalizacao:%lf\n", delta);

   return 0;
}