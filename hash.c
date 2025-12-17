#include "hash.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* ============================
   Estruturas internas
   ============================ */

/* Célula da lista encadeada */
typedef struct HashNode {
    char *key;              // chave associada
    void *value;            // valor genérico
    struct HashNode *next;  // próximo nó
} HashNode;

/* Estrutura principal da hash */
typedef struct HashTable {
    int size;               // tamanho da tabela
    int used;               // quantidade de posições ocupadas
    double loadFactor;      // fator máximo de ocupação
    HashNode **table;       // vetor de listas
} HashTable;

/* ============================
   Funções auxiliares
   ============================ */

static int isPrime(int n) {
    if (n < 2) return 0;
    for (int i = 2; i <= sqrt(n); i++)
        if (n % i == 0) return 0;
    return 1;
}

static int nextPrime(int n) {
    while (!isPrime(n)) n++;
    return n;
}

/* Função hash (variação do djb2) */
static unsigned long hashCode(const char *str) {
    unsigned long h = 5381;
    int c;

    while ((c = *str++))
        h = ((h << 5) + h) + c;

    return h;
}

static int indexFor(const char *key, int size) {
    return hashCode(key) % size;
}

/* ============================
   Rehash (redimensionamento)
   ============================ */

static void rehash(HashTable *h) {
    int newSize = nextPrime(h->size * 2);
    HashNode **newTable = calloc(newSize, sizeof(HashNode*));

    for (int i = 0; i < h->size; i++) {
        HashNode *node = h->table[i];
        while (node) {
            HashNode *next = node->next;
            int idx = indexFor(node->key, newSize);

            node->next = newTable[idx];
            newTable[idx] = node;

            node = next;
        }
    }

    free(h->table);
    h->table = newTable;
    h->size = newSize;
}

/* ============================
   Interface pública
   ============================ */

Hash hashCreate(int initialSize, double loadFactor) {
    if (initialSize <= 0) return NULL;

    HashTable *h = malloc(sizeof(HashTable));
    if (!h) return NULL;

    h->size = nextPrime(initialSize);
    h->used = 0;
    h->loadFactor = loadFactor;
    h->table = calloc(h->size, sizeof(HashNode*));

    return h;
}

void hashInsert(Hash hash, const char *key, void *value) {
    if (!hash || !key) return;

    HashTable *h = (HashTable*) hash;
    int idx = indexFor(key, h->size);

    /* Verifica se a chave já existe */
    for (HashNode *n = h->table[idx]; n; n = n->next) {
        if (strcmp(n->key, key) == 0) {
            n->value = value;
            return;
        }
    }

    /* Cria novo nó */
    HashNode *node = malloc(sizeof(HashNode));
    node->key = strdup(key);
    node->value = value;
    node->next = h->table[idx];

    h->table[idx] = node;
    h->used++;

    /* Checa fator de carga */
    if ((double)h->used / h->size >= h->loadFactor) {
        rehash(h);
    }
}

void* hashGet(Hash hash, const char *key) {
    if (!hash || !key) return NULL;

    HashTable *h = (HashTable*) hash;
    int idx = indexFor(key, h->size);

    for (HashNode *n = h->table[idx]; n; n = n->next) {
        if (strcmp(n->key, key) == 0)
            return n->value;
    }

    return NULL;
}

void* hashRemove(Hash hash, const char *key) {
    if (!hash || !key) return NULL;

    HashTable *h = (HashTable*) hash;
    int idx = indexFor(key, h->size);

    HashNode *prev = NULL;
    HashNode *n = h->table[idx];

    while (n) {
        if (strcmp(n->key, key) == 0) {
            void *val = n->value;

            if (prev) prev->next = n->next;
            else h->table[idx] = n->next;

            free(n->key);
            free(n);
            h->used--;

            return val;
        }
        prev = n;
        n = n->next;
    }

    return NULL;
}

void hashDestroy(Hash hash, void (*freeFunc)(void*)) {
    if (!hash) return;

    HashTable *h = (HashTable*) hash;

    for (int i = 0; i < h->size; i++) {
        HashNode *n = h->table[i];
        while (n) {
            HashNode *next = n->next;

            free(n->key);
            if (freeFunc) freeFunc(n->value);
            free(n);

            n = next;
        }
    }

    free(h->table);
    free(h);
}
}
