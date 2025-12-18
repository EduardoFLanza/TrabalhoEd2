#ifndef ARQSVG__
#define ARQSVG__

/*
 * Módulo: arqsvg
 *
 * Descrição:
 *     Provê operações para criação e escrita de arquivos no formato SVG.
 *     O módulo permite inserir figuras básicas (círculos, retângulos, linhas)
 *     e textos, bem como definir atributos de apresentação (cores, transparência,
 *     fonte, alinhamento, etc).
 *
 *     O arquivo SVG é criado por abreEscritaSvg e deve ser fechado por fechaSvg.
 *     Não há operações para modificação de um arquivo já existente.
 *
 * Abstração:
 *     Uma instância de ArqSvg representa um arquivo SVG aberto para escrita,
 *     gerenciado exclusivamente pelas funções deste módulo.
 */

typedef void *ArqSvg;

/*
 * Função: abreEscritaSvg
 * Descrição: Cria um arquivo SVG para escrita, substituindo-o se já existir.
 * Parâmetros: fn – nome (ou caminho completo) do arquivo SVG; w - largura; h - altura.
 * Retorno: Instância de ArqSvg ou NULL em caso de erro.
 */
ArqSvg abreEscritaSvg(char *fn, double w, double h);

/*
 * Função: escreveRetanguloSvg
 * Descrição: Insere um retângulo no arquivo SVG.
 * Parâmetros: fsvg – arquivo SVG.
 *             x, y – coordenada do canto inferior esquerdo.
 *             larg – largura.
 *             alt – altura.
 *             corP - cor de preenchimento.
 *             corB - cor da borda.
 *             espB - espessura da borda.
 *             opacidade - opacidade.
 * Retorno: Nenhum.
 */
void escreveRetanguloSvg(ArqSvg fsvg, double x, double y,
                          double larg, double alt, const char *corP,
                          const char *corB, const char *espB,
                          double opacidade);

/*
 * Função: escreveLinhaSvg
 * Descrição: Insere uma linha no arquivo SVG.
 * Parâmetros: fsvg – arquivo SVG.
 *             x1, y1 – ponto inicial.
 *             x2, y2 – ponto final.
 *             cor - cor da linha.
 *             espL - espessura da linha.
 * Retorno: Nenhum.
 */
void escreveLinhaSvg(ArqSvg fsvg,
                     double x1, double y1,
                     double x2, double y2,
                     const char *cor, const char* espL);

/*
 * Função: escreveTextoSvg
 * Descrição: Insere um texto no arquivo SVG.
 * Parâmetros: fsvg – arquivo SVG.
 *             x, y – posição do texto.
 *             txt – texto a ser escrito.
 *             textAnchor – alinhamento.
 * Retorno: Nenhum.
 */
void escreveTextoSvg(ArqSvg fsvg, double x, double y,
                     char *txt, char *textAnchor);

/*
 * Função: fechaSvg
 * Descrição: Fecha o arquivo SVG liberando seus recursos.
 * Parâmetros: fsvg – arquivo SVG.
 * Retorno: Nenhum.
 */
void fechaSvg(ArqSvg fsvg);

#endif

