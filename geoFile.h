#ifndef GEO_FILE_H
#define GEO_FILE_H

#include "streap.h"
#include "geometry.h"
#include "cmdsFiles.h"

/*
 Módulo que provê as funções essenciais para leitura e interpretação de
 arquivos .geo, conforme definido no Trabalho II e na descrição geral
 do projeto.

 Um arquivo .geo pode conter comandos para criação de:
   - círculos (c)
   - retângulos (r)
   - linhas (l)
   - textos (t)
   - estilos de texto (ts)

 Cada função interpreta uma linha completa válida do arquivo .geo e
 executa as ações correspondentes, criando entidades geométricas
 (Geometry) e inserindo-as na STreap.

 Para estilos de texto, o novo estilo lido substitui o estilo padrão
 previamente definido.
*/

/* Lê um arquivo .geo e processa todos os seus comandos */
int ReadGeoFile(STreap elements, char *geoPath, Style defaultStyle);

/* Interpreta comando de criação de círculo */
Geometry readActCircle(ArqCmds geoFile, char *lineBuffer);

/* Interpreta comando de criação de retângulo */
Geometry readActRectangle(ArqCmds geoFile, char *lineBuffer);

/* Interpreta comando de criação de linha */
Geometry readActLine(ArqCmds geoFile, char *lineBuffer);

/* Interpreta comando de criação de texto */
Geometry readActText(ArqCmds geoFile, char *lineBuffer, Style style);

/* Interpreta comando de definição de estilo de texto */
void readActTextStyle(ArqCmds geoFile, char *lineBuffer, Style createdStyle);

#endif /* GEO_FILE_H */
