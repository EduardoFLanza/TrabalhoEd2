#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "via.h"

/* ============================================================
   Estruturas internas
   ============================================================ */

typedef struct {
    double x;
    double y;
} ViaVertex;

typedef struct {
    char* name;
    char* right;
    char* left;
    double length;
    double speed;
    bool enabled;
} ViaEdge;

/* ============================================================
   Leitura do arquivo .via
   ============================================================ */

Graph viaReadFile(const char* path)
{
    if (!path || !strstr(path, ".via")) {
        fprintf(stderr, "Arquivo inválido: %s\n", path);
        return NULL;
    }

    FILE* file = fopen(path, "r");
    if (!file) {
        perror("Erro ao abrir arquivo .via");
        return NULL;
    }

    int totalVertices;
    fscanf(file, "%d", &totalVertices);

    Graph g = createGraph(totalVertices);

    char cmd;
    while (fscanf(file, " %c", &cmd) == 1) {

        if (cmd == 'v') {
            char id[128];
            double x, y;

            fscanf(file, "%s %lf %lf", id, &x, &y);

            ViaVertex* v = malloc(sizeof(ViaVertex));
            v->x = x;
            v->y = y;

            addNode(g, id, v);
        }

        else if (cmd == 'e') {
            char from[128], to[128];
            char name[128], right[128], left[128];
            double length, speed;

            fscanf(file, "%s %s %s %s %lf %lf %s",
                   from, to, right, left, &length, &speed, name);

            Node origin = getNode(g, from);
            Node dest   = getNode(g, to);

            if (origin == -1 || dest == -1) {
                continue;
            }

            ViaEdge* e = malloc(sizeof(ViaEdge));

            e->name   = strdup(name);
            e->right  = strdup(right);
            e->left   = strdup(left);
            e->length = length;
            e->speed  = speed;
            e->enabled = true;

            addEdge(g, origin, dest, e);
        }
    }

    fclose(file);
    return g;
}

/* ============================================================
   Operações sobre vias
   ============================================================ */

void viaDisable(ArestaVia via)
{
    ((ViaEdge*)via)->enabled = false;
}

void viaEnable(ArestaVia via)
{
    ((ViaEdge*)via)->enabled = true;
}

void viaSetSpeed(ArestaVia via, double speed)
{
    ((ViaEdge*)via)->speed = speed;
}

/* ============================================================
   Acesso aos vértices
   ============================================================ */

double viaGetX(VerticeVia v)
{
    return ((ViaVertex*)v)->x;
}

double viaGetY(VerticeVia v)
{
    return ((ViaVertex*)v)->y;
}

/* ============================================================
   Acesso às vias
   ============================================================ */

const char* viaGetName(ArestaVia via)
{
    return ((ViaEdge*)via)->name;
}

double viaGetLength(ArestaVia via)
{
    return ((ViaEdge*)via)->length;
}

double viaGetSpeed(ArestaVia via)
{
    return ((ViaEdge*)via)->speed;
}

bool viaIsEnabled(ArestaVia via)
{
    return ((ViaEdge*)via)->enabled;
}

/* ============================================================
   Liberação de memória
   ============================================================ */

void viaFreeEdge(ArestaVia via, void* extra)
{
    ViaEdge* e = (ViaEdge*)via;

    free(e->name);
    free(e->right);
    free(e->left);
    free(e);
}

