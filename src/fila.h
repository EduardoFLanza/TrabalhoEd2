#ifndef FILA_H
#define FILA_H

#include <stdbool.h>

/*
 * Módulo: fila
 * Descrição:
 *     Este módulo implementa uma fila estática circular de itens genéricos.
 *     A fila possui capacidade fixa definida na criação e segue a política
 *     FIFO (First In, First Out).
 *
 *     Uma instância da abstração Fila representa uma estrutura que armazena
 *     elementos de forma ordenada, permitindo inserção no final e remoção
 *     apenas no início, sem acesso direto por índice.
 */

/* Tipo opaco exportado pelo módulo */
typedef void *Fila;
typedef void *ItemF;

/*
 * Função: createFila
 * Descrição: Cria uma fila com capacidade máxima para N elementos.
 * Parâmetros: N – número máximo de elementos da fila (N > 0).
 * Retorno: Nova instância de Fila.
 */
Fila createFila(int N);

/*
 * Função: insertFila
 * Descrição: Insere um item no final da fila.
 * Parâmetros: fila – instância da fila; item – elemento a ser inserido.
 * Retorno: Nenhum.
 */
void insertFila(Fila fila, ItemF item);

/*
 * Função: popFila
 * Descrição: Remove e retorna o elemento do início da fila.
 * Parâmetros: fila – instância da fila.
 * Retorno: Item removido ou NULL se a fila estiver vazia.
 */
ItemF popFila(Fila fila);

/*
 * Função: isFilaEmpty
 * Descrição: Verifica se a fila está vazia.
 * Parâmetros: fila – instância da fila.
 * Retorno: true se vazia, false caso contrário.
 */
bool isFilaEmpty(Fila fila);

/*
 * Função: isFilaFull
 * Descrição: Verifica se a fila está cheia.
 * Parâmetros: fila – instância da fila.
 * Retorno: true se cheia, false caso contrário.
 */
bool isFilaFull(Fila fila);

/*
 * Função: countFila
 * Descrição: Retorna a quantidade de elementos armazenados na fila.
 * Parâmetros: fila – instância da fila.
 * Retorno: Número de elementos da fila.
 */
int countFila(Fila fila);

/*
 * Função: getFilaInicio
 * Descrição: Retorna o índice do início lógico da fila.
 * Parâmetros: fila – instância da fila.
 * Retorno: Índice do início da fila.
 */
int getFilaInicio(Fila fila);

/*
 * Função: getFilaFim
 * Descrição: Retorna o índice do final lógico da fila.
 * Parâmetros: fila – instância da fila.
 * Retorno: Índice do final da fila.
 */
int getFilaFim(Fila fila);

/*
 * Função: getFilaLength
 * Descrição: Retorna a capacidade máxima da fila.
 * Parâmetros: fila – instância da fila.
 * Retorno: Capacidade máxima da fila.
 */
int getFilaLength(Fila fila);

/*
 * Função: removeFila
 * Descrição: Libera todos os recursos associados à fila.
 * Parâmetros: fila – instância da fila.
 * Retorno: Nenhum.
 */
void removeFila(Fila fila);

#endif
