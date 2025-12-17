#ifndef _QRY_H_
#define _QRY_H_

#include <stdio.h>

#include "graph.h"
#include "stree.h"
#include "svg.h"

/**
 * Módulo: QRY
 *
 * Finalidade:
 *   Responsável pelo processamento do arquivo .qry do projeto.
 *
 *   Este módulo interpreta todos os comandos definidos no enunciado,
 *   aplicando operações sobre:
 *     - o grafo viário (vias e vértices),
 *     - a estrutura espacial de quadras (STree),
 *     - a geração de saídas em SVG (incluindo animações),
 *     - e a geração do relatório textual (.txt).
 *
 *   Toda a lógica de execução dos comandos do .qry fica encapsulada
 *   neste módulo, evitando dependências da main.
 */

/* ============================================================
   Processamento principal
   ============================================================ */

/**
 * Processa um arquivo .qry.
 *
 * Parâmetros:
 *   path    : caminho para o arquivo .qry
 *   g       : grafo viário previamente carregado (.via)
 *   quadras : estrutura espacial contendo as quadras (STree)
 *   svg     : arquivo SVG de saída (já inicializado)
 *   txt     : arquivo TXT de relatório
 *
 * Observações:
 *   - Todos os comandos do .qry são interpretados aqui.
 *   - Inclui comandos de:
 *       * catac
 *       * bl / rebl
 *       * @o / @p
 *       * p? (menor caminho por tempo ou comprimento)
 *   - Gera animações SVG para os caminhos calculados.
 */
void processQryFile(
    const char* path,
    Graph g,
    STree quadras,
    Svg svg,
    FILE* txt
);

#endif  _QRY_H_ 

