#include "quadras.h"
#include "via.h"
#include "qry.h"
#include "dotFile.h"
#include "svg.h"
#include "lista.h"
#include "path.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Função principal
 */
int main(int argc, char *argv[])
{
    char *entryArg = NULL;   /* -e */
    char *outputArg = NULL;  /* -o */
    char *geoArg = NULL;     /* -f */
    char *qryArg = NULL;     /* -q */
    char *viaArg = NULL;     /* -v */

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-e")) entryArg = argv[++i];
        else if (!strcmp(argv[i], "-o")) outputArg = argv[++i];
        else if (!strcmp(argv[i], "-f")) geoArg = argv[++i];
        else if (!strcmp(argv[i], "-q")) qryArg = argv[++i];
        else if (!strcmp(argv[i], "-v")) viaArg = argv[++i];
    }

    if (!outputArg || !geoArg) {
        fprintf(stderr, "Parâmetros obrigatórios ausentes\n");
        return 1;
    }

    char entryPath[512] = ".";
    if (entryArg)
        normalizePath(entryArg, entryPath, sizeof(entryPath));

    char outputPath[512];
    normalizePath(outputArg, outputPath, sizeof(outputPath));

    /* ===== GEO ===== */

    char geoFile[512];
    sprintf(geoFile, "%s/%s", entryPath, geoArg);

    Quadras quadras = processGeoFile(geoFile);
    if (!quadras) {
        fprintf(stderr, "Erro ao ler .geo\n");
        return 1;
    }

    /* ===== VIA ===== */

    Graph vias = NULL;
    if (viaArg) {
        char viaFile[512];
        sprintf(viaFile, "%s/%s", entryPath, viaArg);

        vias = viaReadFile(viaFile);
        if (!vias) {
            fprintf(stderr, "Erro ao ler .via\n");
            return 1;
        }
    }

    /* ===== SVG INICIAL ===== */

    char geoName[256];
    getFileNameWithoutExt(geoArg, geoName, sizeof(geoName));

    char svgGeo[512];
    sprintf(svgGeo, "%s/%s.svg", outputPath, geoName);

    Svg svg = svgOpen(svgGeo);
    drawQuadras(svg, quadras);
    svgClose(svg);

    char dotGeo[512];
    sprintf(dotGeo, "%s/%s.dot", outputPath, geoName);
    printSTrp(getQuadrasSTrp(quadras), dotGeo);

    /* ===== QRY ===== */

    if (qryArg) {
        char qryFile[512];
        sprintf(qryFile, "%s/%s", entryPath, qryArg);

        char qryName[256];
        getFileNameWithoutExt(qryArg, qryName, sizeof(qryName));

        char svgFinal[512];
        sprintf(svgFinal, "%s/%s-%s.svg", outputPath, geoName, qryName);

        char txtFinal[512];
        sprintf(txtFinal, "%s/%s-%s.txt", outputPath, geoName, qryName);

        FILE *txt = fopen(txtFinal, "w");
        Svg svgF = svgOpen(svgFinal);

        processQryFile(qryFile, vias, quadras, svgF, txt);

        svgClose(svgF);
        fclose(txt);

        char dotFinal[512];
        sprintf(dotFinal, "%s/%s-%s.dot", outputPath, geoName, qryName);
        printSTrp(getQuadrasSTrp(quadras), dotFinal);
    }

    /* ===== LIBERAÇÃO ===== */

    if (vias)
        killDG(vias, NULL, viaFreeEdge);

    freeQuadras(quadras, NULL);

    return 0;
}



