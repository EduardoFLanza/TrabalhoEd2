/**
 * Módulo: enderecos
 * Finalidade:
 *     Este módulo define a interface pública da abstração Endereco, utilizada
 *     para representar a localização de uma entidade urbana associada a uma
 *     quadra, uma face e uma numeração. O módulo disponibiliza funções para
 *     criação, consulta e destruição dessas instâncias.
 *
 * Descrição Geral:
 *     Um endereço é formado por:
 *         - Identificador da quadra (string)
 *         - Face da quadra (N, S, L ou O)
 *         - Número na respectiva face
 *
 *     A função endereco_toXY permite converter essas informações em coordenadas
 *     cartesianas, utilizando os dados geométricos da quadra correspondente
 *     fornecida pelo módulo Retangulo.
 */

#ifndef ENDERECOS_H
#define ENDERECOS_H

#include "retangulo.h"

typedef void* Endereco;

/*
 * Função: endereco_criar
 * Descrição:
 *     Cria e inicializa uma nova instância de Endereco com os parâmetros
 *     fornecidos.
 * Parâmetros:
 *     quadraID — identificador da quadra
 *     face     — face da quadra (N, S, L ou O)
 *     numero   — número associado à face
 * Retorno:
 *     A instância Endereco recém-criada.
 */
Endereco endereco_criar(const char* quadraID, char face, double numero);

/*
 * Função: endereco_destruir
 * Descrição:
 *     Libera todos os recursos associados ao endereço.
 */
void endereco_destruir(Endereco e);

/*
 * Função: endereco_getQuadraID
 * Descrição:
 *     Retorna a string identificadora da quadra associada ao endereço.
 */
const char* endereco_getQuadraID(Endereco e);

/*
 * Função: endereco_getFace
 * Descrição:
 *     Retorna a face da quadra associada ao endereço.
 */
char endereco_getFace(Endereco e);

/*
 * Função: endereco_getNumero
 * Descrição:
 *     Retorna o número associado à face do endereço.
 */
double endereco_getNumero(Endereco e);

/*
 * Função: endereco_toXY
 * Descrição:
 *     Converte o endereço para coordenadas cartesianas (x, y) com base na
 *     quadra referenciada.
 * Parâmetros:
 *     e      — endereço
 *     quadra — instância de Retangulo correspondente à quadra
 *     x, y   — ponteiros que receberão as coordenadas calculadas
 */
void endereco_toXY(Endereco e, Retangulo quadra, double* x, double* y);

#endif

