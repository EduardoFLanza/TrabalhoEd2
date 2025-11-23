/**
 * Módulo: entity
 * Finalidade:
 *   Implementar um tipo genérico de entidade gráfica, permitindo encapsular
 *   diferentes objetos (círculos, retângulos, linhas e textos) sob uma mesma
 *   interface unificada.
 *
 * Descrição Geral:
 *   Este módulo fornece funções para criação, destruição e consulta de
 *   propriedades básicas de entidades, como ID e posição. A estrutura interna
 *   é privada ao módulo e acessada apenas via ponteiro opaco (Entity).
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "entity.h"

/* Tipos internos de entidade */
typedef enum { ENT_CIRC, ENT_RET, ENT_LIN, ENT_TXT } EType;

/* Estrutura privada da entidade */
typedef struct {
    EType tipo;      /* tipo do objeto armazenado */
    void* obj;       /* ponteiro genérico para o objeto */
    char id[64];     /* identificador interno */
    double x, y;     /* posição associada (quando aplicável) */
} EntityStruct;

/* ============================================================================
 *    Funções de criação (construtores de entidades encapsuladas)
 * ==========================================================================*/

/**
 * Função: entity_fromCirculo
 * Cria uma entidade do tipo círculo.
 */
Entity entity_fromCirculo(void* c){
    EntityStruct* e = malloc(sizeof(EntityStruct));
    if(!e) return NULL;

    e->tipo = ENT_CIRC;
    e->obj  = c;
    snprintf(e->id, sizeof(e->id), "c_%p", c);
    e->x = 0;
    e->y = 0;

    return (Entity)e;
}

/**
 * Função: entity_fromRetangulo
 * Cria uma entidade do tipo retângulo.
 */
Entity entity_fromRetangulo(void* r){
    EntityStruct* e = malloc(sizeof(EntityStruct));
    if(!e) return NULL;

    e->tipo = ENT_RET;
    e->obj  = r;
    snprintf(e->id, sizeof(e->id), "r_%p", r);
    e->x = 0;
    e->y = 0;

    return (Entity)e;
}

/**
 * Função: entity_fromLinha
 * Cria uma entidade do tipo linha.
 */
Entity entity_fromLinha(void* l){
    EntityStruct* e = malloc(sizeof(EntityStruct));
    if(!e) return NULL;

    e->tipo = ENT_LIN;
    e->obj  = l;
    snprintf(e->id, sizeof(e->id), "l_%p", l);
    e->x = 0;
    e->y = 0;

    return (Entity)e;
}

/**
 * Função: entity_fromTexto
 * Cria uma entidade do tipo texto.
 */
Entity entity_fromTexto(void* t){
    EntityStruct* e = malloc(sizeof(EntityStruct));
    if(!e) return NULL;

    e->tipo = ENT_TXT;
    e->obj  = t;
    snprintf(e->id, sizeof(e->id), "t_%p", t);
    e->x = 0;
    e->y = 0;

    return (Entity)e;
}

/* ============================================================================
 *    Funções de destruição
 * ==========================================================================*/

/**
 * Função: entity_destroy
 * Libera a memória da estrutura Entity (não libera o objeto interno).
 */
void entity_destroy(Entity E){
    if(!E) return;
    free(E);
}

/* ============================================================================
 *    Funções de acesso (getters)
 * ==========================================================================*/

/**
 * Função: entity_getID
 * Retorna o identificador textual da entidade.
 */
const char* entity_getID(Entity E){
    if(!E) return NULL;
    return ((EntityStruct*)E)->id;
}

/**
 * Função: entity_getXY
 * Obtém a posição aproximada da entidade (quando aplicável).
 */
void entity_getXY(Entity E, double* x, double* y){
    if(!E) return;
    EntityStruct* e = (EntityStruct*)E;

    if(x) *x = e->x;
    if(y) *y = e->y;
}
