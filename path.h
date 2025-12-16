#ifndef PATH_H
#define PATH_H

/*
 * Módulo: path
 *
 * Descrição:
 *     Este módulo provê funções para manipulação de caminhos de arquivos.
 *     As operações permitem separar um caminho completo em seus componentes
 *     (diretório, nome e extensão) e também construir caminhos completos
 *     a partir desses componentes.
 *
 *     Um caminho completo (fullPath) compreende uma sequência de subdiretórios,
 *     o nome do arquivo e sua extensão.
 *
 *     O path é a subcadeia à esquerda do último separador '/',
 *     incluindo-o quando existir.
 *
 *     A extensão é a subcadeia à direita do último separador '.',
 *     incluindo-o quando existir.
 *
 *     O nome do arquivo é a subcadeia entre o último '/' e o último '.'.
 *
 * Abstração:
 *     Este módulo não cria tipos abstratos.
 *     Opera diretamente sobre strings (vetores de caracteres).
 *
 * Importante:
 *     Todos os resultados são escritos em vetores pré-alocados.
 *     Cada vetor possui um parâmetro associado que indica seu tamanho máximo.
 */

/*
 * Função: splitPath
 * Descrição: Separa um caminho completo em path, nome do arquivo e extensão.
 * Parâmetros: fullPath – caminho completo do arquivo.
 *             path – vetor para armazenar o diretório.
 *             lenPath – tamanho máximo de path.
 *             nomeArq – vetor para armazenar o nome do arquivo.
 *             lenNomeArq – tamanho máximo de nomeArq.
 *             extArq – vetor para armazenar a extensão do arquivo.
 *             lenExtArq – tamanho máximo de extArq.
 * Retorno: Nenhum.
 */
void splitPath(char *fullPath,
               char *path, int lenPath,
               char *nomeArq, int lenNomeArq,
               char *extArq, int lenExtArq);

/*
 * Função: joinFilePath
 * Descrição: Constrói um caminho completo a partir do path e do nome do arquivo.
 * Parâmetros: path – diretório do arquivo.
 *             fileName – nome do arquivo, com ou sem extensão.
 *             fullPath – vetor onde será armazenado o caminho completo.
 *             lenFullPath – tamanho máximo de fullPath.
 * Retorno: Nenhum.
 */
void joinFilePath(char *path, char *fileName, char *fullPath, int lenFullPath);

/*
 * Função: joinAll
 * Descrição: Constrói um caminho completo a partir do path, nome e extensão.
 * Parâmetros: path – diretório do arquivo.
 *             fileName – nome do arquivo.
 *             ext – extensão do arquivo.
 *             fullPath – vetor onde será armazenado o caminho completo.
 *             lenFullPath – tamanho máximo de fullPath.
 * Retorno: Nenhum.
 */
void joinAll(char *path, char *fileName, char *ext, char *fullPath, int lenFullPath);

/*
 * Função: getFileName
 * Descrição: Extrai o nome do arquivo (com extensão) de um caminho completo.
 * Parâmetros: fullPath – caminho completo do arquivo.
 *             fileName – vetor onde será armazenado o nome do arquivo.
 *             lenFileName – tamanho máximo de fileName.
 * Retorno: Nenhum.
 */
void getFileName(char *fullPath, char *fileName, int lenFileName);

/*
 * Função: getPath
 * Descrição: Extrai o diretório de um caminho completo.
 * Parâmetros: fullPath – caminho completo do arquivo.
 *             path – vetor onde será armazenado o diretório.
 *             lenPath – tamanho máximo de path.
 * Retorno: Nenhum.
 */
void getPath(char *fullPath, char *path, int lenPath);

/*
 * Função: normalizePath
 * Descrição: Remove o separador '/' final de um caminho, se existir.
 * Parâmetros: path – caminho original.
 *             normPath – vetor onde será armazenado o caminho normalizado.
 *             lenNormPath – tamanho máximo de normPath.
 * Retorno: Nenhum.
 */
void normalizePath(char *path, char *normPath, int lenNormPath);

/*
 * Função: getExt
 * Descrição: Extrai a extensão do arquivo de um caminho completo.
 * Parâmetros: fullPath – caminho completo do arquivo.
 *             ext – vetor onde será armazenada a extensão.
 *             lenExt – tamanho máximo de ext.
 * Retorno: Nenhum.
 */
void getExt(char *fullPath, char *ext, int lenExt);

/*
 * Função: getFileNameWithoutExt
 * Descrição: Extrai o nome do arquivo sem a extensão.
 * Parâmetros: fullPath – caminho completo do arquivo.
 *             fileName – vetor onde será armazenado o nome do arquivo.
 *             lenFileName – tamanho máximo de fileName.
 * Retorno: Nenhum.
 */
void getFileNameWithoutExt(char *fullPath, char *fileName, int lenFileName);

#endif
