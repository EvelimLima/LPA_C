#include <stdio.h>
#include <pthread.h>
// 2, 4, 8, 16
#define N_THREADS 8
#define N_ITERS 1000000

int contador = 0;
pthread_spinlock_t splock;

void* tarefa(void* arg) {
    for (int i = 0; i < N_ITERS; i++) {
        pthread_spin_lock(&splock);
        contador++;
        pthread_spin_unlock(&splock);
    }
    return NULL;
}

int main() {
    pthread_t threads[N_THREADS];
    pthread_spin_init(&splock, PTHREAD_PROCESS_PRIVATE);
    for (int i = 0; i < N_THREADS; i++)
        pthread_create(&threads[i], NULL, tarefa, NULL);
    for (int i = 0; i < N_THREADS; i++)
        pthread_join(threads[i], NULL);
    printf("Contador final: %d (esperado: %d)\n", contador, N_THREADS * N_ITERS);
    pthread_spin_destroy(&splock);
    return 0;
}