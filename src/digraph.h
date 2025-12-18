#ifndef _DIGRAPH__
#define _DIGRAPH__

#include <stdbool.h>
#include "lista.h"

/*
Um Grafo direcionado G e' constituido por um conjunto de vertices V e
um conjunto de arestas E, denotado por G=(V,E).

Pode-se associar um dado a arestas e a vertices de G.
A cada vertice e' associado um nome.

Existem tambem operacoes que possibilitam percorrer o grafo,
invocando funcoes quando arestas do grafo sao "descobertas",
podendo, inclusive, interromper o percurso.

Este modulo faz parte de um trabalho de Estrutura de Dados do
curso de Ciencia da Computacao da Universidade Estadual de Londrina.
E' expressamente proibido o uso de ferramentas de IA para a sua implementacao.
A documentacao deste modulo deve ser melhorada.
*/

/* Tipos opacos */
typedef void* Graph;
typedef int   Node;
typedef void* Edge;
typedef void* Info;

/* Funcoes auxiliares genericas */
typedef void   (*freeFuncDG)(Info);
typedef double (*getNumberValue)(Info);

/*
    Invocado quando uma aresta e' descoberta/classificada durante o DFS/BFS.
    td = tempo de descoberta, tf = tempo de finalizacao (quando aplicavel).
    Retornar false interrompe o percurso.
*/
typedef bool (*procEdge)(Graph g, Edge e, int td, int tf, void *extra);

/*
    Invocado quando o DFS reinicia em uma nova arvore da floresta.
*/
typedef bool (*dfsRestarted)(Graph g, void *extra);

/* Criacao e informacoes basicas */
Graph createGraph(int nVert);
int   getMaxNodes(Graph g);
int   getTotalNodes(Graph g);
int   getTotalEdges(Graph g);

/* Operacoes sobre vertices */
Node  addNode(Graph g, char* nome, Info info);
Node  getNode(Graph g, char* nome);
Info  getNodeInfo(Graph g, Node node);
char* getNodeName(Graph g, Node node);
void  setNodeInfo(Graph g, Node node, Info info);

/* Operacoes sobre arestas */
Edge  addEdge(Graph g, Node from, Node to, Info info);
Edge  getEdge(Graph g, Node from, Node to);
Node  getFromNode(Graph g, Edge e);
Node  getToNode(Graph g, Edge e);
Info  getEdgeInfo(Graph g, Edge e);
void  setEdgeInfo(Graph g, Edge e, Info info);
void  removeEdge(Graph g, Edge e, freeFuncDG freeEdgeFunc);
bool  isAdjacent(Graph g, Node from, Node to);

/* Consultas de adjacencia */
void adjacentNodes(Graph g, Node node, Lista nosAdjacentes);
void adjacentEdges(Graph g, Node node, Lista arestasAdjacentes);
void adjacentEdgesReverse(Graph g, Node node, Lista arestasAdjacentes);

/* Listagens */
void getNodeNames(Graph g, Lista nomesNodes);
void getEdges(Graph g, Lista arestas);
void getAllVerticesInfo(Graph g, Lista allInfo);

/* Percursos */
bool dfs(Graph g, Node node,
         procEdge treeEdge,
         procEdge forwardEdge,
         procEdge returnEdge,
         procEdge crossEdge,
         dfsRestarted newTree,
         void *extra);

bool bfs(Graph g, Node node, procEdge discoverEdge, void *extra);

/* Dijkstra */
typedef void* Caminho;

Caminho getShortestPath(Graph g, Node from, Node to,
                        getNumberValue getDistanceFunc);
Lista  getDijkstraList(Caminho caminho);
double getDijkstraDistance(Caminho caminho);

/* Destruicao */
void killDG(Graph g, freeFuncDG freeVerticeFunc, freeFuncDG freeEdgeFunc);
void freeCaminho(Caminho caminho);

#endif

