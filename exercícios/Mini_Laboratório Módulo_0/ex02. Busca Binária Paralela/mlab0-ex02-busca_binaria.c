#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Variável global para armazenar o resultado da busca
int resultado_global = -1;
pthread_mutex_t lock;

typedef struct {
    int* vetor;
    int inicio;
    int fim;
    int chave;
} ThreadArgs;

// Função de busca binária executada por cada thread
void* thread_func(void* arg) {
    ThreadArgs* args = (ThreadArgs*) arg;
    int ini = args->inicio;
    int fim = args->fim;
    int* v = args->vetor;
    int chave = args->chave;

    while (ini <= fim) {
        int meio = (ini + fim) / 2;

        pthread_mutex_lock(&lock);
        if (resultado_global != -1) {
            // Se já foi encontrado por outra thread, libera e sai
            pthread_mutex_unlock(&lock);
            pthread_exit(NULL);
        }
        pthread_mutex_unlock(&lock);

        if (v[meio] == chave) {
            pthread_mutex_lock(&lock);
            resultado_global = meio;
            pthread_mutex_unlock(&lock);
            pthread_exit(NULL);
        } else if (v[meio] < chave) {
            ini = meio + 1;
        } else {
            fim = meio - 1;
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    printf("Executando busca_binaria...\n");

    if (argc != 2) {
        fprintf(stderr, "Uso: %s <valor_a_buscar>\n", argv[0]);
        return 1;
    }

    int chave = atoi(argv[1]);

    // Vetor ordenado de exemplo
    int vetor[] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19,
                   21, 23, 25, 27, 29, 31, 33, 35, 37, 39};
    int tamanho = sizeof(vetor) / sizeof(int);

    // Inicializa o mutex
    pthread_mutex_init(&lock, NULL);

    // Prepara os argumentos para as duas threads
    ThreadArgs args1 = { .vetor = vetor, .inicio = 0, .fim = (tamanho / 2) - 1, .chave = chave };
    ThreadArgs args2 = { .vetor = vetor, .inicio = (tamanho / 2), .fim = tamanho - 1, .chave = chave };

    pthread_t t1, t2;

    pthread_create(&t1, NULL, thread_func, &args1);
    pthread_create(&t2, NULL, thread_func, &args2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    if (resultado_global != -1) {
        printf("Valor %d encontrado na posição %d\n", chave, resultado_global);
    } else {
        printf("Valor %d não encontrado no vetor.\n", chave);
    }

    pthread_mutex_destroy(&lock);
    return 0;
}
