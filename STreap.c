#include "streap.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/* ======================= ESTRUTURAS ======================= */

typedef struct snode {
    double x, y;
    SInfo info;
    unsigned priority;
    struct snode *left, *right;
} Node;

typedef struct {
    Node *root;
    double eps;
} ST;

/* ======================= UTIL ======================= */

static int seeded = 0;

static unsigned rndPriority() {
    if (!seeded) {
        srand((unsigned)time(NULL));
        seeded = 1;
    }
    return ((unsigned)rand() << 16) ^ rand();
}

static int cmp(ST *t, double x1, double y1, double x2, double y2) {
    double e = t->eps;
    if (x1 + e < x2) return -1;
    if (x2 + e < x1) return 1;
    if (y1 + e < y2) return -1;
    if (y2 + e < y1) return 1;
    return 0;
}

static Node *newNode(double x, double y, SInfo info) {
    Node *n = malloc(sizeof(Node));
    if (!n) return NULL;
    n->x = x;
    n->y = y;
    n->info = info;
    n->priority = rndPriority();
    n->left = n->right = NULL;
    return n;
}

/* ======================= ROTAÇÕES ======================= */

static Node *rotRight(Node *y) {
    Node *x = y->left;
    y->left = x->right;
    x->right = y;
    return x;
}

static Node *rotLeft(Node *x) {
    Node *y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

/* ======================= INSERÇÃO ======================= */

static Node *insertRec(ST *t, Node *r, double x, double y, SInfo info, int *ok) {
    if (!r) {
        *ok = 1;
        return newNode(x, y, info);
    }

    int c = cmp(t, x, y, r->x, r->y);
    if (c == 0) {
        *ok = 0;
        return r;
    }

    if (c < 0) {
        r->left = insertRec(t, r->left, x, y, info, ok);
        if (r->left && r->left->priority > r->priority)
            r = rotRight(r);
    } else {
        r->right = insertRec(t, r->right, x, y, info, ok);
        if (r->right && r->right->priority > r->priority)
            r = rotLeft(r);
    }
    return r;
}

/* ======================= BUSCA ======================= */

static Node *searchRec(ST *t, Node *r, double x, double y) {
    if (!r) return NULL;
    int c = cmp(t, x, y, r->x, r->y);
    if (c == 0) return r;
    if (c < 0) return searchRec(t, r->left, x, y);
    return searchRec(t, r->right, x, y);
}

/* ======================= REMOÇÃO ======================= */

static Node *removeRec(ST *t, Node *r, double x, double y, SInfo *ret, int *done) {
    if (!r) return NULL;

    int c = cmp(t, x, y, r->x, r->y);

    if (c < 0)
        r->left = removeRec(t, r->left, x, y, ret, done);
    else if (c > 0)
        r->right = removeRec(t, r->right, x, y, ret, done);
    else {
        *ret = r->info;
        *done = 1;

        if (!r->left) {
            Node *tmp = r->right;
            free(r);
            return tmp;
        }
        if (!r->right) {
            Node *tmp = r->left;
            free(r);
            return tmp;
        }

        if (r->left->priority > r->right->priority) {
            r = rotRight(r);
            r->right = removeRec(t, r->right, x, y, ret, done);
        } else {
            r = rotLeft(r);
            r->left = removeRec(t, r->left, x, y, ret, done);
        }
    }
    return r;
}

/* ======================= API ======================= */

STreap st_create(double epsilon) {
    ST *t = calloc(1, sizeof(ST));
    if (!t) return NULL;
    t->eps = (epsilon > 0) ? epsilon : 1e-9;
    return t;
}

SNode st_insert(STreap t, double x, double y, SInfo info) {
    ST *st = t;
    int ok = 0;
    st->root = insertRec(st, st->root, x, y, info, &ok);
    return ok ? searchRec(st, st->root, x, y) : NULL;
}

SNode st_search(STreap t, double x, double y) {
    return searchRec((ST *)t, ((ST *)t)->root, x, y);
}

SInfo removeSTrp(STreap t, double x, double y) {
    ST *st = t;
    SInfo ret = NULL;
    int done = 0;
    st->root = removeRec(st, st->root, x, y, &ret, &done);
    return ret;
}

SInfo deleteNodeSTrp(STreap t, SNode n) {
    Node *nd = n;
    if (!nd) return NULL;
    return removeSTrp(t, nd->x, nd->y);
}

SInfo st_getInfo(SNode n) {
    return ((Node *)n)->info;
}

void st_getKey(SNode n, double *x, double *y) {
    Node *nd = n;
    if (x) *x = nd->x;
    if (y) *y = nd->y;
}

void updateInfoSTrp(STreap t, SNode n, SInfo i) {
    (void)t;
    if (n) ((Node *)n)->info = i;
}

/* ======================= PERCURSOS ======================= */

static void preRec(Node *n, FvisitaNo f, void *aux) {
    if (!n) return;
    f(n->info, n->x, n->y, n->x, n->y, n->x, n->y, aux);
    preRec(n->left, f, aux);
    preRec(n->right, f, aux);
}

static void inRec(Node *n, FvisitaNo f, void *aux) {
    if (!n) return;
    inRec(n->left, f, aux);
    f(n->info, n->x, n->y, n->x, n->y, n->x, n->y, aux);
    inRec(n->right, f, aux);
}

void percursoProfundidade(STreap t, FvisitaNo f, void *aux) {
    preRec(((ST *)t)->root, f, aux);
}

void percursoSimetrico(STreap t, FvisitaNo f, void *aux) {
    inRec(((ST *)t)->root, f, aux);
}

void percursoLargura(STreap t, FvisitaNo f, void *aux) {
    Lista q = criaLista();
    Node *r = ((ST *)t)->root;
    if (!r) return;

    inserirInicio(q, r);
    while (!isListaVazia(q)) {
        Node *n = removerFim(q);
        f(n->info, n->x, n->y, n->x, n->y, n->x, n->y, aux);
        if (n->left) inserirInicio(q, n->left);
        if (n->right) inserirInicio(q, n->right);
    }
}

/* ======================= REGIÃO ======================= */

static void rangeRec(Node *n, double x1, double y1, double x2, double y2, void (*v)(SNode)) {
    if (!n) return;
    if (n->x >= x1 && n->x <= x2 && n->y >= y1 && n->y <= y2)
        v(n);
    rangeRec(n->left, x1, y1, x2, y2, v);
    rangeRec(n->right, x1, y1, x2, y2, v);
}

void st_rangeSearch(STreap t, double xmin, double ymin, double xmax, double ymax, void (*visit)(SNode)) {
    rangeRec(((ST *)t)->root, xmin, ymin, xmax, ymax, visit);
}

void getNodeRegiaoSTrp(STreap t, double x, double y, double w, double h, Lista r) {
    double x2 = x + w;
    double y2 = y + h;
    void visit(SNode n) { inserirInicio(r, n); }
    st_rangeSearch(t, x, y, x2, y2, visit);
}

/* ======================= MBB ======================= */

int st_getMBB(STreap t, double *xmin, double *ymin, double *xmax, double *ymax) {
    ST *st = t;
    if (!st->root) return 0;

    *xmin = *xmax = st->root->x;
    *ymin = *ymax = st->root->y;

    void visit(SNode n) {
        Node *nd = n;
        if (nd->x < *xmin) *xmin = nd->x;
        if (nd->x > *xmax) *xmax = nd->x;
        if (nd->y < *ymin) *ymin = nd->y;
        if (nd->y > *ymax) *ymax = nd->y;
    }

    st_inorderX(t, visit);
    return 1;
}

/* ======================= INORDER X ======================= */

static void inorderX(Node *n, void (*v)(SNode)) {
    if (!n) return;
    inorderX(n->left, v);
    v(n);
    inorderX(n->right, v);
}

void st_inorderX(STreap t, void (*visit)(SNode)) {
    inorderX(((ST *)t)->root, visit);
}

/* ======================= DESTROY ======================= */

void st_destroy(STreap t, void (*freeInfo)(SInfo)) {
    Lista s = criaLista();
    Node *r = ((ST *)t)->root;
    if (r) inserirInicio(s, r);

    while (!isListaVazia(s)) {
        Node *n = removerFim(s);
        if (n->left) inserirInicio(s, n->left);
        if (n->right) inserirInicio(s, n->right);
        if (freeInfo) freeInfo(n->info);
        free(n);
    }
    removeLista(s, NULL);
    free(t);
}

/* ======================= DEBUG DOT ======================= */

static void dotRec(Node *n, FILE *f) {
    if (!n) return;
    fprintf(f, "\"%p\" [label=\"(%.2lf,%.2lf)\"];\n", (void *)n, n->x, n->y);
    if (n->left) {
        fprintf(f, "\"%p\" -> \"%p\";\n", (void *)n, (void *)n->left);
        dotRec(n->left, f);
    }
    if (n->right) {
        fprintf(f, "\"%p\" -> \"%p\";\n", (void *)n, (void *)n->right);
        dotRec(n->right, f);
    }
}

void printSTrp(STreap t, const char *nomeArq) {
    FILE *f = fopen(nomeArq, "w");
    if (!f) return;

    fprintf(f, "digraph STreap {\n");
    dotRec(((ST *)t)->root, f);
    fprintf(f, "}\n");
    fclose(f);
}
