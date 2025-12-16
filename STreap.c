#include "streap.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

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

static int rand_init = 0;

static unsigned rnd_priority() {
    if (!rand_init) {
        srand((unsigned)time(NULL));
        rand_init = 1;
    }
    return ((unsigned)rand() << 16) ^ (unsigned)rand();
}

static int cmp(ST *t, double x1, double y1, double x2, double y2) {
    double e = t->eps;
    if (x1 + e < x2) return -1;
    if (x2 + e < x1) return 1;
    if (y1 + e < y2) return -1;
    if (y2 + e < y1) return 1;
    return 0;
}

/* rotações */

static Node* rotR(Node *y) {
    Node *x = y->l;
    y->l = x->r;
    x->r = y;
    return x;
}

static Node* rotL(Node *x) {
    Node *y = x->r;
    x->r = y->l;
    y->l = x;
    return y;
}

static Node* newNode(double x, double y, SInfo info) {
    Node *n = malloc(sizeof(Node));
    if (!n) return NULL;
    n->x = x;
    n->y = y;
    n->info = info;
    n->priority = rnd_priority();
    n->l = n->r = NULL;
    return n;
}

/* inserção */

static Node* insertRec(ST *s, Node *r, double x, double y, SInfo info, int *ok) {
    if (!r) {
        *ok = 1;
        return newNode(x, y, info);
    }

    int c = cmp(s, x, y, r->x, r->y);

    if (c == 0) {
        *ok = 0;
        return r;
    }

    if (c < 0) {
        r->l = insertRec(s, r->l, x, y, info, ok);
        if (r->l && r->l->priority > r->priority)
            r = rotR(r);
    } else {
        r->r = insertRec(s, r->r, x, y, info, ok);
        if (r->r && r->r->priority > r->priority)
            r = rotL(r);
    }
    return r;
}

/* busca */

static Node* searchRec(ST *s, Node *r, double x, double y) {
    if (!r) return NULL;
    int c = cmp(s, x, y, r->x, r->y);
    if (c == 0) return r;
    if (c < 0) return searchRec(s, r->l, x, y);
    return searchRec(s, r->r, x, y);
}

/* remoção */

static Node* removeRec(ST *s, Node *r, double x, double y, SInfo *ret, int *rem) {
    if (!r) return NULL;

    int c = cmp(s, x, y, r->x, r->y);

    if (c < 0)
        r->l = removeRec(s, r->l, x, y, ret, rem);
    else if (c > 0)
        r->r = removeRec(s, r->r, x, y, ret, rem);
    else {
        *ret = r->info;
        *rem = 1;

        if (!r->l) {
            Node *tmp = r->r;
            free(r);
            return tmp;
        }
        if (!r->r) {
            Node *tmp = r->l;
            free(r);
            return tmp;
        }

        if (r->l->priority > r->r->priority) {
            r = rotR(r);
            r->r = removeRec(s, r->r, x, y, ret, rem);
        } else {
            r = rotL(r);
            r->l = removeRec(s, r->l, x, y, ret, rem);
        }
    }
    return r;
}

/* API */

STreap st_create(double eps) {
    ST *s = calloc(1, sizeof(ST));
    if (!s) return NULL;
    s->eps = (eps > 0) ? eps : 1e-9;
    return s;
}

SNode st_insert(STreap t, double x, double y, SInfo info) {
    ST *s = t;
    int ok = 0;
    s->root = insertRec(s, s->root, x, y, info, &ok);
    return ok ? searchRec(s, s->root, x, y) : NULL;
}

SNode st_search(STreap t, double x, double y) {
    ST *s = t;
    return searchRec(s, s->root, x, y);
}

SInfo removeSTrp(STreap t, double x, double y) {
    ST *s = t;
    SInfo ret = NULL;
    int rem = 0;
    s->root = removeRec(s, s->root, x, y, &ret, &rem);
    return ret;
}

SInfo deleteNodeSTrp(STreap t, SNode n) {
    Node *nd = n;
    if (!nd) return NULL;
    return removeSTrp(t, nd->x, nd->y);
}

SInfo st_getInfo(SNode n) {
    return ((Node*)n)->info;
}

void st_getKey(SNode n, double *x, double *y) {
    Node *nd = n;
    if (x) *x = nd->x;
    if (y) *y = nd->y;
}

/* percursos */

static void inorder(Node *n, void (*v)(SNode), ST *s) {
    if (!n) return;
    inorder(n->l, v, s);
    v(n);
    inorder(n->r, v, s);
}

void st_inorderX(STreap t, void (*visit)(SNode)) {
    ST *s = t;
    inorder(s->root, visit, s);
}

static void visitRec(Node *n, FvisitaNo f, void *aux) {
    if (!n) return;
    f(n->info, n->x, n->y, n->x, n->y, n->x, n->y, aux);
    visitRec(n->l, f, aux);
    visitRec(n->r, f, aux);
}

void percursoProfundidade(STreap t, FvisitaNo f, void *aux) {
    visitRec(((ST*)t)->root, f, aux);
}

void percursoSimetrico(STreap t, FvisitaNo f, void *aux) {
    st_inorderX(t, (void (*)(SNode))f);
}

void percursoLargura(STreap t, FvisitaNo f, void *aux) {
    Lista q = criaLista();
    Node *r = ((ST*)t)->root;
    if (!r) return;
    inserirInicio(q, r);
    while (!isListaVazia(q)) {
        Node *n = removerFim(q);
        f(n->info, n->x, n->y, n->x, n->y, n->x, n->y, aux);
        if (n->l) inserirInicio(q, n->l);
        if (n->r) inserirInicio(q, n->r);
    }
}

/* região */

static void rangeRec(Node *n, double x1, double y1, double x2, double y2, void (*v)(SNode)) {
    if (!n) return;
    if (n->x >= x1 && n->x <= x2 && n->y >= y1 && n->y <= y2)
        v(n);
    rangeRec(n->l, x1, y1, x2, y2, v);
    rangeRec(n->r, x1, y1, x2, y2, v);
}

void st_rangeSearch(STreap t, double xmin, double ymin, double xmax, double ymax, void (*visit)(SNode)) {
    rangeRec(((ST*)t)->root, xmin, ymin, xmax, ymax, visit);
}

void getNodeRegiaoSTrp(STreap t, double x, double y, double w, double h, Lista r) {
    double x2 = x + w;
    double y2 = y + h;
    void visit(SNode n) { inserirInicio(r, n); }
    st_rangeSearch(t, x, y, x2, y2, visit);
}

/* MBB */

int st_getMBB(STreap t, double *xmin, double *ymin, double *xmax, double *ymax) {
    ST *s = t;
    if (!s->root) return 0;

    *xmin = *xmax = s->root->x;
    *ymin = *ymax = s->root->y;

    void v(SNode n) {
        Node *nd = n;
        if (nd->x < *xmin) *xmin = nd->x;
        if (nd->x > *xmax) *xmax = nd->x;
        if (nd->y < *ymin) *ymin = nd->y;
        if (nd->y > *ymax) *ymax = nd->y;
    }

    st_inorderX(t, v);
    return 1;
}

/* destruição */

void st_destroy(STreap t, void (*freeInfo)(SInfo)) {
    Lista st = criaLista();
    Node *r = ((ST*)t)->root;
    if (r) inserirInicio(st, r);

    while (!isListaVazia(st)) {
        Node *n = removerFim(st);
        if (n->l) inserirInicio(st, n->l);
        if (n->r) inserirInicio(st, n->r);
        if (freeInfo) freeInfo(n->info);
        free(n);
    }
    removeLista(st, NULL);
    free(t);
}
