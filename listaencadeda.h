typedef struct cel {
    int valor;
    struct cel* prox;
} Celula;

typedef struct lista {
    Celula* inicio;
    Celula* fim;
    int tamanho;
} Lista;

Lista* criaLista();

int inserirFim(Lista* lista, int v);

int removerInicio(Lista* lista);

void imprimeLista(Lista* lista);

void inverterLista(Lista* lista);

void liberaLista(Lista* lista);

