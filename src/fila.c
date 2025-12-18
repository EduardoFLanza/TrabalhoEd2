#include "fila.h"

#include <stdlib.h>

/* ============================================================
   Implementação interna da fila
   ============================================================ */

typedef struct {
    int inicio;
    int fim;
    int tamanho;
    ItemF *stack;
} FilaImpl;

/* ============================================================
   Operações da fila
   ============================================================ */

/*
 * Função: createFila
 * Descrição: cria uma fila com capacidade fixa N
 * Parâmetros:
 *   N – número máximo de elementos da fila
 * Retorno:
 *   Ponteiro para a fila criada ou NULL se N <= 0
 */
Fila createFila(int N)
{
    if (N <= 0)
        return NULL;

    FilaImpl *p = malloc(sizeof(FilaImpl));
    p->stack = malloc(sizeof(ItemF) * N);
    p->tamanho = N;
    p->inicio = 0;
    p->fim = -1;

    return p;
}

/*
 * Função: insertFila
 * Descrição: insere um elemento no final da fila
 * Parâmetros:
 *   fila – fila onde o elemento será inserido
 *   ItemF – elemento a ser inserido
 * Retorno:
 *   Nenhum (se a fila estiver cheia, a inserção é ignorada)
 */
void insertFila(Fila fila, ItemF ItemF)
{
    if (isFilaFull(fila))
        return;

    FilaImpl *p = (FilaImpl *)fila;

    int indice = (p->fim + 1) % p->tamanho;
    p->stack[indice] = ItemF;
    p->fim = indice;
}

/*
 * Função: popFila
 * Descrição: remove e retorna o primeiro elemento da fila
 * Parâmetros:
 *   fila – fila de onde o elemento será removido
 * Retorno:
 *   Elemento removido ou NULL se a fila estiver vazia
 */
ItemF popFila(Fila fila)
{
    if (isFilaEmpty(fila))
        return NULL;

    FilaImpl *p = (FilaImpl *)fila;
    ItemF ItemF = p->stack[p->inicio];

    if (p->inicio == p->fim) {
        p->inicio = 0;
        p->fim = -1;
    } else {
        p->inicio = (p->inicio + 1) % p->tamanho;
    }

    return ItemF;
}

/*
 * Função: isFilaEmpty
 * Descrição: verifica se a fila está vazia
 * Parâmetros:
 *   fila – fila a ser verificada
 * Retorno:
 *   true se a fila estiver vazia, false caso contrário
 */
bool isFilaEmpty(Fila fila)
{
    FilaImpl *p = (FilaImpl *)fila;
    return p->fim == -1;
}

/*
 * Função: isFilaFull
 * Descrição: verifica se a fila está cheia
 * Parâmetros:
 *   fila – fila a ser verificada
 * Retorno:
 *   true se a fila estiver cheia, false caso contrário
 */
bool isFilaFull(Fila fila)
{
    FilaImpl *p = (FilaImpl *)fila;
    return countFila(fila) == p->tamanho;
}

/*
 * Função: countFila
 * Descrição: retorna a quantidade atual de elementos na fila
 * Parâmetros:
 *   fila – fila a ser analisada
 * Retorno:
 *   Número de elementos presentes na fila
 */
int countFila(Fila fila)
{
    FilaImpl *p = (FilaImpl *)fila;

    if (p->fim < 0)
        return 0;

    if (p->fim >= p->inicio)
        return p->fim - p->inicio + 1;

    return p->tamanho - p->inicio + p->fim + 1;
}

/*
 * Função: getFilaInicio
 * Descrição: retorna o índice do início da fila
 * Parâmetros:
 *   fila – fila consultada
 * Retorno:
 *   Índice do início da fila
 */
int getFilaInicio(Fila fila)
{
    return ((FilaImpl *)fila)->inicio;
}

/*
 * Função: getFilaFim
 * Descrição: retorna o índice do fim da fila
 * Parâmetros:
 *   fila – fila consultada
 * Retorno:
 *   Índice do fim da fila
 */
int getFilaFim(Fila fila)
{
    return ((FilaImpl *)fila)->fim;
}

/*
 * Função: getFilaLength
 * Descrição: retorna a capacidade máxima da fila
 * Parâmetros:
 *   fila – fila consultada
 * Retorno:
 *   Capacidade total da fila
 */
int getFilaLength(Fila fila)
{
    return ((FilaImpl *)fila)->tamanho;
}

/*
 * Função: removeFila
 * Descrição: libera toda a memória associada à fila
 * Parâmetros:
 *   fila – fila a ser destruída
 */
void removeFila(Fila fila)
{
    FilaImpl *p = (FilaImpl *)fila;
    free(p->stack);
    free(p);
}

