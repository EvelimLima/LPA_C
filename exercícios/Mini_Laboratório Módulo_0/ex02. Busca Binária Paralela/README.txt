Mini-Laboratório 0 — Exercício 02: Busca Binária com Threads

Arquivo fonte:
mlab0-ex02-busca_binaria.c

Compilação:
make

Execução:
./mlab0-ex02-busca_binaria <valor>

Descriçao:
Este programa realiza a busca binária paralela de um valor dentro de um vetor ordenado.
O vetor é dividido em duas partes, cada uma processada por uma thread distinta.
Se o valor for encontrado por qualquer uma das threads, o índice correspondente é
salvo em uma variável global protegida por mutex.

Observações:
- O vetor utilizado no programa contém 20 números inteiros ordenados de forma crescente.
- A busca é feita simultaneamente em duas metades do vetor.
- A variável `resultado_global` armazena o índice encontrado, e é protegida por um
  `pthread_mutex_t` para evitar condições de corrida.
- Se uma thread encontra o valor, a outra pode encerrar cedo ao verificar `resultado_global`.
- O programa não exibe mensagem de erro caso o argumento esteja ausente, apenas finaliza silenciosamente.
