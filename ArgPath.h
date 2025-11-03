#ifndef PATH_H
#define PATH_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//
// ============================================================
//  Módulo: Manipulação de caminhos e nomes de arquivos
//  Descrição:
//  Funções utilitárias para lidar com diretórios, nomes e
//  extensões de arquivos — leitura, junção e extração.
// ============================================================
//

/*
 * Remove a barra final de um caminho, se houver.
 * Exemplo: "/usr/local/" → "/usr/local"
 */
void normalizarCaminho(char *caminhoOriginal, char *caminhoNormalizado, int tamCaminhoNormalizado);

/*
 * Extrai apenas o diretório (sem o nome do arquivo) de um caminho completo.
 * Exemplo: "/usr/local/teste.txt" → "/usr/local"
 */
void obterDiretorio(char *caminhoCompleto, char *diretorio, int tamDiretorio);

/*
 * Extrai apenas o nome do arquivo (com extensão) de um caminho completo.
 * Exemplo: "/usr/local/teste.txt" → "teste.txt"
 */
void obterNomeArquivo(char *caminhoCompleto, char *nomeArquivo, int tamNomeArquivo);

/*
 * Junta diretório, nome do arquivo e extensão para formar um caminho completo.
 * Exemplo: ("usr/local", "teste", ".txt") → "usr/local/teste.txt"
 */
void juntarCaminhoCompleto(char *diretorio, char *nomeArquivo, char *extensao, char *caminhoCompleto, int tamCaminhoCompleto);

/*
 * Junta diretório e nome do arquivo (sem extensão).
 * Exemplo: ("usr/local", "teste.txt") → "usr/local/teste.txt"
 */
void juntarDiretorioArquivo(char *diretorio, char *nomeArquivo, char *caminhoCompleto, int tamCaminhoCompleto);

/*
 * Divide um caminho completo em diretório, nome de arquivo e extensão.
 * Exemplo: "/usr/local/teste.txt" → dir="/usr/local", nome="teste", ext=".txt"
 */
void dividirCaminho(char *caminhoCompleto,
                    char *diretorio, int tamDiretorio,
                    char *nomeArquivo, int tamNomeArquivo,
                    char *extensao, int tamExtensao);

/*
 * Extrai apenas a extensão do arquivo.
 * Exemplo: "teste.txt" → ".txt"
 */
void obterExtensaoArquivo(char *caminhoCompleto, char *extensao, int tamExtensao);

/*
 * Extrai o nome do arquivo sem a extensão.
 * Exemplo: "teste.txt" → "teste"
 */
void obterNomeArquivoSemExtensao(char *caminhoCompleto, char *nomeArquivo, int tamNomeArquivo);

#endif // PATH_H


