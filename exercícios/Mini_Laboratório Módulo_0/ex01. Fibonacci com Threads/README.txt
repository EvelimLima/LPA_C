Mini-Laboratório 0 — Exercício 01: Fibonacci com Threads

Arquivo fonte:
mlab0-ex01-fibonacci.c

Compilação:
make

Execução:
./mlab0-ex01-fibonacci <n>

Descrição:
Este programa calcula o n-ésimo número da sequência de Fibonacci utilizando chamadas
recursivas com threads. Para cada chamada fib(n), duas threads são criadas para calcular
fib(n-1) e fib(n-2), respeitando o limite definido.

Observações:
- Threads são criadas apenas quando n > 5, para evitar sobrecarga de criação recursiva.
- Para n <= 5, o cálculo é feito sequencialmente.
- A comunicação entre chamadas é feita por structs contendo os argumentos e o campo de retorno.
- A sincronização é garantida com pthread_join, assegurando que os resultados das threads
  filhas sejam utilizados corretamente antes de prosseguir.
- O uso de variáveis globais foi evitado para prevenir condições de corrida.
