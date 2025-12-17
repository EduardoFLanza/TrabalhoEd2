#include "quadras.h"
#include "via.h"
#include "qryFile.h"
#include "dotFile.h"
#include "path.h"
#include "drawSvg.h"
#include "lista.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Função principal do programa
 */
int main(int argc, char *argv[])
{
    int i;

    /* Argumentos da linha de comando */
    char *entryArg  = NULL;  /* -e */
    char *outputArg = NULL;  /* -o */
    char *geoArg    = NULL;  /* -f */
    char *qryArg    = NULL;  /* -q */
    char *viaArg    = NULL;  /* -v */

    /* Caminhos base */
    char *entryPath  = NULL;
    char *outputPath = NULL;

    /* Arquivo .geo */
    char *geoFilePath = NULL;
    char *geoFileName = NULL;
    char *geoFile     = NULL;

    /* Arquivo .qry */
    char *qryFilePath = NULL;
    char *qryFileName = NULL;
    char *qryFile     = NULL;

    /* Arquivo .via */
    char *viaFilePath = NULL;
    char *viaFileName = NULL;
    char *viaFile     = NULL;

    /* Leitura dos argumentos */
    for (i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-e")) {
            entryArg = argv[++i];
        } else if (!strcmp(argv[i], "-o")) {
            outputArg = argv[++i];
        } else if (!strcmp(argv[i], "-f")) {
            geoArg = argv[++i];
        } else if (!strcmp(argv[i], "-q")) {
            qryArg = argv[++i];
        } else if (!strcmp(argv[i], "-v")) {
            viaArg = argv[++i];
        }
    }

    if (!outputArg || !geoArg) {
        fprintf(stderr, "Erro: parâmetros obrigatórios ausentes\n");
        return 1;
    }

    /* Diretório de entrada */
    if (entryArg) {
        entryPath = malloc(strlen(entryArg) + 1);
        normalizePath(entryArg, entryPath, strlen(entryArg) + 1);
    } else {
        entryPath = malloc(2);
        strcpy(entryPath, ".");
    }

    /* Diretório de saída */
    outputPath = malloc(strlen(outputArg) + 1);
    normalizePath(outputArg, outputPath, strlen(outputArg) + 1);

    /* ===== Processamento do .geo ===== */

    geoFileName = malloc(strlen(geoArg) + 1);
    geoFilePath = malloc(strlen(geoArg) + 1);

    getFileName(geoArg, geoFileName, strlen(geoArg) + 1);
    getPath(geoArg, geoFilePath, strlen(geoArg) + 1);

    geoFile = malloc(strlen(entryPath) + strlen(geoFilePath) +
                     strlen(geoFileName) + 3);

    sprintf(geoFile, "%s/%s%s%s",
            entryPath,
            geoFilePath,
            strlen(geoFilePath) ? "/" : "",
            geoFileName);

    Quadras quadras = processGeoFile(geoFile);
    if (!quadras) {
        fprintf(stderr, "Erro ao processar arquivo .geo\n");
        return 1;
    }

    /* ===== Processamento do .via (opcional) ===== */

    Graph vias = NULL;

    if (viaArg) {
        viaFileName = malloc(strlen(viaArg) + 1);
        viaFilePath = malloc(strlen(viaArg) + 1);

        getFileName(viaArg, viaFileName, strlen(viaArg) + 1);
        getPath(viaArg, viaFilePath, strlen(viaArg) + 1);

        viaFile = malloc(strlen(entryPath) + strlen(viaFilePath) +
                         strlen(viaFileName) + 3);

        sprintf(viaFile, "%s/%s%s%s",
                entryPath,
                viaFilePath,
                strlen(viaFilePath) ? "/" : "",
                viaFileName);

        vias = processViaFile(viaFile);
        if (!vias) {
            fprintf(stderr, "Erro ao processar arquivo .via\n");
            return 1;
        }
    }

    /* ===== SVG inicial ===== */

    char geoNameNoExt[256];
    getFileNameWithoutExt(geoFileName, geoNameNoExt, 256);

    char svgGeo[512];
    sprintf(svgGeo, "%s/%s.svg", outputPath, geoNameNoExt);

    ArqSvg svg = abreEscritaSvg(svgGeo);
    WriteQuadrasInSVG(svg, quadras);
    fechaSvg(svg);

    char dotGeo[512];
    sprintf(dotGeo, "%s/%s.dot", outputPath, geoNameNoExt);
    printSTrp(getQuadrasSTrp(quadras), dotGeo);

    /* ===== Processamento do .qry ===== */

    if (qryArg) {
        qryFileName = malloc(strlen(qryArg) + 1);
        qryFilePath = malloc(strlen(qryArg) + 1);

        getFileName(qryArg, qryFileName, strlen(qryArg) + 1);
        getPath(qryArg, qryFilePath, strlen(qryArg) + 1);

        qryFile = malloc(strlen(entryPath) + strlen(qryFilePath) +
                         strlen(qryFileName) + 3);

        sprintf(qryFile, "%s/%s%s%s",
                entryPath,
                qryFilePath,
                strlen(qryFilePath) ? "/" : "",
                qryFileName);

        Lista Decos = createLst(-1);

        char qryNameNoExt[256];
        getFileNameWithoutExt(qryFileName, qryNameNoExt, 256);

        char svgFinal[512];
        sprintf(svgFinal, "%s/%s-%s.svg",
                outputPath, geoNameNoExt, qryNameNoExt);

        if (ReadQryFile(quadras, vias, qryFile, Decos)) {
            fprintf(stderr, "Erro ao processar arquivo .qry\n");
            return 1;
        }

        ArqSvg svgF = abreEscritaSvg(svgFinal);
        WriteQuadrasInSVG(svgF, quadras);
        if (!isEmptyLst(Decos))
            WriteGeoListInSvg(svgF, Decos, 0, 0);
        fechaSvg(svgF);

        char dotFinal[512];
        sprintf(dotFinal, "%s/%s-%s.dot",
                outputPath, geoNameNoExt, qryNameNoExt);
        printSTrp(getQuadrasSTrp(quadras), dotFinal);

        removeLista(Decos, NULL);
    }

    /* ===== Liberação de memória ===== */

    if (vias)
        killDG(vias, NULL, freeArestaVia);

    freeQuadras(quadras, NULL);

    free(entryPath);
    free(outputPath);
    free(geoFile);
    free(geoFileName);
    free(geoFilePath);

    if (qryFile) {
        free(qryFile);
        free(qryFileName);
        free(qryFilePath);
    }

    if (viaFile) {
        free(viaFile);
        free(viaFileName);
        free(viaFilePath);
    }

    printf("Executado com sucesso\n");
    return 0;
}


