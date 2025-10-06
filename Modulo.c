#ifndef <NOME_MODULO>_H
#define <NOME_MODULO>_H
// Importa apenas os cabeçalhos necessários da biblioteca padrão da linguagem
#include <stdio.h>
// Apenas os cabeçalhos necessários de módulos do projeto
#include “xxxx.h”
/*
* Fazer uma boa descrição da finalidade deste módulo.
* Normalmente o módulo cria e gerencia uma “entidade”, um objeto.
* Talvez seja oportuno fazer um “overview” sobre as operações providas
* pelo módulo.
* IMPORTANTÍSSIMO: responder com clareza a seguinte pergunta:
* “O que é uma instância da abstração criada por este módulo?”.
* IMPORTANTÍSSIMO: saber escrever este comentário com clareza é condição
* indispensável para ser capaz de fazer programação Orientada a Objetos.
*/

// Declarar tipos e constantes “exportados” pelo módulo.
// NUNCA escrever a palavra struct no arquivo.h. (Information Hiding)
typedef void *TipoEntidade;
// Operações que criam, destroem e manipulam entidades projetadas neste
// módulo
TipoEntidade criaEntidade(<parâmetros>);
/* Descrever os parâmetros, restrições de valores para estes parâmetros.
* Existe alguma particularidade na entidade criada dependendo dos valores
* dos parâmetros?
*/
void operacao1TipoEnt(TipoEntidade instancia, <pars>);
/*
* Qual é o efeito desta operação sobre uma instância do tipo de entidade?
* Ou seja, como instância é modificada?
* Descrever os parâmetros, restrições de valores para estes parâmetros.
*/
void destroiEntidade(TipoEntidade instancia);
/*
* Verifique se é necessário mencionar algo importante sobre os recursos
* que são liberados, especialmente em relação com os parâmetros usados na
* criação da entidade.
*/
#endif
