#ifndef VIA_H
#define VIA_H

#include <stdbool.h>
#include "digraph.h"

/**
 * Módulo: Via
 *
 * Objetivo:
 *   Implementar o TAD Via, responsável por representar e manipular
 *   um sistema viário modelado como um grafo direcionado.
 *
 *   Os vértices representam pontos geográficos (x, y) e as arestas
 *   representam vias urbanas, contendo atributos como nome,
 *   CEPs laterais, comprimento, velocidade média e estado
 *   de habilitação.
 *
 *   O módulo encapsula completamente as estruturas internas,
 *   expondo apenas funções de criação, acesso, modificação
 *   e liberação de memória.
 */

/* Tipos opaco */

/* Vértice do sistema viário */
typedef void* VerticeVia;

/* Aresta do sistema viário */
typedef void* ArestaVia;

/* Construção do grafo viário */

/**
 * Lê um arquivo .via e constrói o grafo correspondente.
 *
 * Parâmetros:
 *   path - caminho para o arquivo .via
 * Retorno:
 *   Grafo direcionado contendo vértices e vias,
 *   ou NULL em caso de erro.
 */
Graph viaReadFile(const char* path);

/* Operações sobre vias (arestas) */

/* Desativa uma via */
void viaDisable(ArestaVia via);

/* Reativa uma via */
void viaEnable(ArestaVia via);

/* Atualiza a velocidade média da via */
void viaSetSpeed(ArestaVia via, double speed);

/* Acesso aos dados dos vértices */

/* Retorna a coordenada X */
double viaGetX(VerticeVia v);

/* Retorna a coordenada Y */
double viaGetY(VerticeVia v);

/* Acesso aos dados das vias */

/* Retorna o nome da via */
const char* viaGetName(ArestaVia via);

/* Retorna o comprimento da via */
double viaGetLength(ArestaVia via);

/* Retorna a velocidade média */
double viaGetSpeed(ArestaVia via);

/* Indica se a via está habilitada */
bool viaIsEnabled(ArestaVia via);

/* Liberação de memória */

/**
 * Libera a memória associada a uma via.
 * Parâmetros: via   - aresta */
void viaFreeEdge(ArestaVia via);

#endif /* VIA_H */



