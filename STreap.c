/*
 * Implementação simplificada da STreap.
 * Observações:
 *  - A versão a seguir implementa uma treap básica organizada por x (e y como tie-breaker).
 *  - Prioridades são geradas aleatoriamente; não é uma treap perfeita, mas suficiente.
 *  - Funções essenciais implementadas: criação, inserção, busca, remoção, rangeSearch,
 *    inorder, percursos e destruição.
 *
 * Nota: para simplicidade e robustez a função de remoção por nó e por chave retornam a SInfo.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <float.h>
#include <time.h>

#include "STreap.h"
#include "lista.h"

/* Estruturas privadas */
typedef struct node {
    double x, y;
    SInfo info;
    unsigned priority;
    struct node *l, *r;
} Node;

typedef struct streap {
    Node *root;
    double eps;
} ST;

/* util */
static unsigned rnd_priority() {
    return ((unsigned)rand() << 16) ^ (unsigned)rand();
}

/* comparação considerando eps; retorna -1 se (x1,y1) < (x2,y2), 0 se igual, 1 se maior */
static int cmp(ST *t, double x1, double y1, double x2, double y2) {
    double eps = t->eps;
    if (x1 + eps < x2) return -1;
    if (x2 + eps < x1) return 1;
    /* x aproximadamente iguais */
    if (y1 + eps < y2) return -1;
    if (y2 + eps < y1) return 1;
    return 0;
}

/* rotações */
static Node* rotate_right(Node* y) {
    Node* x = y->l;
    y->l = x->r;
    x->r = y;
    return x;
}
static Node* rotate_left(Node* x) {
    Node* y = x->r;
    x->r = y->l;
    y->l = x;
    return y;
}

/* criação de nó */
static Node* new_node(double x, double y, SInfo info) {
    Node* n = malloc(sizeof(Node));
    if (!n) return NULL;
    n->x = x; n->y = y; n->info = info;
    n->priority = rnd_priority();
    n->l = n->r = NULL;
    return n;
}

/* rotação / inserção recursiva */
static Node* insert_rec(ST* s, Node* root, double x, double y, SInfo info, int *inserted) {
    if (!root) {
        *inserted = 1;
        return new_node(x,y,info);
    }
    int c = cmp(s, x,y, root->x, root->y);
    if (c == 0) { *inserted = 0; return root; } /* chave já existe */
    if (c < 0) {
        root->l = insert_rec(s, root->l, x,y, info, inserted);
        if (root->l && root->l->priority > root->priority)
            root = rotate_right(root);
    } else {
        root->r = insert_rec(s, root->r, x,y, info, inserted);
        if (root->r && root->r->priority > root->priority)
            root = rotate_left(root);
    }
    return root;
}

/* busca por chave */
static Node* search_rec(ST* s, Node* root, double x, double y) {
    if (!root) return NULL;
    int c = cmp(s, x,y, root->x, root->y);
    if (c == 0) return root;
    if (c < 0) return search_rec(s, root->l, x,y);
    return search_rec(s, root->r, x,y);
}

/* findMin / findMax auxiliar para remoção por fusão */
static Node* find_min(Node* r) { while (r && r->l) r = r->l; return r; }

/* remoção por chave */
static Node* remove_rec(ST* s, Node* root, double x, double y, SInfo *retInfo, int *removed) {
    if (!root) return NULL;
    int c = cmp(s, x,y, root->x, root->y);
    if (c < 0) {
        root->l = remove_rec(s, root->l, x,y, retInfo, removed);
    } else if (c > 0) {
        root->r = remove_rec(s, root->r, x,y, retInfo, removed);
    } else {
        /* found: remove this node */
        *retInfo = root->info;
        *removed = 1;
        /* if leaf */
        if (!root->l && !root->r) {
            free(root);
            return NULL;
        }
        /* rotate the child with higher priority up */
        if (!root->l) {
            root = rotate_left(root);
            root->l = remove_rec(s, root->l, x,y, retInfo, removed);
        } else if (!root->r) {
            root = rotate_right(root);
            root->r = remove_rec(s, root->r, x,y, retInfo, removed);
        } else {
            if (root->l->priority > root->r->priority) {
                root = rotate_right(root);
                root->r = remove_rec(s, root->r, x,y, retInfo, removed);
            } else {
                root = rotate_left(root);
                root->l = remove_rec(s, root->l, x,y, retInfo, removed);
            }
        }
    }
    return root;
}

/* remoção por nó (ponto) - wrapper que usa remove_rec */
static SInfo delete_node_ptr(ST* s, Node* target) {
    if (!target) return NULL;
    /* remove by key using target's coords */
    SInfo ret = NULL;
    int removed = 0;
    s->root = remove_rec(s, s->root, target->x, target->y, &ret, &removed);
    return ret;
}

/* inorder X (rec) */
static void inorder_rec(Node* r, void (*visit)(SNode)) {
    if (!r) return;
    inorder_rec(r->l, visit);
    visit(r);
    inorder_rec(r->r, visit);
}

/* rangeSearch rec (naive) */
static void range_rec(Node* r, double xmin, double ymin, double xmax, double ymax, void (*visit)(SNode)) {
    if (!r) return;
    if (r->x >= xmin && r->x <= xmax && r->y >= ymin && r->y <= ymax)
        visit(r);
    /* prune by x */
    if (r->l && r->x >= xmin) range_rec(r->l, xmin, ymin, xmax, ymax, visit);
    if (r->r && r->x <= xmax) range_rec(r->r, xmin, ymin, xmax, ymax, visit);
}

/* percorre largura */
static void largura(ST* s, FvisitaNo f, void *aux) {
    if (!s || !s->root || !f) return;
    /* use simple queue of nodes */
    Lista q = criaLista();
    insertLst(q, s->root);
    while (lengthLst(q) > 0) {
        Posic p = getFirstLst(q);
        Node* n = (Node*)getLst(p);
        removePosicLst(q, p);
        /* compute mbb for this node as trivial point */
        f(n->info, n->x, n->y, n->x, n->y, n->x, n->y, aux);
        if (n->l) insertLst(q, n->l);
        if (n->r) insertLst(q, n->r);
    }
    removeLista(q, NULL);
}

/* percurso profundidade (pré-ordem) */
static void profundidade_rec(Node* r, FvisitaNo f, void* aux) {
    if (!r) return;
    f(r->info, r->x, r->y, r->x, r->y, r->x, r->y, aux);
    profundidade_rec(r->l, f, aux);
    profundidade_rec(r->r, f, aux);
}

/* getNodeRegiao -> preenche Lista (resultado) com SNode */
static void getNodeRegion_rec(Node* r, double x, double y, double w, double h, Lista resultado, ST* s) {
    if (!r) return;
    double xmin = x, ymin = y, xmax = x + w, ymax = y + h;
    if (r->x >= xmin && r->x <= xmax && r->y >= ymin && r->y <= ymax)
        insertLst(resultado, r);
    getNodeRegion_rec(r->l, x, y, w, h, resultado, s);
    getNodeRegion_rec(r->r, x, y, w, h, resultado, s);
}

/* get MBB (naive traversal) */
static void upd_mbb(Node* r, double *xmin, double* ymin, double* xmax, double* ymax) {
    if (!r) return;
    if (r->x < *xmin) *xmin = r->x;
    if (r->y < *ymin) *ymin = r->y;
    if (r->x > *xmax) *xmax = r->x;
    if (r->y > *ymax) *ymax = r->y;
    upd_mbb(r->l, xmin, ymin, xmax, ymax);
    upd_mbb(r->r, xmin, ymin, xmax, ymax);
}

/* API functions */

STreap st_create(double epsilon) {
    srand((unsigned)time(NULL));
    ST* s = calloc(1, sizeof(ST));
    if (!s) return NULL;
    s->root = NULL;
    s->eps = epsilon > 0 ? epsilon : 1e-9;
    return s;
}

SNode st_insert(STreap T, double x, double y, SInfo info) {
    if (!T) return NULL;
    ST* s = (ST*)T;
    int inserted = 0;
    s->root = insert_rec(s, s->root, x, y, info, &inserted);
    if (!inserted) return NULL;
    /* return the inserted node (search for key) */
    return search_rec(s, s->root, x, y);
}

SInfo removeSTrp(STreap T, double xa, double ya) {
    if (!T) return NULL;
    ST* s = (ST*)T;
    SInfo ret = NULL;
    int removed = 0;
    s->root = remove_rec(s, s->root, xa, ya, &ret, &removed);
    return ret;
}

SInfo deleteNodeSTrp(STreap T, SNode n) {
    if (!T || !n) return NULL;
    ST* s = (ST*)T;
    return delete_node_ptr(s, (Node*)n);
}

SNode st_search(STreap T, double x, double y) {
    if (!T) return NULL;
    ST* s = (ST*)T;
    return search_rec(s, s->root, x, y);
}

SInfo st_getInfo(SNode n) {
    if (!n) return NULL;
    return ((Node*)n)->info;
}

void st_getKey(SNode n,double *x,double *y) {
    if (!n) return;
    *x = ((Node*)n)->x;
    *y = ((Node*)n)->y;
}

void getNodeRegiaoSTrp(STreap T, double x, double y, double w, double h, Lista resultado) {
    if (!T || !resultado) return;
    ST* s = (ST*)T;
    getNodeRegion_rec(s->root, x, y, w, h, resultado, s);
}

void updateInfoSTrp(STreap T, SNode n, SInfo i) {
    if (!T || !n) return;
    ((Node*)n)->info = i;
}

void st_inorderX(STreap T, void (*visit)(SNode)) {
    if (!T || !visit) return;
    ST* s = (ST*)T;
    inorder_rec(s->root, visit);
}

void st_rangeSearch(STreap T,double xmin,double ymin,double xmax,double ymax, void (*visit)(SNode)) {
    if (!T || !visit) return;
    ST* s = (ST*)T;
    range_rec(s->root, xmin, ymin, xmax, ymax, visit);
}

int st_getMBB(STreap T,double *xmin,double *ymin,double *xmax,double *ymax) {
    if (!T) return 0;
    ST* s = (ST*)T;
    if (!s->root) return 0;
    *xmin = *ymin =  1e300;
    *xmax = *ymax = -1e300;
    upd_mbb(s->root, xmin, ymin, xmax, ymax);
    return 1;
}

void percursoLargura(STreap T, FvisitaNo fVisita, void *aux) {
    if (!T || !fVisita) return;
    largura((ST*)T, fVisita, aux);
}

void percursoProfundidade(STreap T, FvisitaNo fVisita, void *aux) {
    if (!T || !fVisita) return;
    profundidade_rec(((ST*)T)->root, fVisita, aux);
}

void percursoSimetrico(STreap T, FvisitaNo fVisita, void *aux) {
    /* simétrico: usamos inorder e adaptamos para FvisitaNo */
    if (!T || !fVisita) return;
    ST* s = (ST*)T;
    /* adaptador simples */
    void adapter(SNode n) {
        Node* nd = (Node*)n;
        fVisita(nd->info, nd->x, nd->y, nd->x, nd->y, nd->x, nd->y, aux);
    }
    st_inorderX(T, adapter);
}

void st_inorderX_rec(Node* r, void (*visit)(SNode)); /* forward for compatibility */
void st_inorderX_rec(Node* r, void (*visit)(SNode)) { inorder_rec(r, visit); }

void percursoLargura(STreap t, FvisitaNo fVisita, void *aux); /* declared above */

void printSTrp(STreap T, const char *nomeArq) {
    /* optional: print dot; implement naive traversal */
    if (!T || !nomeArq) return;
    FILE *f = fopen(nomeArq, "w");
    if (!f) return;
    fprintf(f, "digraph STreap {\n");
    ST* s = (ST*)T;
    /* simple stack traversal */
    Lista st = criaLista();
    if (s->root) insertLst(st, s->root);
    while (lengthLst(st) > 0) {
        Posic p = getFirstLst(st);
        Node* n = (Node*)getLst(p);
        removePosicLst(st, p);
        fprintf(f, "n%p [label=\"(%.2f,%.2f)\"];\n", (void*)n, n->x, n->y);
        if (n->l) {
            fprintf(f, "n%p -> n%p;\n", (void*)n, (void*)n->l);
            insertLst(st, n->l);
        }
        if (n->r) {
            fprintf(f, "n%p -> n%p;\n", (void*)n, (void*)n->r);
            insertLst(st, n->r);
        }
    }
    removeLista(st, NULL);
    fprintf(f, "}\n");
    fclose(f);
}

void st_destroy(STreap T, void (*freeInfo)(SInfo)) {
    if (!T) return;
    ST* s = (ST*)T;
    /* post-order free */
    Lista stack = criaLista();
    if (s->root) insertLst(stack, s->root);
    while (lengthLst(stack) > 0) {
        Posic p = getFirstLst(stack);
        Node* n = (Node*)getLst(p);
        removePosicLst(stack, p);
        if (n->l) insertLst(stack, n->l);
        if (n->r) insertLst(stack, n->r);
        if (freeInfo && n->info) freeInfo(n->info);
        free(n);
    }
    removeLista(stack, NULL);
    free(s);
}
