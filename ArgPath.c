
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "path.h"

/*
 * normalizarCaminho()
 * -------------------
 * Garante que a string resultante (caminhoNormalizado) seja uma cópia de caminhoOriginal,
 * mas sem uma barra '/' no final, caso exista.
 */
void normalizarCaminho(char *caminhoOriginal, char *caminhoNormalizado, int tamCaminhoNormalizado)
{
    int tamCaminho = strlen(caminhoOriginal);

    strcpy(caminhoNormalizado, caminhoOriginal);
    if ((tamCaminho - 1) >= 0 && caminhoOriginal[tamCaminho - 1] == '/')
        caminhoNormalizado[tamCaminho - 1] = '\0';
}


/*
 * obterDiretorio()
 * ----------------
 * Extrai apenas o diretório de um caminho completo, removendo o nome do arquivo.
 * Exemplo: "/usr/local/bin/teste.txt" → "/usr/local/bin"
 */
void obterDiretorio(char *caminhoCompleto, char *diretorio, int tamDiretorio)
{
    int i = strlen(caminhoCompleto) - 1;
    int tamTotal = strlen(caminhoCompleto) + 1;
    char *bufferTemp = malloc(sizeof(char) * tamTotal);
    
    strcpy(bufferTemp, caminhoCompleto);

    // Procura a última barra '/'
    while (i >= 0 && caminhoCompleto[i] != '/')
        i--;
    if (i >= 0)
    {
        bufferTemp[i] = '\0';
        strcpy(diretorio, bufferTemp);
    }
    else
        strcpy(diretorio, "");

    free(bufferTemp);
}


/*
 * obterNomeArquivo()
 * ------------------
 * Obtém o nome do arquivo a partir de um caminho completo.
 * Exemplo: "/usr/local/teste.txt" → "teste.txt"
 */
void obterNomeArquivo(char *caminhoCompleto, char *nomeArquivo, int tamNomeArquivo)
{
    int i = strlen(caminhoCompleto) - 1;
    int tamTotal = strlen(caminhoCompleto) + 1;
    char *bufferTemp = malloc(sizeof(char) * tamTotal);
    
    strcpy(bufferTemp, caminhoCompleto);

    // Encontra a última barra '/'
    while (i >= 0 && caminhoCompleto[i] != '/')
        i--;
    if (i >= 0)
        strcpy(nomeArquivo, bufferTemp + i + 1);
    else
        strcpy(nomeArquivo, caminhoCompleto);
    
    free(bufferTemp);
}


/*
 * juntarCaminhoCompleto()
 * -----------------------
 * Junta diretório, nome do arquivo e extensão em um caminho completo.
 * Exemplo: ("usr/local", "teste", ".txt") → "usr/local/teste.txt"
 */
void juntarCaminhoCompleto(char *diretorio, char *nomeArquivo, char *extensao, char *caminhoCompleto, int tamCaminhoCompleto)
{
    int tamDir = strlen(diretorio) + 1;
    char *bufferTemp = malloc(sizeof(char) * tamDir);

    normalizarCaminho(diretorio, bufferTemp, tamDir);

    strcat(caminhoCompleto, bufferTemp);
    if (strcmp(diretorio, "") != 0)
        strcat(caminhoCompleto, "/");
    strcat(caminhoCompleto, nomeArquivo);
    strcat(caminhoCompleto, extensao);

    free(bufferTemp);
}


/*
 * juntarDiretorioArquivo()
 * ------------------------
 * Junta apenas o diretório e o nome do arquivo.
 * Exemplo: ("usr/local", "teste.txt") → "usr/local/teste.txt"
 */
void juntarDiretorioArquivo(char *diretorio, char *nomeArquivo, char *caminhoCompleto, int tamCaminhoCompleto)
{
    int tamDir = strlen(diretorio) + 1;
    char *bufferTemp = malloc(sizeof(char) * tamDir);

    normalizarCaminho(diretorio, bufferTemp, tamDir);

    strcat(caminhoCompleto, bufferTemp);
    if (strcmp(diretorio, "") != 0)
        strcat(caminhoCompleto, "/");
    strcat(caminhoCompleto, nomeArquivo);

    free(bufferTemp);
}


/*
 * dividirCaminho()
 * ----------------
 * Divide um caminho completo em diretório, nome do arquivo e extensão.
 * Exemplo: "/usr/local/teste.txt" → dir="/usr/local", nome="teste", ext=".txt"
 */
void dividirCaminho(char *caminhoCompleto,
                    char *diretorio, int tamDiretorio,
                    char *nomeArquivo, int tamNomeArquivo,
                    char *extensao, int tamExtensao)
{
    int i, k;
    int tamTotal = strlen(caminhoCompleto) + 1;
    char *bufferTemp = malloc(sizeof(char) * tamTotal);
    
    strcpy(bufferTemp, caminhoCompleto);

    // Localiza a última ocorrência de '.'
    i = strlen(caminhoCompleto) - 1;
    while (i >= 0 && caminhoCompleto[i] != '.')
        i--;
    if (i >= 0)
    {
        strcpy(extensao, caminhoCompleto + i);
        bufferTemp[i] = '\0';

        // Localiza a última barra '/'
        k = strlen(caminhoCompleto) - 1;
        while (k >= 0 && caminhoCompleto[k] != '/')
            k--;
        if (k >= 0)
        {
            strcpy(nomeArquivo, bufferTemp + k + 1);
            bufferTemp[k] = '\0';
            strcpy(diretorio, bufferTemp);
        }
        else
        {
            strcpy(nomeArquivo, bufferTemp);
            strcpy(diretorio, "");
        }
    }
    else
    {
        normalizarCaminho(caminhoCompleto, diretorio, tamDiretorio);
        strcpy(nomeArquivo, "");
        strcpy(extensao, "");
    }

    free(bufferTemp);
}


/*
 * obterExtensaoArquivo()
 * ----------------------
 * Obtém apenas a extensão de um arquivo.
 * Exemplo: "teste.txt" → ".txt"
 */
void obterExtensaoArquivo(char *caminhoCompleto, char *extensao, int tamExtensao)
{
    int i = strlen(caminhoCompleto) - 1;
    while (i >= 0 && caminhoCompleto[i] != '.')
        i--;
    if (i >= 0)
        strcpy(extensao, caminhoCompleto + i);
    else
        strcpy(extensao, "");
}


/*
 * obterNomeArquivoSemExtensao()
 * -----------------------------
 * Retorna o nome do arquivo sem a extensão.
 * Exemplo: "teste.txt" → "teste"
 */
void obterNomeArquivoSemExtensao(char *caminhoCompleto, char *nomeArquivo, int tamNomeArquivo)
{
    int i = strlen(caminhoCompleto) - 1;
    while (i >= 0 && caminhoCompleto[i] != '.')
        i--;
    if (i >= 0)
    {
        strncpy(nomeArquivo, caminhoCompleto, i);
        nomeArquivo[i] = '\0';
    }
    else
        strcpy(nomeArquivo, caminhoCompleto);
}
