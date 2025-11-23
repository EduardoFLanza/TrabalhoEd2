/**
 * ============================================================================
 * Implementação do módulo digraph
 * ============================================================================
 */

#include <stdlib.h>
#include <string.h>
#include "digraph.h"
#include "listadj.h"
#include "lista.h"

/* Estrutura concreta: NÃO aparece no .h */
typedef struct digraph {
    ListAdj la;  /* Estrutura interna baseada em lista de adjacência */
} DG;

/* ============================================================================
 * Criação e destruição
 * ============================================================================
 */

Digraph dg_create() {
    DG* g = calloc(1, sizeof(DG));
    g->la = criaListAdj();
    return g;
}

void dg_destroy(Digraph G,
                void (*freeV)(GInfoVert),
                void (*freeA)(GInfoArest))
{
    DG* g = (DG*) G;

    /* Percorre vértices */
    int N = numVertices(g->la);

    for (int i = 0; i < N; i++) {
        const char* nome = getVertNameByIndex(g->la, i);
        Lista adj = getAdjListByIndex(g->la, i);

        /* libera informações de arestas */
        for (Posic p = getFirstLst(adj); p != NULL; p = getNextLst(p)) {
            void* ar = getLst(p);
            if (freeA) freeA( ((void**)ar)[3] );
        }

        /* libera info vertice */
        if (freeV) {
            GInfoVert iv = getInfoVert(g->la, nome);
            if (iv) freeV(iv);
        }
    }

    /* libera estrutura interna */
    free(g);
}

/* ============================================================================
 * Operações sobre vértices
 * ============================================================================
 */

int dg_addVertex(Digraph G, const char* nome, GInfoVert info) {
    DG* g = (DG*) G;
    return addVertice(g->la, nome, info);
}

GInfoVert dg_getVertexInfo(Digraph G, const char* nome) {
    DG* g = (DG*) G;
    return getInfoVert(g->la, nome);
}

int dg_numVertices(Digraph G) {
    DG* g = (DG*) G;
    return numVertices(g->la);
}

int dg_getVertexIndex(Digraph G, const char* nome) {
    DG* g = (DG*) G;
    return getVertIndex(g->la, nome);
}

const char* dg_getVertexNameByIndex(Digraph G, int index) {
    DG* g = (DG*) G;
    return getVertNameByIndex(g->la, index);
}

/* ============================================================================
 * Operações sobre arestas
 * ============================================================================
 */

int dg_addEdge(Digraph G, const char* origem, const char* destino,
               double peso, GInfoArest info)
{
    DG* g = (DG*) G;
    return addAresta(g->la, origem, destino, peso, info);
}

Lista dg_getAdjList(Digraph G, const char* nome) {
    DG* g = (DG*) G;
    return getAdjacentes(g->la, nome);
}

/* ============================================================================
 * BFS, DFS, Shortest Path
 * (placeholders — você pode implementar se precisar)
 * ============================================================================
 */

void dg_bfs(Digraph G, const char* start, void (*visit)(const char*)) {
    (void)G; (void)start; (void)visit;
}

void dg_dfs(Digraph G, const char* start, void (*visit)(const char*)) {
    (void)G; (void)start; (void)visit;
}

Lista dg_shortestPath(Digraph G, const char* origem, const char* destino) {
    (void)G; (void)origem; (void)destino;
    return NULL;
}
