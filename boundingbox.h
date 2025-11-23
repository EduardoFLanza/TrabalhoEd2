/**
 * Módulo: boundingbox
 * Finalidade:
 *     Este módulo define a interface pública para a abstração “Bounding Box”
 *     (caixa delimitadora), responsável por armazenar e organizar entidades
 *     espaciais indexadas por coordenadas (x, y). A estrutura suporta operações
 *     de inserção, remoção, consulta por faixa (range search) e iteração
 *     ordenada. Todas as estruturas concretas são ocultadas (information hiding),
 *     sendo expostas apenas através de tipos opacos.
 *
 * O que é uma instância desta abstração?
 *     Uma instância de BBox representa um conjunto de elementos posicionados
 *     em um plano bidimensional e organizados internamente por uma STreap —
 *     uma árvore binária de busca probabilística (treap) que combina
 *     propriedades de heap e BST para garantir bom desempenho mesmo em
 *     cenários degenerados.
 *
 * Visão Geral das Operações:
 *     - Criação e destruição da BBox
 *     - Inserção de elementos com coordenadas espaciais
 *     - Remoção de elementos
 *     - Busca por região (xmin, ymin, xmax, ymax)
 *     - Visitação sistemática dos nós
 *     - Obtenção da caixa mínima que contém todos os elementos (MBB)
 *     - Acesso direto à STreap interna para módulos especializados
 *
 */





#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H
#include "STreap.h"
typedef void* BBox;
/*
 * Função: bb_create
 * Descrição: Esta função pertence ao módulo boundingbox e realiza a operação associada ao seu nome.
 * Parâmetros: (sem parênteses)
 * Retorno: BBox
 */
BBox bb_create();
/*
 * Função: bb_destroy
 * Descrição: Esta função pertence ao módulo boundingbox e realiza a operação associada ao seu nome.
 * Parâmetros: (BBox b, void (*freeFunc)(SInfo))
 * Retorno: void
 */
void bb_destroy(BBox b, void (*freeFunc)(SInfo));
/*
 * Função: bb_insert
 * Descrição: Esta função pertence ao módulo boundingbox e realiza a operação associada ao seu nome.
 * Parâmetros: (BBox b, double x, double y, SInfo info)
 * Retorno: void
 */
void bb_insert(BBox b, double x, double y, SInfo info);
/*
 * Função: bb_remove
 * Descrição: Esta função pertence ao módulo boundingbox e realiza a operação associada ao seu nome.
 * Parâmetros: (BBox b, double x, double y, void (*freeFunc)(SInfo))
 * Retorno: void
 */
void bb_remove(BBox b, double x, double y, void (*freeFunc)(SInfo));
/*
 * Função: bb_rangeSearch
 * Descrição: Esta função pertence ao módulo boundingbox e realiza a operação associada ao seu nome.
 * Parâmetros: (BBox b, double xmin, double ymin, double xmax, double ymax, void (*visit)(SNode))
 * Retorno: void
 */
void bb_rangeSearch(BBox b, double xmin, double ymin, double xmax, double ymax, void (*visit)(SNode));
/*
 * Função: bb_forEach
 * Descrição: Esta função pertence ao módulo boundingbox e realiza a operação associada ao seu nome.
 * Parâmetros: (BBox b, void (*visit)(SNode))
 * Retorno: void
 */
void bb_forEach(BBox b, void (*visit)(SNode));
/*
 * Função: bb_getMBB
 * Descrição: Esta função pertence ao módulo boundingbox e realiza a operação associada ao seu nome.
 * Parâmetros: (BBox b, double *xmin, double *ymin, double *xmax, double *ymax)
 * Retorno: int
 */
int bb_getMBB(BBox b, double *xmin, double *ymin, double *xmax, double *ymax);
/*
 * Função: bb_getTreap
 * Descrição: Esta função pertence ao módulo boundingbox e realiza a operação associada ao seu nome.
 * Parâmetros: (BBox b)
 * Retorno: STreap
 */
STreap bb_getTreap(BBox b);
#endif
