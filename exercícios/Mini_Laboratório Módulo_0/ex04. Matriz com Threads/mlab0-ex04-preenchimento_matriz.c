#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

//Exercício 04: Preenchimento de Matriz com Threads. ma matriz N x M com malloc e usar T threads 
//para preencher as linhas com números aleatórios entre 0 e 99.

// Mutex para proteger rand() se usado
pthread_mutex_t rand_mutex;

typedef struct {
    int** matriz;
    int inicio;
    int fim;
    int colunas;
    unsigned int seed;
} ThreadArgs;

void* thread_func(void* arg) {
    ThreadArgs* args = (ThreadArgs*) arg;

    for (int i = args->inicio; i < args->fim; i++) {
        for (int j = 0; j < args->colunas; j++) {
            // rand_r é thread-safe, diferente de rand()
            args->matriz[i][j] = rand_r(&(args->seed)) % 100;
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    printf("Executando preenchimento_matriz...\n");

    int linhas = 10;
    int colunas = 5;
    int num_threads = 2;

    // Alocando matriz dinamicamente
    int** matriz = malloc(linhas * sizeof(int*));
    for (int i = 0; i < linhas; i++) {
        matriz[i] = malloc(colunas * sizeof(int));
    }

    pthread_t threads[num_threads];
    ThreadArgs args[num_threads];

    int bloco = linhas / num_threads;

    for (int i = 0; i < num_threads; i++) {
        args[i].matriz = matriz;
        args[i].inicio = i * bloco;
        args[i].fim = (i == num_threads - 1) ? linhas : (i + 1) * bloco;
        args[i].colunas = colunas;
        args[i].seed = time(NULL) + i; // seed diferente por thread

        pthread_create(&threads[i], NULL, thread_func, &args[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    //Exibe a matriz preenchida
    printf("Matriz preenchida:\n");
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            printf("%2d ", matriz[i][j]);
        }
        printf("\n");
    }

    //libera a matriz
    for (int i = 0; i < linhas; i++) {
        free(matriz[i]);
    }
    free(matriz);

    return 0;
}
