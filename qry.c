#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "qry.h"
#include "graph.h"
#include "via.h"
#include "quadra.h"
#include "svg.h"
#include "path.h"
#include "lista.h"
#include "stree.h"

/* =========================================================
   Estruturas auxiliares
   ========================================================= */

typedef struct {
    Node origem;
    Node destino;
} RotaAtual;

/* =========================================================
   Funções de custo para Dijkstra
   ========================================================= */

static double custoPorComprimento(Info info) {
    if (!info) return 1e12;
    return getArestaCMP(info);
}

static double custoPorTempo(Info info) {
    if (!info) return 1e12;
    double vm = getArestaVM(info);
    if (vm <= 0) return 1e12;
    return getArestaCMP(info) / vm;
}

/* =========================================================
   CATAC
   ========================================================= */

static void cmdCatac(FILE* txt, Svg svg,
                      STree quadras,
                      Graph g,
                      double x, double y, double w, double h) {

    fprintf(txt,
        "CATAC x=%lf y=%lf w=%lf h=%lf\n", x, y, w, h);

    Lista removidas = createLista();
    streeSearchRect(quadras, x, y, w, h, removidas);

    void* q;
    while ((q = popLista(removidas)) != NULL) {
        drawQuadra(svg, q, "red", "none");
        fprintf(txt, "Quadra removida: %s\n", getQuadraCep(q));
        streeRemove(quadras, getQuadraX(q), getQuadraY(q));
    }
    freeLista(removidas);

    Lista arestas = createLista();
    getEdges(g, arestas);

    Edge e;
    while ((e = popLista(arestas)) != NULL) {
        ArestaVia av = getEdgeInfo(g, e);
        if (!av || !isArestaEnabled(av)) continue;

        Node u = getFromNode(g, e);
        Node v = getToNode(g, e);

        VerticeVia vu = getNodeInfo(g, u);
        VerticeVia vv = getNodeInfo(g, v);

        double mx = (getVerticeViaX(vu) + getVerticeViaX(vv)) / 2;
        double my = (getVerticeViaY(vu) + getVerticeViaY(vv)) / 2;

        if (mx >= x && mx <= x + w && my >= y && my <= y + h) {
            blockVia(av);
            fprintf(txt, "Via desabilitada: %s\n",
                    getArestaViaNome(av));
        }
    }
    freeLista(arestas);
}

/* =========================================================
   BL / REBL
   ========================================================= */

static void cmdBl(FILE* txt, Graph g,
                  double x, double y, double w, double h) {

    fprintf(txt, "BL x=%lf y=%lf w=%lf h=%lf\n", x, y, w, h);

    Lista arestas = createLista();
    getEdges(g, arestas);

    Edge e;
    while ((e = popLista(arestas)) != NULL) {
        ArestaVia av = getEdgeInfo(g, e);
        if (!av) continue;

        Node u = getFromNode(g, e);
        Node v = getToNode(g, e);

        VerticeVia vu = getNodeInfo(g, u);
        VerticeVia vv = getNodeInfo(g, v);

        double mx = (getVerticeViaX(vu) + getVerticeViaX(vv)) / 2;
        double my = (getVerticeViaY(vu) + getVerticeViaY(vv)) / 2;

        if (mx >= x && mx <= x + w && my >= y && my <= y + h) {
            blockVia(av);
            fprintf(txt, "Via bloqueada: %s\n",
                    getArestaViaNome(av));
        }
    }
    freeLista(arestas);
}

static void cmdRebl(FILE* txt, Graph g, const char* nome) {

    fprintf(txt, "REBL %s\n", nome);

    Lista arestas = createLista();
    getEdges(g, arestas);

    Edge e;
    while ((e = popLista(arestas)) != NULL) {
        ArestaVia av = getEdgeInfo(g, e);
        if (!av) continue;

        if (strcmp(getArestaViaNome(av), nome) == 0) {
            unblockVia(av);
            fprintf(txt, "Via reabilitada: %s\n", nome);
        }
    }
    freeLista(arestas);
}

/* =========================================================
   ORIGEM / DESTINO
   ========================================================= */

static void cmdOrigem(FILE* txt, Graph g,
                      RotaAtual* rota, const char* id) {

    rota->origem = getNode(g, (char*)id);
    fprintf(txt, "Origem definida: %s\n", id);
}

static void cmdDestino(FILE* txt, Graph g,
                       RotaAtual* rota, const char* id) {

    rota->destino = getNode(g, (char*)id);
    fprintf(txt, "Destino definido: %s\n", id);
}

/* =========================================================
   P? — CAMINHO COM ANIMAÇÃO
   ========================================================= */

static void cmdPath(FILE* txt, Svg svg, Graph g,
                    RotaAtual* rota, bool porTempo) {

    if (rota->origem < 0 || rota->destino < 0) return;

    Caminho c = getShortestPath(
        g,
        rota->origem,
        rota->destino,
        porTempo ? custoPorTempo : custoPorComprimento
    );

    if (!c) {
        fprintf(txt, "Caminho inexistente\n");
        return;
    }

    fprintf(txt, "Caminho %s | Distancia: %.2lf\n",
            porTempo ? "tempo" : "comprimento",
            getDijkstraDistance(c));

    drawAnimatedPathSvg(svg, g, c,
        porTempo ? "blue" : "green");

    freeCaminho(c);
}

/* =========================================================
   PROCESSAMENTO DO QRY
   ========================================================= */

void processQryFile(const char* path,
                    Graph g,
                    STree quadras,
                    Svg svg,
                    FILE* txt) {

    FILE* f = fopen(path, "r");
    if (!f) return;

    RotaAtual rota;
    rota.origem = -1;
    rota.destino = -1;

    char cmd[32];

    while (fscanf(f, "%s", cmd) != EOF) {

        if (!strcmp(cmd, "catac")) {
            double x, y, w, h;
            fscanf(f, "%lf %lf %lf %lf", &x, &y, &w, &h);
            cmdCatac(txt, svg, quadras, g, x, y, w, h);
        }
        else if (!strcmp(cmd, "bl")) {
            double x, y, w, h;
            fscanf(f, "%lf %lf %lf %lf", &x, &y, &w, &h);
            cmdBl(txt, g, x, y, w, h);
        }
        else if (!strcmp(cmd, "rebl")) {
            char nome[128];
            fscanf(f, "%s", nome);
            cmdRebl(txt, g, nome);
        }
        else if (!strcmp(cmd, "@o")) {
            char id[64];
            fscanf(f, "%s", id);
            cmdOrigem(txt, g, &rota, id);
        }
        else if (!strcmp(cmd, "@p")) {
            char id[64];
            fscanf(f, "%s", id);
            cmdDestino(txt, g, &rota, id);
        }
        else if (!strcmp(cmd, "p?")) {
            char tipo;
            fscanf(f, " %c", &tipo);
            cmdPath(txt, svg, g, &rota, tipo == 't');
        }
    }

    fclose(f);
}

