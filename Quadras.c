#include "quadras.h"
#include "fileManager.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Estrutura interna que representa uma quadra urbana.
 */
typedef struct QuadraStr {
    char *id;
    double x, y;
    double width, height;
    char *sw;
    char *cfill;
    char *cstrk;
    double opacidade;
} QuadraStr;

/*
 * Estrutura principal do TAD Quadras.
 */
typedef struct QuadrasStr {
    int nQuadras;
    Hash tabelaHash;
    STreap streap;
} QuadrasStr;

/*
 * Função auxiliar para liberação de uma quadra.
 */
static void freeQuadra(Quadra q, void *aux)
{
    QuadraStr *quadra = (QuadraStr *) q;

    free(quadra->id);
    free(quadra->sw);
    free(quadra->cfill);
    free(quadra->cstrk);
    free(quadra);
}

/*
 * Processa arquivo .geo e cria o conjunto de quadras.
 */
Quadras processGeoFile(const char *path)
{
    if (!strstr(path, ".geo")) {
        printf("Arquivo inválido: %s\n", path);
        return NULL;
    }

    FILE *f = fopen(path, "r");
    if (!f) return NULL;

    QuadrasStr *qs = malloc(sizeof(QuadrasStr));
    qs->nQuadras = 0;
    qs->tabelaHash = criaHash(50, true, 0.75f);
    qs->streap = st_create(1e-7);

    char op[32], id[64];
    char sw[32] = "", cfill[32] = "", cstrk[32] = "";
    double x, y, w, h;

    while (fscanf(f, "%s", op) == 1)
    {
        if (strcmp(op, "cq") == 0)
        {
            fscanf(f, "%s %s %s", sw, cfill, cstrk);
        }
        else if (strcmp(op, "q") == 0)
        {
            fscanf(f, "%s %lf %lf %lf %lf", id, &x, &y, &w, &h);

            QuadraStr *q = malloc(sizeof(QuadraStr));
            q->id = strdup(id);
            q->x = x;
            q->y = y;
            q->width = w;
            q->height = h;
            q->sw = strdup(sw);
            q->cfill = strdup(cfill);
            q->cstrk = strdup(cstrk);
            q->opacidade = 1.0;

            inserirHash(qs->tabelaHash, q->id, q);
            st_insert(qs->streap, x, y, q);
            qs->nQuadras++;
        }
    }

    fclose(f);
    return qs;
}

/*
 * Percorre todas as quadras.
 */
void percorrerQuadras(Quadras quadras, FvisitaQuadra f, void *aux)
{
    if (!quadras || !f) return;
    percursoLargura(((QuadrasStr *) quadras)->streap,
                     (FvisitaNo) f, aux);
}

/*
 * Obtém quadras em uma região retangular.
 */
void getQuadrasRegion(
    Quadras quadras,
    double x, double y,
    double w, double h,
    Lista resultado)
{
    if (!quadras) return;
    getNodeRegiaoSTrp(((QuadrasStr *) quadras)->streap,
                      x, y, w, h, resultado);
}

/*
 * Busca quadra pelo identificador.
 */
Quadra getQuadraByID(Quadras quadras, const char *id)
{
    return getHashValue(((QuadrasStr *) quadras)->tabelaHash, id);
}

/* ===== GETTERS ===== */

const char *getQuadraID(Quadra q)       { return ((QuadraStr *) q)->id; }
double getQuadraX(Quadra q)             { return ((QuadraStr *) q)->x; }
double getQuadraY(Quadra q)             { return ((QuadraStr *) q)->y; }
double getQuadraWidth(Quadra q)         { return ((QuadraStr *) q)->width; }
double getQuadraHeight(Quadra q)        { return ((QuadraStr *) q)->height; }
const char *getQuadraCFill(Quadra q)    { return ((QuadraStr *) q)->cfill; }
const char *getQuadraCStrk(Quadra q)    { return ((QuadraStr *) q)->cstrk; }
const char *getQuadraSW(Quadra q)       { return ((QuadraStr *) q)->sw; }
double getQuadraOpacidade(Quadra q)     { return ((QuadraStr *) q)->opacidade; }

/* ===== SETTERS ===== */

void setQuadraCFill(Quadra q, const char *cfill)
{
    QuadraStr *quadra = (QuadraStr *) q;
    free(quadra->cfill);
    quadra->cfill = strdup(cfill);
}

void setQuadraCStrk(Quadra q, const char *cstrk)
{
    QuadraStr *quadra = (QuadraStr *) q;
    free(quadra->cstrk);
    quadra->cstrk = strdup(cstrk);
}

void setQuadraOpacidade(Quadra q, double opacidade)
{
    ((QuadraStr *) q)->opacidade = opacidade;
}

/*
 * Remove uma quadra do sistema.
 */
void removerQuadra(Quadras quadras, Quadra q)
{
    QuadrasStr *qs = (QuadrasStr *) quadras;
    QuadraStr  *quadra = (QuadraStr *) q;

    SNode n = st_search(qs->streap, quadra->x, quadra->y);
    if (n) deleteNodeSTrp(qs->streap, n);

    removeHashValue(qs->tabelaHash, quadra->id);
    freeQuadra(q, NULL);
}

/*
 * Retorna a STreap interna.
 */
STreap getQuadrasSTrp(Quadras quadras)
{
    return ((QuadrasStr *) quadras)->streap;
}

/*
 * Libera todas as quadras.
 */
void freeQuadras(Quadras quadras, void *aux)
{
    QuadrasStr *qs = (QuadrasStr *) quadras;
    st_destroy(qs->streap, freeQuadra);
    destroiHash(qs->tabelaHash, NULL, NULL);
    free(qs);
}
