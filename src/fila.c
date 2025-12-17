#include "fila.h"

#include <stdlib.h>

/* Implementação interna */

typedef struct {
    int inicio;
    int fim;
    int tamanho;
    Item *stack;
} FilaImpl;

/* Operações */

Fila createFila(int N)
{
    if (N <= 0)
        return NULL;

    FilaImpl *p = malloc(sizeof(FilaImpl));
    p->stack = malloc(sizeof(Item) * N);
    p->tamanho = N;
    p->inicio = 0;
    p->fim = -1;

    return p;
}

void insertFila(Fila fila, Item item)
{
    if (isFilaFull(fila))
        return;

    FilaImpl *p = (FilaImpl *)fila;

    int indice = (p->fim + 1) % p->tamanho;
    p->stack[indice] = item;
    p->fim = indice;
}

Item popFila(Fila fila)
{
    if (isFilaEmpty(fila))
        return NULL;

    FilaImpl *p = (FilaImpl *)fila;
    Item item = p->stack[p->inicio];

    if (p->inicio == p->fim) {
        p->inicio = 0;
        p->fim = -1;
    } else {
        p->inicio = (p->inicio + 1) % p->tamanho;
    }

    return item;
}

bool isFilaEmpty(Fila fila)
{
    FilaImpl *p = (FilaImpl *)fila;
    return p->fim == -1;
}

bool isFilaFull(Fila fila)
{
    FilaImpl *p = (FilaImpl *)fila;
    return countFila(fila) == p->tamanho;
}

int countFila(Fila fila)
{
    FilaImpl *p = (FilaImpl *)fila;

    if (p->fim < 0)
        return 0;

    if (p->fim >= p->inicio)
        return p->fim - p->inicio + 1;

    return p->tamanho - p->inicio + p->fim + 1;
}

int getFilaInicio(Fila fila)
{
    return ((FilaImpl *)fila)->inicio;
}

int getFilaFim(Fila fila)
{
    return ((FilaImpl *)fila)->fim;
}

int getFilaLength(Fila fila)
{
    return ((FilaImpl *)fila)->tamanho;
}

void removeFila(Fila fila)
{
    FilaImpl *p = (FilaImpl *)fila;
    free(p->stack);
    free(p);
}
