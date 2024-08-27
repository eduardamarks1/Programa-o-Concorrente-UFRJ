/*
Programa auxiliar para gerar dois vetores aleatórios de floats com dimensão N e salvar em um arquivo binário
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 1000 
#define TEXTO

double calcula_produto_interno(float *vetor1, float *vetor2, int dimensao){
    double produto_interno = 0.0;
    for(long int i=0; i<dimensao; i++) {
      produto_interno += vetor1[i] * vetor2[i];
    }
    return produto_interno;
}

int main(int argc, char*argv[]) {
   float *vetor1; 
   float *vetor2;

   long int n; // tde de elementos do vetor que serao gerados
   float elem; // valor gerado para incluir no vetor
   double produto_interno = 0; // produto interno entre os dois vetores gerados
   int fator=1; // fator multiplicador para gerar números negativos
   FILE * arquivo; // descritor do arquivo de saida
   size_t ret; // retorno da funcao de escrita no arquivo de saida

   
   if(argc < 3) {
      fprintf(stderr, "Digite: %s <dimensao> <arquivo saida>\n", argv[0]);
      return 1;
   }

   n = atoi(argv[1]);

   
   vetor1 = (float*) malloc(sizeof(float) * n);
   if(!vetor1) {
      fprintf(stderr, "Erro de alocao da memoria do vetor\n");
      return 2;
   }

   vetor2 = (float*) malloc(sizeof(float) * n);
   if(!vetor2) {
      fprintf(stderr, "Erro de alocao da memoria do vetor\n");
      return 2;
   }


   //preenche ambos os vetores com valores float gerados aleatoriamente
   srand(time(NULL));

   for(long int i=0; i<n; i++){
        elem = (rand() % MAX)/3.0 * fator;
        vetor1[i] = elem;
        fator*=-1;
    }

    for(long int i=0; i<n; i++){
        elem = (rand() % MAX)/3.0 * fator;
        vetor2[i] = elem;
        fator*=-1;
    }

    produto_interno = calcula_produto_interno(vetor1,vetor2,n);
   
   //escreve o vetor no arquivo
   arquivo = fopen(argv[2], "wb");    //abre o arquivo para escrita binaria

   if(!arquivo) {
      fprintf(stderr, "Erro de abertura do arquivo\n");
      return 3;
   }
   //escreve a dimensao
   ret = fwrite(&n, sizeof(long int), 1, arquivo);

   //escreve os elementos do vetor1 no arquivo
   ret = fwrite(vetor1, sizeof(float), n, arquivo);
   if(ret < n) {
      fprintf(stderr, "Erro de escrita no  arquivo\n");
      return 4;
   }

    //escreve os elementos do vetor2 no arquivo
   ret = fwrite(vetor2, sizeof(float), n, arquivo);
   if(ret < n) {
      fprintf(stderr, "Erro de escrita no  arquivo\n");
      return 4;
   }

   //escreve o produto_interno no arquivo
   ret = fwrite(&produto_interno, sizeof(double), 1, arquivo);

   
    #ifdef TEXTO
    fprintf(stdout, "%ld\n", n);
    for(long int i=0; i<n; i++) {
        fprintf(stdout, "%f ",vetor1[i]);
    }
    fprintf(stdout, "\n");
    for(long int i=0; i<n; i++) {
        fprintf(stdout, "%f ",vetor2[i]);
    }
    fprintf(stdout, "\n");
    fprintf(stdout, "produto interno = %lf\n", produto_interno);
    #endif

   fclose(arquivo);
   free(vetor1);
   free(vetor2);
   return 0;
} 