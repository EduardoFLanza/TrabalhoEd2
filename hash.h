#ifndef _HASH_H_
#define _HASH_H_

#include <stdbool.h>

/**
 * Módulo: Hash
 * Finalidade:
 *     Implementa o TAD Hash, responsável por armazenar e recuperar pares
 *     (chave, valor) de forma eficiente, utilizando espalhamento (hashing)
 *     com tratamento de colisões por encadeamento separado.
 *
 *     As chaves são cadeias de caracteres (char*) e os valores são genéricos
 *     (void*), permitindo que este módulo seja reutilizado por diferentes
 *     estruturas do sistema, como quadras, vias, entidades, etc.
 *
 *     O módulo controla automaticamente o fator de carga da tabela, realizando
 *     redimensionamento (rehash) quando necessário, de modo a manter o desempenho
 *     das operações de inserção, busca e remoção.
 *
 *     Este módulo faz parte de um trabalho da disciplina de Estrutura de Dados
 *     do curso de Ciência da Computação.
 */

/* ============================
   Tipos opacos
   ============================ */

typedef void* Hash;

/* ============================
   Funções auxiliares genéricas
   ============================ */

/*
 * Função usada para liberação de memória associada aos valores armazenados.
 */
typedef void (*freeFunc)(void*);

/* ============================
   Criação e destruição
   ============================ */

/*
 * Cria uma tabela hash.
 *
 * Parâmetros:
 *   initialSize : tamanho inicial da tabela
 *   loadFactor  : fator máximo de ocupação (0 < loadFactor <= 1)
 *
 * Retorno:
 *   Estrutura Hash criada ou NULL em caso de erro.
 */
Hash hashCreate(int initialSize, double loadFactor);

/*
 * Libera completamente a tabela hash.
 *
 * Parâmetros:
 *   hash     : tabela hash
 *   freeFunc : função opcional para liberar os valores armazenados
 */
void hashDestroy(Hash hash, freeFunc freeFunc);

/* ============================
   Operações básicas
   ============================ */

/*
 * Insere ou atualiza um par (chave, valor) na tabela hash.
 *
 * Parâmetros:
 *   hash  : tabela hash
 *   key   : chave de acesso
 *   value : valor associado
 */
void hashInsert(Hash hash, const char *key, void *value);

/*
 * Recupera o valor associado a uma chave.
 *
 * Parâmetros:
 *   hash : tabela hash
 *   key  : chave de acesso
 *
 * Retorno:
 *   Valor associado à chave ou NULL caso não exista.
 */
void* hashGet(Hash hash, const char *key);

/*
 * Remove um par (chave, valor) da tabela hash.
 *
 * Parâmetros:
 *   hash : tabela hash
 *   key  : chave a ser removida
 *
 * Retorno:
 *   Valor associado à chave removida ou NULL caso não exista.
 */
void* hashRemove(Hash hash, const char *key);

#endif

