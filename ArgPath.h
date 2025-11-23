/**
 * Módulo: args
 * Finalidade:
 *   Este módulo é responsável pela análise e tratamento dos parâmetros
 *   fornecidos via linha de comando para o programa "ted".
 *   Ele interpreta as opções -e, -f, -q e -o, validando sua presença e
 *   armazenando os caminhos correspondentes em uma estrutura opaca.
 *
 * Notas:
 *   - O tipo Parametros é opaco; sua implementação concreta está em args.c.
 *   - Apenas funções públicas de acesso e gerenciamento são expostas aqui.
 *   - Este módulo segue um estilo de projeto orientado a objetos em C,
 *     onde os detalhes internos são encapsulados.
 */

#ifndef ARGS_H
#define ARGS_H

/* Tipo opaco que representa o conjunto de parâmetros interpretados. */
typedef void* Parametros;

/*
 * Função: lerParametros
 * Descrição:
 *   Analisa os argumentos fornecidos na execução do programa,
 *   identifica e valida as opções -e, -f, -q e -o e constrói uma
 *   instância de Parametros contendo essas informações.
 *
 * Requisitos:
 *   - argc e argv devem representar corretamente a chamada do programa.
 *   - As opções obrigatórias (-f e -o) devem estar presentes.
 *   - Após detectar erro, a função finaliza a execução com mensagem apropriada.
 *
 * Retorno:
 *   Uma instância opaca de Parametros devidamente preenchida.
 */
Parametros lerParametros(int argc, char **argv);

/*
 * Função: liberarParametros
 * Descrição:
 *   Libera toda a memória associada a uma instância de Parametros.
 *
 * Requisitos:
 *   - O ponteiro pp deve ser válido e apontar para uma instância alocada
 *     previamente por lerParametros.
 *
 * Saída:
 *   - O ponteiro *pp é definido como NULL após a liberação.
 */
void liberarParametros(Parametros *p);

/*
 * Função: args_getEntrada
 * Descrição:
 *   Retorna o caminho da pasta base de entrada informado pela opção -e,
 *   caso presente.
 *
 * Requisitos:
 *   - p deve ser uma instância válida de Parametros.
 *
 * Retorno:
 *   - Ponteiro para string interna (não deve ser liberada pelo usuário).
 *   - Pode retornar NULL caso a opção -e não tenha sido fornecida.
 */
char* args_getEntrada(Parametros p);

/*
 * Função: args_getGeo
 * Descrição:
 *   Retorna o nome/caminho do arquivo .geo fornecido pela opção -f.
 *
 * Requisitos:
 *   - p deve ser válido.
 *
 * Retorno:
 *   - String interna representando o arquivo .geo.
 *   - Nunca retorna NULL, pois -f é obrigatório.
 */
char* args_getGeo(Parametros p);

/*
 * Função: args_getQry
 * Descrição:
 *   Retorna o nome/caminho do arquivo .qry especificado pela opção -q.
 *
 * Requisitos:
 *   - p deve ser válido.
 *
 * Retorno:
 *   - String interna com o caminho do arquivo .qry.
 *   - Pode ser NULL caso -q não tenha sido fornecido.
 */
char* args_getQry(Parametros p);

/*
 * Função: args_getOut
 * Descrição:
 *   Retorna o diretório onde os arquivos de saída devem ser gerados,
 *   informado pela opção obrigatória -o.
 *
 * Requisitos:
 *   - p deve ser válido.
 *
 * Retorno:
 *   - String interna contendo o diretório de saída.
 *   - Nunca retorna NULL, pois -o é obrigatório.
 */
char* args_getOut(Parametros p);

#endif /* ARGS_H */

