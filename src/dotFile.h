#ifndef DOT_FILE_H
#define DOT_FILE_H

#include "streap.h"

/*
 * Módulo: dotFile
 *
 * Descrição:
 *     Este módulo provê funções para exportar uma STreap
 *     para um arquivo texto no formato DOT, compatível com
 *     a ferramenta Graphviz.
 *
 *     A árvore é percorrida e cada nó é representado por sua
 *     âncora (x,y) e arestas pai-filho.
 *
 * Abstração:
 *     - A estrutura interna da STreap não é acessada.
 *     - A exportação utiliza apenas a interface pública do módulo STreap.
 */

/*
 * Função: DrawSTreap
 * Descrição: Exporta a STreap para um arquivo no formato DOT.
 * Parâmetros: t – instância da STreap.
 *             fullfilename – caminho completo do arquivo .dot.
 * Retorno: Nenhum.
 */
void DrawSTreap(STreap t, char *fullfilename);

#endif
