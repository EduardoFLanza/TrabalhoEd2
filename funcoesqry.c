/**
 * Módulo: funcoesqry
 * Finalidade:
 *   Este módulo implementa um conjunto de operações utilizadas durante a leitura
 *   e interpretação dos comandos presentes em ficheiros QRY. Tais operações
 *   envolvem manipulação de entidades gráficas, remoção espacial, destaques,
 *   caminhos em grafos, operações sobre estruturas de dados espaciais
 *   (BoundingBox / Treap), bem como interações diretas com o módulo de imagem
 *   (Picture) e com o módulo de grafos (Digraph).
 *
 * Visão Geral:
 *   As funções aqui disponibilizadas têm como propósito executar ações de alto nível
 *   solicitadas por comandos QRY, tais como:
 *       - Remover elementos em áreas definidas.
 *       - Remover entidades por ID.
 *       - Criar destaques (highlight) visuais com círculos.
 *       - Bloquear e desbloquear arestas de um grafo.
 *       - Encontrar e desenhar caminhos mínimos.
 *       - Percorrer regiões espaciais aplicando funções callback.
 *
 *   O módulo é, portanto, um intermediário entre:
 *       (i)   Estruturas de armazenamento espacial (BBox/Treap),
 *       (ii)  Estruturas de armazenamento lógico (Picture),
 *       (iii) Estruturas de conectividade (Digraph),
 *       (iv)  Objetos geométricos (círculos, linhas e entidades).
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "funcoesqry.h"

#ifndef _POSIX_C_SOURCE
static char* my_strdup(const char* s){
    if(!s) return NULL;
    char* r = malloc(strlen(s)+1);
    if(!r) return NULL;
    strcpy(r,s);
    return r;
}
#define strdup my_strdup
#endif

#include "boundingbox.h"
#include "picture.h"
#include "entity.h"
#include "tratar_strings.h"
#include "lista.h"
#include "circulo.h"
#include "linha.h"
#include "digraph.h"
#include "listadj.h"

/* ========================================================================= */
/* IMPLEMENTAÇÃO DAS FUNÇÕES DO MÓDULO */
/* ========================================================================= */

static double dist(double x1,double y1,double x2,double y2){
    double dx=x1-x2, dy=y1-y2;
    return sqrt(dx*dx+dy*dy);
}

void fq_removeInRect(Picture p, BBox bb, double xmin, double ymin, double xmax, double ymax){
    Lista toRemove=criaLista();

    void collect(SNode n){
        SInfo info=st_getInfo(n);
        Entity e=(Entity)info;
        insertLst(toRemove, strdup(entity_getID(e)));
    }

    bb_rangeSearch(bb, xmin, ymin, xmax, ymax, collect);

    for(Posic it=getFirstLst(toRemove); it; it=getNextLst(it)){
        char* id = getLst(it);
        fq_removeByID(p, bb, id);
        free(id);
    }

    removeLista(toRemove,NULL);
}

int fq_removeByID(Picture p, BBox bb, const char* id){
    Entity e = picture_get(p,id);
    if(!e) return 0;

    double x,y;
    entity_getXY(e,&x,&y);

    picture_remove(p,id);
    bb_remove(bb,x,y,(void(*)(SInfo))entity_destroy);

    return 1;
}

void fq_highlightRadius(Picture p, BBox bb, double x, double y, double r, const char* stroke, const char* fill){
    void visit(SNode n){
        SInfo info=st_getInfo(n);
        Entity e=(Entity)info;

        double ex,ey;
        entity_getXY(e,&ex,&ey);

        if(dist(x,y,ex,ey)<=r){
            char tmpid[64];
            sprintf(tmpid,"hl_%p",(void*)e);

            Circulo c = circulo_criar(ex,ey, r*0.05, stroke, fill, tmpid);
            Entity he = entity_fromCirculo(c);
            picture_add(p,he);
        }
    }

    bb_rangeSearch(bb,x-r,y-r,x+r,y+r,visit);
}

int fq_blockEdge(Digraph g, const char* origem, const char* destino){
    return dg_disableEdge(g,origem,destino);
}

int fq_unblockEdge(Digraph g, const char* origem, const char* destino){
    return dg_enableEdge(g,origem,destino);
}

Lista fq_shortestPath(Digraph g, const char* origem, const char* destino){
    return dg_shortestPath(g,origem,destino);
}

void fq_drawPath(Picture p, BBox bb, Digraph g, Lista path, const char* idPrefix, const char* stroke){
    if(!path) return;

    int n = lengthLst(path);
    char** arr = malloc(n*sizeof(char*));
    int i = 0;

    for(Posic it=getFirstLst(path); it; it=getNextLst(it))
        arr[i++] = strdup((char*)getLst(it));

    for(int j=n-1; j>0; j--){
        double x1,y1,x2,y2;

        if(dg_getVertexXY(g,arr[j],&x1,&y1) &&
           dg_getVertexXY(g,arr[j-1],&x2,&y2))
        {
            char idtmp[128];
            sprintf(idtmp,"%s_path_%d",idPrefix,j);

            Linha l = linha_criar(x1,y1,x2,y2,stroke,2.0,idtmp);
            Entity e = entity_fromLinha(l);

            picture_add(p,e);
            bb_insert(bb,x1,y1,e);
        }
    }

    for(int k=0;k<n;k++) free(arr[k]);
    free(arr);
}

void fq_catac(Digraph g, BBox bb, double xmin, double ymin, double xmax, double ymax){
    int n = dg_numVertices(g);

    for(int i=0;i<n;i++){
        const char* name = dg_getVertexNameByIndex(g,i);

        double x,y;
        if(!dg_getVertexXY(g,name,&x,&y)) continue;

        if(x>=xmin && x<=xmax && y>=ymin && y<=ymax){
            Lista adj = dg_getAdjList(g,name);

            for(Posic it=getFirstLst(adj); it; it=getNextLst(it)){
                void* ar = getLst(it);
                const char* destName = aresta_getDestName(ar);
                if (!destName) continue;
            }
        }
    }
}

void fq_forEachInRect(BBox bb, double xmin, double ymin, double xmax, double ymax,
                      void (*visit)(SNode)){
    bb_rangeSearch(bb,xmin,ymin,xmax,ymax,visit);
}

