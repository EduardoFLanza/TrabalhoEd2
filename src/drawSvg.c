#include "drawSvg.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Normaliza valores de transparência */
static double Normalizar(double value)
{
    if (value < 0)
        return 1.0;
    return value;
}

/*
 * Função: abreEscritaSvg
 * Descrição: Cria um arquivo SVG para escrita.
 * Parâmetros: fn – nome ou caminho do arquivo.
 * Retorno: ArqSvg ou NULL.
 */
ArqSvg abreEscritaSvg(char *fn)
{
    FILE *file = fopen(fn, "w");
    if (file == NULL)
        return NULL;

    fprintf(file, "<svg xmlns=\"http://www.w3.org/2000/svg\">\n");
    return (ArqSvg) file;
}

/*
 * Função: preparaDecoracao
 * Descrição: Monta string de atributos SVG para figuras.
 * Parâmetros: ver definição no .h.
 * Retorno: Nenhum.
 */
void preparaDecoracao(ArqSvg fsvg, char *deco, int decoLen,
                      char *corBorda, char *corPreenchimento,
                      char *larguraBorda, double transparencia,
                      double transparenciaPreenchimento,
                      double transparenciaBorda)
{
    transparencia              = Normalizar(transparencia);
    transparenciaPreenchimento = Normalizar(transparenciaPreenchimento);
    transparenciaBorda         = Normalizar(transparenciaBorda);

    snprintf(deco, decoLen,
             "style=\"stroke:%s;fill:%s;stroke-width:%s;"
             "opacity:%lf;fill-opacity:%lf;stroke-opacity:%lf\"",
             corBorda, corPreenchimento, larguraBorda,
             transparencia, transparenciaPreenchimento, transparenciaBorda);
}

/*
 * Função: escreveCirculoSvg
 * Descrição: Insere um círculo no SVG.
 */
void escreveCirculoSvg(ArqSvg fsvg, double xc, double yc, double r, char *deco)
{
    if (fsvg == NULL)
        return;

    fprintf((FILE *) fsvg,
            "\n\t<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\"",
            xc, yc, r);

    if (deco != NULL)
        fprintf((FILE *) fsvg, " %s", deco);

    fprintf((FILE *) fsvg, " />");
}

/*
 * Função: escreveRetanguloSvg
 * Descrição: Insere um retângulo no SVG.
 */
void escreveRetanguloSvg(ArqSvg fsvg, double x, double y,
                         double larg, double alt, char *deco)
{
    if (fsvg == NULL)
        return;

    fprintf((FILE *) fsvg,
            "\n\t<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\"",
            x, y, larg, alt);

    if (deco != NULL)
        fprintf((FILE *) fsvg, " %s", deco);

    fprintf((FILE *) fsvg, " />");
}

/*
 * Função: escreveLinhaSvg
 * Descrição: Insere uma linha no SVG.
 */
void escreveLinhaSvg(ArqSvg fsvg,
                     double x1, double y1,
                     double x2, double y2,
                     char *deco)
{
    if (fsvg == NULL)
        return;

    fprintf((FILE *) fsvg,
            "\n\t<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\"",
            x1, y1, x2, y2);

    if (deco != NULL)
        fprintf((FILE *) fsvg, " %s", deco);

    fprintf((FILE *) fsvg, " />");
}

/*
 * Função: preparaDecoracaoTexto
 * Descrição: Monta string de atributos SVG para textos.
 */
void preparaDecoracaoTexto(ArqSvg fsvg, char *deco, int decoLen,
                           char *fontFamily, char *fontStyle,
                           char *fontWeight, char *fontSize,
                           char *fontColor, char *fontStroke,
                           char *textAnchor)
{
    snprintf(deco, decoLen,
             "style=\"font-family:%s;font-style:%s;"
             "font-weight:%s;font-size:%s;"
             "fill:%s;stroke:%s\"",
             fontFamily, fontStyle, fontWeight,
             fontSize, fontColor, fontStroke);
}

/*
 * Função: escreveTextoSvg
 * Descrição: Insere um texto no SVG.
 */
void escreveTextoSvg(ArqSvg fsvg, double x, double y,
                     char *txt, char *textAnchor, char *decoTxt)
{
    if (fsvg == NULL)
        return;

    fprintf((FILE *) fsvg,
            "\n\t<text text-anchor=\"%s\" x=\"%lf\" y=\"%lf\"",
            textAnchor, x, y);

    if (decoTxt != NULL)
        fprintf((FILE *) fsvg, " %s", decoTxt);

    fprintf((FILE *) fsvg, ">%s</text>", txt);
}

/*
 * Função: fechaSvg
 * Descrição: Finaliza e fecha o arquivo SVG.
 */
void fechaSvg(ArqSvg fsvg)
{
    if (fsvg == NULL)
        return;

    fprintf((FILE *) fsvg, "\n</svg>");
    fclose((FILE *) fsvg);
}

/*
 * Função: applyRotation
 * Descrição: Aplica rotação à decoração SVG.
 */
void applyRotation(char *deco, int decoLen,
                   double theta, double xCenter, double yCenter)
{
    char buffer[decoLen];

    snprintf(buffer, decoLen, "%s", deco);
    snprintf(deco, decoLen,
             "transform=\"rotate(%lf,%lf,%lf)\" %s",
             theta, xCenter, yCenter, buffer);
}
