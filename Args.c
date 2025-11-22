#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "args.h"

static char *strCopiar(const char *s) {
    if (!s) return NULL;
    char *c = malloc(strlen(s) + 1);
    if (!c) {
        fprintf(stderr, "Erro de alocação ao copiar string.\n");
        exit(1);
    }
    strcpy(c, s);
    return c;
}

static void erro(const char *msg) {
    fprintf(stderr, "Erro nos parâmetros: %s\n", msg);
    exit(1);
}

Parametros lerParametros(int argc, char **argv) {
    Parametros p;
    p.dirEntrada = NULL;
    p.arquivoGeo = NULL;
    p.arquivoQry = NULL;
    p.dirSaida   = NULL;

    bool usado_e = false;
    bool usado_f = false;
    bool usado_q = false;
    bool usado_o = false;

    for (int i = 1; i < argc; i++) {

        if (!strcmp(argv[i], "-e")) {
            if (usado_e) erro("Parâmetro -e informado duas vezes.");
            if (i+1 >= argc) erro("Faltando caminho após -e.");
            p.dirEntrada = strCopiar(argv[++i]);
            usado_e = true;
            continue;
        }

        if (!strcmp(argv[i], "-f")) {
            if (usado_f) erro("Parâmetro -f informado duas vezes.");
            if (i+1 >= argc) erro("Faltando arquivo após -f.");
            p.arquivoGeo = strCopiar(argv[++i]);
            usado_f = true;
            continue;
        }

        if (!strcmp(argv[i], "-q")) {
            if (usado_q) erro("Parâmetro -q informado duas vezes.");
            if (i+1 >= argc) erro("Faltando arquivo após -q.");
            p.arquivoQry = strCopiar(argv[++i]);
            usado_q = true;
            continue;
        }

        if (!strcmp(argv[i], "-o")) {
            if (usado_o) erro("Parâmetro -o informado duas vezes.");
            if (i+1 >= argc) erro("Faltando diretório após -o.");
            p.dirSaida = strCopiar(argv[++i]);
            usado_o = true;
            continue;
        }

        char buff[128];
        snprintf(buff, sizeof(buff), "Parâmetro desconhecido: %s", argv[i]);
        erro(buff);
    }

    if (!p.arquivoGeo) erro("O parâmetro -f é obrigatório.");
    if (!p.dirSaida)   erro("O parâmetro -o é obrigatório.");

    return p;
}

void liberarParametros(Parametros *p) {
    if (!p) return;
    free(p->dirEntrada);
    free(p->arquivoGeo);
    free(p->arquivoQry);
    free(p->dirSaida);
    p->dirEntrada = p->arquivoGeo = p->arquivoQry = p->dirSaida = NULL;
}
