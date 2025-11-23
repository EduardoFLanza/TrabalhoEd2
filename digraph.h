#ifndef _GRAPH__
#define _GRAPH__

/*
Um Grafo direcionado  G e' constituido por um conjunto de vertices V e 
um conjunto de arestas E, denotado por G=(V,E). 

Pode-se associar um dado a arestas e a vertices de G.
A cada vertice e' associado um nome.

Existem tambem operacoes que possibilitam percorrer o grafo,
invocando funcoes quando arestas do grafo 
sao "descobertas", podendo, inclusive, interromper o percurso.

Este modulo faz parte de um trabalho de Estrutura de Dados do 
curso de Ciencia da Computacao da Universidade Estadual de Londrina.
E' expressamente proibido o uso de ferramentas de IA para a sua implementacao.
A documentacao deste modulo deve ser melhorada.
*/

/**
 * ============================================================================
 * Módulo: digraph
 * Finalidade:
 *   Este módulo implementa um Grafo Dirigido (DIGRAPH) utilizando uma lista de
 *   adjacência genérica (ListAdj). O módulo segue o paradigma de programação
 *   orientada a objetos em C, com ocultação de informações: nenhuma estrutura
 *   concreta é exposta neste cabeçalho.
 *
 *
 * Operações principais:
 *   - Criação e destruição do grafo.
 *   - Inserção de vértices e arestas.
 *   - Consulta de informações (nome, info, adjacência).
 *   - Busca de caminhos e varreduras (BFS, DFS).
 *
 * Observação:
 *   A implementação utiliza o módulo ListAdj como estrutura interna.
 * ============================================================================
 */

#ifndef DIGRAPH_H
#define DIGRAPH_H

#include "lista.h"
#include "listadj.h"

/* Tipos opacos exportados */
typedef void* Digraph;
typedef void* GInfoVert;
typedef void* GInfoArest;

/* ============================================================================
 * Criação e destruição
 * ============================================================================
 */

/**
 * Função: dg_create
 * Finalidade:
 *   Cria uma nova instância de Digraph vazia.
 *
 * Retorno:
 *   Digraph - instância alocada. Deve ser destruída com dg_destroy().
 */
Digraph dg_create();

/**
 * Função: dg_destroy
 * Finalidade:
 *   Destrói completamente o grafo, liberando:
 *     - Todos os vértices,
 *     - Todas as arestas,
 *     - Informações associadas (caso fornecidas funções free).
 *
 * Parâmetros:
 *   GInfoVert (*freeV)(void*)  -> função para liberar InfoVert
 *   GInfoArest (*freeA)(void*) -> função para liberar InfoArest
 */
void dg_destroy(Digraph g,
                void (*freeV)(GInfoVert),
                void (*freeA)(GInfoArest));

/* ============================================================================
 * Operações sobre vértices
 * ============================================================================
 */

/**
 * Função: dg_addVertex
 * Finalidade:
 *   Insere um novo vértice de nome único no grafo.
 *
 * Parâmetros:
 *   nome - identificador textual do vértice.
 *   info - informação associada ao vértice.
 *
 * Retorno:
 *   1 se inserido com sucesso
 *   0 se já existia
 */
int dg_addVertex(Digraph g, const char* nome, GInfoVert info);

/**
 * Função: dg_getVertexInfo
 * Finalidade:
 *   Retorna a informação associada a um vértice já existente.
 */
GInfoVert dg_getVertexInfo(Digraph g, const char* nome);

/**
 * Função: dg_getVertexIndex
 * Finalidade:
 *   Retorna o índice interno do vértice no vetor do ListAdj.
 *   Retorna -1 se não existir.
 */
int dg_getVertexIndex(Digraph g, const char* nome);

/**
 * Função: dg_getVertexNameByIndex
 * Finalidade:
 *   Retorna o nome textual do vértice armazenado no índice informado.
 */
const char* dg_getVertexNameByIndex(Digraph g, int index);

/**
 * Função: dg_numVertices
 * Finalidade:
 *   Retorna o número total de vértices do grafo.
 */
int dg_numVertices(Digraph g);

/* ============================================================================
 * Operações sobre arestas
 * ============================================================================
 */

/**
 * Função: dg_addEdge
 * Finalidade:
 *   Insere uma aresta dirigida origem → destino.
 *
 * Parâmetros:
 *   origem  - nome do vértice de partida
 *   destino - nome do vértice de chegada
 *   peso    - peso da aresta
 *   info    - informação associada à aresta
 *
 * Retorno:
 *   1 se inserida com sucesso
 *   0 caso contrário
 */
int dg_addEdge(Digraph g, const char* origem, const char* destino,
               double peso, GInfoArest info);

/**
 * Função: dg_getAdjList
 * Finalidade:
 *   Retorna a lista de arestas que saem de um vértice.
 */
Lista dg_getAdjList(Digraph g, const char* nome);

/* ============================================================================
 * Capacidades extras (opcional)
 * ============================================================================
 */

void dg_bfs(Digraph g, const char* start, void (*visit)(const char*));
void dg_dfs(Digraph g, const char* start, void (*visit)(const char*));
Lista dg_shortestPath(Digraph g, const char* origem, const char* destino);

#endif
