#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "qry.h"
#include "via.h"
#include "path.h"
#include "lista.h"

/* Estruturas auxiliares */

typedef struct {
    Node origem;
    Node destino;
} RotaAtual;

/* Funções auxiliares */

void drawRecSvgQry(SInfo info, double x, double y, double mbbX1, double mbbY1, double mbbX2, double mbbY2, void *aux)
{
    ArqSvg svgArq = (ArqSvg)aux;
    escreveRetanguloSvg(svgArq, getQuadraX(info), getQuadraY(info), getQuadraWidth(info), getQuadraHeight(info), getQuadraCFill(info), getQuadraCStrk(info), getQuadraSW(info), getQuadraOpacidade(info));
}

typedef struct {
    Graph grafo;
    FILE* file;
    double* vmTotal;
    double placement;
} ResourcesAnimation;

void animateEdgeSVG(void* item, void* extra) {
    Edge e = (Edge)item;
    ResourcesAnimation* res = (ResourcesAnimation*)extra;

    Node f = getFromNode(res->grafo, e);
    Node t = getToNode(res->grafo, e);

    // Pegamos a informação dos vértices (posições x,y)
    VerticeVia infoF = getNodeInfo(res->grafo, f);
    VerticeVia infoT = getNodeInfo(res->grafo, t);

    // Ajuste de coordenadas (offset para caminhos sobrepostos)
    double x1 = viaGetX(infoF) - res->placement;
    double y1 = viaGetY(infoF) - res->placement;
    double x2 = viaGetX(infoT) - res->placement;
    double y2 = viaGetY(infoT) - res->placement;

    // Cálculo do custo (tempo) para a duração da animação
    ArestaVia av = getEdgeInfo(res->grafo, e);
    double comprimento = viaGetLength(av);
    double velocidade = viaGetSpeed(av);
    
    if (velocidade > 0) {
        *(res->vmTotal) += (comprimento / velocidade);
    }

    // Adiciona os pontos ao atributo 'd' do path
    // M (moveto) já foi impresso, aqui usamos L (lineto) implicitamente ou explicitamente
    fprintf(res->file, " L %.2f,%.2f", x1, y1);
    fprintf(res->file, " L %.2f,%.2f", x2, y2);
}

void drawAnimatedPathSvg(ArqSvg svg, Graph g, Caminho c, char* color) {
    if (!svg || !g || !c) return;

    static int pathID = 0;
    double tempoTotal = 0;
    Lista edgesDestinoOrigem = getDijkstraList(c);
    
    // Invertemos a lista para que a animação comece na Origem
    // Se a sua lista for duplamente encadeada, podemos percorrer do fim para o início
    
    // Imprime o início do path
    fprintf(svg, "\n<path id=\"path%d\" d=\"", pathID);
    
    // Ponto inicial (Origem)
    Edge ultimaAresta = (Edge)getLst(edgesDestinoOrigem, getLastLst(edgesDestinoOrigem));
    Node origemNode = getFromNode(g, ultimaAresta);
    VerticeVia infoOrigem = getNodeInfo(g, origemNode);
    fprintf(svg, "M %.2f,%.2f", viaGetX(infoOrigem) - (pathID * 2.0), viaGetY(infoOrigem) - (pathID * 2.0));

    // Percorre as arestas do fim (origem) para o início (destino)
    ResourcesAnimation res = {g, svg, &tempoTotal, (double)pathID * 2.0};
    
    Posic p = getLastLst(edgesDestinoOrigem);
    while (p != NIL) {
        animateEdgeSVG(getLst(edgesDestinoOrigem, p), &res);
        p = getPreviousLst(edgesDestinoOrigem, p);
    }

    // Fecha o path e define estilo
    fprintf(svg, "\" fill=\"none\" stroke=\"%s\" stroke-width=\"4\" stroke-opacity=\"0.6\" />", color);

    // Adiciona o círculo animado (o "carro")
    // O tempo (dur) é baseado no cálculo de tempo real (distância/velocidade)
    fprintf(svg, "\n<circle r=\"6\" fill=\"%s\">", color);
    fprintf(svg, "\n  <animateMotion dur=\"%.1fs\" repeatCount=\"indefinite\" rotate=\"auto\">", tempoTotal);
    fprintf(svg, "\n    <mpath href=\"#path%d\"/>", pathID);
    fprintf(svg, "\n  </animateMotion>");
    fprintf(svg, "\n</circle>\n");

    pathID++;
    killLst(edgesDestinoOrigem, NULL);
}

/* Funções de custo para Dijkstra */

static double custoPorComprimento(Info info) {
    if (!info) return 1e12;
    return viaGetLength(info);
}

static double custoPorTempo(Info info) {
    if (!info) return 1e12;
    double vm = viaGetSpeed(info);
    if (vm <= 0) return 1e12;
    return viaGetLength(info) / vm;
}

/* CATAC */
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
                                "white", "red", "2px", 1.0f);

            removeSTrp(quadrasStreap, getQuadraX(q), getQuadraY(q));

            removerQuadra(quadras, q);
        }
    }
    killLst(removidas, NULL);
    
    Lista arestas = createLst(-1);
    getEdges(g, arestas);
    Edge e;
    while (!isEmptyLst(arestas)) {
        e = popLst(arestas);
        ArestaVia av = getEdgeInfo(g, e);
        if (!av || !viaIsEnabled(av)) continue;

        Node u = getFromNode(g, e);
        Node v = getToNode(g, e);
        VerticeVia vu = getNodeInfo(g, u);
        VerticeVia vv = getNodeInfo(g, v);

        double mx = (viaGetX(vu) + viaGetX(vv)) / 2;
        double my = (viaGetY(vu) + viaGetY(vv)) / 2;

        if (mx >= x && mx <= x + w && my >= y && my <= y + h) {
            viaDisable(av);
            fprintf(txt, "Via desabilitada: %s\n", viaGetName(av));
        }
    }
    killLst(arestas, NULL);
}

/*  BL / REBL */

static void cmdBl(FILE* txt, Graph g,
                  double x, double y, double w, double h) {

    fprintf(txt, "BL x=%lf y=%lf w=%lf h=%lf\n", x, y, w, h);

    Lista arestas = createLst(-1);
    getEdges(g, arestas);

    Edge e;
    while ((e = popLst(arestas)) != NULL) {
        ArestaVia av = getEdgeInfo(g, e);
        if (!av) continue;

        Node u = getFromNode(g, e);
        Node v = getToNode(g, e);

        VerticeVia vu = getNodeInfo(g, u);
        VerticeVia vv = getNodeInfo(g, v);

        double mx = (viaGetX(vu) + viaGetX(vv)) / 2;
        double my = (viaGetY(vu) + viaGetY(vv)) / 2;

        if (mx >= x && mx <= x + w &&
            my >= y && my <= y + h) {

            viaDisable(av);
            fprintf(txt, "Via bloqueada: %s\n",
                    viaGetName(av));
        }
    }
    killLst(arestas, NULL);
}

static void cmdRebl(FILE* txt, Graph g, const char* nome) {

    fprintf(txt, "REBL %s\n", nome);

    Lista arestas = createLst(-1);
    getEdges(g, arestas);

    Edge e;
    while ((e = popLst(arestas)) != NULL) {
        ArestaVia av = getEdgeInfo(g, e);
        if (!av) continue;

        if (strcmp(viaGetName(av), nome) == 0) {
            viaEnable(av);
            fprintf(txt, "Via reabilitada: %s\n", nome);
        }
    }
    killLst(arestas, NULL);
}

/* ORIGEM / DESTINO */
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

/* p? — caminho */
static void cmdPath(FILE* txt, ArqSvg svg, Graph g,
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

    printf("\n [aklghskl] \n");

    drawAnimatedPathSvg(svg, g, c, porTempo ? "blue" : "green");

    freeCaminho(c);
}

/* PROCESSAMENTO DO QRY */
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
            char nome[128];
            fscanf(f, "%s", nome);
            cmdRebl(txt, g, nome);
        }
        else if (!strcmp(cmd, "@o?")) {
            char idOrigem[64], sufixo[64], numero[64];
            fscanf(f, "%s %s %s", idOrigem, sufixo, numero);
            cmdOrigem(txt, g, &rota, idOrigem);
        }
        else if (!strcmp(cmd, "p?")) {
            char idDestino[64], sufixo[64], cor1[64], cor2[64];

            if (fscanf(f, "%s %s %s %s", idDestino, sufixo, cor1, cor2) == 4) {    
                cmdDestino(txt, g, &rota, idDestino);
                
                cmdPath(txt, svg, g, &rota, true);
                cmdPath(txt, svg, g, &rota, false);
                
                printf("[p?] Caminho calculado de %s para %s\n", 
                        getNodeName(g, rota.origem), idDestino);
            }
        }
    }

    percursoLargura(quadrasStreap, drawRecSvgQry, svg);

    fclose(f);
}


