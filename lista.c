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

#include <stdlib.h>
#include <string.h>
#include "lista.h"

/* -------------------------------------------------------------------------
 * Estruturas internas do módulo (ocultas ao usuário)
 * ------------------------------------------------------------------------- */

/* Estrutura de uma célula da lista encadeada. */
typedef struct cel {
    void* it;          /* Conteúdo armazenado na célula */
    struct cel* next;  /* Próxima célula da lista */
} Cel;

/* Estrutura principal da lista. */
typedef struct lista {
    Cel* first;        /* Primeira célula */
    Cel* last;         /* Última célula */
    int size;          /* Quantidade de elementos */
} Lst;

/* -------------------------------------------------------------------------
 * Função: criaLista
 * Finalidade:
 *     Criar e inicializar uma lista genérica vazia.
 *
 * Retorno:
 *     - Instância opaca representando uma nova lista.
 * ------------------------------------------------------------------------- */
Lista criaLista() {
    Lst* l = calloc(1, sizeof(Lst));
    return l;
}

/* -------------------------------------------------------------------------
 * Função: removeLista
 * Finalidade:
 *     Remover toda a lista, liberando suas células.
 *
 * Parâmetros:
 *     - L        : instância da lista.
 *     - freeFunc : função opcional para liberar o conteúdo de cada célula.
 *
 * Requisitos:
 *     - Se o conteúdo das células foi alocado dinamicamente, freeFunc deve
 *       ser fornecida; caso contrário, conteúdo não é liberado.
 *
 * Efeitos:
 *     - Libera todas as células e a estrutura da lista.
 * ------------------------------------------------------------------------- */
void removeLista(Lista L, void (*freeFunc)(void*)) {
    Lst* l = L;
    Cel* c = l->first;

    while (c) {
        Cel* n = c->next;
        if (freeFunc) freeFunc(c->it);
        free(c);
        c = n;
    }

    free(l);
}

/* -------------------------------------------------------------------------
 * Função: insertLst
 * Finalidade:
 *     Inserir um novo elemento ao final da lista.
 *
 * Parâmetros:
 *     - L  : lista de destino.
 *     - it : ponteiro para o conteúdo a ser inserido.
 *
 * Retorno:
 *     - Um ponteiro (Posic) representando a posição da nova célula.
 *
 * Observação:
 *     - O conteúdo não é copiado, apenas armazenado como ponteiro.
 * ------------------------------------------------------------------------- */
Posic insertLst(Lista L, void* it) {
    Lst* l = L;
    Cel* c = malloc(sizeof(Cel));

    c->it = it;
    c->next = NULL;

    if (!l->first)
        l->first = c;
    else
        l->last->next = c;

    l->last = c;
    l->size++;

    return c;
}

/* -------------------------------------------------------------------------
 * Função: getFirstLst
 * Finalidade:
 *     Retornar a primeira posição da lista para iteração sequencial.
 *
 * Retorno:
 *     - Posic para a primeira célula.
 *     - NULL se lista vazia.
 * ------------------------------------------------------------------------- */
Posic getFirstLst(Lista L) {
    return ((Lst*)L)->first;
}

/* -------------------------------------------------------------------------
 * Função: getNextLst
 * Finalidade:
 *     Avançar para a próxima posição da lista.
 *
 * Parâmetros:
 *     - p : posição atual.
 *
 * Retorno:
 *     - Próxima posição ou NULL.
 * ------------------------------------------------------------------------- */
Posic getNextLst(Posic p) {
    return ((Cel*)p)->next;
}

/* -------------------------------------------------------------------------
 * Função: getLst
 * Finalidade:
 *     Acessar o conteúdo armazenado em uma posição.
 *
 * Retorno:
 *     - Ponteiro para o conteúdo (void*).
 * ------------------------------------------------------------------------- */
void* getLst(Posic p) {
    return ((Cel*)p)->it;
}

/* -------------------------------------------------------------------------
 * Função: lengthLst
 * Finalidade:
 *     Retornar o número de elementos da lista.
 *
 * Retorno:
 *     - Inteiro representando o tamanho da lista.
 * ------------------------------------------------------------------------- */
int lengthLst(Lista L) {
    return ((Lst*)L)->size;
}

/* -------------------------------------------------------------------------
 * Função: removePosicLst
 * Finalidade:
 *     Remover uma posição específica da lista.
 *
 * Parâmetros:
 *     - L : instância da lista.
 *     - p : posição a ser removida.
 *
 * Observação:
 *     - NÃO libera o conteúdo da célula; apenas remove a posição.
 *       Caso o conteúdo tenha sido alocado, o módulo chamador deve liberá-lo.
 *
 * Efeitos:
 *     - Atualiza ponteiros interno e reduz o tamanho da lista.
 * ------------------------------------------------------------------------- */
void removePosicLst(Lista L, Posic p) {
    Lst* l = L;
    Cel* c = (Cel*)p;

    if (!l->first || !c) return;

    if (l->first == c) {
        l->first = c->next;
        if (l->last == c)
            l->last = NULL;

        free(c);
        l->size--;
        return;
    }

    Cel* prev = l->first;
    while (prev && prev->next != c)
        prev = prev->next;

    if (prev) {
        prev->next = c->next;
        if (l->last == c)
            l->last = prev;
        free(c);
        l->size--;
    }
}
