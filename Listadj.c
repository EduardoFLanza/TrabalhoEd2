/**
 * Módulo: listadj
 * Finalidade:
 *     Este módulo implementa uma estrutura de dados baseada em lista de
 *     adjacência, utilizada para representar grafos direcionados ou não.
 *
 *     Cada vértice possui:
 *       - um nome (chave identificadora),
 *       - informações adicionais associadas (InfoVert),
 *       - coordenadas (x,y),
 *       - uma lista de arestas que representam suas adjacências.
 *
 *     Cada aresta armazena:
 *       - o nome do vértice de destino,
 *       - o índice do destino na lista de vértices,
 *       - o peso (double),
 *       - um ponteiro para informação associada (InfoArest).
 *
 *     IMPORTANTE:
 *         • Este módulo cria e manipula instâncias de grafos estruturados
 *           em listas de adjacência.
 *         • O tipo ListAdj é opaco no arquivo .h (information hiding).
 *         • Este .c contém as estruturas concretas de implementação.
 *
 * Notas:
 *     - Expansão dinâmica é utilizada para os vetores de vértices.
 *     - Lista de adjacência utiliza o módulo "lista" para armazenar arestas.
 */

#include <stdlib.h>
#include <string.h>
#include "listadj.h"

#ifndef _POSIX_C_SOURCE
/* Função auxiliar interna para duplicação de strings
 * Usada caso a função strdup não esteja disponível no sistema.
 */
static char* my_strdup(const char* s){
    if(!s) return NULL;
    char* r = malloc(strlen(s) + 1);
    if(!r) return NULL;
    strcpy(r,s);
    return r;
}
#define strdup my_strdup
#endif

#include "lista.h"

/* -------------------------------------------------------------------------
 * Estruturas internas do módulo (NUNCA aparecem no .h)
 * ------------------------------------------------------------------------- */

/* Estrutura que representa uma aresta de um vértice. */
typedef struct aresta{
    char* destinoNome;   /* Nome do vértice de destino */
    int destIndex;       /* Índice do vértice de destino */
    double peso;         /* Peso da aresta */
    InfoArest info;      /* Dado adicional associado à aresta */
} Aresta;

/* Estrutura que representa um vértice do grafo. */
typedef struct vert{
    char* nome;          /* Identificador do vértice */
    InfoVert info;       /* Dados adicionais do vértice */
    Lista adj;           /* Lista de adjacência (arestas) */
    double x,y;          /* Coordenadas opcionais */
} Vert;

/* Estrutura raiz da lista de adjacência. */
typedef struct listadj{
    Vert* v;             /* Vetor dinâmico de vértices */
    int size;            /* Número atual de vértices */
    int cap;             /* Capacidade do vetor de vértices */
} LA;

/* -------------------------------------------------------------------------
 * Função: criaListAdj
 * Finalidade:
 *     Cria e inicializa uma nova lista de adjacência vazia.
 *
 * Retorno:
 *     Ponteiro para instância opaca representando o grafo.
 * ------------------------------------------------------------------------- */
ListAdj criaListAdj(){
    LA* la = malloc(sizeof(LA));
    la->size = 0;
    la->cap = 8;
    la->v = calloc(la->cap, sizeof(Vert));
    return la;
}

/* -------------------------------------------------------------------------
 * Função interna: procura
 * Finalidade:
 *     Busca a posição de um vértice pelo nome.
 *
 * Retorno:
 *     Índice do vértice ou -1 caso não exista.
 * ------------------------------------------------------------------------- */
static int procura(LA* la, const char* n){
    for(int i=0;i<la->size;i++)
        if(strcmp(la->v[i].nome,n)==0)
            return i;
    return -1;
}

/* -------------------------------------------------------------------------
 * Função: addVertice
 * Finalidade:
 *     Adiciona um novo vértice ao grafo, caso ainda não exista.
 *
 * Parâmetros:
 *     - L: instância da lista de adjacência.
 *     - nome: identificador do vértice.
 *     - info: informações associadas ao vértice.
 *
 * Retorno:
 *     1 se o vértice foi inserido com sucesso.
 *     0 se já existia.
 * ------------------------------------------------------------------------- */
int addVertice(ListAdj L, const char* nome, InfoVert info){
    LA* la=L;
    if(procura(la,nome)!=-1) return 0;

    if(la->size==la->cap){
        la->cap*=2;
        la->v=realloc(la->v, la->cap*sizeof(Vert));
    }

    Vert* vt=&la->v[la->size++];
    vt->nome=strdup(nome);
    vt->info=info;
    vt->adj=criaLista();
    vt->x=0;
    vt->y=0;

    return 1;
}

/* -------------------------------------------------------------------------
 * Função: addAresta
 * Finalidade:
 *     Cria uma nova aresta conectando dois vértices existentes.
 *
 * Parâmetros:
 *     - o: nome do vértice de origem
 *     - d: nome do vértice de destino
 *     - p: peso
 *     - info: informação associada à aresta
 *
 * Retorno:
 *     1 se inseriu com sucesso
 *     0 se algum vértice não existir
 * ------------------------------------------------------------------------- */
int addAresta(ListAdj L, const char* o, const char* d, double p, InfoArest info){
    LA* la=L;
    int oi=procura(la,o), di=procura(la,d);
    if(oi==-1||di==-1) return 0;

    Aresta* ar=malloc(sizeof(Aresta));
    ar->destIndex=di;
    ar->destinoNome=strdup(d);
    ar->peso=p;
    ar->info=info;

    insertLst(la->v[oi].adj, ar);
    return 1;
}

/* -------------------------------------------------------------------------
 * Funções diversas de acesso a dados do grafo
 * ------------------------------------------------------------------------- */

Lista getAdjacentes(ListAdj L, const char* v){
    LA* la=L;
    int i=procura(la,v);
    if(i==-1) return NULL;
    return la->v[i].adj;
}

InfoVert getInfoVert(ListAdj L, const char* nome){
    LA* la=L;
    int i=procura(la,nome);
    if(i==-1) return NULL;
    return la->v[i].info;
}

int numVertices(ListAdj L){ return ((LA*)L)->size; }

int getVertIndex(ListAdj L, const char* nome){
    return procura((LA*)L,nome);
}

const char* getVertNameByIndex(ListAdj L, int index){
    LA* la=L;
    if(index<0||index>=la->size) return NULL;
    return la->v[index].nome;
}

int getVertCoords(ListAdj L, const char* nome, double *x, double *y){
    LA* la=L;
    int i=procura(la,nome);
    if(i==-1) return 0;
    *x=la->v[i].x;
    *y=la->v[i].y;
    return 1;
}

Lista getAdjListByIndex(ListAdj L, int index){
    LA* la=L;
    if(index<0||index>=la->size) return NULL;
    return la->v[index].adj;
}

/* -------------------------------------------------------------------------
 * Getters das arestas (objetos retornados pelas listas)
 * ------------------------------------------------------------------------- */

const char* aresta_getDestName(void* a) {
    Aresta* ar = (Aresta*)a;
    return ar->destinoNome;
}

int aresta_getDestIndex(void* a) {
    Aresta* ar = (Aresta*)a;
    return ar->destIndex;
}

double aresta_getPeso(void* a) {
    Aresta* ar = (Aresta*)a;
    return ar->peso;
}

