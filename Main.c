#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "args.h"
#include "picture.h"
#include "boundingbox.h"
#include "digraph.h"
#include "hash_table.h"
#include "geoFile.h"
#include "viaFile.h"
#include "qryFile.h"
#include "writer.h"

int main(int argc, char** argv) {

    /* ----------------------------------------------------------
       1. Leitura e validação dos parâmetros de linha de comando
       ---------------------------------------------------------- */
    Parametros params = lerParametros(argc, argv);

    char *dirE = args_getEntrada(params);
    char *fileF = args_getGeo(params);
    char *fileQ = args_getQry(params);
    char *dirO  = args_getOut(params);

    /* ----------------------------------------------------------
       2. Criação das principais estruturas de dados do sistema
       ---------------------------------------------------------- */
    Picture pic = picture_create();
    BBox bb     = bb_create();
    Digraph g   = dg_create();
    HashTable quadras = ht_create(1024);

    /* ----------------------------------------------------------
       3. Processamento do arquivo .geo
       ---------------------------------------------------------- */
    char path_geo[1024];
    if (dirE)
        snprintf(path_geo, sizeof(path_geo), "%s/%s", dirE, fileF);
    else
        snprintf(path_geo, sizeof(path_geo), "%s", fileF);

    FILE *fgeo = fopen(path_geo, "r");
    if (!fgeo) {
        fprintf(stderr, "Erro ao abrir arquivo .geo: %s\n", path_geo);
        liberarParametros(&params);
        return 1;
    }

    geo_processar(fgeo, pic, bb, quadras);
    fclose(fgeo);

    /* ----------------------------------------------------------
       4. Processamento opcional do arquivo .qry
       ---------------------------------------------------------- */
    if (fileQ != NULL) {
        char path_qry[1024];

        if (dirE)
            snprintf(path_qry, sizeof(path_qry), "%s/%s", dirE, fileQ);
        else
            snprintf(path_qry, sizeof(path_qry), "%s", fileQ);

        FILE *fq = fopen(path_qry, "r");
        if (fq != NULL) {
            qry_processar(fq, pic, bb, g, quadras, dirO);
            fclose(fq);
        } else {
            fprintf(stderr, "Aviso: não foi possível abrir o arquivo .qry: %s\n",
                    path_qry);
        }
    }

    /* ----------------------------------------------------------
       5. Geração dos arquivos de saída (SVG / TXT)
       ---------------------------------------------------------- */
    char base[256];
    strncpy(base, fileF, sizeof(base));
    char *dot = strrchr(base, '.');
    if (dot) *dot = '\0';

    char svg_geo[1024];
    char svg_qry[1024];
    char txt_qry[1024];

    snprintf(svg_geo, sizeof(svg_geo), "%s/%s.svg",   dirO, base);
    snprintf(svg_qry, sizeof(svg_qry), "%s/%s-q.svg", dirO, base);
    snprintf(txt_qry, sizeof(txt_qry), "%s/%s-q.txt", dirO, base);

    writer_writeSVG(svg_geo, pic);
    writer_writeSVG(svg_qry, pic);
    writer_writeTXT(txt_qry, ""); /* Conteúdo real será gerado pelo QRY */

    /* ----------------------------------------------------------
       6. Liberação e encerramento
       ---------------------------------------------------------- */
    bb_destroy(bb, (void (*)(SInfo)) entity_destroy);
    picture_destroy(pic);
    dg_destroy(g, NULL, NULL);
    ht_destroy(quadras);
    liberarParametros(&params);

    return 0;
}
