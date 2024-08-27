/* Disciplina: Computacao Concorrente */
/* Profa.: Silvana Rossetto */
/* Lab2:  */
/* Codigo: um programa concorrente em C que recebe como entrada o numero de 
threads T e um nome de arquivo, carrega desse arquivo a dimensao N e dois
vetores de entrada, executa o calculo do produto interno desses dois vetores dividindo a tarefa entre as T threads de forma balanceada, e ao final compare o
valor calculado com o valor registrado no arquivo de entrada. */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h> 


// dimensao do vetor
long int n;

//vetores 
float *vet1;
float *vet2;

//numero de threads
int nthreads;

// funcao pra calcular o produto interno de dois vetores de forma concorrente
double calcula_produto_interno_conc(float *vetor1, float *vetor2, int inicio, int fim){
    double produto_interno = 0.0;
    for(long int i=inicio; i<fim; i++) {
      produto_interno += vetor1[i] * vetor2[i];
    }
    return produto_interno;
}

// main das threads

//estrategia de divisao de tarefas: blocos de n/nthreads elementos

void *ProdutoInterno (void *tid) {

  long int id = (long int) tid; //identificador da thread
  int ini, fim, bloco; // auxiliares para divisao do vetor em blocos
  float *ret; // produto interno local
  double produto_interno_local = 0.0;
  
  bloco = n/nthreads; //tamanho do bloco de dados de cada thread

  ini = id*bloco; //posicao inicial do vetor
  fim = ini + bloco; //posicao final do vetor
  if (id==(nthreads-1)) fim = n; //a ultima thread trata os elementos restantes no caso de divisao nao exata

  //calcula o produto interno
  produto_interno_local = calcula_produto_interno_conc(vet1,vet2,ini,fim);

  //retorna o resultado do produto interno 
  ret = malloc(sizeof(float));
  if (ret!=NULL) *ret = produto_interno_local;
  else printf("--ERRO: malloc() thread\n");
  pthread_exit((void*) ret);

}

//funcao principal do programa

int main(int argc, char *argv[]) {
  FILE *arq; 
  size_t ret; //retorno da funcao de leitura no arquivo de entrada
	      
  double produto_interno_ori; // produto interno registrado no arquivo original 
  float pi_seq, pi_seq_blocos, pi_par_global; //resultados dos produtos internos
  float pi1, pi2; //auxiliares para o cálculo do produto interno sequencial alternado
  float *pi_retorno_threads; //auxiliar para retorno das threads


  pthread_t *tid_sistema; //vetor de identificadores das threads no sistema

  //valida e recebe os valores de entrada
  if(argc < 3) { printf("Use: %s <arquivo de entrada> <numero de threads> \n", argv[0]); exit(-1); }


  arq = fopen(argv[1], "rb");
  if(arq==NULL) { printf("--ERRO: fopen()\n"); exit(-1); }

  //le o tamanho do vetor (primeira linha do arquivo)
  ret = fread(&n, sizeof(long int), 1, arq);
  if(!ret) {
     fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo \n");
     return 3;
  }

  //aloca espaco de memoria e carrega os vetores de entrada lidos a partir do arquivo binário

  vet1 = malloc (sizeof(float) * n);
  vet2 = malloc (sizeof(float) * n);

  if(vet1==NULL) { printf("--ERRO: malloc()\n"); exit(-1); }
  ret = fread(vet1, sizeof(float), n, arq);
  if(ret < n) {
     fprintf(stderr, "Erro de leitura dos elementos do vetor\n");
     return 4;
  }

  if(vet2==NULL) { printf("--ERRO: malloc()\n"); exit(-1); }
  ret = fread(vet2, sizeof(float), n, arq);
  if(ret < n) {
     fprintf(stderr, "Erro de leitura dos elementos do vetor\n");
     return 4;
  }


  //le o numero de threads da entrada do usuario 
  nthreads = atoi(argv[2]);
  //limita o numero de threads ao tamanho do vetor
  if(nthreads>n) nthreads = n;

  //aloca espaco para o vetor de identificadores das threads no sistema
  tid_sistema = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
  if(tid_sistema==NULL) { printf("--ERRO: malloc()\n"); exit(-1); }

  //cria as threads
  for(long int i=0; i<nthreads; i++) {
    if (pthread_create(&tid_sistema[i], NULL, ProdutoInterno, (void*) i)) {
       printf("--ERRO: pthread_create()\n"); exit(-1);
    }
  }

  // cálculo sequencial do produto interno sequencial
pi_seq = 0;
 for(long int i=n; i>=0; i--) {
      pi_seq += vet1[i] * vet2[i];
 }

  // produto iinterno sequencial bloco (== dividindo em 2 threads)
  pi1=0;
  pi1 = calcula_produto_interno_conc(vet1, vet2, 0, n/2);
  pi2=0;
  pi2 = calcula_produto_interno_conc(vet1, vet2, n/2, n);
  pi_seq_blocos = pi1 + pi2;
 
  //espera todas as threads terminarem e calcula a soma total das threads
  //retorno = (float*) malloc(sizeof(float));
  pi_par_global=0;
  for(int i=0; i<nthreads; i++) {
     if (pthread_join(tid_sistema[i], (void *) &pi_retorno_threads)) {
        printf("--ERRO: pthread_join()\n"); exit(-1);
     }
     pi_par_global += *pi_retorno_threads;
     free(pi_retorno_threads);
  }

  //imprime os resultados
  printf("\n");
  printf("pi_seq (invertida)         = %.26f\n\n", pi_seq);
  printf("pi_seq_blocos (2 blocos)   = %.26f\n\n", pi_seq_blocos);
  printf("pi_concorrente             = %.26f\n", pi_par_global);
 
  //le o somatorio registrado no arquivo
  ret = fread(&produto_interno_ori, sizeof(double), 1, arq); 
  printf("\nProduto_Interno_Original                   = %.26lf\n", produto_interno_ori);


  free(vet1);
  free(vet2);
  free(tid_sistema);
  fclose(arq);

  return 0;
}