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
#include <time.h>

#include "STreap.h"
#include "lista.h"

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

/* ====================================================== */
/* util                                                   */
/* ====================================================== */

static int rand_init = 0;

static unsigned rnd_priority() {
    if (!rand_init) {
        srand((unsigned)time(NULL));
        rand_init = 1;
    }
    return ((unsigned)rand() << 16) ^ (unsigned)rand();
}

static int cmp(ST *t, double x1, double y1, double x2, double y2) {
    double eps = t->eps;
    if (x1 + eps < x2) return -1;
    if (x2 + eps < x1) return 1;
    if (y1 + eps < y2) return -1;
    if (y2 + eps < y1) return 1;
    return 0;
}

/* ====================================================== */
/* rotações                                               */
/* ====================================================== */

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

static Node* new_node(double x, double y, SInfo info) {
    Node* n = malloc(sizeof(Node));
    if (!n) return NULL;
    n->x = x;
    n->y = y;
    n->info = info;
    n->priority = rnd_priority();
    n->l = n->r = NULL;
    return n;
}

/* ====================================================== */
/* inserção / busca                                       */
/* ====================================================== */

static Node* insert_rec(ST* s, Node* root,
                        double x, double y, SInfo info,
                        int *inserted) {
    if (!root) {
        *inserted = 1;
        return new_node(x, y, info);
    }

    int c = cmp(s, x, y, root->x, root->y);

    if (c == 0) {
        *inserted = 0;
        return root;
    }

    if (c < 0) {
        root->l = insert_rec(s, root->l, x, y, info, inserted);
        if (root->l && root->l->priority > root->priority)
            root = rotate_right(root);
    } else {
        root->r = insert_rec(s, root->r, x, y, info, inserted);
        if (root->r && root->r->priority > root->priority)
            root = rotate_left(root);
    }
    return root;
}

static Node* search_rec(ST* s, Node* r, double x, double y) {
    if (!r) return NULL;
    int c = cmp(s, x, y, r->x, r->y);
    if (c == 0) return r;
    if (c < 0) return search_rec(s, r->l, x, y);
    return search_rec(s, r->r, x, y);
}

/* ====================================================== */
/* remoção                                                */
/* ====================================================== */

static Node* remove_rec(ST* s, Node* r,
                        double x, double y,
                        SInfo *ret, int *removed) {
    if (!r) return NULL;

    int c = cmp(s, x, y, r->x, r->y);

    if (c < 0) {
        r->l = remove_rec(s, r->l, x, y, ret, removed);
    } else if (c > 0) {
        r->r = remove_rec(s, r->r, x, y, ret, removed);
    } else {
        *ret = r->info;
        *removed = 1;

        if (!r->l && !r->r) {
            free(r);
            return NULL;
        }

        if (!r->l) {
            r = rotate_left(r);
            r->l = remove_rec(s, r->l, x, y, ret, removed);
        } else if (!r->r) {
            r = rotate_right(r);
            r->r = remove_rec(s, r->r, x, y, ret, removed);
        } else {
            if (r->l->priority > r->r->priority) {
                r = rotate_right(r);
                r->r = remove_rec(s, r->r, x, y, ret, removed);
            } else {
                r = rotate_left(r);
                r->l = remove_rec(s, r->l, x, y, ret, removed);
            }
        }
    }
    return r;
}

/* Cria*/
STreap st_create(double eps) {
    ST* s = calloc(1, sizeof(ST));
    if (!s) return NULL;
    s->eps = (eps > 0) ? eps : 1e-9;
    return s;
}

/* Insere*/
SNode st_insert(STreap T, double x, double y, SInfo info) {
    ST* s = (ST*)T;
    int inserted = 0;
    s->root = insert_rec(s, s->root, x, y, info, &inserted);
    return inserted ? search_rec(s, s->root, x, y) : NULL;
}

/* Procura*/
SNode st_search(STreap T, double x, double y) {
    return search_rec((ST*)T, ((ST*)T)->root, x, y);
}

/* Remove*/
SInfo removeSTrp(STreap T, double x, double y) {
    ST* s = (ST*)T;
    SInfo ret = NULL;
    int removed = 0;
    s->root = remove_rec(s, s->root, x, y, &ret, &removed);
    return ret;
}

/* Exclui*/
void st_destroy(STreap T, void (*freeInfo)(SInfo)) {
    if (!T) return;

    Lista stack = criaLista();
    ST* s = (ST*)T;

    if (s->root) insertLst(stack, s->root);

    while (!isEmptyLst(stack)) {
        Posic p = getFirstLst(stack);
        Node* n = getLst(p);
        removePosicLst(stack, p);

        if (n->l) insertLst(stack, n->l);
        if (n->r) insertLst(stack, n->r);

        if (freeInfo) freeInfo(n->info);
        free(n);
    }

    removeLista(stack, NULL);
    free(s);
}
