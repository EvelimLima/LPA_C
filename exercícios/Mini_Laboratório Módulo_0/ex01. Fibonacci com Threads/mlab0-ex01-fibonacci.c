#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Exercicio 01 - Fibonacci
// Descrição: Calcula o n-ésimo número da sequência de Fibonacci usando chamadas recursivas com threads.

// Estrutura que armazena os argumentos e o resultado
typedef struct {
    int n;
    long resul;
} ThreadArgs;

// Função que será executada por cada thread
void* thread_func(void* arg) {
    ThreadArgs* args = (ThreadArgs*) arg;

    // Caso base da recursão
    if(args->n <= 1){
        args->resul = args->n;
        pthread_exit(NULL);
    }

    // Aloca argumentos para as threads filhas
    ThreadArgs* a1 = malloc(sizeof(ThreadArgs));
    ThreadArgs* a2 = malloc(sizeof(ThreadArgs));

    if(!a1 || !a2){
        printf("erro na alocação");
        pthread_exit(NULL);
    }

    a1->n = args->n - 1;
    a2->n = args->n - 2;

    pthread_t t1, t2;

    // Cria as threads filhas
    if(pthread_create(&t1, NULL, thread_func, a1) != 0 || 
        pthread_create(&t2, NULL, thread_func, a2) != 0) {
        printf("erro ao ciras threads");
        // Libera memória alocada
        free(a1);
        free(a2);
        pthread_exit(NULL);

    }

    // Espera as threads terminarem
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    // Soma os resultados das threads filhas
    args->resul = a1->resul + a2->resul;

    free(a1);
    free(a2);
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    if (argc != 2){
        fprintf(stderr, "Uso: %s <número>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if(n < 0){
        fprintf(stderr, "Número inválido\n");
        return 1;
    }

    ThreadArgs args = { .n = n };
    pthread_t thread;

    printf("Executando fibonacci(%d)...\n", n);

    // Cria a thread principal para iniciar o cálculo
    if (pthread_create(&thread, NULL, thread_func, &args) != 0){
        printf("erro thread principal");
        return 1;
    }
    pthread_join(thread, NULL);

    // Imprime o resultado
    printf("Resultado %ld\n", args.resul);
    return 0;
}
