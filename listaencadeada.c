#include "listaencadeada.h"
#include <stdlib.h>
#include <stdio.h>

Lista* criaLista() {
    Lista* lista = (Lista*) malloc(sizeof(Lista));
    if (!lista) return NULL;
    lista->inicio = NULL;
    lista->fim = NULL;
    lista->tamanho = 0;
    return lista;
}

int inserirFim(Lista* lista, int v) {
    if (!lista) return -1;
    Celula* nova = (Celula*) malloc(sizeof(Celula));
    if (!nova) return -1;
    nova->valor = v;
    nova->prox = NULL;

    if (lista->inicio == NULL) {
        lista->inicio = nova;
        lista->fim = nova;
    } else {
        lista->fim->prox = nova;
        lista->fim = nova;
    }

    lista->tamanho++;
    return 0;
}

int removerInicio(Lista* lista) {
    if (!lista || !lista->inicio) return -1;

    Celula* temp = lista->inicio;
    int v = temp->valor;

    lista->inicio = lista->inicio->prox;
    if (lista->inicio == NULL) {
        lista->fim = NULL;
    }
    free(temp);
    lista->tamanho--;

    return v;
}

void imprimeLista(Lista* lista) {
    if (!lista) return;
    Celula* aux = lista->inicio;
    while (aux != NULL) {
        printf("%d -> ", aux->valor);
        aux = aux->prox;
    }
    printf("NULL\n");
}

void inverterLista(Lista* lista) {
    if (!lista || !lista->inicio) return;

    Celula* prev = NULL;
    Celula* curr = lista->inicio;
    Celula* next = NULL;

    lista->fim = lista->inicio;

    while (curr != NULL) {
        next = curr->prox;
        curr->prox = prev;
        prev = curr;
        curr = next;
    }

    lista->inicio = prev;
}

void liberaLista(Lista* lista) {
    if (!lista) return;
    Celula* aux = lista->inicio;
    while (aux != NULL) {
        Celula* tmp = aux;
        aux = aux->prox;
        free(tmp);
    }
    free(lista);
}
