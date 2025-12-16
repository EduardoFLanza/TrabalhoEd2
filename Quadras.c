#include "quadras.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct QuadraStr {
    char* id;

    double x, y;
    double w, h;

    char* sw;
    char* cfill;
    char* cstrk;

    double opacity;
} QuadraStr;

typedef struct QuadrasStr {
    int count;

    Hash hash;
    STreap streap;
} QuadrasStr;

/* Funções internas         */

static void freeQuadraInfo(SInfo info) {
    QuadraStr* q = (QuadraStr*)info;

    free(q->id);
    free(q->sw);
    free(q->cfill);
    free(q->cstrk);
    free(q);
}

static Item filterNode(Item item, void* extra) {
    QuadraStr* q = (QuadraStr*)item;

    double* r = (double*)extra;

    return quadraInside(
        r[0], r[1], r[2], r[3],
        q->x, q->y, q->w, q->h
    ) ? q : NULL;
}

/* API pública              */

Quadras createQuadras(void) {
    QuadrasStr* qs = malloc(sizeof(QuadrasStr));
    if (!qs) return NULL;

    qs->count = 0;
    qs->hash = criaHash(50, true, 0.75f);
    qs->streap = st_create(1e-7);

    return qs;
}

void destroyQuadras(Quadras quadras) {
    QuadrasStr* qs = (QuadrasStr*)quadras;
    if (!qs) return;

    st_destroy(qs->streap, freeQuadraInfo);
    destroiHash(qs->hash, NULL, NULL);
    free(qs);
}

Quadra createQuadra(
    const char* id,
    double x, double y,
    double w, double h,
    const char* sw,
    const char* cfill,
    const char* cstrk
) {
    QuadraStr* q = malloc(sizeof(QuadraStr));
    if (!q) return NULL;

    q->id = strdup(id);
    q->x = x;
    q->y = y;
    q->w = w;
    q->h = h;

    q->sw = strdup(sw);
    q->cfill = strdup(cfill);
    q->cstrk = strdup(cstrk);

    q->opacity = 1.0;

    return q;
}

void insertQuadra(Quadras quadras, Quadra quadra) {
    QuadrasStr* qs = (QuadrasStr*)quadras;
    QuadraStr* q = (QuadraStr*)quadra;

    inserirHash(qs->hash, q->id, q);
    st_insert(qs->streap, q->x, q->y, q);
    qs->count++;
}

void removeQuadra(Quadras quadras, Quadra quadra) {
    QuadrasStr* qs = (QuadrasStr*)quadras;
    QuadraStr* q = (QuadraStr*)quadra;

    SNode n = st_search(qs->streap, q->x, q->y);
    if (n) st_deleteNode(qs->streap, n);

    removeHashValue(qs->hash, q->id);
}

Quadra getQuadraById(Quadras quadras, const char* id) {
    QuadrasStr* qs = (QuadrasStr*)quadras;
    return getHashValue(qs->hash, id);
}

void traverseQuadras(
    Quadras quadras,
    FvisitaNo visit,
    void* aux
) {
    QuadrasStr* qs = (QuadrasStr*)quadras;
    percursoLargura(qs->streap, visit, aux);
}

void getQuadrasInRegion(
    Quadras quadras,
    double x, double y,
    double w, double h,
    Lista resultado
) {
    QuadrasStr* qs = (QuadrasStr*)quadras;

    Lista nodes = criaLista();
    getNodeRegiaoSTrp(qs->streap, x, y, w, h, nodes);

    double region[4] = {x, y, w, h};
    mapTo(nodes, resultado, filterNode, NULL, region);
}

STreap quadrasGetSTreap(Quadras quadras) {
    return ((QuadrasStr*)quadras)->streap;
}

/* Getters                  */

const char* quadraGetId(Quadra q) { return ((QuadraStr*)q)->id; }
double quadraGetX(Quadra q) { return ((QuadraStr*)q)->x; }
double quadraGetY(Quadra q) { return ((QuadraStr*)q)->y; }
double quadraGetW(Quadra q) { return ((QuadraStr*)q)->w; }
double quadraGetH(Quadra q) { return ((QuadraStr*)q)->h; }
const char* quadraGetFill(Quadra q) { return ((QuadraStr*)q)->cfill; }
const char* quadraGetStroke(Quadra q) { return ((QuadraStr*)q)->cstrk; }
const char* quadraGetSW(Quadra q) { return ((QuadraStr*)q)->sw; }
double quadraGetOpacity(Quadra q) { return ((QuadraStr*)q)->opacity; }

/* Setters                  */

void quadraSetFill(Quadra q, const char* cfill) {
    QuadraStr* quadra = (QuadraStr*)q;
    free(quadra->cfill);
    quadra->cfill = strdup(cfill);
}

void quadraSetStroke(Quadra q, const char* cstrk) {
    QuadraStr* quadra = (QuadraStr*)q;
    free(quadra->cstrk);
    quadra->cstrk = strdup(cstrk);
}

void quadraSetOpacity(Quadra q, double opacity) {
    ((QuadraStr*)q)->opacity = opacity;
}

/* Utilitário geométrico    */
bool quadraInside(
    double rx, double ry, double rw, double rh,
    double qx, double qy, double qw, double qh
) {
    return (
        qx >= rx &&
        qy >= ry &&
        (qx + qw) <= (rx + rw) &&
        (qy + qh) <= (ry + rh)
    );
}
