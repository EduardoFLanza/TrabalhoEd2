/**
 * Módulo: geoFile
 * Finalidade:
 *    Responsável pela leitura e interpretação dos comandos presentes no arquivo .geo.
 *    Cada comando lido deve resultar na criação e inserção de entidades gráficas
 *    (ex.: quadras, círculos, retângulos, textos etc.) dentro das estruturas
 *    principais do sistema (Picture, BBox, HashTable e demais módulos).
 *
 * Descrição Geral:
 *    A função principal deste módulo realiza a leitura sequencial de um arquivo
 *    texto no formato .geo, identificando comandos e seus parâmetros, e
 *    repassando-os para os módulos apropriados. Este módulo não armazena
 *    entidades; apenas coordena a criação e envio delas para as estruturas
 *    responsáveis.
 *
 * Observações:
 *    - Todas as entidades criadas dentro desta função devem ser registradas
 *      corretamente no Picture e na Bounding Box.
 *    - Quadras devem ser registradas também na HashTable correspondente.
 *    - Apenas a lógica de leitura pertence a este módulo; a lógica de desenho e
 *      armazenamento pertence aos outros módulos.
 */

#include <stdio.h>
#include <stdlib.h>
#include "geoFile.h"

/*
 * Função: geo_processar
 * ---------------------
 * Lê um arquivo .geo linha por linha, interpreta seus comandos e envia tais
 * comandos para os módulos responsáveis pela criação e armazenamento das
 * entidades.
 *
 * Parâmetros:
 *    (FILE* f)            Arquivo .geo já aberto para leitura.
 *    (Picture p)          Estrutura onde as entidades gráficas serão inseridas.
 *    (BBox bb)            Estrutura espacial usada para indexação das entidades.
 *    (HashTable quadras)  HashTable usada para registrar quadras pelo CEP.
 *
 * Retorno:
 *    void — esta função não retorna valores; atua apenas sobre as estruturas
 *    fornecidas como parâmetro.
 *
 * Observação:
 *    Esta implementação é mínima e serve como esqueleto para a lógica completa
 *    de leitura. O comportamento real deve incluir o parsing dos comandos
 *    especificados no arquivo .geo conforme definido no projeto.
 */
void geo_processar(FILE* f, Picture p, BBox bb, HashTable quadras){
    /* Implementação mínima (apenas esqueleto) */
    (void)f;
    (void)p;
    (void)bb;
    (void)quadras;

    /* A implementação real deve:
       - Ler linha por linha
       - Identificar comandos (ex.: "q", "cq", "h", "s", "t" etc.)
       - Criar entidades correspondentes usando os módulos adequados
       - Inserir as entidades no Picture
       - Inserir as entidades no BBox
       - Registrar quadras na HashTable, se aplicável
    */
}
