Mini-Laboratório 0 — Exercício 03: Soma de Vetor com Threads

Arquivo fonte:
mlab0-ex03-soma_vetor.c

Compilação:
make

Execução:
./mlab0-ex03-soma_vetor

Descrição
Este programa calcula a soma de todos os elementos de um vetor de forma paralela, 
dividindo o trabalho entre múltiplas threads. Cada thread é responsável por somar uma 
faixa do vetor e armazenar o resultado parcial em uma posição específica de um vetor 
de inteiros. Ao final, a thread principal soma os parciais e exibe o resultado final.

Observações:
- O vetor é preenchido com os valores de 1 a 100.
- A soma é dividida entre 4 threads.
- As somas parciais são armazenadas em um vetor `somas[]`, e somadas no final pela main.
- O uso de ponteiros para armazenar os resultados evita a necessidade de variáveis globais.
- e não há entrada de dados via terminal — os parâmetros estão fixos no código.
