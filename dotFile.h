#ifndef DOT_FILE_H
#define DOT_FILE_H

#include <stdio.h>
#include "streap.h"

/*
 * Módulo: dotFile
 *
 * Descrição:
 *     Este módulo provê operações para exportação de uma estrutura Streap
 *     (árvore balanceada probabilística) em um arquivo de texto no formato DOT,
 *     compatível com a ferramenta Graphviz.
 *
 *     A exportação percorre toda a Streap, registrando seus nós e arestas,
 *     permitindo a visualização da estrutura hierárquica da árvore.
 *
 * Abstração:
 *     - Este módulo não cria nem mantém uma entidade própria.
 *     - Ele opera sobre uma instância existente de Streap.
 *
 * Importante:
 *     Uma instância manipulada por este módulo é apenas lida durante o processo
 *     de exportação, não havendo qualquer modificação estrutural.
 */

/*
 * Função: DrawStreap
 * Descrição: Exporta uma Streap para um arquivo no formato DOT.
 * Parâmetros: tree – instância da Streap a ser exportada.
 *             fullfilename – caminho completo do arquivo .dot a ser gerado.
 * Retorno: Nenhum.
 */
void DrawStreap(Streap tree, char *fullfilename);

#endif
