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
 * Parâmetros: fn – nome ou caminho do arquivo; w - largura; h - altura.
 * Retorno: ArqSvg ou NULL.
 */
ArqSvg abreEscritaSvg(char *fn, double w, double h)
{
    FILE *file = fopen(fn, "w");
    if (file == NULL)
        return NULL;

    fprintf(file, "<svg width=\"%lf\" height=\"%lf\" viewBox=\"0 0 %lf %lf\" xmlns=\"http://www.w3.org/2000/svg\">\n", 
            w, h, w, h);
            
    return (ArqSvg) file;
}
/*
 * Função: escreveRetanguloSvg
 * Descrição: Insere um retângulo no arquivo SVG com estilos.
 */
void escreveRetanguloSvg(ArqSvg fsvg, double x, double y,
                         double larg, double alt, const char *corP,
                         const char *corB, const char *espB,
                         double opacidade)
{
    if (fsvg == NULL)
        return;

    fprintf((FILE *) fsvg,
            "\n\t<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" "
            "fill=\"%s\" stroke=\"%s\" stroke-width=\"%s\" fill-opacity=\"%.1f\" />",
            x, y, larg, alt, corP, corB, espB, opacidade);
}

/*
 * Função: escreveLinhaSvg
 * Descrição: Insere uma linha no arquivo SVG com estilos.
 */
void escreveLinhaSvg(ArqSvg fsvg,
                     double x1, double y1,
                     double x2, double y2,
                     const char *cor, const char* espL)
{
    if (fsvg == NULL)
        return;

    fprintf((FILE *) fsvg,
            "\n\t<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" "
            "stroke=\"%s\" stroke-width=\"%s\" />",
            x1, y1, x2, y2, cor, espL);
}

/*
 * Função: escreveTextoSvg
 * Descrição: Insere um texto no SVG.
 */
void escreveTextoSvg(ArqSvg fsvg, double x, double y,
                     char *txt, char *textAnchor)
{
    if (fsvg == NULL)
        return;

    fprintf((FILE *) fsvg,
            "\n\t<text text-anchor=\"%s\" x=\"%lf\" y=\"%lf\"",
            textAnchor, x, y);

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

