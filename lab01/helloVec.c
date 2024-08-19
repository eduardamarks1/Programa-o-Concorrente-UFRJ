/* Disciplina: Programacao Concorrente */
/* Profa.: Silvana Rossetto */
/* Laboratório: 1 */
/* Codigo: programa concorrente com M threads, para somar 1 a cada elemento
de um vetor de N elementos do tipo inteiro, dividindo a tarefa entre as M threads de forma mais balanceada
posssível */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <time.h>

// argumentos da thread
typedef struct {
   int idThread, M, inicio, fim;
   int* vetor;
} t_Args;

// inicializa o vetor com valores aleatórios
void preencherVetor(int *vetor, int N){
    int num = 0;
    printf("\t Vetor de %d posições criado: \n", N);
    for(int i = 0; i < N; i++){
        num = rand() % 10;
        vetor[i] = num;
        printf("[%d]: %d\n", i, num);
    }
}

// exibe os valores do vetor
void printaVetor(int *vetor, int N){
    printf("\n");
    for(int i = 0; i < N; i++){
        printf("[%d]: %d \n", i, vetor[i]);
    }
}

// função executada pelas threads (main das threads)
void *soma_um (void *arg) {
    t_Args *args = (t_Args *) arg;

    for(int i = args->inicio; i < args->fim; i++) {
        args->vetor[i] += 1; 
    }

    free(arg); // libera a alocação feita na main
    pthread_exit(NULL);
}

void verificarIncremento(int *vetorOriginal, int *vetor, int N){
    int erros = 0;
    int i;
    for(i = 0; i < N; i++){
        if(vetor[i] != vetorOriginal[i] + 1) {
            printf("---ERRO: Houve um erro no incremento na posição [%d] do vetor.\n", i);
            erros++;
        }
    }
    if(erros == 0){ 
        printf("---Todas as posições foram incrementadas corretamente.\n");
    }
}

// função principal do programa
int main(int argc, char* argv[]) {

    int M; // qtde de threads que serão criadas (recebida na linha de comando)
    int N; // tamanho do vetor
    int *vetor, *vetorOriginal;

    // verifica se o argumento 'qtde de threads' e 'tamanho do vetor' foram passados
    if(argc < 3) {
        printf("--ERRO: informe a qtde de threads e o tamanho do vetor <%s> <N> <M>\n", argv[0]);
        return 1;
    }

    // os valores de M e N devem são informados na chamada do programa
    N = atoi(argv[1]);
    M = atoi(argv[2]);
    if (M <= 0 || N < M) {
        printf("--ERRO: quantidade de threads deve ser positiva e o tamanho do vetor deve ser maior ou igual ao número de threads\n");
        return 1;
    }

    // aloca memória para o vetor que o usuário deu de input e p/ que as threads vão retornar
    vetorOriginal = (int *)malloc(N * sizeof(int));
    vetor = (int *)malloc(N * sizeof(int));

    if (vetor == NULL || vetorOriginal == NULL) {
        printf("Erro ao alocar memória.\n");
        return 1;
    }
  
    srand(time(NULL)); 
    // inicializa o vetor com valores aleatórios
    preencherVetor(vetor, N);
  
    // copia os vetores para fazer a comparação no final
    for (int i = 0; i < N; i++) {
        vetorOriginal[i] = vetor[i];
    }
    

    pthread_t tid_sistema[M];
    int elementosPorThread = N / M;
    int resto = N % M;

    // cria as threads
    for(int i = 0; i < M; i++) {
        //printf("--Aloca e preenche argumentos para thread %d\n", i);
        t_Args *args = malloc(sizeof(t_Args));
        if (args == NULL) {
            printf("--ERRO: malloc()\n"); 
            return 1;
        }
        args->idThread = i; 
        args->M = M; 

        // define a posicao de inicio do vetor em q cada thread ira atuar 
        if (i < resto) {
            args->inicio = i * elementosPorThread + i;
        } else {
            args->inicio = i * elementosPorThread + resto;
        }

        // define a posicao do fim do vetor em q cada thread ira atuar   
        if (i < resto){
            args->fim = args->inicio + elementosPorThread + 1;
        }
        else{
            args->fim = args->inicio + elementosPorThread;
        }

        args->vetor = vetor;
        
         if (pthread_create(&tid_sistema[i], NULL, soma_um, (void*) args)) {
            printf("--ERRO: pthread_create()\n"); 
            return 2;
        }
    }

    // espera todas as threads terminarem
    for (int i = 0; i < M; i++) {
        if (pthread_join(tid_sistema[i], NULL)) {
            printf("--ERRO: pthread_join() da thread %d\n", i); 
        } 
    }

    printf("\t Vetor incrementado em 1 em todas as posições: \n");
    printaVetor(vetor, N);

    // verifica se o vetor foi incrementado corretamente
    verificarIncremento(vetorOriginal, vetor, N);
   

    free(vetor);
    free(vetorOriginal);

    return 0;
}
