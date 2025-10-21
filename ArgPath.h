
// Remove a barra final de um caminho, se houver. (verificar se é melhor tirar ou adicionar)
void normalizarArg(char *arg, char *argNormalizado);

// Extrai apenas o caminho do arquivo a partir do caminho completo.
void obterArg(char *argCompleto, char *arg);

// Extrai apenas o nome do arquivo (com extensão) a partir do caminho completo.
void obterNomeArq(char *argCompleto, char *nomeArq);

// Junta caminho, nome do arquivo e extensão para formar um caminho completo.
void juntarTudo(char *arg, char *nomeArq, char *ext, char *argCompleto);

// Junta caminho e nome do arquivo, formando o caminho completo.
void juntarArgNomeArq(char *arg, char *nomeArq, char *argCompleto);

// Divide um caminho completo em caminho, nome do arquivo (sem extensão) e extensão.
void dividirArg(char *argCompleto, char *arg, char *nomeArq, char *extArq);

// Extrai a extensão do arquivo a partir de um caminho completo.
void obterExtensao(char *argCompleto, char *ext);

// Extrai o nome do arquivo sem extensão a partir de um caminho completo.
void obterNomeArqSemExt(char *argCompleto, char *nomeArq;

