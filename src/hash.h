#ifndef HASH_H
#define HASH_H

#include <stdbool.h>

/**
 * Módulo: Hash
 *
 * Descrição:
 *   Implementa uma tabela hash com encadeamento externo,
 *   utilizando chaves do tipo string (char*).
 *
 *   A estrutura é genérica: os valores armazenados são
 *   ponteiros void*, permitindo reutilização do módulo
 *   em diferentes contextos.
 *
 *   A tabela realiza redimensionamento automático
 *   quando o fator máximo de preenchimento é atingido.
 */

/* Tipos opacos */
typedef void* Hash;
typedef void* HashItem;

/* Funções auxiliares */
typedef void (*freeFuncHash)(void* item, void* extra);
typedef char* (*printFunc)(void* item, void* extra);

/**
 * Cria uma nova tabela hash.
 *
 * Parâmetros:
 *   initialSize  - tamanho inicial da tabela
 *   maxLoad      - fator máximo de preenchimento (0 < maxLoad <= 1)
 *
 * Retorno:
 *   Ponteiro para a tabela hash criada, ou NULL em caso de erro.
 */
Hash createHash(int initialSize, double maxLoad);

/**
 * Insere ou atualiza um par (chave, valor) na tabela hash.
 *
 * Parâmetros:
 *   hash   - tabela hash
 *   key    - chave associada ao valor
 *   value  - valor a ser armazenado
 */
void insertHash(Hash hash, const char* key, HashItem value);

/**
 * Obtém o valor associado a uma chave.
 *
 * Parâmetros:
 *   hash - tabela hash
 *   key  - chave a ser buscada
 *
 * Retorno:
 *   Valor associado à chave ou NULL se não encontrado.
 */
HashItem getHashValue(Hash hash, const char* key);

/**
 * Remove um elemento da tabela hash.
 *
 * Parâmetros:
 *   hash - tabela hash
 *   key  - chave a ser removida
 *
 * Retorno:
 *   Valor removido ou NULL se a chave não existir.
 */
HashItem removeHashValue(Hash hash, const char* key);

/**
 * Destrói a tabela hash e libera toda a memória alocada.
 *
 * Parâmetros:
 *   hash      - tabela hash
 *   freeValue - função para liberar os valores armazenados
 *   extra     - parâmetro auxiliar repassado para freeValue
 */
void destroyHash(Hash hash, freeFuncHash freeValue, void* extra);

#endif





