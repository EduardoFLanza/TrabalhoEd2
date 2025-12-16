/**
 * Módulo: lista
 * Finalidade:
 *     Este módulo implementa uma lista simplesmente encadeada genérica,
 *     capaz de armazenar ponteiros para qualquer tipo de elemento.
 *
 *     Uma instância de Lista representa uma coleção ordenada de elementos,
 *     onde cada célula contém:
 *         - um ponteiro genérico para conteúdo (void*)
 *         - um ponteiro para a próxima célula
 *
 *     A lista oferece operações para:
 *         - criação e destruição
 *         - inserção ao final
 *         - iteração sequencial (first/next)
 *         - remoção de uma posição específica
 *         - obtenção de tamanho
 *
 *     Este módulo segue o princípio de encapsulamento:
 *         - Tipos concretos são definidos apenas neste arquivo (.c).
 *         - O header lista.h expõe apenas tipos opacos e protótipos.
 */

#include "lista.h"

#include <stdio.h>
#include <stdlib.h>

/* Estruturas internas */

struct StNode {
    struct StNode *prox, *ant;
    Item info;
};

struct StLista {
    struct StNode *prim, *ult;
    int capac;
    int length;
};

typedef struct StLista ListaImpl;
typedef struct StNode Node;

struct StIterator {
    Node *curr;
    bool reverso;
};

typedef struct StIterator IteratorImpl;

/* Operações básicas */

Lista createLst(int capacidade)
{
    ListaImpl *lst = (ListaImpl *) malloc(sizeof(ListaImpl));
    lst->prim = NULL;
    lst->ult = NULL;
    lst->capac = capacidade;
    lst->length = 0;
    return lst;
}

int lengthLst(Lista L)
{
    ListaImpl *lst = (ListaImpl *) L;
    return lst->length;
}

int maxLengthLst(Lista L)
{
    ListaImpl *lst = (ListaImpl *) L;
    return (lst->capac < 0) ? -1 : lst->capac;
}

bool isEmptyLst(Lista L)
{
    ListaImpl *lst = (ListaImpl *) L;
    return lst->length == 0;
}

bool isFullLst(Lista L)
{
    ListaImpl *lst = (ListaImpl *) L;
    return lst->length == lst->capac;
}

Posic insertLst(Lista L, Item info)
{
    ListaImpl *lst = (ListaImpl *) L;
    Node *newNode = (Node *) malloc(sizeof(Node));

    newNode->info = info;
    newNode->prox = NULL;
    newNode->ant = lst->ult;

    if (isEmptyLst(L))
        lst->prim = newNode;
    else
        lst->ult->prox = newNode;

    lst->ult = newNode;
    lst->length++;
    return newNode;
}

Item popLst(Lista L)
{
    ListaImpl *lst = (ListaImpl *) L;

    if (isEmptyLst(L)) {
        printf("ERRO: popLst em lista vazia\n");
        return NULL;
    }

    Node *node = lst->prim;
    Item info = node->info;
    removeLst(L, node);
    return info;
}

void removeLst(Lista L, Posic p)
{
    ListaImpl *lst = (ListaImpl *) L;
    Node *node = (Node *) p;

    if (!node) {
        printf("ERRO: removeLst com posicao invalida\n");
        return;
    }

    if (node->ant)
        node->ant->prox = node->prox;
    else
        lst->prim = node->prox;

    if (node->prox)
        node->prox->ant = node->ant;
    else
        lst->ult = node->ant;

    free(node);
    lst->length--;
}

Item getLst(Lista L, Posic p)
{
    Node *node = (Node *) p;

    if (!node) {
        printf("ERRO: getLst com posicao invalida\n");
        return NULL;
    }

    return node->info;
}

Posic insertBeforeLst(Lista L, Posic p, Item info)
{
    ListaImpl *lst = (ListaImpl *) L;
    Node *node = (Node *) p;
    Node *newNode = (Node *) malloc(sizeof(Node));

    newNode->info = info;
    newNode->prox = node;
    newNode->ant = node->ant;

    if (node->ant)
        node->ant->prox = newNode;
    else
        lst->prim = newNode;

    node->ant = newNode;
    lst->length++;
    return newNode;
}

Posic insertAfterLst(Lista L, Posic p, Item info)
{
    ListaImpl *lst = (ListaImpl *) L;
    Node *node = (Node *) p;
    Node *newNode = (Node *) malloc(sizeof(Node));

    newNode->info = info;
    newNode->ant = node;
    newNode->prox = node->prox;

    if (node->prox)
        node->prox->ant = newNode;
    else
        lst->ult = newNode;

    node->prox = newNode;
    lst->length++;
    return newNode;
}

Posic getFirstLst(Lista L)
{
    ListaImpl *lst = (ListaImpl *) L;
    return lst->prim;
}

Posic getNextLst(Lista L, Posic p)
{
    ListaImpl *lst = (ListaImpl *) L;
    Node *node = (Node *) p;

    if (lst->ult == node)
        return NIL;
    return node->prox;
}

Posic getLastLst(Lista L)
{
    ListaImpl *lst = (ListaImpl *) L;
    return isEmptyLst(L) ? NIL : lst->ult;
}

Posic getPrevLst(Lista L, Posic p)
{
    ListaImpl *lst = (ListaImpl *) L;
    Node *node = (Node *) p;

    if (lst->prim == node)
        return NIL;
    return node->ant;
}

void killLst(Lista L)
{
    ListaImpl *lst = (ListaImpl *) L;
    Node *node = lst->prim;

    while (node) {
        Node *next = node->prox;
        free(node);
        node = next;
    }

    free(lst);
}

/* Iteradores */

Iterador createIterador(Lista L, bool reverso)
{
    ListaImpl *lst = (ListaImpl *) L;
    IteratorImpl *iter = (IteratorImpl *) malloc(sizeof(IteratorImpl));

    iter->reverso = reverso;
    iter->curr = reverso ? lst->ult : lst->prim;
    return iter;
}

void setIteratorPosition(Lista L, Iterador it, Posic p)
{
    IteratorImpl *iter = (IteratorImpl *) it;
    iter->curr = (Node *) p;
}

bool isIteratorEmpty(Lista L, Iterador it)
{
    IteratorImpl *iter = (IteratorImpl *) it;
    return iter->curr == NULL;
}

Iterador getIteratorNext(Lista L, Iterador it)
{
    IteratorImpl *iter = (IteratorImpl *) it;

    if (!iter->curr)
        return NULL;

    iter->curr = iter->reverso ? iter->curr->ant : iter->curr->prox;
    return iter;
}

Item getIteratorItem(Lista L, Iterador it)
{
    IteratorImpl *iter = (IteratorImpl *) it;
    return iter->curr ? iter->curr->info : NULL;
}

Posic getIteratorPosic(Lista L, Iterador it)
{
    IteratorImpl *iter = (IteratorImpl *) it;
    return iter->curr;
}

void killIterator(Lista L, Iterador it)
{
    free(it);
}

/* Funções de ordem superior */

Lista map(Lista L, Apply f)
{
    ListaImpl *lst = (ListaImpl *) L;
    Lista newLst = createLst(lst->capac);

    for (Node *n = lst->prim; n; n = n->prox)
        insertLst(newLst, f(n->info));

    return newLst;
}

Lista filter(Lista L, Check f)
{
    ListaImpl *lst = (ListaImpl *) L;
    Lista newLst = createLst(lst->capac);

    for (Node *n = lst->prim; n; n = n->prox)
        if (f(n->info))
            insertLst(newLst, n->info);

    return newLst;
}

void fold(Lista L, ApplyClosure f, Clausura c)
{
    ListaImpl *lst = (ListaImpl *) L;

    for (Node *n = lst->prim; n; n = n->prox)
        f(n->info, c);
}

Lista filterClausure(Lista L, CheckClausure f, Clausura c)
{
    ListaImpl *lst = (ListaImpl *) L;
    Lista newLst = createLst(lst->capac);

    for (Node *n = lst->prim; n; n = n->prox)
        if (f(n->info, c))
            insertLst(newLst, n->info);

    return newLst;
}
