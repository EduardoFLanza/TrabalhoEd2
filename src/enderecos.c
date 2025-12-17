/**
 * Módulo: enderecos
 * Finalidade:
 *     Este módulo implementa a abstração Endereco, utilizada para representar
 *     a localização de uma entidade urbana referenciada por uma quadra, uma
 *     face da quadra e uma numeração específica. O módulo fornece operações
 *     para criação, destruição e consulta dessas informações.
 *
 * Descrição Geral:
 *     Um endereço é definido pelos seguintes elementos:
 *         - Identificador da quadra (string)
 *         - Face da quadra (N, S, L, O)
 *         - Número associado à face
 *
 *     A combinação desses dados permite converter o endereço em coordenadas
 *     (x, y) absolutas, desde que as dimensões da quadra estejam disponíveis
 *     através do módulo Retangulo.
 *
 * Observação:
 *     Este módulo não calcula dimensões próprias; a conversão para coordenadas
 *     depende das informações geométricas fornecidas pela quadra associada.
 */

#include <stdlib.h>
#include <string.h>
#include "enderecos.h"
typedef struct{ char* quad; char face; double num; } E;
Endereco endereco_criar(const char* quadraID, char face, double numero){ E* e=malloc(sizeof(E)); e->quad=strdup(quadraID); e->face=face; e->num=numero; return e; }
void endereco_destruir(Endereco e){ E* x=e; free(x->quad); free(x); }
const char* endereco_getQuadraID(Endereco e){ return ((E*)e)->quad; }
char endereco_getFace(Endereco e){ return ((E*)e)->face; }
double endereco_getNumero(Endereco e){ return ((E*)e)->num; }
void endereco_toXY(Endereco e, Retangulo quadra, double* x, double* y){ double qx,qy; retangulo_getPos(quadra,&qx,&qy); *x=qx; *y=qy; }
