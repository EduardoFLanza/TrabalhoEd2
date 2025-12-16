#include "graph.h"
#include "lista.h"
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <stdbool.h>
#include <limits.h>
#include <stdio.h>

/* ============================================================
   Estruturas internas
   ============================================================ */

typedef struct EdgeImpl {
    int from;
    int to;
    double weight;
    Info info;
} EdgeImpl;

typedef struct NodeImpl {
    char* name;
    Info info;
    Lista edgesOut;  // Lista de Edge*
    Lista edgesIn;   // Lista de Edge*
} NodeImpl;

typedef struct GraphImpl {
    NodeImpl** nodes;
    int numNodes;
    int maxNodes;
    Lista allEdges;  // Lista de Edge*
} GraphImpl;

typedef struct CaminhoImpl {
    Lista pathEdges; // Lista de Edge*
    double distance;
} CaminhoImpl;

/* ============================================================
   Funções auxiliares internas
   ============================================================ */

static NodeImpl* getNodeInternal(Graph g, Node node) {
    GraphImpl* gr = (GraphImpl*) g;
    if(node < 0 || node >= gr->numNodes) return NULL;
    return gr->nodes[node];
}

static EdgeImpl* getEdgeInternal(Edge e) {
    return (EdgeImpl*) e;
}

static Node findNodeIndex(Graph g, const char* name) {
    GraphImpl* gr = (GraphImpl*) g;
    for(int i=0;i<gr->numNodes;i++) {
        if(strcmp(gr->nodes[i]->name, name)==0) return i;
    }
    return -1;
}

/* ============================================================
   Criação / destruição
   ============================================================ */

Graph createGraph(int nVert) {
    GraphImpl* g = malloc(sizeof(GraphImpl));
    g->nodes = malloc(sizeof(NodeImpl*) * nVert);
    g->numNodes = 0;
    g->maxNodes = nVert;
    g->allEdges = createLst(-1);
    return g;
}

void killDG(Graph g, freeFunc freeVerticeFunc, freeFunc freeEdgeFunc) {
    if(!g) return;
    GraphImpl* gr = (GraphImpl*) g;
    for(int i=0;i<gr->numNodes;i++){
        NodeImpl* n = gr->nodes[i];
        if(freeVerticeFunc) freeVerticeFunc(n->info);
        freeLst(n->edgesOut, freeEdgeFunc);
        freeLst(n->edgesIn, NULL);
        free(n->name);
        free(n);
    }
    freeLst(gr->allEdges, freeEdgeFunc);
    free(gr->nodes);
    free(gr);
}

void freeCaminho(Caminho caminho){
    CaminhoImpl* c = (CaminhoImpl*) caminho;
    freeLst(c->pathEdges, NULL);
    free(c);
}

/* ============================================================
   Vértices
   ============================================================ */

Node addNode(Graph g, char* nome, Info info){
    GraphImpl* gr = (GraphImpl*) g;
    if(gr->numNodes >= gr->maxNodes) return -1;
    if(findNodeIndex(g, nome)!=-1) return -1;
    NodeImpl* n = malloc(sizeof(NodeImpl));
    n->name = strdup(nome);
    n->info = info;
    n->edgesOut = createLst(-1);
    n->edgesIn  = createLst(-1);
    gr->nodes[gr->numNodes] = n;
    return gr->numNodes++;
}

Node getNode(Graph g, char* nome){
    return findNodeIndex(g, nome);
}

Info getNodeInfo(Graph g, Node node){
    NodeImpl* n = getNodeInternal(g, node);
    return n ? n->info : NULL;
}

char* getNodeName(Graph g, Node node){
    NodeImpl* n = getNodeInternal(g, node);
    return n ? n->name : NULL;
}

void setNodeInfo(Graph g, Node node, Info info){
    NodeImpl* n = getNodeInternal(g, node);
    if(n) n->info = info;
}

int getTotalNodes(Graph g){
    return ((GraphImpl*)g)->numNodes;
}

int getMaxNodes(Graph g){
    return ((GraphImpl*)g)->maxNodes;
}

/* ============================================================
   Arestas
   ============================================================ */

Edge addEdge(Graph g, Node from, Node to, Info info){
    GraphImpl* gr = (GraphImpl*) g;
    NodeImpl* nFrom = getNodeInternal(g, from);
    NodeImpl* nTo = getNodeInternal(g, to);
    if(!nFrom || !nTo) return NULL;
    EdgeImpl* e = malloc(sizeof(EdgeImpl));
    e->from = from;
    e->to   = to;
    e->info = info;
    e->weight = 1.0;
    insertLst(gr->allEdges, e);
    insertLst(nFrom->edgesOut, e);
    insertLst(nTo->edgesIn, e);
    return e;
}

Edge getEdge(Graph g, Node from, Node to){
    NodeImpl* nFrom = getNodeInternal(g, from);
    for(Posic p = getFirstLst(nFrom->edgesOut); p!=NIL; p=getNextLst(nFrom->edgesOut,p)){
        EdgeImpl* e = getLst(nFrom->edgesOut,p);
        if(e->to==to) return e;
    }
    return NULL;
}

Node getFromNode(Graph g, Edge e){
    return getEdgeInternal(e)->from;
}

Node getToNode(Graph g, Edge e){
    return getEdgeInternal(e)->to;
}

Info getEdgeInfo(Graph g, Edge e){
    return getEdgeInternal(e)->info;
}

void setEdgeInfo(Graph g, Edge e, Info info){
    getEdgeInternal(e)->info = info;
}

void removeEdge(Graph g, Edge e, freeFunc freeEdgeFunc){
    EdgeImpl* edge = getEdgeInternal(e);
    GraphImpl* gr = (GraphImpl*) g;
    if(freeEdgeFunc) freeEdgeFunc(edge->info);
    NodeImpl* from = getNodeInternal(g, edge->from);
    NodeImpl* to   = getNodeInternal(g, edge->to);
    for(Posic p=getFirstLst(from->edgesOut);p!=NIL;p=getNextLst(from->edgesOut,p)){
        if(getLst(from->edgesOut,p)==edge){
            removeLst(from->edgesOut,p,NULL);
            break;
        }
    }
    for(Posic p=getFirstLst(to->edgesIn);p!=NIL;p=getNextLst(to->edgesIn,p)){
        if(getLst(to->edgesIn,p)==edge){
            removeLst(to->edgesIn,p,NULL);
            break;
        }
    }
    for(Posic p=getFirstLst(gr->allEdges);p!=NIL;p=getNextLst(gr->allEdges,p)){
        if(getLst(gr->allEdges,p)==edge){
            removeLst(gr->allEdges,p,NULL);
            break;
        }
    }
    free(edge);
}

bool isAdjacent(Graph g, Node from, Node to){
    return getEdge(g,from,to)!=NULL;
}

void adjacentNodes(Graph g, Node node, Lista nosAdjacentes){
    NodeImpl* n = getNodeInternal(g,node);
    for(Posic p=getFirstLst(n->edgesOut);p!=NIL;p=getNextLst(n->edgesOut,p)){
        EdgeImpl* e = getLst(n->edgesOut,p);
        insertLst(nosAdjacentes,(void*)(size_t)e->to);
    }
}

void adjacentEdges(Graph g, Node node, Lista arestasAdjacentes){
    NodeImpl* n = getNodeInternal(g,node);
    for(Posic p=getFirstLst(n->edgesOut);p!=NIL;p=getNextLst(n->edgesOut,p)){
        insertLst(arestasAdjacentes,getLst(n->edgesOut,p));
    }
}

void adjacentEdgesReverse(Graph g, Node node, Lista arestasAdjacentes){
    NodeImpl* n = getNodeInternal(g,node);
    for(Posic p=getFirstLst(n->edgesIn);p!=NIL;p=getNextLst(n->edgesIn,p)){
        insertLst(arestasAdjacentes,getLst(n->edgesIn,p));
    }
}

void getNodeNames(Graph g, Lista nomesNodes){
    GraphImpl* gr = (GraphImpl*) g;
    for(int i=0;i<gr->numNodes;i++){
        insertLst(nomesNodes, gr->nodes[i]->name);
    }
}

void getEdges(Graph g, Lista arestas){
    GraphImpl* gr = (GraphImpl*) g;
    for(Posic p=getFirstLst(gr->allEdges); p!=NIL; p=getNextLst(gr->allEdges,p)){
        insertLst(arestas,getLst(gr->allEdges,p));
    }
}

void getAllVerticesInfo(Graph g, Lista allInfo){
    GraphImpl* gr = (GraphImpl*) g;
    for(int i=0;i<gr->numNodes;i++){
        insertLst(allInfo, gr->nodes[i]->info);
    }
}

int getTotalEdges(Graph g){
    return sizeLst(((GraphImpl*)g)->allEdges);
}

/* ============================================================
   DFS completo
   ============================================================ */

static bool dfs_visit(Graph g, Node u, bool* visited, int* disc, int* finish, int* time,
                      procEdge treeEdge, procEdge forwardEdge, procEdge returnEdge, procEdge crossEdge, void* extra) {
    visited[u] = true;
    disc[u] = ++(*time);
    NodeImpl* nodeU = getNodeInternal(g,u);

    for(Posic p=getFirstLst(nodeU->edgesOut); p!=NIL; p=getNextLst(nodeU->edgesOut,p)){
        Edge e = getLst(nodeU->edgesOut,p);
        Node v = getToNode(g,e);
        if(!visited[v]){
            if(treeEdge && !treeEdge(g,e,disc[u],finish[u],extra)) return false;
            if(!dfs_visit(g,v,visited,disc,finish,time,treeEdge,forwardEdge,returnEdge,crossEdge,extra)) return false;
        } else {
            if(forwardEdge && disc[u] < disc[v]) forwardEdge(g,e,disc[u],finish[u],extra);
            else if(returnEdge && v==u) returnEdge(g,e,disc[u],finish[u],extra);
            else if(crossEdge) crossEdge(g,e,disc[u],finish[u],extra);
        }
    }

    finish[u] = ++(*time);
    return true;
}

bool dfs(Graph g, Node node, procEdge treeEdge, procEdge forwardEdge, procEdge returnEdge, procEdge crossEdge, dfsRestarted newTree, void *extra){
    GraphImpl* gr = (GraphImpl*) g;
    bool visited[gr->numNodes];
    int disc[gr->numNodes], finish[gr->numNodes], time=0;
    for(int i=0;i<gr->numNodes;i++) visited[i]=false;

    if(newTree) newTree(g,extra);

    return dfs_visit(g,node,visited,disc,finish,&time,treeEdge,forwardEdge,returnEdge,crossEdge,extra);
}

/* ============================================================
   BFS completo
   ============================================================ */

bool bfs(Graph g, Node node, procEdge discoverNode, void *extra){
    GraphImpl* gr = (GraphImpl*) g;
    bool visited[gr->numNodes];
    for(int i=0;i<gr->numNodes;i++) visited[i] = false;

    Lista queue = createLst(-1);
    insertLst(queue,(void*)(size_t)node);
    visited[node] = true;

    while(sizeLst(queue)>0){
        Posic p = getFirstLst(queue);
        Node u = (Node)(size_t)getLst(queue,p);
        removeLst(queue,p,NULL);

        NodeImpl* nodeU = getNodeInternal(g,u);
        for(Posic q=getFirstLst(nodeU->edgesOut); q!=NIL; q=getNextLst(nodeU->edgesOut,q)){
            Edge e = getLst(nodeU->edgesOut,q);
            Node v = getToNode(g,e);
            if(!visited[v]){
                visited[v]=true;
                insertLst(queue,(void*)(size_t)v);
                if(discoverNode && !discoverNode(g,e,0,0,extra)){
                    freeLst(queue,NULL);
                    return false;
                }
            }
        }
    }
    freeLst(queue,NULL);
    return true;
}

/* ============================================================
   Dijkstra
   ============================================================ */

Caminho getShortestPath(Graph g, Node from, Node to, getNumberValue getDistanceFunc){
    GraphImpl* gr = (GraphImpl*) g;
    int n = gr->numNodes;
    double dist[n];
    int prev[n];
    bool visited[n];
    for(int i=0;i<n;i++){
        dist[i] = DBL_MAX;
        prev[i] = -1;
        visited[i] = false;
    }
    dist[from] = 0;

    for(int i=0;i<n;i++){
        double minDist = DBL_MAX;
        int u = -1;
        for(int j=0;j<n;j++){
            if(!visited[j] && dist[j]<minDist){
                minDist = dist[j];
                u = j;
            }
        }
        if(u==-1) break;
        visited[u] = true;
        NodeImpl* nodeU = gr->nodes[u];
        for(Posic p=getFirstLst(nodeU->edgesOut);p!=NIL;p=getNextLst(nodeU->edgesOut,p)){
            EdgeImpl* e = getLst(nodeU->edgesOut,p);
            Node v = e->to;
            double weight = getDistanceFunc ? getDistanceFunc(e->info) : e->weight;
            if(dist[u]+weight < dist[v]){
                dist[v] = dist[u]+weight;
                prev[v] = u;
            }
        }
    }

    CaminhoImpl* caminho = malloc(sizeof(CaminhoImpl));
    caminho->pathEdges = createLst(-1);
    caminho->distance = dist[to];
    // Reconstruir caminho
    int cur = to;
    while(prev[cur]!=-1){
        Edge e = getEdge(g, prev[cur], cur);
        insertLst(caminho->pathEdges, e);
        cur = prev[cur];
    }
    return caminho;
}

Lista getDijkstraList(Caminho caminho){
    CaminhoImpl* c = (CaminhoImpl*) caminho;
    Lista pathCopy = createLst(-1);
    for(Posic p=getFirstLst(c->pathEdges);p!=NIL;p=getNextLst(c->pathEdges,p)){
        insertLst(pathCopy, getLst(c->pathEdges,p));
    }
    return pathCopy;
}

double getDijkstraDistance(Caminho caminho){
    return ((CaminhoImpl*)caminho)->distance;
}
