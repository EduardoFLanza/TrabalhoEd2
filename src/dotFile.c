#include "dotFile.h"

#include <stdio.h>

/*
 * Estrutura auxiliar usada durante o percurso da STreap.
 */
typedef struct {
    FILE *dot;
} DotAux;

/*
 * Função: visitNodeDot
 * Descrição: Função de visita usada no percurso da STreap para
 *            escrever a representação DOT de cada nó.
 * Parâmetros: info – informação associada ao nó.
 *             x,y – coordenadas da âncora.
 *             mbbX1, mbbY1, mbbX2, mbbY2 – MBB do subárvore (não usado).
 *             aux – estrutura auxiliar contendo o arquivo.
 * Retorno: Nenhum.
 */
static void visitNodeDot(SInfo info,
                         double x, double y,
                         double mbbX1, double mbbY1,
                         double mbbX2, double mbbY2,
                         void *aux)
{
    DotAux *daux = (DotAux *) aux;
    FILE *dot = daux->dot;

    fprintf(dot,
            "\t\"(%lf,%lf)\" [label=\"(%lf,%lf)\"];\n",
            x, y, x, y);
}

/*
 * Função: DrawSTreap
 * Descrição: Percorre a STreap em profundidade e gera um
 *            arquivo DOT representando sua estrutura.
 * Parâmetros: t – instância da STreap.
 *             fullfilename – caminho completo do arquivo .dot.
 * Retorno: Nenhum.
 */
void DrawSTreap(STreap t, char *fullfilename)
{
    FILE *dot = fopen(fullfilename, "w");
    DotAux aux;

    if (dot == NULL)
    {
        printf("ERRO: [DrawSTreap] nao foi possivel abrir arquivo .dot\n");
        return;
    }

    aux.dot = dot;

    fprintf(dot, "digraph STreap\n");
    fprintf(dot, "{\n");
    fprintf(dot, "\tnode [fontname=\"Arial\"];\n");

    percursoProfundidade(t, visitNodeDot, (void *) &aux);

    fprintf(dot, "}\n");
    fclose(dot);
}

