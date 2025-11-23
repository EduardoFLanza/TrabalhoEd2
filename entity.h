/**
 * Módulo: entity
 * Finalidade:
 *   Declarar a interface do módulo responsável por encapsular diferentes
 *   tipos de objetos gráficos (círculo, retângulo, linha e texto) em uma
 *   abstração única chamada Entity.
 *
 * Descrição Geral:
 *   O módulo fornece funções para criação de entidades a partir de objetos
 *   gráficos, destruição e consulta de atributos básicos como ID e posição.
 */

#ifndef ENTITY_H
#define ENTITY_H

typedef void* Entity;

/* ============================================================================
 *    Construtores de entidades
 * ==========================================================================*/

/*
 * Função: entity_fromCirculo
 * Cria uma entidade encapsulando um objeto círculo.
 * Parâmetros: (void* c) — ponteiro para o círculo.
 * Retorno: Entity — nova entidade criada.
 */
Entity entity_fromCirculo(void* c);

/*
 * Função: entity_fromRetangulo
 * Cria uma entidade encapsulando um objeto retângulo.
 * Parâmetros: (void* r) — ponteiro para o retângulo.
 * Retorno: Entity
 */
Entity entity_fromRetangulo(void* r);

/*
 * Função: entity_fromLinha
 * Cria uma entidade encapsulando um objeto linha.
 * Parâmetros: (void* l) — ponteiro para a linha.
 * Retorno: Entity
 */
Entity entity_fromLinha(void* l);

/*
 * Função: entity_fromTexto
 * Cria uma entidade encapsulando um objeto texto.
 * Parâmetros: (void* t) — ponteiro para o texto.
 * Retorno: Entity
 */
Entity entity_fromTexto(void* t);

/* ============================================================================
 *    Destrutor e acessores
 * ==========================================================================*/

/*
 * Função: entity_destroy
 * Libera a memória da entidade (não libera o objeto interno).
 * Parâmetros: (Entity e)
 * Retorno: void
 */
void entity_destroy(Entity e);

/*
 * Função: entity_getID
 * Retorna o identificador interno da entidade.
 * Parâmetros: (Entity e)
 * Retorno: const char*
 */
const char* entity_getID(Entity e);

/*
 * Função: entity_getXY
 * Obtém posição aproximada associada à entidade (quando aplicável).
 * Parâmetros: (Entity e, double* x, double* y)
 * Retorno: void
 */
void entity_getXY(Entity e, double* x, double* y);

#endif
