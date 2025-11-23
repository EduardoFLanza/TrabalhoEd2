/**
 * Módulo: geoFile
 * Finalidade:
 *     Disponibilizar a função responsável por interpretar arquivos .geo
 *     e criar as entidades correspondentes.
 */

#ifndef GEOFILE_H
#define GEOFILE_H

#include <stdio.h>
#include "picture.h"
#include "boundingbox.h"
#include "hash_table.h"

/*
 * Função: geo_processar
 * Descrição:
 *     Lê e interpreta comandos do arquivo .geo, produzindo as entidades
 *     gráficas correspondentes e registrando quadras.
 *
 * Parâmetros:
 *     f        – Arquivo .geo aberto.
 *     p        – Estrutura onde as entidades serão adicionadas.
 *     bb       – Estrutura usada para indexação espacial.
 *     quadras  – Tabela hash com quadras cadastradas.
 *
 * Retorno:
 *     Não possui retorno.
 */
void geo_processar(FILE* f, Picture p, BBox bb, HashTable quadras);

#endif
