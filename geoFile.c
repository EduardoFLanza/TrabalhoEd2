#include "geoFile.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Função: ReadGeoFile
 * Descrição: Lê um arquivo .geo e processa todos os comandos válidos,
 *            criando entidades geométricas e inserindo-as na STreap.
 * Parâmetros: elements – STreap onde as entidades serão inseridas;
 *             geoPath – caminho do arquivo .geo;
 *             defaultStyle – estilo de texto padrão.
 * Retorno: 0 em sucesso, 1 em erro.
 */
int ReadGeoFile(STreap elements, char *geoPath, Style defaultStyle)
{
    ArqCmds GeoFile = abreArquivoCmd(geoPath);
    Style   style   = defaultStyle;

    char buffer[DEFAULT_BUFFER_SIZE];
    char parameter[SIMPLE_PARAMETER_SIZE];

    if (GeoFile == NULL)
    {
        printf("ERRO: [ReadGeoFile]: nao foi possivel abrir arquivo '%s'\n", geoPath);
        return 1;
    }

    while (leLinha(GeoFile, buffer, DEFAULT_BUFFER_SIZE))
    {
        Entity entity = NULL;

        getParametroI(GeoFile, buffer, 0, parameter, SIMPLE_PARAMETER_SIZE);

        if (strcmp(parameter, "ts") == 0)
        {
            readActTextStyle(GeoFile, buffer, style);
        }
        else if (strcmp(parameter, "c") == 0)
        {
            entity = readActCircle(GeoFile, buffer);
            st_insert(elements,
                      getGeoCords(getEntGeo(entity))[0],
                      getGeoCords(getEntGeo(entity))[1],
                      entity);
        }
        else if (strcmp(parameter, "r") == 0)
        {
            entity = readActRectangle(GeoFile, buffer);
            st_insert(elements,
                      getGeoCords(getEntGeo(entity))[0],
                      getGeoCords(getEntGeo(entity))[1],
                      entity);
        }
        else if (strcmp(parameter, "l") == 0)
        {
            entity = readActLine(GeoFile, buffer);
            st_insert(elements,
                      getGeoAnchor_1(getEntGeo(entity))[0],
                      getGeoAnchor_1(getEntGeo(entity))[1],
                      entity);
        }
        else if (strcmp(parameter, "t") == 0)
        {
            entity = readActText(GeoFile, buffer, style);
            st_insert(elements,
                      getGeoCords(getEntGeo(entity))[0],
                      getGeoCords(getEntGeo(entity))[1],
                      entity);
        }
        else
        {
            printf("ERRO: [ReadGeoFile]: comando '%s' nao reconhecido em '%s'\n",
                   parameter, geoPath);
        }
    }

    fechaArquivoCmd(GeoFile);
    return 0;
}

/*
 * Função: readActCircle
 * Descrição: Interpreta comando de criação de círculo.
 * Parâmetros: GeoFile – arquivo de comandos;
 *             lineBuffer – linha do arquivo .geo.
 * Retorno: Entidade criada.
 */
Entity readActCircle(ArqCmds GeoFile, char *lineBuffer)
{
    char attr[SIMPLE_PARAMETER_SIZE];
    char border[SIMPLE_PARAMETER_SIZE];
    char fill[SIMPLE_PARAMETER_SIZE];

    getParametroI(GeoFile, lineBuffer, 1, attr, SIMPLE_PARAMETER_SIZE);
    int id = atoi(attr);
    getParametroI(GeoFile, lineBuffer, 2, attr, SIMPLE_PARAMETER_SIZE);
    double x = atof(attr);
    getParametroI(GeoFile, lineBuffer, 3, attr, SIMPLE_PARAMETER_SIZE);
    double y = atof(attr);
    getParametroI(GeoFile, lineBuffer, 4, attr, SIMPLE_PARAMETER_SIZE);
    double r = atof(attr);
    getParametroI(GeoFile, lineBuffer, 5, border, SIMPLE_PARAMETER_SIZE);
    getParametroI(GeoFile, lineBuffer, 6, fill, SIMPLE_PARAMETER_SIZE);

    Geometry geo = createCircle(id, x, y, r, border, fill);
    return createEntity(geo, id);
}

/*
 * Função: readActRectangle
 * Descrição: Interpreta comando de criação de retângulo.
 * Parâmetros: GeoFile – arquivo de comandos;
 *             lineBuffer – linha do arquivo .geo.
 * Retorno: Entidade criada.
 */
Entity readActRectangle(ArqCmds GeoFile, char *lineBuffer)
{
    char attr[SIMPLE_PARAMETER_SIZE];
    char border[SIMPLE_PARAMETER_SIZE];
    char fill[SIMPLE_PARAMETER_SIZE];

    getParametroI(GeoFile, lineBuffer, 1, attr, SIMPLE_PARAMETER_SIZE);
    int id = atoi(attr);
    getParametroI(GeoFile, lineBuffer, 2, attr, SIMPLE_PARAMETER_SIZE);
    double x = atof(attr);
    getParametroI(GeoFile, lineBuffer, 3, attr, SIMPLE_PARAMETER_SIZE);
    double y = atof(attr);
    getParametroI(GeoFile, lineBuffer, 4, attr, SIMPLE_PARAMETER_SIZE);
    double w = atof(attr);
    getParametroI(GeoFile, lineBuffer, 5, attr, SIMPLE_PARAMETER_SIZE);
    double h = atof(attr);
    getParametroI(GeoFile, lineBuffer, 6, border, SIMPLE_PARAMETER_SIZE);
    getParametroI(GeoFile, lineBuffer, 7, fill, SIMPLE_PARAMETER_SIZE);

    Geometry geo = createRectangle(id, x, y, w, h, border, fill);
    return createEntity(geo, id);
}

/*
 * Função: readActLine
 * Descrição: Interpreta comando de criação de linha.
 * Parâmetros: GeoFile – arquivo de comandos;
 *             lineBuffer – linha do arquivo .geo.
 * Retorno: Entidade criada.
 */
Entity readActLine(ArqCmds GeoFile, char *lineBuffer)
{
    char attr[SIMPLE_PARAMETER_SIZE];
    char color[SIMPLE_PARAMETER_SIZE];

    getParametroI(GeoFile, lineBuffer, 1, attr, SIMPLE_PARAMETER_SIZE);
    int id = atoi(attr);
    getParametroI(GeoFile, lineBuffer, 2, attr, SIMPLE_PARAMETER_SIZE);
    double x1 = atof(attr);
    getParametroI(GeoFile, lineBuffer, 3, attr, SIMPLE_PARAMETER_SIZE);
    double y1 = atof(attr);
    getParametroI(GeoFile, lineBuffer, 4, attr, SIMPLE_PARAMETER_SIZE);
    double x2 = atof(attr);
    getParametroI(GeoFile, lineBuffer, 5, attr, SIMPLE_PARAMETER_SIZE);
    double y2 = atof(attr);
    getParametroI(GeoFile, lineBuffer, 6, color, SIMPLE_PARAMETER_SIZE);

    Geometry geo = createLine(id, x1, y1, x2, y2, color);
    return createEntity(geo, id);
}

/*
 * Função: readActText
 * Descrição: Interpreta comando de criação de texto.
 * Parâmetros: GeoFile – arquivo de comandos;
 *             lineBuffer – linha do arquivo .geo;
 *             style – estilo de texto atual.
 * Retorno: Entidade criada.
 */
Entity readActText(ArqCmds GeoFile, char *lineBuffer, Style style)
{
    char attr[SIMPLE_PARAMETER_SIZE];
    char border[SIMPLE_PARAMETER_SIZE];
    char fill[SIMPLE_PARAMETER_SIZE];
    char text[DEFAULT_BUFFER_SIZE];

    getParametroI(GeoFile, lineBuffer, 1, attr, SIMPLE_PARAMETER_SIZE);
    int id = atoi(attr);
    getParametroI(GeoFile, lineBuffer, 2, attr, SIMPLE_PARAMETER_SIZE);
    double x = atof(attr);
    getParametroI(GeoFile, lineBuffer, 3, attr, SIMPLE_PARAMETER_SIZE);
    double y = atof(attr);
    getParametroI(GeoFile, lineBuffer, 4, border, SIMPLE_PARAMETER_SIZE);
    getParametroI(GeoFile, lineBuffer, 5, fill, SIMPLE_PARAMETER_SIZE);
    getParametroI(GeoFile, lineBuffer, 6, attr, SIMPLE_PARAMETER_SIZE);
    char anchor = attr[0];
    getParametroDepoisI(GeoFile, lineBuffer, 7, text, DEFAULT_BUFFER_SIZE);

    if (text[strlen(text) - 1] == 13)
        text[strlen(text) - 1] = '\0';

    Geometry geo = createText(id, x, y, border, fill, anchor, text);
    setGeoStyle(geo, style);
    return createEntity(geo, id);
}

/*
 * Função: readActTextStyle
 * Descrição: Atualiza o estilo de texto padrão.
 * Parâmetros: GeoFile – arquivo de comandos;
 *             lineBuffer – linha do arquivo .geo;
 *             createdStyle – estilo a ser atualizado.
 * Retorno: Nenhum.
 */
void readActTextStyle(ArqCmds GeoFile, char *lineBuffer, Style createdStyle)
{
    char attr[SIMPLE_PARAMETER_SIZE];

    getParametroI(GeoFile, lineBuffer, 1, attr, SIMPLE_PARAMETER_SIZE);
    setStyleFontFamily(createdStyle, attr);

    getParametroI(GeoFile, lineBuffer, 2, attr, SIMPLE_PARAMETER_SIZE);

    if      (strcmp(attr, "n")  == 0) setStyleFontWeight(createdStyle, "normal");
    else if (strcmp(attr, "b")  == 0) setStyleFontWeight(createdStyle, "bold");
    else if (strcmp(attr, "b+") == 0) setStyleFontWeight(createdStyle, "bolder");
    else if (strcmp(attr, "l")  == 0) setStyleFontWeight(createdStyle, "lighter");
    else
        setStyleFontWeight(createdStyle, attr);

    getParametroI(GeoFile, lineBuffer, 3, attr, SIMPLE_PARAMETER_SIZE);
    setStyleFontSize(createdStyle, atof(attr));
}
