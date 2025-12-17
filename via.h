#ifndef _VIA_H_
#define _VIA_H_

#include <stdbool.h>
#include "graph.h"

/**
 * Módulo: Via
 *
 * Finalidade:
 *     Responsável pela leitura e manipulação de arquivos do tipo .via,
 *     que descrevem um sistema viário representado por um grafo direcionado.
 *
 *     Cada vértice representa um ponto do mapa (coordenadas x, y) e
 *     cada aresta representa uma via, contendo informações como nome,
 *     CEPs laterais, comprimento, velocidade média e estado de habilitação.
 *
 *     Este módulo encapsula os dados internos de vértices e arestas,
 *     expondo apenas funções de acesso e modificação controladas.
 *
 *     O grafo retornado é compatível com o TAD Graph do projeto.
 */

/* ============================================================
   Tipos opacos
   ============================================================ */

/* Estrutura de vértice do sistema viário */
typedef void* VerticeVia;

/* Estrutura de aresta do sistema viário */
typedef void* ArestaVia;

/* ============================================================
   Leitura de arquivo
   ============================================================ */

/**
 * Processa um arquivo .via e constrói um grafo direcionado.
 *
 * Parâmetros:
 *   path : caminho completo para o arquivo .via
 *
 * Retorno:
 *   Grafo contendo os vértices e arestas descritos no arquivo,
 *   ou NULL em caso de erro.
 */
Graph processViaFile(const char* path);

/* ============================================================
   Operações sobre arestas (vias)
   ============================================================ */

/*
 * Desabilita uma via (aresta).
 */
void blockVia(ArestaVia av);

/*
 * Habilita uma via previamente bloqueada.
 */
void unblockVia(ArestaVia av);

/*
 * Atualiza a velocidade média de uma via.
 */
void setArestaVM(ArestaVia av, double vm);

/* ============================================================
   Funções de acesso — vértices
   ============================================================ */

/*
 * Retorna a coordenada X do vértice.
 */
double getVerticeViaX(VerticeVia vv);

/*
 * Retorna a coordenada Y do vértice.
 */
double getVerticeViaY(VerticeVia vv);

/* ============================================================
   Funções de acesso — arestas
   ============================================================ */

/*
 * Retorna o nome da via.
 */
const char* getArestaViaNome(ArestaVia av);

/*
 * Retorna a velocidade média da via.
 */
double getArestaVM(ArestaVia av);

/*
 * Retorna o comprimento da via.
 */
double getArestaCMP(ArestaVia av);

/*
 * Indica se a via está habilitada.
 */
bool isArestaEnabled(ArestaVia av);

/* ============================================================
   Liberação de memória
   ============================================================ */

/*
 * Libera a memória associada a uma aresta do sistema viário.
 *
 * Parâmetros:
 *   av    : aresta
 *   extra : ponteiro auxiliar (não utilizado)
 */
void freeArestaVia(ArestaVia av, void* extra);

#endif
