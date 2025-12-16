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
 * Parâmetros: fn – nome (ou caminho completo) do arquivo SVG.
 * Retorno: Instância de ArqSvg ou NULL em caso de erro.
 */
ArqSvg abreEscritaSvg(char *fn);

/*
 * Função: preparaDecoracao
 * Descrição: Preenche uma string com atributos SVG de formatação para figuras.
 * Parâmetros: fsvg – arquivo SVG.
 *             deco – vetor de caracteres para receber a decoração.
 *             decoLen – capacidade do vetor deco.
 *             corBorda – cor da borda.
 *             corPreenchimento – cor de preenchimento.
 *             larguraBorda – largura da borda.
 *             transparencia – transparência geral.
 *             transparenciaPreenchimento – transparência do preenchimento.
 *             transparenciaBorda – transparência da borda.
 * Retorno: Nenhum.
 */
void preparaDecoracao(ArqSvg fsvg, char *deco, int decoLen,
                      char *corBorda, char *corPreenchimento,
                      char *larguraBorda, double transparencia,
                      double transparenciaPreenchimento,
                      double transparenciaBorda);

/*
 * Função: escreveCirculoSvg
 * Descrição: Insere um círculo no arquivo SVG.
 * Parâmetros: fsvg – arquivo SVG.
 *             xc, yc – coordenadas do centro.
 *             r – raio.
 *             deco – string de decoração ou NULL.
 * Retorno: Nenhum.
 */
void escreveCirculoSvg(ArqSvg fsvg, double xc, double yc, double r, char *deco);

/*
 * Função: escreveRetanguloSvg
 * Descrição: Insere um retângulo no arquivo SVG.
 * Parâmetros: fsvg – arquivo SVG.
 *             x, y – coordenada do canto inferior esquerdo.
 *             larg – largura.
 *             alt – altura.
 *             deco – string de decoração ou NULL.
 * Retorno: Nenhum.
 */
void escreveRetanguloSvg(ArqSvg fsvg, double x, double y,
                          double larg, double alt, char *deco);

/*
 * Função: escreveLinhaSvg
 * Descrição: Insere uma linha no arquivo SVG.
 * Parâmetros: fsvg – arquivo SVG.
 *             x1, y1 – ponto inicial.
 *             x2, y2 – ponto final.
 *             deco – string de decoração ou NULL.
 * Retorno: Nenhum.
 */
void escreveLinhaSvg(ArqSvg fsvg,
                     double x1, double y1,
                     double x2, double y2,
                     char *deco);

/* Font style */
#define FS_NORMAL   "normal"
#define FS_ITALIC   "italic"
#define FS_OBLIQUE  "oblique"

/* Font weight */
#define FW_NORMAL   "normal"
#define FW_BOLD     "bold"
#define FW_BOLDER   "bolder"
#define FW_LIGHTER  "lighter"

/* Text anchor */
#define TA_START  "start"
#define TA_MIDDLE "middle"
#define TA_END    "end"

/*
 * Função: preparaDecoracaoTexto
 * Descrição: Preenche uma string com atributos SVG de formatação para textos.
 * Parâmetros: fsvg – arquivo SVG.
 *             deco – vetor para receber a decoração.
 *             decoLen – capacidade do vetor.
 *             fontFamily – família da fonte.
 *             fontStyle – estilo da fonte.
 *             fontWeight – peso da fonte.
 *             fontSize – tamanho da fonte.
 *             fontColor – cor do texto.
 *             fontStroke – cor do contorno.
 *             textAnchor – alinhamento do texto.
 * Retorno: Nenhum.
 */
void preparaDecoracaoTexto(ArqSvg fsvg, char *deco, int decoLen,
                           char *fontFamily, char *fontStyle,
                           char *fontWeight, char *fontSize,
                           char *fontColor, char *fontStroke,
                           char *textAnchor);

/*
 * Função: escreveTextoSvg
 * Descrição: Insere um texto no arquivo SVG.
 * Parâmetros: fsvg – arquivo SVG.
 *             x, y – posição do texto.
 *             txt – texto a ser escrito.
 *             textAnchor – alinhamento.
 *             decoTxt – string de decoração ou NULL.
 * Retorno: Nenhum.
 */
void escreveTextoSvg(ArqSvg fsvg, double x, double y,
                     char *txt, char *textAnchor, char *decoTxt);

/*
 * Função: fechaSvg
 * Descrição: Fecha o arquivo SVG liberando seus recursos.
 * Parâmetros: fsvg – arquivo SVG.
 * Retorno: Nenhum.
 */
void fechaSvg(ArqSvg fsvg);

/*
 * Função: applyRotation
 * Descrição: Aplica rotação a uma decoração SVG.
 * Parâmetros: deco – string de decoração.
 *             decoLen – capacidade da string.
 *             theta – ângulo de rotação.
 *             xCenter, yCenter – centro da rotação.
 * Retorno: Nenhum.
 */
void applyRotation(char *deco, int decoLen,
                   double theta, double xCenter, double yCenter);

#endif
