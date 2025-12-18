/**
 * Módulo: STreap
 * Finalidade:
 *     Implementa uma STreap, que associa a cada ponto-âncora (x,y)
 *     uma informação arbitrária. A chave de busca é a coordenada da âncora.
 *
 *     A comparação entre duas chaves segue a regra:
 *
 *         - (x1 < x2)
 *         - OU (x1 == x2 E y1 < y2)
 *
 *     A árvore mantém um parâmetro de precisão epsilon, usado para comparar
 *     valores em ponto flutuante.
 *
 *     Os nós retornados por operações de busca são válidos apenas
 *     enquanto não ocorrer nenhuma remoção posterior.
 */

/**
 *   Streap para indexação de elementos por
 *   coordenada (x,y). Fornece inserção, remoção, busca por região, percurso
 *   e obtenção da menor bounding box que engloba todos os elementos.
 *
 *   A chave de busca é a coordenada (x,y). Algumas operações retornam nós
 *   internos (SNode) que devem ser considerados inválidos após remoções.
 *
 */

#ifndef STREAP_H
#define STREAP_H

#include "lista.h"

typedef void* STreap;
typedef void* SInfo;
typedef void* SNode;

/* * Assinatura para função de visita a um nó durante percursos.
 * Recebe a informação, a coordenada do nó (x,y) e os limites da 
 * subárvore enraizada neste nó (mbbX1, mbbY1, mbbX2, mbbY2).
 */
typedef void (*FvisitaNo)(SInfo info, double x, double y,
                          double mbbX1, double mbbY1,
                          double mbbX2, double mbbY2,
                          void* aux);

/* Cria uma STreap com precisão epsilon; retorna NULL em falha */
STreap st_create(double epsilon);

/* Insere info na treap com ancora (x,y). Retorna SNode (não-NULL) ou NULL se chave já existe */
SNode st_insert(STreap t, double x, double y, SInfo info);

/* Remove o nó com ancora (x,y) e retorna a SInfo associada; NULL se não encontrado */
SInfo removeSTrp(STreap t, double x, double y);

/* Remove um nó por ponteiro SNode; retorna a SInfo associada */
SInfo deleteNodeSTrp(STreap t, SNode n);

/* Busca nó com ancora (x,y); NULL se não encontrado */
SNode st_search(STreap t, double x, double y);

/* Retorna a SInfo associada ao nó */
SInfo st_getInfo(SNode n);

/* Obtém a chave (x,y) do nó */
void st_getKey(SNode n, double *x, double *y);

/* Obtém o Bounding Box (MBB) da subárvore enraizada no nó 'n' */
void st_getNodeBB(SNode n, double *x1, double *y1, double *x2, double *y2);

/* Obtém a raiz da STreap */
SNode st_getRoot(STreap s);

/* Insere em `resultado` os nós cuja ancora esteja dentro do retângulo (x,y,w,h) */
void getNodeRegiaoSTrp(STreap t, double x, double y, double w, double h, Lista resultado);

/* Atualiza a informação associada a um nó (sem alterar a ancora) */
void updateInfoSTrp(STreap t, SNode n, SInfo i);

/* Percursos: largura, simétrico (inorder) e profundidade (pré-ordem) */
void percursoLargura(STreap t, FvisitaNo fVisita, void *aux);
void percursoSimetrico(STreap t, FvisitaNo fVisita, void *aux);
void percursoProfundidade(STreap t, FvisitaNo fVisita, void *aux);

/* Busca por região (range search) aplicando visit a cada nó encontrado. 
   Otimizado pelo uso dos MBBs internos para poda de ramos. */
void st_rangeSearch(STreap t, double xmin, double ymin, double xmax, double ymax, void (*visit)(SNode));

/* Realiza percurso inorder por X e aplica visit */
void st_inorderX(STreap t, void (*visit)(SNode));

/* Retorna 1 se existirem elementos e preenche xmin,ymin,xmax,ymax com os limites globais; caso contrário 0 */
int st_getMBB(STreap t, double *xmin, double *ymin, double *xmax, double *ymax);

/* Destrói a STreap; freeInfo é callback opcional para liberar cada SInfo */
void st_destroy(STreap t, void (*freeInfo)(SInfo));

/* Debug: imprime em formato dot para visualização com Graphviz */
void printSTrp(STreap t, const char *nomeArq);

#endif /* STREAP_H */
