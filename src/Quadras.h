#ifndef QUADRAS_H
#define QUADRAS_H

#include "hash.h"
#include "streap.h"
#include "lista.h"

/**
 * Módulo: Quadras
 *
 * Finalidade:
 *     Implementa o TAD Quadras, responsável por armazenar, indexar e
 *     gerenciar quadras urbanas lidas do arquivo .geo.
 *
 *     As quadras são guardadas em uma lista generica e também
 *     associadas por identificador (id) através de uma tabela hash.
 *
 *     Este módulo provê operações de criação, percurso da lista,
 *     acesso por identificador, atualização de atributos e liberação
 *     completa da estrutura.
 *
 * Abstração:
 *     - Quadras e Quadra são tipos opacos.
 *     - O usuário não tem acesso às estruturas internas.
 *
 * Importante:
 *     - A remoção de uma quadra invalida seu ponteiro.
 */

/* ============================================================
   Tipos opacos
   ============================================================ */

typedef void *Quadras;
typedef void *Quadra;

/* ============================================================
   Criação e destruição
   ============================================================ */

/*
 * Função: processGeoFile
 * Descrição: Processa um arquivo .geo e cria a estrutura Quadras.
 * Parâmetros:
 *     path – caminho do arquivo .geo.
 * Retorno:
 *     Estrutura Quadras ou NULL em caso de erro.
 */
Quadras processGeoFile(const char *path);

/*
 * Função: freeQuadras
 * Descrição: Libera toda a estrutura Quadras.
 * Parâmetros:
 *     quadras – estrutura Quadras.
 */
void freeQuadras(Quadras quadras);

/* ============================================================
   Percurso
   ============================================================ */

/*
 * Tipo: FvisitaQuadra
 * Descrição:
 *     Função de visita utilizada no percurso das quadras.
 *
 * Parâmetros:
 *     q   – quadra visitada.
 *     x,y – coordenadas da quadra.
 *     aux – dado auxiliar.
 */
typedef void (*FvisitaQuadra)(Quadra q, double x, double y, void *aux);

/*
 * Função: percorrerQuadras
 * Descrição: Percorre todas as quadras armazenadas.
 * Parâmetros:
 *     quadras – estrutura Quadras.
 *     f       – função de visita.
 *     aux     – dado auxiliar.
 */
void percorrerQuadras(Quadras quadras, FvisitaQuadra f, void *aux);

/* ============================================================
   Acesso por identificador
   ============================================================ */

/*
 * Função: getQuadraByID
 * Descrição: Busca uma quadra pelo identificador.
 * Parâmetros:
 *     quadras – estrutura Quadras.
 *     id      – identificador da quadra.
 * Retorno:
 *     Quadra ou NULL se não encontrada.
 */
Quadra getQuadraByID(Quadras quadras, const char *id);

/* ============================================================
   Getters
   ============================================================ */

/*
 * Função: getQuadraID
 * Retorno: Identificador da quadra.
 */
const char *getQuadraID(Quadra q);

/*
 * Função: getQuadraX
 * Retorno: Coordenada x da quadra.
 */
double getQuadraX(Quadra q);

/*
 * Função: getQuadraY
 * Retorno: Coordenada y da quadra.
 */
double getQuadraY(Quadra q);

/*
 * Função: getQuadraWidth
 * Retorno: Largura da quadra.
 */
double getQuadraWidth(Quadra q);

/*
 * Função: getQuadraHeight
 * Retorno: Altura da quadra.
 */
double getQuadraHeight(Quadra q);

/*
 * Função: getQuadraCFill
 * Retorno: Cor de preenchimento.
 */
const char *getQuadraCFill(Quadra q);

/*
 * Função: getQuadraCStrk
 * Retorno: Cor da borda.
 */
const char *getQuadraCStrk(Quadra q);

/*
 * Função: getQuadraSW
 * Retorno: Espessura da borda.
 */
const char *getQuadraSW(Quadra q);

/*
 * Função: getQuadraOpacidade
 * Retorno: Opacidade da quadra.
 */
double getQuadraOpacidade(Quadra q);

/* ============================================================
   Setters
   ============================================================ */

/*
 * Função: setQuadraCFill
 * Descrição: Atualiza a cor de preenchimento.
 */
void setQuadraCFill(Quadra q, const char *cfill);

/*
 * Função: setQuadraCStrk
 * Descrição: Atualiza a cor da borda.
 */
void setQuadraCStrk(Quadra q, const char *cstrk);

/*
 * Função: setQuadraOpacidade
 * Descrição: Atualiza a opacidade da quadra.
 */
void setQuadraOpacidade(Quadra q, double opacidade);

/* ============================================================
   Remoção
   ============================================================ */

/*
 * Função: removerQuadra
 * Descrição: Remove uma quadra da estrutura.
 * Observação:
 *     O ponteiro da quadra torna-se inválido após a remoção.
 */
void removerQuadra(Quadras quadras, Quadra q);

#endif
