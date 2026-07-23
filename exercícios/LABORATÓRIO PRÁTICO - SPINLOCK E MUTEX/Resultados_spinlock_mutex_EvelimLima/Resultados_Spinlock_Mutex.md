Aluno(a): Evelim Lima

# Laboratório: Controle de Concorrência com Spinlock e Mutex

## Informações gerais

- **N_ITERS**: 1.000.000
- **Valores testados de N_THREADS**: 2, 4, 8, 16

## Códigos Utilizados (spinlock e mutex):

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

---

#include <stdio.h>
#include <pthread.h>
// 2, 4, 8, 16
#define N_THREADS 16
#define N_ITERS 1000000

int contador = 0;
pthread_mutex_t lock;

void* tarefa(void* arg) {
    for (int i = 0; i < N_ITERS; i++) {
        pthread_mutex_lock(&lock);
        contador++;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main() {
    pthread_t threads[N_THREADS];
    pthread_mutex_init(&lock, PTHREAD_PROCESS_PRIVATE);
    for (int i = 0; i < N_THREADS; i++)
        pthread_create(&threads[i], NULL, tarefa, NULL);
    for (int i = 0; i < N_THREADS; i++)
        pthread_join(threads[i], NULL);
    printf("Contador final: %d (esperado: %d)\n", contador, N_THREADS * N_ITERS);
    pthread_mutex_destroy(&lock);
    return 0;
}

---

## Resultados dos Testes

| Nro Threads | Tempo (Spinlock) | Tempo (Mutex) | Contador Correto? | Observações Spinlock    | Observações Mutex |
|-------------|------------------|----------------|------------------|-------------------------|-------------------|
| 8           | 0m0.048s         |                | Não              | Race condicion, resultado incorreto |
| 8           |                  | 0m0.049s       | Não              |                                     | Race condicion, sincronização desativada |
| 2           | 0m0.098s         | 0m0.074s       | Sim              | Tempo bom | Execução baixo rápida |
| 4           | 0m0.148s         | 0m0.161s       | Sim              | Boa performace | Um pouco mais lento que o spinlock | 
| 8           | 0m0.989s         | 0m0.301s       | Sim              | Tempo cresceu com mais threads | Mais eficiente que o spinlock |
| 16          | 0m1.807s         | 0m0.605s       | Sim              | Uso de CPU aumentou bastante | Melhor desempenho |

---

## Análise

### 1. Quando usar **spinlock**?

Spinlock é útil em cenários onde o tempo de espera é esperado ser muito curto, como em sistemas com poucos threads ou quando a seção crítica é rápida de ser executada. O spinlock evita o overhead de bloqueios pesados (como mutexes), mas o custo de "girar" enquanto espera pode ser prejudicial em sistemas com muitos threads, onde o lock permanece ocupado por mais tempo, resultando em um alto uso de CPU.

---

### 2. Quando usar **mutex**?

Mutex é preferível em cenários em que a seção crítica é mais complexa ou onde o número de threads é grande, e os bloqueios podem ser mais demorados. Ao contrário do spinlock, o mutex coloca a thread em espera até que o bloqueio seja liberado, o que é mais eficiente em ambientes com maior contenda de recursos. Ele é ideal para evitar o consumo excessivo de CPU, especialmente em situações de maior concorrência.

---

### 3. Qual teve melhor desempenho?

O mutex apresentou um desempenho melhor em ambientes com maior número de threads, como no teste com 8 e 16 threads. Embora o spinlock tenha se mostrado mais rápido com 2 e 4 threads, o mutex foi mais eficiente em configurações de maior contenda, evitando o consumo excessivo de CPU. Conclusão, o desempenho do mutex tende a ser melhor quando a sincronização é mais complexa e as threads são muitas, pois o mutex lida melhor com bloqueios demorados.

---

### 4. O que acontece sem sincronização?

Sem sincronização, ocorre condição de corrida (race condition), onde múltiplas threads podem acessar e modificar o valor do contador simultaneamente, levando a um resultado final incorreto. Isso acontece porque sem bloqueios, as threads podem interferir entre si ao tentar atualizar o contador ao mesmo tempo, o que causa inconsistência nos resultados. Os testes demonstraram que a execução sem sincronização resultou em contadores incorretos.

---

## Referências

- `man pthread_spin_lock`
- `man pthread_mutex_lock`
