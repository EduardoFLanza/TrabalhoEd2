/**
 * Módulo: args
 * Finalidade:
 *   Este módulo realiza a interpretação, validação e armazenamento dos
 *   parâmetros fornecidos pela linha de comando do programa "ted".
 *
 * Descrição Geral:
 *   O programa aceita as opções:
 *      -e <path>   Diretório de entrada (opcional)
 *      -f <arquivo.geo>   Arquivo .geo (obrigatório)
 *      -q <arquivo.qry>   Arquivo .qry (opcional)
 *      -o <dir>    Diretório de saída (obrigatório)
 *
 *   O módulo utiliza um tipo opaco (Parametros), cuja estrutura interna
 *   é ocultada do utilizador e definida apenas neste .c.
 *
 * Observações:
 *   - Em caso de erro, mensagens claras são exibidas e o programa encerra.
 *   - As funções de acesso retornam ponteiros internos; estes **não devem**
 *     ser liberados diretamente pelo utilizador.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "args.h"

/* ------------------------------------------------------------------------- */
/* Estrutura interna do módulo (oculta do .h)                                */
/* ------------------------------------------------------------------------- */
typedef struct {
    char *dirEntrada;   /* Pode ser NULL caso -e não seja fornecido */
    char *arquivoGeo;   /* Obrigatório (-f) */
    char *arquivoQry;   /* Opcional (-q) */
    char *dirSaida;     /* Obrigatório (-o) */
} Params;

/* ------------------------------------------------------------------------- */
/* Função auxiliar: copiar string com alocação dinâmica                      */
/* ------------------------------------------------------------------------- */
/*
 * Função: strCopiar
 * Descrição:
 *   Copia uma string para uma nova região de memória alocada dinamicamente.
 *
 * Requisitos:
 *   - s pode ser NULL; nesse caso, retorna NULL.
 *
 * Retorno:
 *   - Nova string alocada; deve ser liberada posteriormente pelo módulo.
 */
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

/* ------------------------------------------------------------------------- */
/* Função auxiliar: imprimir erro e encerrar                                 */
/* ------------------------------------------------------------------------- */
/*
 * Função: erro
 * Descrição:
 *   Exibe mensagem de erro referente aos parâmetros e encerra execução.
 *
 * Requisitos:
 *   - msg deve ser uma string válida.
 *
 * Saída:
 *   - Nunca retorna; finaliza o programa com status de erro.
 */
static void erro(const char *msg) {
    fprintf(stderr, "Erro nos parâmetros: %s\n", msg);
    exit(1);
}

/* ------------------------------------------------------------------------- */
/* Função: lerParametros                                                     */
/* ------------------------------------------------------------------------- */
/*
 * Função: lerParametros
 * Descrição:
 *   Analisa a linha de comando, interpreta as opções e cria a estrutura
 *   Parametros contendo todos os caminhos informados.
 *
 * Requisitos:
 *   - argc e argv devem ser válidos.
 *   - As opções obrigatórias -f e -o devem ser fornecidas exatamente uma vez.
 *
 * Retorno:
 *   - Instância opaca Parametros preenchida corretamente.
 */
Parametros lerParametros(int argc, char **argv) {
    Params *p = calloc(1, sizeof(Params));
    if (!p) erro("Memória insuficiente ao criar estrutura.");

    bool usado_e = false;
    bool usado_f = false;
    bool usado_q = false;
    bool usado_o = false;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-e") == 0) {
            if (usado_e) erro("Parâmetro -e informado mais de uma vez.");
            if (i + 1 >= argc) erro("Faltando caminho após -e.");
            p->dirEntrada = strCopiar(argv[++i]);
            usado_e = true;
            continue;
        }

        if (strcmp(argv[i], "-f") == 0) {
            if (usado_f) erro("Parâmetro -f informado mais de uma vez.");
            if (i + 1 >= argc) erro("Faltando arquivo .geo após -f.");
            p->arquivoGeo = strCopiar(argv[++i]);
            usado_f = true;
            continue;
        }

        if (strcmp(argv[i], "-q") == 0) {
            if (usado_q) erro("Parâmetro -q informado mais de uma vez.");
            if (i + 1 >= argc) erro("Faltando arquivo .qry após -q.");
            p->arquivoQry = strCopiar(argv[++i]);
            usado_q = true;
            continue;
        }

        if (strcmp(argv[i], "-o") == 0) {
            if (usado_o) erro("Parâmetro -o informado mais de uma vez.");
            if (i + 1 >= argc) erro("Faltando diretório após -o.");
            p->dirSaida = strCopiar(argv[++i]);
            usado_o = true;
            continue;
        }

        char buff[256];
        snprintf(buff, sizeof(buff), "Parâmetro desconhecido: %s", argv[i]);
        erro(buff);
    }

    /* Validações obrigatórias */
    if (!p->arquivoGeo) erro("O parâmetro obrigatório -f não foi informado.");
    if (!p->dirSaida)  erro("O parâmetro obrigatório -o não foi informado.");

    return (Parametros)p;
}

/* ------------------------------------------------------------------------- */
/* Função: liberarParametros                                                 */
/* ------------------------------------------------------------------------- */
/*
 * Função: liberarParametros
 * Descrição:
 *   Libera toda a memória associada a uma instância de Parametros.
 *
 * Requisitos:
 *   - pp deve apontar para uma instância previamente criada por lerParametros.
 *
 * Saída:
 *   - Após a liberação, *pp é definido como NULL.
 */
void liberarParametros(Parametros *pp) {
    if (!pp || !*pp) return;

    Params *p = (Params*)(*pp);
    free(p->dirEntrada);
    free(p->arquivoGeo);
    free(p->arquivoQry);
    free(p->dirSaida);
    free(p);

    *pp = NULL;
}

/* ------------------------------------------------------------------------- */
/* Funções de acesso                                                         */
/* ------------------------------------------------------------------------- */

/*
 * Função: args_getEntrada
 * Descrição: Retorna o diretório de entrada (-e), caso fornecido.
 * Retorno: String interna que NÃO deve ser liberada pelo usuário.
 */
char* args_getEntrada(Parametros p) {
    return p ? ((Params*)p)->dirEntrada : NULL;
}

/*
 * Função: args_getGeo
 * Descrição: Retorna o arquivo .geo especificado na opção obrigatória -f.
 * Retorno: String interna não liberável pelo usuário.
 */
char* args_getGeo(Parametros p) {
    return p ? ((Params*)p)->arquivoGeo : NULL;
}

/*
 * Função: args_getQry
 * Descrição: Retorna o arquivo .qry passado pela opção -q.
 * Retorno: Pode ser NULL se o usuário não forneceu -q.
 */
char* args_getQry(Parametros p) {
    return p ? ((Params*)p)->arquivoQry : NULL;
}

/*
 * Função: args_getOut
 * Descrição: Retorna o diretório de saída fornecido pela opção obrigatória -o.
 * Retorno: String interna não liberável pelo usuário.
 */
char* args_getOut(Parametros p) {
    return p ? ((Params*)p)->dirSaida : NULL;
}

