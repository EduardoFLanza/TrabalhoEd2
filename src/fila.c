#include "fila.h"

#include <stdlib.h>

/* ============================================================
   Implementação interna da fila
   ============================================================ */

/* Estrutura interna da fila */
typedef struct {
    int inicio;
    int fim;
    int tamanho;
    ItemF *stack;
} FilaImpl;

/* ============================================================
   Operações da fila
   ============================================================ */

/* Cria uma fila com tamanho máximo N */
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

/* Insere um elemento no final da fila */
void insertFila(Fila fila, ItemF ItemF)
{
    if (isFilaFull(fila))
        return;

    FilaImpl *p = (FilaImpl *)fila;

    int indice = (p->fim + 1) % p->tamanho;
    p->stack[indice] = ItemF;
    p->fim = indice;
}

/* Remove e retorna o primeiro elemento da fila */
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

/* Verifica se a fila está vazia */
bool isFilaEmpty(Fila fila)
{
    FilaImpl *p = (FilaImpl *)fila;
    return p->fim == -1;
}

/* Verifica se a fila está cheia */
bool isFilaFull(Fila fila)
{
    FilaImpl *p = (FilaImpl *)fila;
    return countFila(fila) == p->tamanho;
}

/* Retorna a quantidade de elementos na fila */
int countFila(Fila fila)
{
    FilaImpl *p = (FilaImpl *)fila;

    if (p->fim < 0)
        return 0;

    if (p->fim >= p->inicio)
        return p->fim - p->inicio + 1;

    return p->tamanho - p->inicio + p->fim + 1;
}

/* Retorna o índice do início da fila */
int getFilaInicio(Fila fila)
{
    return ((FilaImpl *)fila)->inicio;
}

/* Retorna o índice do fim da fila */
int getFilaFim(Fila fila)
{
    return ((FilaImpl *)fila)->fim;
}

/* Retorna a capacidade máxima da fila */
int getFilaLength(Fila fila)
{
    return ((FilaImpl *)fila)->tamanho;
}

/* Libera a memória da fila */
void removeFila(Fila fila)
{
    FilaImpl *p = (FilaImpl *)fila;
    free(p->stack);
    free(p);
}

