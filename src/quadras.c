#define _XOPEN_SOURCE 700
#include "quadras.h"

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
 *
 * Uma lista quadras para acesso das mesmas,
 * enquanto a Hash permite acesso direto por identificador.
 */
typedef struct QuadrasStr {
    int nQuadras;
    Lista lista;
    Hash tabelaHash;
} QuadrasStr;

/*
 * Função auxiliar para liberação de uma quadra.
 */
static void freeQuadra(Quadra q)
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

    QuadrasStr *qs = calloc(1, sizeof(QuadrasStr));
    qs->lista = createLst(-1);
    qs->tabelaHash = createHash(50, 0.75f);

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

            insertLst(qs->lista, q);
            insertHash(qs->tabelaHash, q->id, q);
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

    Lista lista = ((QuadrasStr*)quadras)->lista;

    Iterador it = createIterador(lista, false);

    while (!isIteratorEmpty(lista, it)) {
        QuadraStr* quadra = (QuadraStr*)getIteratorItem(lista, it);
        
        f(quadra, quadra->x, quadra->y, aux);

        it = getIteratorNext(lista, it);
    }

    killIterator(lista, it);
}

/*
 * Busca quadra pelo identificador.
 */
Quadra getQuadraByID(Quadras quadras, const char *id)
{
    if (!quadras || !id) return NULL;
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
    if (!quadras || !q) return;

    QuadrasStr *qs = (QuadrasStr *) quadras;
    QuadraStr *quadra = (QuadraStr *) q;

    removeHashValue(qs->tabelaHash, quadra->id);

    Iterador it = createIterador(qs->lista, false);
    while (!isIteratorEmpty(qs->lista, it)) {
        if (getIteratorItem(qs->lista, it) == quadra) {
            Posic p = getIteratorPosic(qs->lista, it);
            
            getIteratorNext(qs->lista, it); 
            
            removeLst(qs->lista, p); 
            break;
        } else {
            getIteratorNext(qs->lista, it);
        }
    }
    
    killIterator(qs->lista, it);

    freeQuadra(quadra);
    qs->nQuadras--;
}

/*
 * Libera todas as quadras.
 *
 * A lista de quadra é responsavel pela liberacao.
 * A Hash apenas referencia os ponteiros, portanto
 * não deve liberar os valores.
 */
void freeQuadras(Quadras quadras)
{
    if (!quadras) return;

    QuadrasStr *qs = (QuadrasStr *) quadras;

    killLst(qs->lista, freeQuadra);
    destroyHash(qs->tabelaHash, NULL, NULL);
    free(qs);
}



