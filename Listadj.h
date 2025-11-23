/**
 * Módulo: listadj
 * Finalidade:
 *     Este módulo implementa a interface pública para uma estrutura de dados
 *     baseada em lista de adjacência. Ele provê operações para criação,
 *     manipulação e consulta de grafos representados por vértices e arestas.
 *
 *     Uma instância de ListAdj representa um grafo em que:
 *         - Cada vértice possui um nome (identificador), informação associada
 *           (InfoVert) e, opcionalmente, coordenadas.
 *         - Cada aresta armazena o destino, o índice do vértice destino,
 *           um peso (double) e uma informação adicional (InfoArest).
 *         - As listas de adjacência são compostas por estruturas opacas,
 *           manipuladas por funções deste módulo e do módulo "lista".
 *
 *     Este módulo segue um estilo orientado a objetos em C: a implementação
 *     concreta das estruturas reside exclusivamente no arquivo .c, enquanto
 *     este arquivo expõe apenas tipos opacos e protótipos de funções.
 *
 * Notas:
 *     - NÃO há qualquer definição de struct neste header.
 *       (Information Hiding / Encapsulamento)
 *     - As estruturas concretas estão definidas apenas em listadj.c.
 *     - Todas as funções aqui documentadas operam sobre instâncias opacas.
 */

#ifndef LISTADJ_H
#define LISTADJ_H

#include "lista.h"

/* --------------------------------------------------------------------------
 * Tipos opacos exportados pelo módulo
 * -------------------------------------------------------------------------- */

/* Informação associada a um vértice (tipo definido pelo usuário). */
typedef void* InfoVert;

/* Informação associada a uma aresta (tipo definido pelo usuário). */
typedef void* InfoArest;

/* Tipo opaco que representa a instância da lista de adjacência. */
typedef void* ListAdj;

/* --------------------------------------------------------------------------
 * Operações principais do módulo
 * -------------------------------------------------------------------------- */

/*
 * Função: criaListAdj
 * Finalidade:
 *     Cria uma nova instância da estrutura de lista de adjacência.
 *
 * Requisitos:
 *     - Nenhum.
 *
 * Retorno:
 *     - Uma nova instância de ListAdj, inicialmente vazia.
 *     - Deve ser destruída posteriormente pelo módulo responsável (geralmente
 *       o módulo digraph).
 */
ListAdj criaListAdj();

/*
 * Função: addVertice
 * Finalidade:
 *     Insere um novo vértice no grafo com nome e informação associada.
 *
 * Parâmetros:
 *     - la    : instância da lista de adjacência.
 *     - nome  : identificador do vértice.
 *     - info  : informação associada ao vértice.
 *
 * Restrições:
 *     - Não insere vértices duplicados.
 *
 * Retorno:
 *     - 1 se o vértice foi adicionado com sucesso.
 *     - 0 caso o vértice já exista.
 */
int addVertice(ListAdj la, const char* nome, InfoVert info);

/*
 * Função: addAresta
 * Finalidade:
 *     Adiciona uma aresta direcionada entre dois vértices já existentes.
 *
 * Parâmetros:
 *     - origem   : nome do vértice de origem.
 *     - destino  : nome do vértice de destino.
 *     - peso     : peso associado à aresta.
 *     - info     : dado associado à aresta.
 *
 * Requisitos:
 *     - Tanto origem quanto destino devem existir.
 *
 * Retorno:
 *     - 1 se a aresta foi criada.
 *     - 0 caso algum vértice não exista.
 */
int addAresta(ListAdj la, const char* origem, const char* destino, double peso, InfoArest info);

/*
 * Função: getAdjacentes
 * Finalidade:
 *     Retorna a lista de arestas que partem de determinado vértice.
 *
 * Parâmetros:
 *     - la   : estrutura ListAdj.
 *     - vert : nome do vértice.
 *
 * Retorno:
 *     - Lista de arestas (Lista), pertencente ao grafo.
 *     - NULL se o vértice não existir.
 *
 * Observação:
 *     - O usuário NÃO deve liberar essa lista.
 */
Lista getAdjacentes(ListAdj la, const char* vert);

/*
 * Função: getInfoVert
 * Finalidade:
 *     Obter os dados associados a um vértice.
 *
 * Parâmetros:
 *     - la   : instância do grafo.
 *     - nome : identificador do vértice.
 *
 * Retorno:
 *     - InfoVert associado.
 *     - NULL se o vértice não existir.
 */
InfoVert getInfoVert(ListAdj la, const char* nome);

/*
 * Função: numVertices
 * Finalidade:
 *     Retorna o número de vértices presentes no grafo.
 *
 * Parâmetros:
 *     - la: instância ListAdj.
 *
 * Retorno:
 *     - Quantidade de vértices.
 */
int numVertices(ListAdj la);

/* --------------------------------------------------------------------------
 * Funções auxiliares de acesso (acessores)
 * -------------------------------------------------------------------------- */

/*
 * Função: getVertIndex
 * Finalidade:
 *     Obter o índice interno de um vértice.
 *
 * Retorno:
 *     - Índice do vértice.
 *     - -1 se não existir.
 */
int getVertIndex(ListAdj la, const char* nome);

/*
 * Função: getVertNameByIndex
 * Finalidade:
 *     Obter o nome do vértice dado seu índice.
 *
 * Retorno:
 *     - Nome do vértice.
 *     - NULL se o índice for inválido.
 */
const char* getVertNameByIndex(ListAdj la, int index);

/*
 * Função: getVertCoords
 * Finalidade:
 *     Recuperar coordenadas (x,y) de um vértice.
 *
 * Parâmetros:
 *     - nome : nome do vértice.
 *     - x,y  : ponteiros onde os valores serão armazenados.
 *
 * Retorno:
 *     - 1 se o vértice existir.
 *     - 0 caso contrário.
 */
int getVertCoords(ListAdj la, const char* nome, double *x, double *y);

/*
 * Função: getAdjListByIndex
 * Finalidade:
 *     Retornar a lista de adjacência de um vértice pelo seu índice.
 */
Lista getAdjListByIndex(ListAdj la, int index);

/* --------------------------------------------------------------------------
 * Acessores de arestas (objetos opacos retornados nas listas)
 * -------------------------------------------------------------------------- */

/*
 * Função: aresta_getDestName
 * Finalidade:
 *     Retornar o nome do vértice destino de uma aresta.
 */
const char* aresta_getDestName(void* a);

/*
 * Função: aresta_getDestIndex
 * Finalidade:
 *     Recuperar o índice interno do vértice destino.
 */
int aresta_getDestIndex(void* a);

/*
 * Função: aresta_getPeso
 * Finalidade:
 *     Obter o peso associado à aresta.
 */
double aresta_getPeso(void* a);

#endif /* LISTADJ_H */

