#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define TAM_BUFFER 5
#define N_PRODUTORES 2
#define N_CONSUMIDORES 2
#define N_OPERACOES 15  // limite de operações do produtor e consumidor

int buffer[TAM_BUFFER];
int in = 0, out = 0,          //inserir e remover
    count = 0;                // contador de itens no buffer


pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;                                              //proteção da região crítica - buffer
pthread_cond_t cond_prod = PTHREAD_COND_INITIALIZER;                                           // condição para o produtor esperar caso o buffer esteja cheio
pthread_cond_t cond_cons = PTHREAD_COND_INITIALIZER;                                           // condição para o consumidor esperar caso o buffer esteja vazio

void* produtor(void* arg) {
    int id = *((int*)arg);                                                                     // id do produtor

    for(int i = 0; i < N_OPERACOES; ++i) {                                                    // n de operações do produtor   
            int item = rand() % 100;                                                           // produz um item aleatório


            pthread_mutex_lock(&lock);                                                         // entra na região crítica
            while (count == TAM_BUFFER) {                                                      // buffer cheio
                //printf("Produtor %d: buffer cheio, esperando...\n", id);
                pthread_cond_wait(&cond_prod, &lock);                                          // espera o consumidor consumir  
            }


            buffer[in] = item;                                                                // insere o item no buffer
            in = (in + 1) % TAM_BUFFER;                                                       // atualiza o índice de entrada, circularmente
            count++;                                                                          // incrementa o contador de itens no buffer


            printf("[Produtor %d] Inseriu: %d\n", id, item);


            pthread_cond_signal(&cond_cons);                                                 // sinaliza/acorda o consumidor que há itens no buffer
            pthread_mutex_unlock(&lock);                                                     // sai da região crítica
            sleep(1.5);                                                                    // simula o tempo de produção
        }
        return NULL;
}


void* consumidor(void* arg) {
    int id = *((int*)arg);

    for (int i = 0; i < N_OPERACOES; ++i) {                                                  
        pthread_mutex_lock(&lock);                                                          // entra na região crítica
                                               
        while (count == 0) {                                                            
            //printf("Consumidor %d: buffer vazio, esperando...\n", id);
            pthread_cond_wait(&cond_cons, &lock);                                            // espera o produtor produzir
        }

        int item =  buffer[out];                                                            // consome/remove um item
        printf("[Consumidor %d] Removeu: %d\n", id, item);
        out = (out + 1) % TAM_BUFFER;                                                       // atualiza o índice de saída
        count--;                                                                            // decrementa o contador de itens no buffer

        pthread_cond_signal(&cond_prod);                                                    // sinaliza o produtor que há espaço no buffer
        pthread_mutex_unlock(&lock);                                                        // sai da região crítica
        sleep(1.5);
    }
    
   return NULL;
}


int main() {
    pthread_t produtores[N_PRODUTORES], consumidores[N_CONSUMIDORES]; 
    int ids_prod[N_PRODUTORES], ids_cons[N_CONSUMIDORES];

    for (int i = 0; i < N_PRODUTORES; i++) {
        ids_prod[i] = i + 1;
        pthread_create(&produtores[i], NULL, produtor, &ids_prod[i]); 
    }

    for (int i = 0; i < N_CONSUMIDORES; i++) {
        ids_cons[i] = i + 1;
        pthread_create(&consumidores[i], NULL, consumidor, &ids_cons[i]);
    }

    for (int i = 0; i < N_PRODUTORES; i++)
        pthread_join(produtores[i], NULL);
    for (int i = 0; i < N_CONSUMIDORES; i++)
        pthread_join(consumidores[i], NULL);

    return 0;
}
