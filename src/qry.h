#ifndef _QRY_H_
#define _QRY_H_

#include <stdio.h>

#include "streap.h"
#include "digraph.h"
#include "quadras.h"
#include "drawSvg.h"

/**
 * Módulo: QRY
 *
 * Finalidade:
 *     Responsável pelo processamento do arquivo .qry do projeto.
 *
 *     Este módulo interpreta todos os comandos definidos no enunciado,
 *     aplicando operações sobre:
 *         - o grafo viário (vias e vértices),
 *         - o TAD Quadras (encapsulando a STreap interna),
 *         - a geração de saídas SVG (incluindo animações),
 *         - e a geração do relatório textual (.txt).
 *
 *     Toda a lógica de execução dos comandos do .qry fica encapsulada
 *     neste módulo, mantendo o estilo POO em C e evitando acessos
 *     diretos a estruturas internas de outros módulos.
 */

/* ============================================================
   Processamento principal
   ============================================================ */

/**
 * Função: processQryFile
 *
 * Descrição:
 *     Processa um arquivo .qry, interpretando e executando todos
 *     os comandos definidos no trabalho.
 *
 * Parâmetros:
 *     path          : caminho para o arquivo .qry
 *     g             : grafo viário previamente carregado (.via)
 *     quadras       : estrutura Quadras previamente carregada (.geo)
 *     quadrasStreap : STreap interna usada para consultas espaciais
 *     svg           : arquivo SVG de saída (já inicializado)
 *     txt           : arquivo TXT de relatório
 *
 * Observações:
 *     - Todos os comandos do .qry são tratados aqui.
 *     - Inclui comandos como:
 *           * catac
 *           * bl / rebl
 *           * @o?
 *           * p?
 *     - A geração de animações SVG também é responsabilidade
 *       deste módulo.
 */
void processQryFile(
    const char *path,
    Graph g,
    Quadras quadras,
    STreap quadrasStreap,
    ArqSvg svg,
    FILE *txt
);

#endif /* _QRY_H_ */


