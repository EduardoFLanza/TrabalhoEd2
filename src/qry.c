#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "qry.h"
#include "via.h"
#include "path.h"
#include "lista.h"

/* =========================================================
   Estruturas auxiliares
   ========================================================= */

typedef struct {
    Node origem;
    Node destino;
} RotaAtual;

/* =========================================================
   Função LOCAL: encontra vértice mais próximo (NÃO EXISTIA)
   ========================================================= */

static Node findNearestNode(Graph g, double x, double y) {
    int total = getTotalNodes(g);
    Node best = -1;
    double bestDist = 1e18;

    for (Node n = 0; n < total; n++) {
        VerticeVia v = getNodeInfo(g, n);
        if (!v) continue;

        double dx = viaGetX(v) - x;
        double dy = viaGetY(v) - y;
        double d2 = dx*dx + dy*dy;

        if (d2 < bestDist) {
            bestDist = d2;
            best = n;
        }
    }
    return best;
}

/* =========================================================
   Funções auxiliares SVG
   ========================================================= */

void drawRecSvgQry(SInfo info, double x, double y,
                   double mbbX1, double mbbY1,
                   double mbbX2, double mbbY2,
                   void *aux)
{
    ArqSvg svgArq = (ArqSvg)aux;
    escreveRetanguloSvg(
        svgArq,
        getQuadraX(info),
        getQuadraY(info),
        getQuadraWidth(info),
        getQuadraHeight(info),
        getQuadraCFill(info),
        getQuadraCStrk(info),
        getQuadraSW(info),
        getQuadraOpacidade(info)
    );
}

/* =========================================================
   Animação de caminho
   ========================================================= */

typedef struct {
    Graph grafo;
    FILE* file;
    double* tempoTotal;
    double placement;
} ResourcesAnimation;

void animateEdgeSVG(void* item, void* extra) {
    Edge e = (Edge)item;
    ResourcesAnimation* res = (ResourcesAnimation*)extra;

    Node u = getFromNode(res->grafo, e);
    Node v = getToNode(res->grafo, e);

    VerticeVia vu = getNodeInfo(res->grafo, u);
    VerticeVia vv = getNodeInfo(res->grafo, v);

    double x1 = viaGetX(vu) - res->placement;
    double y1 = viaGetY(vu) - res->placement;
    double x2 = viaGetX(vv) - res->placement;
    double y2 = viaGetY(vv) - res->placement;

    ArestaVia av = getEdgeInfo(res->grafo, e);
    if (av && viaGetSpeed(av) > 0) {
        *(res->tempoTotal) += viaGetLength(av) / viaGetSpeed(av);
    }

    fprintf(res->file, " L %.2f,%.2f", x1, y1);
    fprintf(res->file, " L %.2f,%.2f", x2, y2);
}

void drawAnimatedPathSvg(ArqSvg svg, Graph g, Caminho c, char* color) {
    if (!svg || !g || !c) return;

    static int pathID = 0;
    double tempoTotal = 0;

    Lista edges = getDijkstraList(c);

    fprintf(svg, "\n<path id=\"path%d\" d=\"", pathID);

    Edge last = getLst(edges, getLastLst(edges));
    Node origem = getFromNode(g, last);
    VerticeVia vo = getNodeInfo(g, origem);

    fprintf(svg, "M %.2f,%.2f",
            viaGetX(vo) - pathID * 2.0,
            viaGetY(vo) - pathID * 2.0);

    ResourcesAnimation res = { g, svg, &tempoTotal, pathID * 2.0 };

    for (Posic p = getLastLst(edges); p != NIL; p = getPreviousLst(edges, p)) {
        animateEdgeSVG(getLst(edges, p), &res);
    }

    fprintf(svg,
            "\" fill=\"none\" stroke=\"%s\" stroke-width=\"4\" "
            "stroke-opacity=\"0.6\" />",
            color);

    fprintf(svg,
            "\n<circle r=\"6\" fill=\"%s\">"
            "<animateMotion dur=\"%.1fs\" repeatCount=\"indefinite\" rotate=\"auto\">"
            "<mpath href=\"#path%d\"/>"
            "</animateMotion></circle>\n",
            color, tempoTotal, pathID);

    pathID++;
    killLst(edges, NULL);
}

/* =========================================================
   Custos Dijkstra
   ========================================================= */

static double custoPorComprimento(Info info) {
    if (!info) return 1e12;
    return viaGetLength(info);
}

static double custoPorTempo(Info info) {
    if (!info) return 1e12;
    double v = viaGetSpeed(info);
    if (v <= 0) return 1e12;
    return viaGetLength(info) / v;
}

/* =========================================================
   ORIGEM / DESTINO (quadra + endereço)
   ========================================================= */

static void calculaEndereco(Quadra q, const char* face, double num,
                            double* x, double* y) {

    double qx = getQuadraX(q);
    double qy = getQuadraY(q);
    double w  = getQuadraWidth(q);
    double h  = getQuadraHeight(q);

    if (!strcmp(face, "N")) { *x = qx + num; *y = qy + h; }
    else if (!strcmp(face, "S")) { *x = qx + num; *y = qy; }
    else if (!strcmp(face, "L")) { *x = qx + w; *y = qy + num; }
    else { *x = qx; *y = qy + num; }
}

static void cmdOrigem(FILE* txt, Graph g, Quadras quadras,
                      RotaAtual* rota,
                      const char* id, const char* face, const char* num) {

    Quadra q = getQuadra(quadras, id);
    if (!q) {
        rota->origem = -1;
        return;
    }

    double x, y;
    calculaEndereco(q, face, atof(num), &x, &y);
    rota->origem = findNearestNode(g, x, y);

    fprintf(txt, "Origem: %s %s %s\n", id, face, num);
}

static void cmdDestino(FILE* txt, Graph g, Quadras quadras,
                       RotaAtual* rota,
                       const char* id, const char* face, const char* num) {

    Quadra q = getQuadra(quadras, id);
    if (!q) {
        rota->destino = -1;
        return;
    }

    double x, y;
    calculaEndereco(q, face, atof(num), &x, &y);
    rota->destino = findNearestNode(g, x, y);

    fprintf(txt, "Destino: %s %s %s\n", id, face, num);
}

/* =========================================================
   p?
   ========================================================= */

static void cmdPath(FILE* txt, ArqSvg svg, Graph g,
                    RotaAtual* rota, bool tempo) {

    if (rota->origem < 0 || rota->destino < 0) return;

    Caminho c = getShortestPath(
        g, rota->origem, rota->destino,
        tempo ? custoPorTempo : custoPorComprimento
    );

    if (!c) {
        fprintf(txt, "Caminho inexistente\n");
        return;
    }

    fprintf(txt, "Distancia: %.2lf\n", getDijkstraDistance(c));
    drawAnimatedPathSvg(svg, g, c, tempo ? "blue" : "green");
    freeCaminho(c);
}

/* =========================================================
   PROCESSAMENTO QRY
   ========================================================= */

void processQryFile(const char* path,
                    Graph g,
                    Quadras quadras,
                    STreap quadrasStreap,
                    ArqSvg svg,
                    FILE* txt) {

    FILE* f = fopen(path, "r");
    if (!f) return;

    RotaAtual rota = { -1, -1 };
    char cmd[32];

    while (fscanf(f, "%s", cmd) != EOF) {

        if (!strcmp(cmd, "catac")) {
            double x, y, w, h;
            fscanf(f, "%lf %lf %lf %lf", &x, &y, &w, &h);
            cmdCatac(txt, svg, quadras, quadrasStreap, g, x, y, w, h);
        }
        else if (!strcmp(cmd, "bl")) {
            double x, y, w, h;
            fscanf(f, "%lf %lf %lf %lf", &x, &y, &w, &h);
            cmdBl(txt, g, x, y, w, h);
        }
        else if (!strcmp(cmd, "rebl")) {
            char nome[64];
            fscanf(f, "%s", nome);
            cmdRebl(txt, g, nome);
        }
        else if (!strcmp(cmd, "@o?")) {
            char id[64], face[8], num[32];
            fscanf(f, "%s %s %s", id, face, num);
            cmdOrigem(txt, g, quadras, &rota, id, face, num);
        }
        else if (!strcmp(cmd, "p?")) {
            char id[64], face[8], num[32], c1[32];
            fscanf(f, "%s %s %s %s", id, face, num, c1);
            cmdDestino(txt, g, quadras, &rota, id, face, num);
            cmdPath(txt, svg, g, &rota, true);
            cmdPath(txt, svg, g, &rota, false);
        }
    }

    percursoLargura(quadrasStreap, drawRecSvgQry, svg);
    fclose(f);
}


