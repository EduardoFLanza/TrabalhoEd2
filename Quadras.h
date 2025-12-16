#ifndef QUADRAS_H
#define QUADRAS_H

#include <stdbool.h>
#include "streap.h"
#include "hash.h"
#include "lista.h"

/* Tipos opacos */
typedef void* Quadras;
typedef void* Quadra;

/* Criação e destruição */
Quadras createQuadras(void);
void destroyQuadras(Quadras quadras);

/* Inserção */
Quadra createQuadra(
    const char* id,
    double x, double y,
    double w, double h,
    const char* sw,
    const char* cfill,
    const char* cstrk
);

void insertQuadra(Quadras quadras, Quadra quadra);

/* Remoção */
void removeQuadra(Quadras quadras, Quadra quadra);

/* Busca */
Quadra getQuadraById(Quadras quadras, const char* id);

/* Percursos */
void traverseQuadras(
    Quadras quadras,
    FvisitaNo visit,
    void* aux
);

/* Busca por região */
void getQuadrasInRegion(
    Quadras quadras,
    double x, double y,
    double w, double h,
    Lista resultado
);

/* Getters */
const char* quadraGetId(Quadra q);
double quadraGetX(Quadra q);
double quadraGetY(Quadra q);
double quadraGetW(Quadra q);
double quadraGetH(Quadra q);
const char* quadraGetFill(Quadra q);
const char* quadraGetStroke(Quadra q);
const char* quadraGetSW(Quadra q);
double quadraGetOpacity(Quadra q);

/* Setters */
void quadraSetFill(Quadra q, const char* cfill);
void quadraSetStroke(Quadra q, const char* cstrk);
void quadraSetOpacity(Quadra q, double opacity);

/* Acesso direto à STreap (quando necessário) */
STreap quadrasGetSTreap(Quadras quadras);

/* Utilitários */
bool quadraInside(
    double rx, double ry, double rw, double rh,
    double qx, double qy, double qw, double qh
);

#endif
