/**
 * Módulo: funcoesqry
 * Finalidade:
 *   Implementa operações utilizadas para executar os comandos do arquivo QRY,
 *   manipulando a imagem (Picture), estruturas espaciais (BBox) e o grafo (Digraph).
 *
 */

#ifndef FUNCOESQRY_H
#define FUNCOESQRY_H

#include "picture.h"
#include "boundingbox.h"
#include "digraph.h"
#include "lista.h"

/*
 * Função: fq_removeInRect
 * Descrição:
 *     Remove do Picture todas as entidades cuja posição esteja dentro
 *     do retângulo definido por (xmin, ymin) e (xmax, ymax).
 * Parâmetros:
 *     Picture p — estrutura de desenho onde as entidades estão armazenadas.
 *     BBox bb — estrutura espacial usada para busca eficiente.
 *     double xmin, ymin, xmax, ymax — limites do retângulo.
 * Retorno:
 *     void.
 */
void fq_removeInRect(Picture p, BBox bb,
                     double xmin, double ymin, double xmax, double ymax);

/*
 * Função: fq_removeByID
 * Descrição:
 *     Remove do Picture e do BBox a entidade com o identificador fornecido.
 * Parâmetros:
 *     Picture p — estrutura de desenho.
 *     BBox bb — estrutura espacial.
 *     const char* id — identificador da entidade.
 * Retorno:
 *     int — 1 se removeu, 0 se não encontrada.
 */
int fq_removeByID(Picture p, BBox bb, const char* id);

/*
 * Função: fq_highlightRadius
 * Descrição:
 *     Para cada entidade dentro do raio r a partir de (x, y),
 *     desenha um pequeno círculo de destaque.
 * Parâmetros:
 *     Picture p — estrutura de desenho.
 *     BBox bb — estrutura espacial.
 *     double x, y — centro do raio.
 *     double r — raio de busca.
 *     const char* stroke — cor da borda.
 *     const char* fill — cor interna.
 * Retorno:
 *     void.
 */
void fq_highlightRadius(Picture p, BBox bb,
                        double x, double y, double r,
                        const char* stroke, const char* fill);

/*
 * Função: fq_blockEdge
 * Descrição:
 *     Bloqueia a aresta de origem->destino no grafo (torna indisponível).
 * Parâmetros:
 *     Digraph g — grafo.
 *     const char* origem — vértice de origem.
 *     const char* destino — vértice destino.
 * Retorno:
 *     int — 1 em caso de sucesso, 0 caso contrário.
 */
int fq_blockEdge(Digraph g, const char* origem, const char* destino);

/*
 * Função: fq_unblockEdge
 * Descrição:
 *     Desbloqueia a aresta origem->destino no grafo.
 * Parâmetros:
 *     Digraph g — grafo.
 *     const char* origem — vértice de origem.
 *     const char* destino — vértice destino.
 * Retorno:
 *     int — 1 em caso de sucesso, 0 caso contrário.
 */
int fq_unblockEdge(Digraph g, const char* origem, const char* destino);

/*
 * Função: fq_shortestPath
 * Descrição:
 *     Calcula e retorna o menor caminho entre dois vértices do grafo.
 * Parâmetros:
 *     Digraph g — grafo.
 *     const char* origem — vértice inicial.
 *     const char* destino — vértice final.
 * Retorno:
 *     Lista — lista de nomes dos vértices no caminho.
 */
Lista fq_shortestPath(Digraph g, const char* origem, const char* destino);

/*
 * Função: fq_drawPath
 * Descrição:
 *     Desenha no Picture o caminho calculado, adicionando linhas sucessivas.
 * Parâmetros:
 *     Picture p — imagem onde será desenhado.
 *     BBox bb — estrutura espacial.
 *     Digraph g — grafo.
 *     Lista path — sequência de vértices.
 *     const char* idPrefix — prefixo para os IDs das linhas criadas.
 *     const char* stroke — cor das linhas.
 * Retorno:
 *     void.
 */
void fq_drawPath(Picture p, BBox bb, Digraph g,
                 Lista path, const char* idPrefix, const char* stroke);

/*
 * Função: fq_catac
 * Descrição:
 *     Executa operação específica do QRY sobre vértices e arestas dentro do retângulo.
 * Parâmetros:
 *     Digraph g — grafo.
 *     BBox bb — estrutura espacial.
 *     double xmin, ymin, xmax, ymax — limites da área.
 * Retorno:
 *     void.
 */
void fq_catac(Digraph g, BBox bb,
              double xmin, double ymin, double xmax, double ymax);

/*
 * Função: fq_forEachInRect
 * Descrição:
 *     Executa a função 'visit' sobre todas as entidades dentro da área especificada.
 * Parâmetros:
 *     BBox bb — estrutura espacial.
 *     double xmin, ymin, xmax, ymax — área alvo.
 *     void (*visit)(SNode) — função callback.
 * Retorno:
 *     void.
 */
void fq_forEachInRect(BBox bb,
                      double xmin, double ymin, double xmax, double ymax,
                      void (*visit)(SNode));

#endif

