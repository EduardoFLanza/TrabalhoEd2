#include "path.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * Função: normalizePath
 * Descrição: Remove o separador '/' final de um caminho, se existir.
 * Parâmetros: path – caminho original.
 *             normPath – vetor para armazenar o caminho normalizado.
 *             lenNormPath – tamanho máximo de normPath.
 * Retorno: Nenhum.
 */
void normalizePath(char *path, char *normPath, int lenNormPath)
{
    int pathlen = strlen(path);

    strcpy(normPath, path);
    if (pathlen > 0 && path[pathlen - 1] == '/')
        normPath[pathlen - 1] = '\0';
}

/*
 * Função: substituirUltimaOcorrencia
 * Descrição: Localiza a última instância de um caractere específico em uma string 
 * e a substitui por outro caractere.
 * Parâmetros:  str – a string a ser modificada (deve ser mutável).
 *              antigo – o caractere que se deseja procurar.
 *              novo – o caractere que substituirá a última ocorrência encontrada.
 * Retorno: Inteiro (1 se a substituição foi realizada, 0 caso o caractere 
 * não tenha sido encontrado).
 */
int substituteLastOccourence(char *path, char prev, char new){
    if (path == NULL) return 0;

    char *previous = strrchr(path, prev);

    if (previous != NULL) {
        *previous = new;
        return 1;
    }

    return 0;
}

/*
 * Função: getPath
 * Descrição: Extrai o diretório de um caminho completo.
 * Parâmetros: fullPath – caminho completo do arquivo.
 *             path – vetor para armazenar o diretório.
 *             lenPath – tamanho máximo de path.
 * Retorno: Nenhum.
 */
void getPath(char *fullPath, char *path, int lenPath)
{
    int i = strlen(fullPath) - 1;
    char *buffer = malloc(strlen(fullPath) + 1);

    strcpy(buffer, fullPath);

    while (i >= 0 && fullPath[i] != '/')
        i--;

    if (i >= 0)
    {
        buffer[i] = '\0';
        strcpy(path, buffer);
    }
    else
        strcpy(path, "");

    free(buffer);
}

/*
 * Função: getFileName
 * Descrição: Extrai o nome do arquivo com extensão.
 * Parâmetros: fullPath – caminho completo do arquivo.
 *             fileName – vetor para armazenar o nome do arquivo.
 *             lenFileName – tamanho máximo de fileName.
 * Retorno: Nenhum.
 */
void getFileName(char *fullPath, char *fileName, int lenFileName)
{
    int i = strlen(fullPath) - 1;

    while (i >= 0 && fullPath[i] != '/')
        i--;

    if (i >= 0)
        strcpy(fileName, fullPath + i + 1);
    else
        strcpy(fileName, fullPath);
}

/*
 * Função: joinAll
 * Descrição: Constrói um caminho completo a partir de path, nome e extensão.
 * Parâmetros: path – diretório.
 *             fileName – nome do arquivo.
 *             ext – extensão do arquivo.
 *             fullPath – vetor para armazenar o caminho completo.
 *             lenFullPath – tamanho máximo de fullPath.
 * Retorno: Nenhum.
 */
void joinAll(char *path, char *fileName, char *ext, char *fullPath, int lenFullPath)
{
    char buffer[strlen(path) + 1];

    normalizePath(path, buffer, sizeof(buffer));
    fullPath[0] = '\0';

    if (strcmp(buffer, "") != 0)
    {
        strcat(fullPath, buffer);
        strcat(fullPath, "/");
    }

    strcat(fullPath, fileName);
    strcat(fullPath, ext);
}

/*
 * Função: joinFilePath
 * Descrição: Constrói um caminho completo a partir de path e nome do arquivo.
 * Parâmetros: path – diretório.
 *             fileName – nome do arquivo.
 *             fullPath – vetor para armazenar o caminho completo.
 *             lenFullPath – tamanho máximo de fullPath.
 * Retorno: Nenhum.
 */
void joinFilePath(char *path, char *fileName, char *fullPath, int lenFullPath)
{
    char buffer[strlen(path) + 1];

    normalizePath(path, buffer, sizeof(buffer));
    fullPath[0] = '\0';

    if (strcmp(buffer, "") != 0)
    {
        strcat(fullPath, buffer);
        strcat(fullPath, "/");
    }

    strcat(fullPath, fileName);
}

/*
 * Função: splitPath
 * Descrição: Separa um caminho completo em path, nome e extensão.
 * Parâmetros: fullPath – caminho completo.
 *             path – vetor para armazenar o diretório.
 *             lenPath – tamanho máximo de path.
 *             nomeArq – vetor para armazenar o nome do arquivo.
 *             lenNomeArq – tamanho máximo de nomeArq.
 *             extArq – vetor para armazenar a extensão.
 *             lenExtArq – tamanho máximo de extArq.
 * Retorno: Nenhum.
 */
void splitPath(char *fullPath,
               char *path, int lenPath,
               char *nomeArq, int lenNomeArq,
               char *extArq, int lenExtArq)
{
    int i, k;
    char buffer[strlen(fullPath) + 1];

    strcpy(buffer, fullPath);

    i = strlen(fullPath) - 1;
    while (i >= 0 && fullPath[i] != '.')
        i--;

    if (i >= 0)
    {
        strcpy(extArq, fullPath + i);
        buffer[i] = '\0';

        k = strlen(buffer) - 1;
        while (k >= 0 && buffer[k] != '/')
            k--;

        if (k >= 0)
        {
            strcpy(nomeArq, buffer + k + 1);
            buffer[k] = '\0';
            strcpy(path, buffer);
        }
        else
        {
            strcpy(nomeArq, buffer);
            strcpy(path, "");
        }
    }
    else
    {
        strcpy(path, "");
        strcpy(nomeArq, buffer);
        strcpy(extArq, "");
    }
}

/*
 * Função: getExt
 * Descrição: Extrai a extensão de um arquivo.
 * Parâmetros: fullPath – caminho completo do arquivo.
 *             ext – vetor para armazenar a extensão.
 *             lenExt – tamanho máximo de ext.
 * Retorno: Nenhum.
 */
void getExt(char *fullPath, char *ext, int lenExt)
{
    int i = strlen(fullPath) - 1;

    while (i >= 0 && fullPath[i] != '.')
        i--;

    if (i >= 0)
        strcpy(ext, fullPath + i);
    else
        strcpy(ext, "");
}

/*
 * Função: getFileNameWithoutExt
 * Descrição: Extrai o nome do arquivo sem a extensão.
 * Parâmetros: fullPath – caminho completo do arquivo.
 *             fileName – vetor para armazenar o nome do arquivo.
 *             lenFileName – tamanho máximo de fileName.
 * Retorno: Nenhum.
 */
void getFileNameWithoutExt(char *fullPath, char *fileName, int lenFileName)
{
    int i = strlen(fullPath) - 1;

    while (i >= 0 && fullPath[i] != '.')
        i--;

    if (i >= 0)
    {
        strncpy(fileName, fullPath, i);
        fileName[i] = '\0';
    }
    else
        strcpy(fileName, fullPath);
}
