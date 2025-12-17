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

/* Tipos opacos exportados pelo módulo */
typedef void *Lista;
typedef void *Posic;
typedef void *Iterador;
typedef void *Clausura;

/* Constantes exportadas */
#define NIL NULL
#define CAPAC_ILIMITADA -1

/*
 * Função: createLst
 * Descrição: Cria uma lista vazia com capacidade definida.
 * Parâmetros: capacidade – número máximo de elementos ou valor negativo para ilimitada.
 * Retorno: Nova instância de Lista.
 */
Lista createLst(int capacidade);

/*
 * Função: setIteratorPosition
 * Descrição: Define a posição inicial de um iterador.
 * Parâmetros: L – lista; it – iterador; p – posição inicial.
 * Retorno: Nenhum.
 */
void setIteratorPosition(Lista L, Iterador it, Posic p);

/*
 * Função: lengthLst
 * Descrição: Retorna o número de elementos da lista.
 * Parâmetros: L – lista.
 * Retorno: Quantidade de elementos.
 */
int lengthLst(Lista L);

/*
 * Função: maxLengthLst
 * Descrição: Retorna a capacidade máxima da lista.
 * Parâmetros: L – lista.
 * Retorno: Capacidade máxima ou -1 se ilimitada.
 */
int maxLengthLst(Lista L);

/*
 * Função: isEmptyLst
 * Descrição: Verifica se a lista está vazia.
 * Parâmetros: L – lista.
 * Retorno: true se vazia, false caso contrário.
 */
bool isEmptyLst(Lista L);

/*
 * Função: isFullLst
 * Descrição: Verifica se a lista está cheia.
 * Parâmetros: L – lista.
 * Retorno: true se cheia, false caso contrário.
 */
bool isFullLst(Lista L);

/*
 * Função: insertLst
 * Descrição: Insere um item no final da lista.
 * Parâmetros: L – lista; info – item a ser inserido.
 * Retorno: Posição do elemento inserido ou NIL se a lista estiver cheia.
 */
Posic insertLst(Lista L, Item info);

/*
 * Função: popLst
 * Descrição: Remove e retorna o primeiro elemento da lista.
 * Parâmetros: L – lista.
 * Retorno: Item removido ou NULL se a lista estiver vazia.
 */
Item popLst(Lista L);

/*
 * Função: removeLst
 * Descrição: Remove um elemento da lista indicado por uma posição.
 * Parâmetros: L – lista; p – posição a ser removida.
 * Retorno: Nenhum.
 */
void removeLst(Lista L, Posic p);

/*
 * Função: getLst
 * Descrição: Retorna o item armazenado em uma posição da lista.
 * Parâmetros: L – lista; p – posição.
 * Retorno: Item armazenado.
 */
Item getLst(Lista L, Posic p);

/*
 * Função: insertBeforeLst
 * Descrição: Insere um item imediatamente antes da posição indicada.
 * Parâmetros: L – lista; p – posição de referência; info – item a ser inserido.
 * Retorno: Posição do elemento inserido.
 */
Posic insertBeforeLst(Lista L, Posic p, Item info);

/*
 * Função: insertAfterLst
 * Descrição: Insere um item imediatamente após a posição indicada.
 * Parâmetros: L – lista; p – posição de referência; info – item a ser inserido.
 * Retorno: Posição do elemento inserido.
 */
Posic insertAfterLst(Lista L, Posic p, Item info);

/*
 * Função: getFirstLst
 * Descrição: Retorna a posição do primeiro elemento da lista.
 * Parâmetros: L – lista.
 * Retorno: Posição do primeiro elemento ou NIL se vazia.
 */
Posic getFirstLst(Lista L);

/*
 * Função: getNextLst
 * Descrição: Retorna a posição seguinte à posição indicada.
 * Parâmetros: L – lista; p – posição atual.
 * Retorno: Próxima posição ou NIL se não existir.
 */
Posic getNextLst(Lista L, Posic p);

/*
 * Função: getLastLst
 * Descrição: Retorna a posição do último elemento da lista.
 * Parâmetros: L – lista.
 * Retorno: Posição do último elemento ou NIL se vazia.
 */
Posic getLastLst(Lista L);

/*
 * Função: getPreviousLst
 * Descrição: Retorna a posição anterior à posição indicada.
 * Parâmetros: L – lista; p – posição atual.
 * Retorno: Posição anterior ou NIL se não existir.
 */
Posic getPreviousLst(Lista L, Posic p);

/*
 * Função: killLst
 * Descrição: Libera toda a memória associada à lista.
 * Parâmetros: L – lista.
 * Retorno: Nenhum.
 */
void killLst(Lista L);

/*
 * Função: createIterador
 * Descrição: Cria um iterador para percorrer a lista.
 * Parâmetros: L – lista; reverso – sentido da iteração.
 * Retorno: Novo iterador.
 */
Iterador createIterador(Lista L, bool reverso);

/*
 * Função: isIteratorEmpty
 * Descrição: Verifica se o iterador está esgotado.
 * Parâmetros: L – lista; it – iterador.
 * Retorno: true se esgotado, false caso contrário.
 */
bool isIteratorEmpty(Lista L, Iterador it);

/*
 * Função: getIteratorNext
 * Descrição: Avança o iterador para o próximo elemento.
 * Parâmetros: L – lista; it – iterador.
 * Retorno: Iterador atualizado.
 */
Iterador getIteratorNext(Lista L, Iterador it);

/*
 * Função: getIteratorItem
 * Descrição: Retorna o item apontado pelo iterador.
 * Parâmetros: L – lista; it – iterador.
 * Retorno: Item corrente.
 */
Item getIteratorItem(Lista L, Iterador it);

/*
 * Função: getIteratorPosic
 * Descrição: Retorna a posição apontada pelo iterador.
 * Parâmetros: L – lista; it – iterador.
 * Retorno: Posição corrente.
 */
Posic getIteratorPosic(Lista L, Iterador it);

/*
 * Função: killIterator
 * Descrição: Libera os recursos associados ao iterador.
 * Parâmetros: L – lista; it – iterador.
 * Retorno: Nenhum.
 */
void killIterator(Lista L, Iterador it);

/* Tipos de funções de alta ordem */
typedef Item (*Apply)(Item item);
typedef bool (*Check)(Item item);
typedef bool (*CheckClausure)(Item item, Clausura c);
typedef void (*ApplyClosure)(Item item, Clausura c);

/*
 * Função: map
 * Descrição: Cria uma nova lista aplicando uma função a cada item.
 * Parâmetros: L – lista; f – função aplicada aos itens.
 * Retorno: Nova lista resultante.
 */
Lista map(Lista L, Apply f);

/*
 * Função: filter
 * Descrição: Cria uma nova lista com itens que satisfazem um predicado.
 * Parâmetros: L – lista; f – função de verificação.
 * Retorno: Nova lista filtrada.
 */
Lista filter(Lista L, Check f);

/*
 * Função: fold
 * Descrição: Aplica uma função acumuladora a todos os itens da lista.
 * Parâmetros: L – lista; f – função aplicada; c – clausura.
 * Retorno: Nenhum.
 */
void fold(Lista L, ApplyClosure f, Clausura c);

/*
 * Função: filterClausure
 * Descrição: Filtra a lista usando uma função com clausura.
 * Parâmetros: L – lista; f – função de verificação; c – clausura.
 * Retorno: Nova lista filtrada.
 */
Lista filterClausure(Lista L, CheckClausure f, Clausura c);

#endif
