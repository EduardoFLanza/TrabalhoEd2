// Includes essenciais
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Includes de funcoes auxiliares (manipulação de arquivos)
#include "path.h"
#include "drawSvg.h"
#include "dotFile.h"

// Includes de estruturas de dados (armazenamento)
#include "lista.h"
#include "digraph.h"
#include "STreap.h"

// Includes de funcoes principais do trabalho (processamento)
#include "quadras.h"
#include "via.h"
#include "qry.h"

/* ===== FUNCOES PARA RECALL ===== */
/* =========================================================================== */
void drawRecSvg(SInfo info, double x, double y, double mbbX1, double mbbY1, double mbbX2, double mbbY2, void *aux)
{
    ArqSvg svgArq = (ArqSvg)aux;
    escreveRetanguloSvg(svgArq, getQuadraX(info), getQuadraY(info), getQuadraWidth(info), getQuadraHeight(info), getQuadraCFill(info), getQuadraCStrk(info), getQuadraSW(info), getQuadraOpacidade(info));
}

void inserirNaStreap(Quadra quadra, double x, double y, void *aux)
{
    STreap st = (STreap)aux;
    st_insert(st, x, y, quadra);
}

void viaFreeEdgeFunc(Edge e){
    viaFreeEdge(getEdgeInfo(NULL, e));
    free(e);
}

/* ===== PRINCIPAL ===== */
/* =========================================================================== */
int main(int argc, char *argv[])
{
    /* ===== ARGUMENTOS ===== */
    /* =========================================================================== */

    char *entryArg = NULL;   // -e (Entrada)
    char *outputArg = NULL;  // -o (Saida)
    char *geoArg = NULL;     // -f (Geo)
    char *qryArg = NULL;     // -q (Qry)
    char *viaArg = NULL;     // -v (Via)

    // Pega todos os argumentos dados.
    for (int i = 1; i < argc; i += 2) {
        if      (!strcmp(argv[i], "-e")) entryArg   = argv[i+1];
        else if (!strcmp(argv[i], "-o")) outputArg  = argv[i+1];
        else if (!strcmp(argv[i], "-f")) geoArg     = argv[i+1];
        else if (!strcmp(argv[i], "-q")) qryArg     = argv[i+1];
        else if (!strcmp(argv[i], "-v")) viaArg     = argv[i+1];
    }

    // Se a Saida ou Geo for inexistente, termina o programa.
    if (!outputArg || !geoArg) {
        fprintf(stderr, "Parâmetros obrigatórios ausentes (Geo ou Saida).\n");
        return 1;
    }

    // Caminho de entrada = (entryAgr ? "." : entryArg) (Sem o separador '/' ao final da string)
    char entryPath[512] = ".";
    if (entryArg) normalizePath(entryArg, entryPath, sizeof(entryPath));

    // Caminho de saida = outputArg
    char outputPath[512];
    normalizePath(outputArg, outputPath, sizeof(outputPath));

    /* ===== GEO ===== */
    /* =========================================================================== */

    // geoFile = [entryPath]/[geoArg], ex: ./c1.geo, entryPath = '.' e geoArg = 'c1.geo'.
    char geoFile[512];
    sprintf(geoFile, "%s/%s", entryPath, geoArg);

    printf("\n[ Geo: %s ]\n", geoFile);

    // Ponteiro de estrutura Quadras (quadras) eh o resultado do processamento do geo.
    Quadras quadras = processGeoFile(geoFile);
    if (!quadras) {
        fprintf(stderr, "Erro ao ler .geo\n");
        return 1;
    }

    // Popula uma STreap com as quadras do geo.
    STreap quadrasStreap = st_create(1e-7);
    percorrerQuadras(quadras, inserirNaStreap, quadrasStreap);

    /* ===== SVG INICIAL (Geo) ===== */
    /* =========================================================================== */

    // geoName = geoArg truncado a partir da ultima ocorrencia de um ponto '.', ex: c1.geo -> c1
    char geoName[256];
    getFileNameWithoutExt(geoArg, geoName, sizeof(geoName));

    // svgGeo = [outputPath]/[geoName].svg, ex: ./c1.svg, outputPath = '.', geoName = 'c1'
    char svgGeo[512];
    sprintf(svgGeo, "%s/%s.svg", outputPath, geoName);

    // Pega o boundingBox da raiz das quadrasStreap para dimensão do SVG
    double x1, y1, x2, y2;
    st_getNodeBB(st_getRoot(quadrasStreap), &x1, &y1, &x2, &y2);

    // Desenha as quadras do arquivo Geo no svg.
    ArqSvg svg = abreEscritaSvg(svgGeo, fabs(x1) + fabs(x2), fabs(y1) + fabs(y2));
    percursoLargura(quadrasStreap, drawRecSvg, svg);
    fechaSvg(svg);

    // Mesma coisa do svgGeo, porem com extensao .dot ao inves de .svg
    char dotGeo[512];
    sprintf(dotGeo, "%s/%s.dot", outputPath, geoName);
    printSTrp(quadrasStreap, dotGeo);

    /* ===== VIA ===== */
    /* =========================================================================== */

    // Se viaArg existir, processa e guarda em 'vias' (grafo direcionado).
    Graph vias = NULL;
    if (viaArg) {
        // Mesma coisa do geoFile, mas para a viaArg
        char viaFile[512];
        sprintf(viaFile, "%s/%s", entryPath, viaArg);

        // Mesma coisa das quadras, para o processamento da via.
        vias = viaReadFile(viaFile);
        if (!vias) {
            fprintf(stderr, "Erro ao ler .via\n");
            return 1;
        }
    }

    /* ===== QRY ===== */
    /* =========================================================================== */

    if (qryArg) {
        // qryFile = [geoPath - '.']/[qryArg].qry, ex: ./c1/q.qry, geoPath = './c1.geo' - '.' = './c1', qryArg = 'q'
        char qryFile[512];
        char entryGeoName[512];
        getFileNameWithoutExt(geoFile, entryGeoName, sizeof(geoFile));
        sprintf(qryFile, "%s/%s", entryGeoName, qryArg);

        printf("\n[ Qry: %s ]\n", qryFile);

        // Tira a extensão do qryArg.
        char qryName[256];
        getFileNameWithoutExt(qryArg, qryName, sizeof(qryName));

        // svgFinal = [outputPath]/[geoName]-[qryName].svg, ex: ./c1-q.svg, outputPath = '.', geoName = 'c1', qryName = 'q'
        char svgFinal[512];
        sprintf(svgFinal, "%s/%s-%s.svg", outputPath, geoName, qryName);

        // Mesma coisa do svgFinal, mas .txt
        char txtFinal[512];
        sprintf(txtFinal, "%s/%s-%s.txt", outputPath, geoName, qryName);

        FILE *txt = fopen(txtFinal, "w");

        /* ===== SVG SECUNDARIO (Qry) ===== */
        /* ================================ */
        
        // Abre e imprime todas as modificações do qry, junto com seu processamento.
        ArqSvg svgF = abreEscritaSvg(svgFinal, fabs(x1) + fabs(x2), fabs(y1) + fabs(y2));
        processQryFile(qryFile, vias, quadras, quadrasStreap, svgF, txt);
        fechaSvg(svgF);

        fclose(txt);

        // Faz um .dot com as mudanças do qry nas quadras
        char dotFinal[512];
        sprintf(dotFinal, "%s/%s-%s.dot", outputPath, geoName, qryName);
        printSTrp(quadrasStreap, dotFinal);
    }

    /* ===== LIBERAÇÃO ===== */
    /* =========================================================================== */
    
    if (vias)
        killDG(vias, viaFreeVertex, viaFreeEdgeFunc);
    
    st_destroy(quadrasStreap, NULL);
    freeQuadras(quadras);
    
    printf("\n Programa finalizado. ");

    return 0;
}
