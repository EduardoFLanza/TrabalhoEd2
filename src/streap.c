#include "streap.h"
#include "lista.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/*  ESTRUTURAS  */

typedef struct snode {
    double x, y;
    SInfo info;
    unsigned priority;
    double mbbX1, mbbY1, mbbX2, mbbY2; 
    struct snode *left, *right;
} Node;

typedef struct {
    Node *root;
    double eps;
} ST;

/*  UTIL  */

static int seeded = 0;

static unsigned rndPriority() {
    if (!seeded) {
        srand((unsigned)time(NULL));
        seeded = 1;
    }
    return ((unsigned)rand() << 16) ^ rand();
}

static void updateNodeMBB(Node *n) {
    if (!n) return;

    n->mbbX1 = n->mbbX2 = n->x;
    n->mbbY1 = n->mbbY2 = n->y;

    if (n->left) {
        if (n->left->mbbX1 < n->mbbX1) n->mbbX1 = n->left->mbbX1;
        if (n->left->mbbX2 > n->mbbX2) n->mbbX2 = n->left->mbbX2;
        if (n->left->mbbY1 < n->mbbY1) n->mbbY1 = n->left->mbbY1;
        if (n->left->mbbY2 > n->mbbY2) n->mbbY2 = n->left->mbbY2;
    }

    if (n->right) {
        if (n->right->mbbX1 < n->mbbX1) n->mbbX1 = n->right->mbbX1;
        if (n->right->mbbX2 > n->mbbX2) n->mbbX2 = n->right->mbbX2;
        if (n->right->mbbY1 < n->mbbY1) n->mbbY1 = n->right->mbbY1;
        if (n->right->mbbY2 > n->mbbY2) n->mbbY2 = n->right->mbbY2;
    }
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
    updateNodeMBB(n);
    return n;
}

/*  ROTAÇÕES  */

static Node *rotRight(Node *y) {
    Node *x = y->left;
    y->left = x->right;
    x->right = y;
    updateNodeMBB(y);
    updateNodeMBB(x);
    return x;
}

static Node *rotLeft(Node *x) {
    Node *y = x->right;
    x->right = y->left;
    y->left = x;
    updateNodeMBB(x);
    updateNodeMBB(y);
    return y;
}

/*  INSERÇÃO  */

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
    updateNodeMBB(r);
    return r;
}

/*  REMOÇÃO  */

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
    updateNodeMBB(r);
    return r;
}

/*  API  */

STreap st_create(double epsilon) {
    ST *t = calloc(1, sizeof(ST));
    if (!t) return NULL;
    t->eps = (epsilon > 0) ? epsilon : 1e-9;
    return (STreap)t;
}

SNode st_insert(STreap t, double x, double y, SInfo info) {
    ST *st = (ST *)t;
    int ok = 0;
    st->root = insertRec(st, st->root, x, y, info, &ok);
    return ok ? (SNode)st->root : NULL; 
}

SNode st_search(STreap t, double x, double y) {
    ST *st = (ST *)t;
    Node *r = st->root;
    while (r) {
        int c = cmp(st, x, y, r->x, r->y);
        if (c == 0) return (SNode)r;
        r = (c < 0) ? r->left : r->right;
    }
    return NULL;
}

SInfo removeSTrp(STreap t, double x, double y) {
    ST *st = (ST *)t;
    SInfo ret = NULL;
    int done = 0;
    st->root = removeRec(st, st->root, x, y, &ret, &done);
    return ret;
}

SInfo st_getInfo(SNode n) {
    return (n) ? ((Node *)n)->info : NULL;
}

void st_getKey(SNode n, double *x, double *y) {
    Node *nd = (Node *)n;
    if (nd) {
        if (x) *x = nd->x;
        if (y) *y = nd->y;
    }
}

void st_getNodeBB(SNode n, double *x1, double *y1, double *x2, double *y2) {
    Node *nd = (Node *)n;
    if (nd) {
        if (x1) *x1 = nd->mbbX1;
        if (y1) *y1 = nd->mbbY1;
        if (x2) *x2 = nd->mbbX2;
        if (y2) *y2 = nd->mbbY2;
    }
}

SNode st_getRoot(STreap s) { return ((ST*)s)->root; }

/*  PERCURSOS  */

static void preRec(Node *n, FvisitaNo f, void *aux) {
    if (!n) return;
    f(n->info, n->x, n->y, n->mbbX1, n->mbbY1, n->mbbX2, n->mbbY2, aux);
    preRec(n->left, f, aux);
    preRec(n->right, f, aux);
}

static void inRec(Node *n, FvisitaNo f, void *aux) {
    if (!n) return;
    inRec(n->left, f, aux);
    f(n->info, n->x, n->y, n->mbbX1, n->mbbY1, n->mbbX2, n->mbbY2, aux);
    inRec(n->right, f, aux);
}

void percursoProfundidade(STreap t, FvisitaNo f, void *aux) {
    ST *st = (ST *)t;
    if (st) preRec(st->root, f, aux);
}

void percursoSimetrico(STreap t, FvisitaNo f, void *aux) {
    ST *st = (ST *)t;
    if (st) inRec(st->root, f, aux);
}

void percursoLargura(STreap t, FvisitaNo f, void *aux) {
    ST *st = (ST *)t;
    if (!st || !st->root) return;
    Lista q = createLst(-1); 
    insertLst(q, st->root);
    while (!isEmptyLst(q)) {
        Node *n = (Node *)popLst(q);
        f(n->info, n->x, n->y, n->mbbX1, n->mbbY1, n->mbbX2, n->mbbY2, aux);
        if (n->left) insertLst(q, n->left);
        if (n->right) insertLst(q, n->right);
    }
    killLst(q, NULL);
}

/*  REGIÃO E MBB */

typedef struct {
    double x1, y1, x2, y2;
    Lista resultado;
} RangeCtx;

static void getNodeRegiaoRec(Node *n, RangeCtx *ctx) {
    if (!n) return;

    if (n->mbbX2 < ctx->x1 || n->mbbX1 > ctx->x2 || 
        n->mbbY2 < ctx->y1 || n->mbbY1 > ctx->y2) {
        return;
    }

    if (n->x >= ctx->x1 && n->x <= ctx->x2 && 
        n->y >= ctx->y1 && n->y <= ctx->y2) {
        insertLst(ctx->resultado, (SNode)n);
    }

    getNodeRegiaoRec(n->left, ctx);
    getNodeRegiaoRec(n->right, ctx);
}

void getNodeRegiaoSTrp(STreap t, double x, double y, double w, double h, Lista resultado) {
    ST *st = (ST *)t;
    if (!st || !st->root || !resultado) return;

    RangeCtx ctx;
    ctx.x1 = x;
    ctx.y1 = y;
    ctx.x2 = x + w;
    ctx.y2 = y + h;
    ctx.resultado = resultado;

    getNodeRegiaoRec(st->root, &ctx);
}

static void rangeRec(Node *n, double x1, double y1, double x2, double y2, void (*v)(SNode)) {
    if (!n) return;
    if (n->mbbX2 < x1 || n->mbbX1 > x2 || n->mbbY2 < y1 || n->mbbY1 > y2) return;
    if (n->x >= x1 && n->x <= x2 && n->y >= y1 && n->y <= y2) v((SNode)n);
    rangeRec(n->left, x1, y1, x2, y2, v);
    rangeRec(n->right, x1, y1, x2, y2, v);
}

void st_rangeSearch(STreap t, double xmin, double ymin, double xmax, double ymax, void (*visit)(SNode)) {
    ST *st = (ST *)t;
    if (st) rangeRec(st->root, xmin, ymin, xmax, ymax, visit);
}

int st_getMBB(STreap t, double *xmin, double *ymin, double *xmax, double *ymax) {
    ST *st = (ST *)t;
    if (!st || !st->root) return 0;
    if (xmin) *xmin = st->root->mbbX1;
    if (ymin) *ymin = st->root->mbbY1;
    if (xmax) *xmax = st->root->mbbX2;
    if (ymax) *ymax = st->root->mbbY2;
    return 1;
}

/*  PRINT */

static void dotRec(Node *n, FILE *f) {
    if (!n) return;

    fprintf(f, "  \"%p\" [label=\"(%.2lf,%.2lf) P:%u\"];\n", (void *)n, n->x, n->y, n->priority);

    if (n->left) {
        fprintf(f, "  \"%p\" -> \"%p\" [label=\"L\"];\n", (void *)n, (void *)n->left);
        dotRec(n->left, f);
    }

    if (n->right) {
        fprintf(f, "  \"%p\" -> \"%p\" [label=\"R\"];\n", (void *)n, (void *)n->right);
        dotRec(n->right, f);
    }
}

void printSTrp(STreap t, const char* nome) {
    ST *st = (ST *)t;
    if (!st || !nome) return;

    FILE *f = fopen(nome, "w");
    if (!f) return;

    fprintf(f, "digraph STreap {\n");
    if (st->root) {
        dotRec(st->root, f);
    }
    fprintf(f, "}\n");

    fclose(f);
}

/* = DESTROY  */

void st_destroy(STreap t, void (*freeInfo)(SInfo)) {
    ST *st = (ST *)t;
    if (!st) return;
    Lista s = createLst(-1);
    if (st->root) insertLst(s, st->root);
    while (!isEmptyLst(s)) {
        Node *n = (Node *)popLst(s);
        if (n->left) insertLst(s, n->left);
        if (n->right) insertLst(s, n->right);
        if (freeInfo && n->info) freeInfo(n->info);
        free(n);
    }
    killLst(s, NULL);
    free(st);
}
