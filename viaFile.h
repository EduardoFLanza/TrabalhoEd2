/**
 * Módulo: viaFile
 * Finalidade:
 *   Realizar o processamento das definições de vias presentes nos arquivos de entrada.
 *   Este módulo interpreta comandos relacionados à criação de segmentos viários,
 *   inserção em grafos, indexação espacial e registro gráfico.
 *
 * Descrição geral:
 *   A função principal `via_processar` recebe um arquivo aberto contendo comandos
 *   referentes às vias, além das estruturas principais do programa. Ela deve
 *   interpretar cada comando e atualizar o grafo de vias e demais estruturas.
 *
 * Observação:
 *   Este header define apenas a interface pública do módulo.
 */

#ifndef VIAFILE_H
#define VIAFILE_H

#include <stdio.h>
#include "digraph.h"
#include "picture.h"
#include "hash_table.h"
#include "boundingbox.h"

/**
 * Função: via_processar
 * ---------------------
 * Lê o arquivo contendo especificações de vias e insere as informações nas
 * estruturas fornecidas.
 *
 * Parâmetros:
 *   f        — Ponteiro para o arquivo de entrada.
 *   g        — Grafo de vias onde as conexões serão inseridas.
 *   p        — Estrutura de imagem usada para registrar elementos gráficos.
 *   bb       — Estrutura de bounding boxes para indexação espacial.
 *   quadras  — Tabela hash contendo quadras cadastradas.
 *
 * Retorno:
 *   void — Nenhum valor retornado.
 */
void via_processar(FILE* f, Digraph g, Picture p, BBox bb, HashTable quadras);

#endif

