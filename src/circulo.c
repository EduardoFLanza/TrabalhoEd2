
/**
 * Módulo: circulo
 * Finalidade:
 *     Este módulo implementa a abstração “Círculo”, utilizada pelas operações
 *     geométricas do sistema. Ele fornece um conjunto de funções para criar,
 *     consultar e manipular círculos, incluindo coordenadas, raio e atributos
 *     utilizados para representação gráfica (como cores e identificadores).
 *
 * Descrição Geral:
 *     Uma instância da abstração “Círculo” representa geometricamente uma
 *     região circular definida por:
 *         - coordenada x do centro
 *         - coordenada y do centro
 *         - raio
 *         - informações auxiliares (ex.: identificador, cor de preenchimento,
 *           cor da borda, espessura, metadados etc.)
 *
 *     O módulo segue o princípio de encapsulamento:
 *         - Todas as estruturas concretas são definidas somente no .c
 *         - O arquivo .h expõe apenas tipos opacos e operações públicas
 *
 * Notas Importantes:
 *     - Este módulo fornece apenas as operações fundamentais da entidade.
 *     - O utilizador é responsável por destruir a instância quando necessário
 *       para evitar vazamentos de memória.
 *     - Cada função é documentada individualmente no .h e implementada no .c.
 */


#include <stdlib.h>
#include <string.h>
#include <string.h>
#ifndef _POSIX_C_SOURCE
static char* my_strdup(const char* s){ if(!s) return NULL; char* r=malloc(strlen(s)+1); if(!r) return NULL; strcpy(r,s); return r; }
#define strdup my_strdup
#endif
#include "circulo.h"
typedef struct CirculoStruct { double x,y,r; char* id; } CirculoStruct;
Circulo circulo_criar(double cx,double cy,double r,const char* stroke,const char* fill,const char* id){
    CirculoStruct* c=malloc(sizeof(CirculoStruct));
    c->x=cx; c->y=cy; c->r=r;
    c->id = id ? strdup(id) : strdup("");
    return c;
}
void circulo_destruir(Circulo C){ CirculoStruct* c=(CirculoStruct*)C; if(c->id) free(c->id); free(c); }
const char* circulo_getID(Circulo C){ CirculoStruct* c=(CirculoStruct*)C; return c->id; }
void circulo_getPos(Circulo C,double* x,double* y){ CirculoStruct* c=(CirculoStruct*)C; if(x) *x=c->x; if(y) *y=c->y; }
