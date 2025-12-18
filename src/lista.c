#include "lista.h"

#include <stdio.h>
#include <stdlib.h>

/* Implementação interna */

typedef struct StNode {
    struct StNode *prox;
    struct StNode *ant;
    Item info;
} Node;

typedef struct StLista {
    Node *prim;
    Node *ult;
    int capac;
    int length;
} ListaImpl;

typedef struct StIterator {
    Node *curr;
    bool reverso;
} IteratorImpl;

/* Operações básicas */

Lista createLst(int capacidade)
{
    ListaImpl *lst = malloc(sizeof(ListaImpl));
    lst->prim = NULL;
    lst->ult = NULL;
    lst->capac = capacidade;
    lst->length = 0;
    return lst;
}

int lengthLst(Lista L)
{
    return ((ListaImpl *)L)->length;
}

int maxLengthLst(Lista L)
{
    ListaImpl *lst = (ListaImpl *)L;
    return lst->capac;
}

bool isEmptyLst(Lista L)
{
    return ((ListaImpl *)L)->length == 0;
}

bool isFullLst(Lista L)
{
    ListaImpl *lst = (ListaImpl *)L;
    if (lst->capac < 0)
        return false;
    return lst->length >= lst->capac;
}

Posic insertLst(Lista L, Item info)
{
    ListaImpl *lst = (ListaImpl *)L;

    if (isFullLst(L))
        return NULL;

    Node *n = malloc(sizeof(Node));
    n->info = info;
    n->prox = NULL;
    n->ant = lst->ult;

    if (lst->ult)
        lst->ult->prox = n;
    else
        lst->prim = n;

    lst->ult = n;
    lst->length++;
    return n;
}

Item popLst(Lista L)
{
    ListaImpl *lst = (ListaImpl *)L;

    if (isEmptyLst(L))
        return NULL;

    Node *n = lst->prim;
    Item it = n->info;
    removeLst(L, n);
    return it;
}

void removeLst(Lista L, Posic p)
{
    ListaImpl *lst = (ListaImpl *)L;
    Node *n = (Node *)p;

    if (!n)
        return;

    if (n->ant)
        n->ant->prox = n->prox;
    else
        lst->prim = n->prox;

    if (n->prox)
        n->prox->ant = n->ant;
    else
        lst->ult = n->ant;

    free(n);
    lst->length--;
}

Item getLst(Lista L, Posic p)
{
    Node *n = (Node *)p;
    if (!n)
        return NULL;
    return n->info;
}

Posic insertBeforeLst(Lista L, Posic p, Item info)
{
    ListaImpl *lst = (ListaImpl *)L;
    Node *ref = (Node *)p;

    if (!ref || isFullLst(L))
        return NULL;

    Node *n = malloc(sizeof(Node));
    n->info = info;
    n->prox = ref;
    n->ant = ref->ant;

    if (ref->ant)
        ref->ant->prox = n;
    else
        lst->prim = n;

    ref->ant = n;
    lst->length++;
    return n;
}

Posic insertAfterLst(Lista L, Posic p, Item info)
{
    ListaImpl *lst = (ListaImpl *)L;
    Node *ref = (Node *)p;

    if (!ref || isFullLst(L))
        return NULL;

    Node *n = malloc(sizeof(Node));
    n->info = info;
    n->ant = ref;
    n->prox = ref->prox;

    if (ref->prox)
        ref->prox->ant = n;
    else
        lst->ult = n;

    ref->prox = n;
    lst->length++;
    return n;
}

Posic getFirstLst(Lista L)
{
    return ((ListaImpl *)L)->prim;
}

Posic getLastLst(Lista L)
{
    return ((ListaImpl *)L)->ult;
}

Posic getNextLst(Lista L, Posic p)
{
    Node *n = (Node *)p;
    if (!n)
        return NULL;
    return n->prox;
}

Posic getPreviousLst(Lista L, Posic p)
{
    Node *n = (Node *)p;
    if (!n)
        return NULL;
    return n->ant;
}

void killLst(Lista L, freeFuncLst f)
{
    ListaImpl *lst = (ListaImpl *)L;
    Node *n = lst->prim;

    while (n) {
        Node *next = n->prox;
        if (f)
            f(n->info);
        free(n);
        n = next;
    }

    free(lst);
}

/* Iteradores */

Iterador createIterador(Lista L, bool reverso)
{
    ListaImpl *lst = (ListaImpl *)L;
    IteratorImpl *it = malloc(sizeof(IteratorImpl));
    it->reverso = reverso;
    it->curr = reverso ? lst->ult : lst->prim;
    return it;
}

void setIteratorPosition(Lista L, Iterador it, Posic p)
{
    ((IteratorImpl *)it)->curr = (Node *)p;
}

bool isIteratorEmpty(Lista L, Iterador it)
{
    return ((IteratorImpl *)it)->curr == NULL;
}

Iterador getIteratorNext(Lista L, Iterador it)
{
    IteratorImpl *iter = (IteratorImpl *)it;

    if (!iter->curr)
        return NULL;

    iter->curr = iter->reverso ? iter->curr->ant : iter->curr->prox;
    return it;
}

Item getIteratorItem(Lista L, Iterador it)
{
    IteratorImpl *iter = (IteratorImpl *)it;
    if (!iter->curr)
        return NULL;
    return iter->curr->info;
}

Posic getIteratorPosic(Lista L, Iterador it)
{
    return ((IteratorImpl *)it)->curr;
}

void killIterator(Lista L, Iterador it)
{
    free(it);
}

/* High-order functions */

Lista map(Lista L, Apply f)
{
    ListaImpl *lst = (ListaImpl *)L;
    Lista newL = createLst(lst->capac);

    for (Node *n = lst->prim; n; n = n->prox)
        insertLst(newL, f(n->info));

    return newL;
}

Lista filter(Lista L, Check f)
{
    ListaImpl *lst = (ListaImpl *)L;
    Lista newL = createLst(lst->capac);

    for (Node *n = lst->prim; n; n = n->prox)
        if (f(n->info))
            insertLst(newL, n->info);

    return newL;
}

void fold(Lista L, ApplyClosure f, Clausura c)
{
    ListaImpl *lst = (ListaImpl *)L;

    for (Node *n = lst->prim; n; n = n->prox)
        f(n->info, c);
}

Lista filterClausure(Lista L, CheckClausure f, Clausura c)
{
    ListaImpl *lst = (ListaImpl *)L;
    Lista newL = createLst(lst->capac);

    for (Node *n = lst->prim; n; n = n->prox)
        if (f(n->info, c))
            insertLst(newL, n->info);

    return newL;
}



