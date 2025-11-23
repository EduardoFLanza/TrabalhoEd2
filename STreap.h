/**
 * Módulo: STreap
 * Finalidade:
 *     Implementa uma Treap espacial (STreap), que associa a cada ponto-âncora (x,y)
 *     uma informação arbitrária. A chave de busca é a coordenada da âncora.
 *
 *     A comparação entre duas chaves segue a regra:
 *
 *         - (x1 < x2)
 *         - OU (x1 == x2 E y1 < y2)
 *
 *     A árvore mantém um parâmetro de precisão epsilon, usado para comparar
 *     valores em ponto flutuante.
 *
 *     Os nós retornados por operações de busca são válidos apenas
 *     enquanto não ocorrer nenhuma remoção posterior.
 */

#ifndef STREAP_H
#define STREAP_H

#include "lista.h"

typedef void *STreap;   /* Estrutura da árvore */
typedef void *SInfo;    /* Informação armazenada */
typedef void *SNode;    /* Nó interno */

/* 
 * Tipo de função de visita usada em percursos.
 * Recebe:
 *   - info armazenada
 *   - coordenadas da âncora
 *   - bounding box mínimo (mbbX1,mbbY1) e máximo (mbbX2,mbbY2)
 *   - ponteiro auxiliar para dados compartilhados
 */
typedef void (*FvisitaNo)(
    SInfo i,
    double x, double y,
    double mbbX1, double mbbY1,
    double mbbX2, double mbbY2,
    void *aux
);

/*
 * Função: createSTrp
 * Descrição:
 *     Cria uma STreap vazia, definindo a precisão epsilon.
 *
 * Parâmetros:
 *     epsilon — erro absoluto aceito na comparação de números reais.
 *
 * Retorno:
 *     Um ponteiro para a STreap criada, ou NULL caso não haja memória.
 */
STreap createSTrp(double epsilon);

/*
 * Função: insertSTrp
 * Descrição:
 *     Insere a informação info na STreap associada à âncora (x,y).
 *
 * Parâmetros:
 *     t — STreap
 *     x,y — âncora (chave)
 *     info — informação armazenada
 *
 * Retorno:
 *     Nó onde foi inserido, ou NULL se já existir chave igual.
 */
SNode insertSTrp(STreap t, double x, double y, SInfo info);

/*
 * Função: getNodeRegiaoSTrp
 * Descrição:
 *     Insere na lista resultado os nós cuja âncora esteja dentro da região
 *     retangular de âncora (x,y) com largura w e altura h.
 *
 * Parâmetros:
 *     t — STreap
 *     x,y,w,h — retângulo de busca
 *     resultado — lista onde serão inseridos nós encontrados
 */
void getNodeRegiaoSTrp(STreap t, double x, double y, double w, double h, Lista resultado);

/*
 * Função: getInfoSTrp
 * Descrição:
 *     Obtém a informação armazenada no nó n.
 *
 * Parâmetros:
 *     t — STreap
 *     n — nó válido
 *
 * Retorno:
 *     Informação armazenada no nó.
 */
SInfo getInfoSTrp(STreap t, SNode n);

/*
 * Função: getNodeSTrp
 * Descrição:
 *     Retorna o nó cuja âncora é (xa,ya).
 *
 * Parâmetros:
 *     t — STreap
 *     xa,ya — âncora buscada
 *
 * Retorno:
 *     Nó correspondente, ou NULL se inexistente.
 */
SNode getNodeSTrp(STreap t, double xa, double ya);

/*
 * Função: updateInfoSTrp
 * Descrição:
 *     Atualiza a informação associada ao nó n, sem alterar a âncora.
 *
 * Parâmetros:
 *     t — STreap
 *     n — nó existente
 *     info — nova informação
 */
void updateInfoSTrp(STreap t, SNode n, SInfo info);

/*
 * Função: deleteNodeSTrp
 * Descrição:
 *     Remove o nó n da STreap.
 *
 * Parâmetros:
 *     t — STreap
 *     n — nó válido
 *
 * Retorno:
 *     Informação previamente armazenada no nó.
 */
SInfo deleteNodeSTrp(STreap t, SNode n);

/*
 * Função: removeSTrp
 * Descrição:
 *     Remove da STreap o nó cuja âncora é (xa,ya).
 *
 * Parâmetros:
 *     t — STreap
 *     xa,ya — âncora a ser removida
 *
 * Retorno:
 *     Informação associada ao nó removido, ou NULL se não existir.
 */
SInfo removeSTrp(STreap t, double xa, double ya);

/*
 * Função: printSTrp
 * Descrição:
 *     Gera arquivo no formato DOT contendo a árvore.
 */
void printSTrp(STreap t, char *nomeArq);

/*
 * Percursos: visitam todos os nós da árvore, chamando fVisita.
 */
void percursoLargura(STreap t, FvisitaNo fVisita, void *aux);
void percursoSimetrico(STreap t, FvisitaNo fVisita, void *aux);
void percursoProfundidade(STreap t, FvisitaNo fVisita, void *aux);

/*
 * Função: killSTrp
 * Descrição:
 *     Libera todos os recursos usados pela árvore.
 */
void killSTrp(STreap t);

#endif
