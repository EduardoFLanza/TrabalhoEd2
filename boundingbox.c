/**
 * Módulo: boundingbox
 * Finalidade:
 *     Este módulo implementa a abstração “Bounding Box” (ou caixa delimitadora),
 *     utilizada como estrutura auxiliar para armazenar, organizar e consultar
 *     elementos espaciais em um plano bidimensional. A estrutura é otimizada
 *     para consultas por região, permitindo operações eficientes de busca
 *     utilizando limites mínimos e máximos em x e y.
 *
 * Descrição Geral:
 *     Uma instância da abstração “BoundingBox” representa um conjunto de pontos
 *     ou entidades geométricas indexadas por coordenadas (x, y). Internamente,
 *     é utilizada uma STreap — estrutura de dados balanceada por prioridades
 *     (treap) — que organiza os elementos de forma hierárquica, permitindo:
 *
 *         - Inserção de elementos associados a coordenadas (x, y)
 *         - Remoção controlada com desalocação opcional
 *         - Consultas por região (range search)
 *         - Percursos ordenados (ex.: inorder)
 *         - Obtenção da menor caixa delimitadora (MBB - Minimum Bounding Box)
 *
 *
 * Notas Importantes:
 *     - A STreap utilizada é responsável pela organização espacial interna.
 *     - Cabe ao utilizador destruir a instância criada para evitar vazamentos.
 *     - Cada função pública possui documentação própria no arquivo .h.
 */


#include <stdlib.h>
#include "boundingbox.h"
#include "STreap.h"
typedef struct bbox { STreap t; } BB;
BBox bb_create(){ BB* b=malloc(sizeof(BB)); b->t=st_create(); return b; }
void bb_destroy(BBox B, void (*freeFunc)(SInfo)){ BB* b=B; st_destroy(b->t, freeFunc); free(b); }
void bb_insert(BBox B, double x, double y, SInfo info){ BB* b=B; st_insert(b->t,x,y,info); }
void bb_remove(BBox B, double x, double y, void (*freeFunc)(SInfo)){ BB* b=B; st_remove(b->t,x,y,freeFunc); }
void bb_rangeSearch(BBox B,double xmin,double ymin,double xmax,double ymax,void(*visit)(SNode)){ BB* b=B; st_rangeSearch(b->t,xmin,ymin,xmax,ymax,visit); }
void bb_forEach(BBox B, void(*visit)(SNode)){ BB* b=B; if(!visit) return; st_inorderX(b->t, visit); }
int bb_getMBB(BBox B,double *xmin,double *ymin,double *xmax,double *ymax){ BB* b=B; return st_getMBB(b->t,xmin,ymin,xmax,ymax); }
STreap bb_getTreap(BBox B){ return ((BB*)B)->t; }
