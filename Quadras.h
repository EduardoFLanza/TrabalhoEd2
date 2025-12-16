/**
 * Módulo: Quadras
 * Finalidade:
 *     Implementa o TAD Quadras, responsável por armazenar, indexar e
 *     gerenciar quadras urbanas lidas do arquivo .geo.
 *
 *     As quadras são indexadas espacialmente por meio de uma STreap,
 *     utilizando a coordenada (x,y) como âncora espacial, e também
 *     associadas por identificador (id) através de uma tabela hash.
 *
 *     Este módulo provê operações de criação, percurso, busca espacial,
 *     acesso por identificador, atualização de atributos e liberação
 *     completa da estrutura.
 */

#ifndef QUADRAS_H
#define QUADRAS_H

#include "streap.h"
#include "lista.h"
#include "hash.h"

/* Tipos opacos */
typedef void* Quadras;
typedef void* Quadra;

/* Função de visita usada em percursos */
typedef void (*FvisitaQuadra)(
    Quadra q,
    double x, double y,
    double mbbX1, double mbbY1,
    double mbbX2, double mbbY2,
    void* aux
);

/* Cria e processa as quadras a partir de um arquivo .geo */
Quadras processGeoFile(const char* path);

/* Percorre todas as quadras armazenadas (percurso em largura da STreap) */
void percorrerQuadras(Quadras quadras, FvisitaQuadra f, void* aux);

/* Busca quadras contidas em uma região retangular */
void getQuadrasRegion(
    Quadras quadras,
    double x, double y,
    double w, double h,
    Lista resultado
);

/* Retorna uma quadra a partir de seu identificador */
Quadra getQuadraByID(Quadras quadras, const char* id);

/* Funções de acesso aos atributos da quadra */
const char* getQuadraID(Quadra q);
double getQuadraX(Quadra q);
double getQuadraY(Quadra q);
double getQuadraWidth(Quadra q);
double getQuadraHeight(Quadra q);
const char* getQuadraCFill(Quadra q);
const char* getQuadraCStrk(Quadra q);
const char* getQuadraSW(Quadra q);
double getQuadraOpacidade(Quadra q);

/* Funções de modificação de atributos */
void setQuadraCFill(Quadra q, const char* cfill);
void setQuadraCStrk(Quadra q, const char* cstrk);
void setQuadraOpacidade(Quadra q, double opacidade);

/* Remove uma quadra da estrutura */
void removerQuadra(Quadras quadras, Quadra q);

/* Retorna a STreap interna (uso controlado) */
STreap getQuadrasSTrp(Quadras quadras);

/* Libera toda a estrutura de quadras */
void freeQuadras(Quadras quadras, void* aux);

#endif /* QUADRAS_H */
