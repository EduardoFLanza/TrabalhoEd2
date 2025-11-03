/* --- Funções do grafo --- */

/*
 * createGraph()
 * -------------
 * Cria um grafo direcionado com capacidade máxima de nVert nós.
 * Inicializa o número de nós atuais como 0 e aloca memória para os nós.
 */
Graph createGraph(int nVert) {
    GraphStruct *g = (GraphStruct*) malloc(sizeof(GraphStruct));
    if (!g) return NULL;

    g->maxNodes = nVert;
    g->totalNodes = 0;

    g->nodes = (NodeStruct*) malloc(nVert * sizeof(NodeStruct));
    if (!g->nodes) {
        free(g);
        return NULL;
    }

    for (int i = 0; i < nVert; i++)
        g->nodes[i].adj.inicio = NULL;

    return (Graph) g;
}

/*
 * getMaxNodes()
 * -------------
 * Retorna o número máximo de nós que o grafo pode ter.
 */
int getMaxNodes(Graph g) {
    GraphStruct *gr = (GraphStruct*) g;
    return gr->maxNodes;
}

/*
 * getTotalNodes()
 * ---------------
 * Retorna o número atual de nós adicionados no grafo.
 */
int getTotalNodes(Graph g) {
    GraphStruct *gr = (GraphStruct*) g;
    return gr->totalNodes;
}

/*
 * addNode()
 * ---------
 * Adiciona um novo nó ao grafo com o nome e informação fornecidos.
 * Retorna o índice do nó adicionado ou -1 caso o grafo esteja cheio.
 */
Node addNode(Graph g, const char *nome, Info info) {
    GraphStruct *gr = (GraphStruct*) g;
    if (gr->totalNodes >= gr->maxNodes) return -1;

    Node n = gr->totalNodes;
    gr->nodes[n].nome = strdup(nome);
    gr->nodes[n].info = info;
    gr->nodes[n].adj.inicio = NULL;
    gr->totalNodes++;

    return n;
}

/*
 * getNode()
 * ---------
 * Busca um nó pelo seu nome.
 * Retorna o índice do nó ou -1 caso não exista.
 */
Node getNode(Graph g, const char *nome) {
    GraphStruct *gr = (GraphStruct*) g;
    for (int i = 0; i < gr->totalNodes; i++)
        if (strcmp(gr->nodes[i].nome, nome) == 0)
            return i;
    return -1;
}

/*
 * getNodeInfo()
 * -------------
 * Retorna a informação associada a um nó específico.
 */
Info getNodeInfo(Graph g, Node node) {
    GraphStruct *gr = (GraphStruct*) g;
    return gr->nodes[node].info;
}

/*
 * getNodeName()
 * -------------
 * Retorna o nome do nó especificado pelo índice.
 */
char *getNodeName(Graph g, Node node) {
    GraphStruct *gr = (GraphStruct*) g;
    return gr->nodes[node].nome;
}

/*
 * setNodeInfo()
 * -------------
 * Atualiza a informação de um nó específico.
 */
void setNodeInfo(Graph g, Node node, Info info) {
    GraphStruct *gr = (GraphStruct*) g;
    gr->nodes[node].info = info;
}

/* --- Funções de arestas --- */

/*
 * addEdge()
 * ---------
 * Adiciona uma aresta direcionada do nó 'from' para o nó 'to', com a informação associada.
 * Retorna o ponteiro para a aresta criada (Edge) ou NULL em caso de falha.
 */
Edge addEdge(Graph g, Node from, Node to, Info info) {
    GraphStruct *gr = (GraphStruct*) g;
    Celula *nova = (Celula*) malloc(sizeof(Celula));
    if (!nova) return NULL;

    nova->to = to;
    nova->info = info;
    nova->prox = gr->nodes[from].adj.inicio;
    gr->nodes[from].adj.inicio = nova;

    return nova;
}

/*
 * getEdge()
 * ---------
 * Retorna a aresta que conecta o nó 'from' ao nó 'to'.
 * Retorna NULL se não existir.
 */
Edge getEdge(Graph g, Node from, Node to) {
    GraphStruct *gr = (GraphStruct*) g;
    Celula *aux = gr->nodes[from].adj.inicio;
    while (aux != NULL) {
        if (aux->to == to)
            return aux;
        aux = aux->prox;
    }
    return NULL;
}

/*
 * getFromNode()
 * -------------
 * Dado um ponteiro de aresta, retorna o nó de origem (from).
 */
Node getFromNode(Graph g, Edge e) {
    Celula *c = (Celula*) e;
    GraphStruct *gr = (GraphStruct*) g;
    for (int i = 0; i < gr->totalNodes; i++) {
        Celula *aux = gr->nodes[i].adj.inicio;
        while (aux) {
            if (aux == c) return i;
            aux = aux->prox;
        }
    }
    return -1;
}

/*
 * getToNode()
 * -----------
 * Retorna o nó de destino (to) da aresta especificada.
 */
Node getToNode(Graph g, Edge e) {
    Celula *c = (Celula*) e;
    return c->to;
}

/*
 * getEdgeInfo()
 * -------------
 * Retorna a informação associada a uma aresta.
 */
Info getEdgeInfo(Graph g, Edge e) {
    Celula *c = (Celula*) e;
    return c->info;
}

/*
 * setEdgeInfo()
 * -------------
 * Atualiza a informação de uma aresta.
 */
void setEdgeInfo(Graph g, Edge e, Info info) {
    Celula *c = (Celula*) e;
    c->info = info;
}

/*
 * removeEdge()
 * ------------
 * Remove uma aresta do grafo, liberando a memória associada.
 */
void removeEdge(Graph g, Edge e) {
    GraphStruct *gr = (GraphStruct*) g;
    Node from = getFromNode(g, e);
    Celula *prev = NULL;
    Celula *cur = gr->nodes[from].adj.inicio;

    while (cur) {
        if (cur == e) {
            if (prev)
                prev->prox = cur->prox;
            else
                gr->nodes[from].adj.inicio = cur->prox;
            free(cur);
            return;
        }
        prev = cur;
        cur = cur->prox;
    }
}

/*
 * isAdjacent()
 * ------------
 * Verifica se existe uma aresta do nó 'from' para o nó 'to'.
 * Retorna true se existir, false caso contrário.
 */
bool isAdjacent(Graph g, Node from, Node to) {
    return getEdge(g, from, to) != NULL;
}

/* --- Funções de listas --- */

/*
 * adjacentNodes()
 * ---------------
 * Retorna a lista de nós adjacentes a um dado nó.
 * A lista é representada pelo ponteiro para a lista interna de adjacência.
 */
void adjacentNodes(Graph g, Node node, Lista *nosAdjacentes) {
    GraphStruct *gr = (GraphStruct*) g;
    nosAdjacentes->inicio = gr->nodes[node].adj.inicio;
}

/*
 * killDG()
 * --------
 * Libera toda a memória alocada para o grafo, incluindo nós, nomes e arestas.
 */
void killDG(Graph g) {
    GraphStruct *gr = (GraphStruct*) g;

    for (int i = 0; i < gr->totalNodes; i++) {
        Celula *aux = gr->nodes[i].adj.inicio;
        while (aux != NULL) {
            Celula *tmp = aux;
            aux = aux->prox;
            free(tmp);
        }
        free(gr->nodes[i].nome);
    }

    free(gr->nodes);
    free(gr);
}

