#include "hash.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* ============================================================
   Estruturas internas
   ============================================================ */

/*
 * Célula da tabela hash (lista encadeada)
 */
typedef struct HashCell {
    char* key;
    HashItem value;
    struct HashCell* next;
} HashCell;

/*
 * Estrutura principal da tabela hash
 */
typedef struct HashImpl {
    int size;                 // tamanho do vetor
    int used;                 // quantidade de chaves inseridas
    double maxLoad;           // fator máximo de ocupação
    HashCell** table;         // vetor de listas
} HashImpl;

/* ============================================================
   Funções auxiliares internas
   ============================================================ */

/*
 * Verifica se um número é primo
 */
static bool isPrime(int n) {
    if (n < 2) return false;
    for (int i = 2; i <= sqrt(n); i++)
        if (n % i == 0) return false;
    return true;
}

/*
 * Retorna o próximo número primo maior ou igual a n
 */
static int nextPrime(int n) {
    if (n % 2 == 0) n++;
    while (!isPrime(n)) n += 2;
    return n;
}

/*
 * Função hash baseada em djb2
 */
static unsigned long hashFunction(const char* str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;

    return hash;
}

/*
 * Calcula o índice da chave
 */
static int hashIndex(HashImpl* h, const char* key) {
    return hashFunction(key) % h->size;
}

/*
 * Reorganiza a tabela quando o fator de carga é excedido
 */
static void rehash(HashImpl* h) {
    int newSize = nextPrime(h->size * 2);
    HashCell** newTable = malloc(sizeof(HashCell*) * newSize);

    for (int i = 0; i < newSize; i++)
        newTable[i] = NULL;

    for (int i = 0; i < h->size; i++) {
        HashCell* cell = h->table[i];
        while (cell) {
            HashCell* next = cell->next;

            int idx = hashFunction(cell->key) % newSize;
            cell->next = newTable[idx];
            newTable[idx] = cell;

            cell = next;
        }
    }

    free(h->table);
    h->table = newTable;
    h->size = newSize;
}

/* ============================================================
   Criação e destruição
   ============================================================ */

Hash createHash(int initialSize, bool usePrime, double maxLoad) {
    if (initialSize < 1) return NULL;

    HashImpl* h = malloc(sizeof(HashImpl));
    if (!h) return NULL;

    h->size = usePrime ? nextPrime(initialSize) : initialSize;
    h->used = 0;
    h->maxLoad = maxLoad;

    h->table = malloc(sizeof(HashCell*) * h->size);
    if (!h->table) {
        free(h);
        return NULL;
    }

    for (int i = 0; i < h->size; i++)
        h->table[i] = NULL;

    return h;
}

void destroyHash(Hash hash, freeFunc freeValue, void* extra) {
    if (!hash) return;

    HashImpl* h = (HashImpl*) hash;

    for (int i = 0; i < h->size; i++) {
        HashCell* cell = h->table[i];
        while (cell) {
            HashCell* next = cell->next;

            free(cell->key);
            if (freeValue)
                freeValue(cell->value, extra);

            free(cell);
            cell = next;
        }
    }

    free(h->table);
    free(h);
}

/* ============================================================
   Inserção, remoção e busca
   ============================================================ */

void insertHash(Hash hash, const char* key, HashItem value) {
    if (!hash || !key) return;

    HashImpl* h = (HashImpl*) hash;
    int idx = hashIndex(h, key);

    HashCell* cell = h->table[idx];
    while (cell) {
        if (strcmp(cell->key, key) == 0) {
            cell->value = value;
            return;
        }
        cell = cell->next;
    }

    HashCell* newCell = malloc(sizeof(HashCell));
    if (!newCell) return;

    newCell->key = strdup(key);
    newCell->value = value;
    newCell->next = h->table[idx];
    h->table[idx] = newCell;

    h->used++;

    if ((double)h->used / h->size >= h->maxLoad)
        rehash(h);
}

HashItem getHashValue(Hash hash, const char* key) {
    if (!hash || !key) return NULL;

    HashImpl* h = (HashImpl*) hash;
    int idx = hashIndex(h, key);

    HashCell* cell = h->table[idx];
    while (cell) {
        if (strcmp(cell->key, key) == 0)
            return cell->value;
        cell = cell->next;
    }

    return NULL;
}

HashItem removeHashValue(Hash hash, const char* key) {
    if (!hash || !key) return NULL;

    HashImpl* h = (HashImpl*) hash;
    int idx = hashIndex(h, key);

    HashCell* prev = NULL;
    HashCell* cell = h->table[idx];

    while (cell) {
        if (strcmp(cell->key, key) == 0) {
            if (prev)
                prev->next = cell->next;
            else
                h->table[idx] = cell->next;

            HashItem val = cell->value;
            free(cell->key);
            free(cell);
            h->used--;

            return val;
        }
        prev = cell;
        cell = cell->next;
    }

    return NULL;
}

/* ============================================================
   Depuração
   ============================================================ */

void printHash(Hash hash, printFunc printValue) {
    if (!hash) return;

    HashImpl* h = (HashImpl*) hash;

    for (int i = 0; i < h->size; i++) {
        HashCell* cell = h->table[i];
        if (!cell) continue;

        printf("[%d]: ", i);
        while (cell) {
            printf("(%s)", cell->key);
            if (printValue)
                printf(" -> %s", printValue(cell->value, NULL));
            printf("  ");
            cell = cell->next;
        }
        printf("\n");
    }
}

