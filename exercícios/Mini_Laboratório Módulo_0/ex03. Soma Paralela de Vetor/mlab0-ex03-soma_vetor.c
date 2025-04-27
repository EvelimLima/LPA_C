#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Exercicio 03 - Soma Vetor
// Descrição: Soma os elementos de um vetor de forma paralela, dividindo entre múltiplas threads.

typedef struct {
    int* vetor;
    int inicio;
    int fim;
    int* soma_parcial; // ponteiro para a posição onde a thread vai armazenar a soma
} ThreadArgs;

void* thread_func(void* arg) {
    ThreadArgs* args = (ThreadArgs*) arg;
    int soma = 0;

    for (int i = args->inicio; i < args->fim; i++) {
        soma += args->vetor[i];
    }

    *(args->soma_parcial) = soma;

    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    printf("Executando soma_vetor...\n");

    int tamanho = 100;
    int num_threads = 4;

    int* vetor = malloc(tamanho * sizeof(int));
    for (int i = 0; i < tamanho; i++) {
        vetor[i] = i + 1;
    }

    pthread_t threads[num_threads];
    ThreadArgs args[num_threads];
    int somas[num_threads]; // vetor de somas parciais
    int bloco = tamanho / num_threads;

    for (int i = 0; i < num_threads; i++) {
        args[i].vetor = vetor;
        args[i].inicio = i * bloco;
        args[i].fim = (i == num_threads - 1) ? tamanho : (i + 1) * bloco;
        args[i].soma_parcial = &somas[i];

        pthread_create(&threads[i], NULL, thread_func, &args[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    int soma_total = 0;
    for (int i = 0; i < num_threads; i++) {
        soma_total += somas[i];
    }

    printf("Soma total: %d\n", soma_total);

    free(vetor);
    return 0;
}
