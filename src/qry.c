#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "qry.h"
#include "via.h"
#include "path.h"
#include "lista.h"

typedef struct {
    Node origem;
    Node destino;
} RotaAtual;

typedef struct {
    Graph grafo;
    FILE* file;
    double* tempoTotal;
    double placement;
} ResourcesAnimation;

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

void animateEdgeSVG(void* item, void* extra) {
    Edge e = (Edge)item;
    ResourcesAnimation* res = (ResourcesAnimation*)extra;
    if (!e || !res) return;

    Node u = getFromNode(res->grafo, e);
    Node v = getToNode(res->grafo, e);

    VerticeVia vu = getNodeInfo(res->grafo, u);
    VerticeVia vv = getNodeInfo(res->grafo, v);

    if (!vu || !vv) return;

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

    Lista edges = getDijkstraList(c);
    if (!edges || isEmptyLst(edges)) return;

    static int pathID = 0;
    double tempoTotal = 0;

    fprintf(svg, "\n<path id=\"path%d\" d=\"", pathID);

    Posic lastPos = getLastLst(edges);
    Edge lastEdge = getLst(edges, lastPos);
    Node origem = getFromNode(g, lastEdge);
    VerticeVia vo = getNodeInfo(g, origem);

    if (vo) {
        fprintf(svg, "M %.2f,%.2f",
                viaGetX(vo) - pathID * 2.0,
                viaGetY(vo) - pathID * 2.0);
    }

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
}

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

static void cmdCatac(FILE* txt, ArqSvg svg,
                      Quadras quadras,
                      STreap quadrasStreap,
                      Graph g,
                      double x, double y, double w, double h) {
    
    fprintf(txt, "CATAC x=%lf y=%lf w=%lf h=%lf\n", x, y, w, h);

    Lista removidas = createLst(-1);
    getNodeRegiaoSTrp(quadrasStreap, x, y, w, h, removidas);
    
    while (!isEmptyLst(removidas)) {
        SNode noTreap = popLst(removidas);
        Quadra q = st_getInfo(noTreap);
        
        if (q) {
            fprintf(txt, "Quadra removida: %s\n", getQuadraID(q));
            escreveRetanguloSvg(svg, getQuadraX(q), getQuadraY(q), 
                                getQuadraWidth(q), getQuadraHeight(q), 
                                "#AB37C8", "#AA0044", "2px", 0.5f);

            removeSTrp(quadrasStreap, getQuadraX(q), getQuadraY(q));
            removerQuadra(quadras, q);
        }
    }
    killLst(removidas, NULL);
    
    Lista arestas = createLst(-1);
    getEdges(g, arestas);
    while (!isEmptyLst(arestas)) {
        Edge e = popLst(arestas);
        ArestaVia av = getEdgeInfo(g, e);
        if (!av || !viaIsEnabled(av)) continue;

        Node u = getFromNode(g, e);
        Node v = getToNode(g, e);
        VerticeVia vu = getNodeInfo(g, u);
        VerticeVia vv = getNodeInfo(g, v);

        if (vu && vv) {
            double mx = (viaGetX(vu) + viaGetX(vv)) / 2;
            double my = (viaGetY(vu) + viaGetY(vv)) / 2;

            if (mx >= x && mx <= x + w && my >= y && my <= y + h) {
                viaDisable(av);
                fprintf(txt, "Via desabilitada: %s\n", viaGetName(av));
            }
        }
    }
    killLst(arestas, NULL);
}

static void cmdBl(FILE* txt, Graph g,
                  double x, double y, double w, double h) {

    fprintf(txt, "BL x=%lf y=%lf w=%lf h=%lf\n", x, y, w, h);

    Lista arestas = createLst(-1);
    getEdges(g, arestas);

    while (!isEmptyLst(arestas)) {
        Edge e = popLst(arestas);
        ArestaVia av = getEdgeInfo(g, e);
        if (!av) continue;

        Node u = getFromNode(g, e);
        Node v = getToNode(g, e);
        VerticeVia vu = getNodeInfo(g, u);
        VerticeVia vv = getNodeInfo(g, v);

        if (vu && vv) {
            double mx = (viaGetX(vu) + viaGetX(vv)) / 2;
            double my = (viaGetY(vu) + viaGetY(vv)) / 2;

            if (mx >= x && mx <= x + w && my >= y && my <= y + h) {
                viaDisable(av);
                fprintf(txt, "Via bloqueada: %s\n", viaGetName(av));
            }
        }
    }
    killLst(arestas, NULL);
}

static void cmdRebl(FILE* txt, Graph g, const char* nome) {
    fprintf(txt, "REBL %s\n", nome);
    Lista arestas = createLst(-1);
    getEdges(g, arestas);

    while (!isEmptyLst(arestas)) {
        Edge e = popLst(arestas);
        ArestaVia av = getEdgeInfo(g, e);
        if (av && strcmp(viaGetName(av), nome) == 0) {
            viaEnable(av);
            fprintf(txt, "Via reabilitada: %s\n", nome);
        }
    }
    killLst(arestas, NULL);
}

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
    Quadra q = getQuadraByID(quadras, id);
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
    Quadra q = getQuadraByID(quadras, id);
    if (!q) {
        rota->destino = -1;
        return;
    }
    double x, y;
    calculaEndereco(q, face, atof(num), &x, &y);
    rota->destino = findNearestNode(g, x, y);
    fprintf(txt, "Destino: %s %s %s\n", id, face, num);
}

static void cmdPath(FILE* txt, ArqSvg svg, Graph g,
                    RotaAtual* rota, bool tempo, char* cor) {
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
    drawAnimatedPathSvg(svg, g, c, cor);
    freeCaminho(c);
}

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
            char id[64], face[8], num[32], c1[32], c2[32];
            fscanf(f, "%s %s %s %s %s", id, face, num, c1, c2);
            cmdDestino(txt, g, quadras, &rota, id, face, num);
            cmdPath(txt, svg, g, &rota, true, c1);
            cmdPath(txt, svg, g, &rota, false, c2);
        }
    }

    percursoLargura(quadrasStreap, drawRecSvgQry, svg);
    fclose(f);
}
