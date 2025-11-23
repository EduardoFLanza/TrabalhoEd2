/**
 * Módulo: circulo
 * Finalidade:
 *     Este módulo define a interface pública da entidade Círculo, utilizada
 *     para representar geometricamente um conjunto de atributos associados a
 *     um círculo no plano 2D. O módulo disponibiliza operações de criação,
 *     consulta e destruição dessas instâncias.
 *
 * Descrição Geral:
 *     Um círculo é definido pelo centro (cx, cy), pelo raio r e por informações
 *     complementares como cor de borda, cor de preenchimento e um identificador
 *     textual. As funções deste módulo permitem acessar essas informações e
 *     manipular cada instância conforme necessário.
 */

#ifndef CIRCULO_H
#define CIRCULO_H

typedef void* Circulo;

/*
 * Função: circulo_criar
 * Descrição:
 *     Cria e inicializa uma nova instância de Círculo com os atributos
 *     especificados.
 * Parâmetros:
 *     cx, cy — coordenadas do centro
 *     r      — raio
 *     stroke — cor da borda
 *     fill   — cor de preenchimento
 *     id     — identificador textual
 * Retorno:
 *     Instância Circulo recém-criada.
 */
Circulo circulo_criar(double cx, double cy, double r,
                      const char* stroke, const char* fill, const char* id);

/*
 * Função: circulo_destruir
 * Descrição:
 *     Libera todos os recursos associados a uma instância de Círculo.
 * Parâmetros:
 *     c — instância a ser destruída.
 */
void circulo_destruir(Circulo c);

/*
 * Função: circulo_getID
 * Descrição:
 *     Retorna o identificador textual do círculo.
 * Parâmetros:
 *     c — instância do círculo.
 * Retorno:
 *     Ponteiro constante para a string ID.
 */
const char* circulo_getID(Circulo c);

/*
 * Função: circulo_getPos
 * Descrição:
 *     Obtém a posição (x, y) do centro do círculo.
 * Parâmetros:
 *     c — instância do círculo
 *     x, y — ponteiros para receber as coordenadas extraídas
 */
void circulo_getPos(Circulo c, double* x, double* y);

#endif

