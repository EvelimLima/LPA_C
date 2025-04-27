Mini-Laboratório 0 — Exercício 04: Preenchimento de Matriz com Threads

Arquivo fonte: 
mlab0-ex04-preenchimento_matriz.c

Compilação:
make

Execução:
./mlab0-ex04-preenchimento_matriz <linhas> <colunas> <threads>

Descrição:
programa aloca dinamicamente uma matriz N x M e usa T threads para preencher
as linhas com números aleatórios entre 0 e 99.

- Uso de struct ThreadArgs para passar parâmetros para as threads
- Tratamento especial para quando threads > linhas
- Liberação correta de memória alocada
- Cada thread preenche um bloco contíguo de linhas (divisão igualitária)
- Utiliza rand_r() com seeds diferentes para cada thread (thread-safe)
- Alocação dinâmica de memória com verificação de erros
- Imprime a matriz resultante na saída padrão

