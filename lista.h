#ifndef _LISTA_H
#define _LISTA_H

#include "dataTypes.h"
#include <stdbool.h>

/*
 * Módulo: lista
 *
 * Descrição:
 *     Este módulo implementa uma lista duplamente encadeada genérica,
 *     cujos elementos são armazenados como ponteiros (Item).
 *     A lista permite inserção, remoção, acesso por posição, iteração
 *     sequencial e operações funcionais (map, filter, fold).
 *
 *     Uma instância de Lista representa uma coleção ordenada de itens,
 *     gerenciada exclusivamente pelas funções deste módulo.
 *
 * Abstração:
 *     - Lista, Posic e Iterador são tipos opacos.
 *     - O usuário não tem acesso à estrutura interna da lista.
 *
 * Importante:
 *     Uma instância da abstração criada por este módulo é uma referência
 *     opaca para uma estrutura de lista duplamente encadeada.
 */

/* Tipos opacos */
typedef void *Lista;
typedef void *Posic;
typedef void *Iterador;
typedef void *Clausura;

/* Constantes */
#define NIL NULL
#define CAPAC_ILIMITADA -1

/*
 * Descrição: Cria uma lista vazia.
 * Parâmetros:
 *     capacidade: número máximo de elementos ou valor negativo para lista ilimitada.
 * Retorno:
 *     Uma nova instância de Lista.
 */
Lista createLst(int capacidade);

/*
 * Descrição: Define a posição inicial de um iterador.
 * Parâmetros:
 *     L : lista associada.
 *     it: iterador.
 *     p : posição inicial.
 * Retorno:
 *     Nenhum.
 */
void setIteratorPosition(Lista L, Iterador it, Posic p);

/*
 * Descrição: Retorna o número de elementos da lista.
 * Parâmetros:
 *     L: lista.
 * Retorno:
 *     Quantidade de elementos.
 */
int lengthLst(Lista L);

/*
 * Descrição: Retorna a capacidade máxima da lista.
 * Parâmetros:
 *     L: lista.
 * Retorno:
 *     Capacidade máxima ou -1 se ilimitada.
 */
int maxLengthLst(Lista L);

/*
 * Descrição: Verifica se a lista está vazia.
 * Parâmetros:
 *     L: lista.
 * Retorno:
 *     true se vazia, false caso contrário.
 */
bool isEmptyLst(Lista L);

/*
 * Descrição: Verifica se a lista está cheia.
 * Parâmetros:
 *     L: lista.
 * Retorno:
 *     true se cheia, false caso contrário.
 */
bool isFullLst(Lista L);

/*
 * Descrição: Insere um item no final da lista.
 * Parâmetros:
 *     L   : lista.
 *     info: item a ser inserido.
 * Retorno:
 *     Posição do item inserido ou NIL se falhar.
 */
Posic insertLst(Lista L, Item info);

/*
 * Descrição: Remove e retorna o primeiro elemento da lista.
 * Parâmetros:
 *     L: lista.
 * Retorno:
 *     Item removido.
 */
Item popLst(Lista L);

/*
 * Descrição: Remove o elemento indicado por uma posição.
 * Parâmetros:
 *     L: lista.
 *     p: posição a ser removida.
 * Retorno:
 *     Nenhum.
 */
void removeLst(Lista L, Posic p);

/*
 * Descrição: Retorna o item armazenado em uma posição.
 * Parâmetros:
 *     L: lista.
 *     p: posição.
 * Retorno:
 *     Item armazenado.
 */
Item getLst(Lista L, Posic p);

/*
 * Descrição: Insere um item antes da posição indicada.
 * Parâmetros:
 *     L   : lista.
 *     p   : posição de referência.
 *     info: item a ser inserido.
 * Retorno:
 *     Posição do item inserido.
 */
Posic insertBeforeLst(Lista L, Posic p, Item info);

/*
 * Descrição: Insere um item após a posição indicada.
 * Parâmetros:
 *     L   : lista.
 *     p   : posição de referência.
 *     info: item a ser inserido.
 * Retorno:
 *     Posição do item inserido.
 */
Posic insertAfterLst(Lista L, Posic p, Item info);

/*
 * Descrição: Retorna a primeira posição da lista.
 * Parâmetros:
 *     L: lista.
 * Retorno:
 *     Primeira posição ou NIL.
 */
Posic getFirstLst(Lista L);

/*
 * Descrição: Retorna a próxima posição da lista.
 * Parâmetros:
 *     L: lista.
 *     p: posição atual.
 * Retorno:
 *     Próxima posição ou NIL.
 */
Posic getNextLst(Lista L, Posic p);

/*
 * Descrição: Retorna a última posição da lista.
 * Parâmetros:
 *     L: lista.
 * Retorno:
 *     Última posição ou NIL.
 */
Posic getLastLst(Lista L);

/*
 * Descrição: Retorna a posição anterior da lista.
 * Parâmetros:
 *     L: lista.
 *     p: posição atual.
 * Retorno:
 *     Posição anterior ou NIL.
 */
Posic getPrevLst(Lista L, Posic p);

/*
 * Descrição: Libera toda a memória associada à lista.
 * Parâmetros:
 *     L: lista.
 * Retorno:
 *     Nenhum.
 */
void killLst(Lista L);

/* Iteradores */

Iterador createIterador(Lista L, bool reverso);
bool isIteratorEmpty(Lista L, Iterador it);
Iterador getIteratorNext(Lista L, Iterador it);
Item getIteratorItem(Lista L, Iterador it);
Posic getIteratorPosic(Lista L, Iterador it);
void killIterator(Lista L, Iterador it);

/* Funções de ordem superior */

typedef Item (*Apply)(Item item);
typedef bool (*Check)(Item item);
typedef bool (*CheckClausure)(Item item, Clausura c);
typedef void (*ApplyClosure)(Item item, Clausura c);

Lista map(Lista L, Apply f);
Lista filter(Lista L, Check f);
void fold(Lista L, ApplyClosure f, Clausura c);
Lista filterClausure(Lista L, CheckClausure f, Clausura c);

#endif


