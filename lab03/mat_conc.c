/* Multiplicacao de duas amtrizes de forma concorrente */
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"


// struct para facilitar a manipulação das matrizes e as informações importantes sobre elas que utilizaremos 
typedef struct{
    float *matriz;
    int linha;
    int coluna;
    int tam;
} Matriz;

Matriz *mat1; // matriz de entrada 1
Matriz *mat2; // matriz de entrada 1
Matriz *matRet; // matriz de saída

typedef struct{
// delimitação de quantas multiplicações serão feitas por thread
   int inicio; 
   int fim; 
} tArgs;

// função para ler os arquivos binários que contém as matrizes
Matriz* leMatrizBinario(const char* filename) {
    Matriz* matriz = (Matriz*) malloc(sizeof(Matriz));

    if (!matriz) {
        fprintf(stderr, "Erro de alocacao da memoria da matriz\n");
        return NULL;
    }

    FILE *arq = fopen(filename, "rb");
    if (!arq) {
        fprintf(stderr, "Erro de abertura do arquivo\n");
        free(matriz);
        return NULL;
    }

    size_t ret;
    ret = fread(&matriz->linha, sizeof(int), 1, arq);
    if (ret == 0) {
        fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo\n");
        fclose(arq);
        free(matriz);
        return NULL;
    }

    ret = fread(&matriz->coluna, sizeof(int), 1, arq);
    if (ret == 0) {
        fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo\n");
        fclose(arq);
        free(matriz);
        return NULL;
    }

    matriz->tam = matriz->linha * matriz->coluna;

    matriz->matriz = (float*) malloc(sizeof(float) * matriz->tam);
    if (!matriz->matriz) {
        fprintf(stderr, "Erro de alocao da memoria da matriz\n");
        fclose(arq);
        free(matriz);
        return NULL;
    }

    ret = fread(matriz->matriz, sizeof(float), matriz->tam, arq);
    if (ret < matriz->tam) {
        fprintf(stderr, "Erro de leitura dos elementos da matriz\n");
        fclose(arq);
        free(matriz->matriz);
        free(matriz);
        return NULL;
    }

    fclose(arq);
    return matriz;
}

// função que as threads executarão (main das threads)
void *multiplicacaoMatrizes(void *arg) {
    tArgs *args = (tArgs*) arg;

    for (int i = args->inicio; i < args->fim; i++) {
        for (int j = 0; j < mat2->coluna; j++) {
            float soma = 0.0;
            for (int k = 0; k < mat1->coluna; k++) {
                soma += mat1->matriz[i * mat1->coluna + k] * mat2->matriz[k * mat2->coluna + j];
            }
            matRet->matriz[i * mat2->coluna + j] = soma;
        }
    }
   
    pthread_exit(NULL);
}


// fluxo principal (único)
int main(int argc, char* argv[]) {
    long long int qtElementos;
    int nthreads;
    double inicio, fim, delta;
    FILE *arqSaida;
    size_t ret; // retorno da função de escrita no arquivo de saída
    pthread_t *tid; //identificadores das threads no sistema
    tArgs *args; //identificadores locais das threads e dimensao

    GET_TIME(inicio);

    // leitura e avaliação dos parâmetros de entrada
    if (argc < 5) {
        printf("Digite: %s <arquivo mattriz 1> <arquivo matriz 2> <arquivo matriz saída> <qt threads\n", argv[0]);
        return 1;
    }

    // lê arquivos das matrizes pegando linha1, coluna1, linha2, coluna2 e calculando qt de elementos
    mat1 = leMatrizBinario(argv[1]);
    mat2 = leMatrizBinario(argv[2]);

    nthreads = atoi(argv[4]); // número de threads

    if (!mat1 || !mat2) return 2;

   // checa se é possível fazer a multiplicação das duas matrizes
    if (mat1->coluna != mat2->linha) {
        fprintf(stderr, "Erro: Dimensões incompatíveis\n");
        free(mat1->matriz);
        free(mat1);
        free(mat2->matriz);
        free(mat2);
        return 2;
    }

    qtElementos = mat1->linha * mat2->coluna;

    matRet = (Matriz*) malloc(sizeof(Matriz)); // aloca espaço para a struct da matriz de retorno

    if (!matRet) {
        fprintf(stderr, "Erro de alocao da memoria da matriz\n");
        free(mat1->matriz);
        free(mat1);
        free(mat2->matriz);
        free(mat2);
        return 3;
    }

    // insere as informações da matriz de retorno na struct dela
    matRet->linha = mat1->linha;
    matRet->coluna = mat2->coluna;
    matRet->tam = qtElementos;
    matRet->matriz = (float*) malloc(sizeof(float) * qtElementos);

    if (!matRet->matriz) {
        fprintf(stderr, "Erro de alocao da memoria da matriz\n");
        free(matRet);
        free(mat1->matriz);
        free(mat1);
        free(mat2->matriz);
        free(mat2);
        return 2;
    }

    //alocacao das estruturas pras threads 
    tid = (pthread_t*) malloc(sizeof(pthread_t) * nthreads);
    args = (tArgs*) malloc(sizeof(tArgs) * nthreads);
    if (!tid || !args) {
        fprintf(stderr, "Erro de alocacao da memoria das threads pu dos argumentos\n");
        return 3;
    } 


    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo inicializacao (concorrente): %lf\n", delta);

    // multiplicação das duas matrizes de forma sequencial
    GET_TIME(inicio);

    
     // criação das threads e distribuição das tarefas

    blocos = mat1->linha / nthreads; // número de linhas que cada thread vai processar
    resto = mat1->linha % nthreads;
    inicioLinha = 0;

    for (int i = 0; i < nthreads; i++) {
        args[i].inicio = inicioLinha;
        args[i].fim = inicioLinha + blocos + (i < resto ? 1 : 0); // a sobra fica com as primeiras "resto" threads
        inicioLinha = args[i].fim;
        if (pthread_create(&tid[i], NULL, multiplicacaoMatrizes, (void*) &args[i])) {
            fprintf(stderr, "Erro ao criar thread\n");
            return 3;
        }
    }

    //espera pelo termino da threads
    for(int i=0; i<nthreads; i++) {
        pthread_join(*(tid+i), NULL);
    }

    // Liberação da memória
    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo multiplicação (concorrente): %lf\n", delta);

    GET_TIME(inicio);
    // abre o arquivo de saída para escrita binária
    arqSaida = fopen(argv[3], "wb");
    if (!arqSaida) {
        fprintf(stderr, "Erro de abertura do arquivo\n");
        free(matRet->matriz);
        free(matRet);
        return 3;
    }

    // escreve o número de linhas e de colunas da matriz de saída
    ret = fwrite(&matRet->linha, sizeof(int), 1, arqSaida);
    ret = fwrite(&matRet->coluna, sizeof(int), 1, arqSaida);

    // escreve os elementos da matriz
    ret = fwrite(matRet->matriz, sizeof(float), matRet->tam, arqSaida);
    if (ret < matRet->tam) {
        fprintf(stderr, "Erro de escrita no arquivo\n");
        fclose(arqSaida);
        free(matRet->matriz);
        free(matRet);
        return 4;
    }


    fclose(arqSaida);
    free(mat1->matriz);
    free(mat1);
    free(mat2->matriz);
    free(mat2);
    free(matRet->matriz);
    free(matRet);
    free(tid);
    free(args);
    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo finalização (concorrente): %lf\n", delta);

    return 0;
}
