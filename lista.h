/**
 * Módulo: lista
 * Finalidade:
 *     Este módulo implementa uma lista simplesmente encadeada genérica
 *     baseada em células que armazenam ponteiros (Item). A estrutura permite
 *     inserção, remoção de posições específicas, iteração sequencial e
 *     consulta de tamanho.
 *
 *     Uma instância de Lista representa uma coleção dinâmica de elementos,
 *     organizada de forma linear, em que cada posição (Posic) referencia
 *     internamente uma célula da lista.
 *
 * Abstração:
 *     - Esta é uma estrutura genérica: qualquer ponteiro (void*) pode ser
 *       armazenado como conteúdo.
 *     - O módulo oculta completamente os detalhes internos (Information Hiding).
 *       Não existem “structs” neste arquivo; toda a implementação está no .c.
 *
 * Visão geral das operações:
 *     - criaLista: cria uma lista vazia.
 *     - insertLst: insere um elemento ao final.
 *     - getFirstLst / getNextLst: permite iterar pela lista.
 *     - getLst: recupera o item armazenado em uma posição.
 *     - lengthLst: informa quantos elementos existem.
 *     - removePosicLst: remove uma posição específica.
 *     - removeLista: destrói a lista inteira.
 *
 * Importante:
 *     “O que é uma instância da abstração criada por este módulo?”
 *        → Uma instância de Lista é uma referência opaca para uma estrutura
 *          encadeada gerenciada exclusivamente pelas funções definidas aqui.
 */

#ifndef LISTA_H
#define LISTA_H

/* Tipos opacos exportados pelo módulo */
typedef void* Item;   /* Conteúdo armazenado na lista */
typedef void* Lista;  /* Instância opaca da lista */
typedef void* Posic;  /* Referência para uma posição da lista */


/* -------------------------------------------------------------------------
 * Função: criaLista
 * Finalidade:
 *     Criar uma nova lista vazia.
 *
 * Parâmetros:
 *     - Nenhum.
 *
 * Retorno:
 *     - Uma nova instância de Lista.
 * ------------------------------------------------------------------------- */
Lista criaLista();

/* -------------------------------------------------------------------------
 * Função: removeLista
 * Finalidade:
 *     Remover toda a lista e liberar todas as células.
 *
 * Parâmetros:
 *     - l        : instância da lista.
 *     - freeFunc : função opcional para liberar o conteúdo de cada item.
 *
 * Restrições:
 *     - Se os itens foram alocados dinamicamente, freeFunc deve ser fornecida.
 *
 * Efeitos:
 *     - A lista é destruída e não pode mais ser usada.
 * ------------------------------------------------------------------------- */
void removeLista(Lista l, void (*freeFunc)(Item));

/* -------------------------------------------------------------------------
 * Função: insertLst
 * Finalidade:
 *     Inserir um novo item ao final da lista.
 *
 * Parâmetros:
 *     - l  : instância da lista.
 *     - it : ponteiro para o conteúdo a ser inserido.
 *
 * Retorno:
 *     - A posição (Posic) onde o item foi inserido.
 * ------------------------------------------------------------------------- */
Posic insertLst(Lista l, Item it);

/* -------------------------------------------------------------------------
 * Função: getFirstLst
 * Finalidade:
 *     Recuperar a primeira posição da lista.
 *
 * Retorno:
 *     - Posic da primeira posição, ou NULL se a lista estiver vazia.
 * ------------------------------------------------------------------------- */
Posic getFirstLst(Lista l);

/* -------------------------------------------------------------------------
 * Função: getNextLst
 * Finalidade:
 *     Obter a posição seguinte à informada.
 *
 * Parâmetros:
 *     - p : posição atual.
 *
 * Retorno:
 *     - Próxima posição, ou NULL se não houver.
 * ------------------------------------------------------------------------- */
Posic getNextLst(Posic p);

/* -------------------------------------------------------------------------
 * Função: getLst
 * Finalidade:
 *     Obter o item armazenado em uma posição.
 *
 * Parâmetros:
 *     - p : posição da lista.
 *
 * Retorno:
 *     - Item armazenado na posição.
 * ------------------------------------------------------------------------- */
Item getLst(Posic p);

/* -------------------------------------------------------------------------
 * Função: lengthLst
 * Finalidade:
 *     Determinar o número de elementos na lista.
 *
 * Parâmetros:
 *     - l : instância da lista.
 *
 * Retorno:
 *     - Quantidade de elementos.
 * ------------------------------------------------------------------------- */
int lengthLst(Lista l);

/* -------------------------------------------------------------------------
 * Função: removePosicLst
 * Finalidade:
 *     Remover uma posição específica da lista.
 *
 * Parâmetros:
 *     - l : lista.
 *     - p : posição a ser removida.
 *
 * Observação:
 *     - Esta função remove apenas a célula; NÃO libera o conteúdo.
 * ------------------------------------------------------------------------- */
void removePosicLst(Lista l, Posic p);

#endif


