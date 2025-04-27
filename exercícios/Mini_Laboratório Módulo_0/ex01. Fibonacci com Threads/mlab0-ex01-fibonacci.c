#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Exercicio 01 - Fibonacci
// Descrição: Calcula o n-ésimo número da sequência de Fibonacci usando chamadas recursivas com threads.

//struct que encapsula o valor de entrada e o resultado
typedef struct {
    int n;
    long resultado;
} ThreadArgs;


void* thread_func(void* arg) {
    ThreadArgs* args = (ThreadArgs*) arg;

    //Caso base ou limite inferior para não criar threads
    if (args->n <= 5) {
        long a = 0, b = 1, temp;
        for (int i = 2; i <= args->n; i++) {
            temp = a + b;
            a = b;
            b = temp;
        }
        args->resultado = (args->n == 0) ? 0 : b;
        pthread_exit(NULL);
    }

    //Aloca dinamicamente argumentos para as threads filhas
    ThreadArgs* a1 = malloc(sizeof(ThreadArgs));
    ThreadArgs* a2 = malloc(sizeof(ThreadArgs));
    a1->n = args->n - 1;
    a2->n = args->n - 2;

    pthread_t t1, t2;

    //Cri threads recursivamente
    pthread_create(&t1, NULL, thread_func, a1);
    pthread_create(&t2, NULL, thread_func, a2);

    //espera as threads filhas finalizarem
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    //Soma os resultados
    args->resultado = a1->resultado + a2->resultado;

    //Libera memória alocada
    free(a1);
    free(a2);

    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {//número correto de argumentos 
        return 1;
    }

    int n = atoi(argv[1]);
    if (n < 0) {
        printf("Número inválido\n");
        return 1;
    }

    ThreadArgs args = { .n = n };
    pthread_t thread;

    printf("Calculando fibonacci(%d)...\n", n);

    pthread_create(&thread, NULL, thread_func, &args);
    pthread_join(thread, NULL);

    printf("Resultado: %ld\n", args.resultado);
    return 0;
}
