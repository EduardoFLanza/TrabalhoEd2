#include "STreap.h"
#include "entity.h"
#include "geoFile.h"
#include "qryFile.h"
#include "dotFile.h"
#include "cmdsFiles.h"
#include "path.h"
#include "drawSvg.h"
#include "actions.h"
#include "lista.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Descrição: Imprime informações de um nó da STreap para depuração.
 * Parâmetros:
 *   n: nó da STreap
 *   aux: ponteiro auxiliar não utilizado
 */
void printNodeST(SNode n, void *aux)
{
    double x, y;
    Entity e;

    st_getKey(n, &x, &y);
    e = st_getInfo(n);

    printf("(%.2lf, %.2lf) [%d]\n", x, y, getEntId(e));
}

/*
 * Função principal do programa
 */
int main(int argc, char *argv[])
{
    int i;

    /* Ponteiros para os argumentos recebidos na linha de comando */
    char *entryArg  = NULL;  // Diretório de entrada (-e)
    char *outputArg = NULL;  // Diretório de saída (-o)
    char *geoArg    = NULL;  // Arquivo .geo (-f)
    char *qryArg    = NULL;  // Arquivo .qry (-q)

    /* Caminhos normalizados de entrada e saída */
    char *entryPath  = NULL; // Caminho base de entrada
    char *outputPath = NULL; // Caminho base de saída

    /* Componentes do arquivo .geo */
    char *geoFilePath = NULL; // Caminho relativo do .geo
    char *geoFileName = NULL; // Nome do arquivo .geo
    char *geoFile     = NULL; // Caminho completo do .geo

    /* Componentes do arquivo .qry */
    char *qryFilePath = NULL; // Caminho relativo do .qry
    char *qryFileName = NULL; // Nome do arquivo .qry
    char *qryFile     = NULL; // Caminho completo do .qry

    int numsectors = 0;
    double factor  = 0.0;

    /* Leitura dos argumentos da linha de comando */
    for (i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-e")) {
            entryArg = argv[++i];
        } else if (!strcmp(argv[i], "-o")) {
            outputArg = argv[++i];
        } else if (!strcmp(argv[i], "-f")) {
            geoArg = argv[++i];
        } else if (!strcmp(argv[i], "-q")) {
            qryArg = argv[++i];
        } else if (!strcmp(argv[i], "-ns")) {
            numsectors = atoi(argv[++i]);
        } else if (!strcmp(argv[i], "-fd")) {
            factor = atof(argv[++i]);
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

    /* Processamento do arquivo .geo */
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

    /* Processamento do arquivo .qry (se existir) */
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
    }

    /* Inicialização da STreap */
    factor /= 100.0;

    STreap Elements = st_create(1e-6);
    if (!Elements) {
        fprintf(stderr, "Erro ao criar STreap\n");
        return 1;
    }

    Style style = createTextStyle("arial", "normal", 16);

    /* Leitura do arquivo .geo */
    if (ReadGeoFile(Elements, geoFile, style)) {
        fprintf(stderr, "Erro ao ler arquivo .geo\n");
        return 1;
    }

    /* Geração do SVG inicial */
    char geoNameNoExt[256];
    getFileNameWithoutExt(geoFileName, geoNameNoExt, 256);

    char svgGeo[512];
    sprintf(svgGeo, "%s/%s.svg", outputPath, geoNameNoExt);

    ArqSvg svg = abreEscritaSvg(svgGeo);
    WriteSTreapEntsInSVG(svg, Elements, style);
    fechaSvg(svg);

    char dotGeo[512];
    sprintf(dotGeo, "%s/%s.dot", outputPath, geoNameNoExt);
    printSTrp(Elements, dotGeo);

    /* Processamento do arquivo .qry */
    Lista Decos = createLst(-1);

    if (qryFile) {
        char qryNameNoExt[256];
        getFileNameWithoutExt(qryFileName, qryNameNoExt, 256);

        char svgFinal[512];
        sprintf(svgFinal, "%s/%s-%s.svg",
                outputPath, geoNameNoExt, qryNameNoExt);

        if (ReadQryFile(Elements, qryFile, outputPath,
                        geoNameNoExt, style, Decos)) {
            fprintf(stderr, "Erro ao ler arquivo .qry\n");
            return 1;
        }

        ArqSvg svgF = abreEscritaSvg(svgFinal);
        WriteSTreapEntsInSVG(svgF, Elements, style);
        if (!isEmptyLst(Decos))
            WriteGeoListInSvg(svgF, Decos, style, 0, 0);
        fechaSvg(svgF);

        char dotFinal[512];
        sprintf(dotFinal, "%s/%s-%s.dot",
                outputPath, geoNameNoExt, qryNameNoExt);
        printSTrp(Elements, dotFinal);
    }

    /* Liberação de memória */
    removeLista(Decos, NULL);
    st_destroy(Elements, freeEntity);
    free(style);

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

    printf("Executado com sucesso\n");
    return 0;
}


