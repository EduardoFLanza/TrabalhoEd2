/*fazer implementação de digraph (grafo direcionado)*/

/*
Um Grafo direcionado  G e' constituido por um conjunto de vertices V e 
um conjunto de arestas E, denotado por G=(V,E). 

Pode-se associar um dado a arestas e a vertices de G.
A cada vertice e' associado um nome.

Existem tambem operacoes que possibilitam percorrer o grafo,
invocando funcoes quando arestas do grafo 
sao "descobertas", podendo, inclusive, interromper o percurso.

Este modulo faz parte de um trabalho de Estrutura de Dados do 
curso de Ciencia da Computacao da Universidade Estadual de Londrina.
E' expressamente proibido o uso de ferramentas de IA para a sua implementacao.
A documentacao deste modulo deve ser melhorada.
*/


typedef void *Graph;
typedef int Node;
typedef void *Edge;
typedef void *Info;

/*
  Invocado quando uma aresta é "descoberta"/"percorrida"/"classificada". 
  Tambem informa os tempos de descoberta e finalizacao
 */
bool (*procEdge)(g,e,td,tf, void *extra); 

/*
  Invocado quando percurso e' recomecado
 */
bool (*dfsRestarted)(g, void *extra);



/*
    Cria um grafo com, no maximo, "nVert" vertices.
 */
Graph createGraph(nVert);


/*
    Retorna numero maximo de vertices que grafo g pode conter.
 */
int getMaxNodes(Graph g);


/*
    Retorna numero total de vertices adicionados ao grafo gr.
 */
int getTotalNodes(Graph g);


/*
    Adiciona um novo vértice ao grafo "g" com o nome "nome".
 */
Node addNode(g, nome, info);


/*
    Retorna no' cujo de nome e' "nome". 
 */
Node getNode(g, nome);


/*
 */
Info getNodeInfo(g, node);


/*
 */
char *getNodeName(g, node);


/*
 */
void setNodeInfo(g, node, info);


/*
 */
Edge addEdge(g, from, to, info);


/*
 */
Edge getEdge(g, from, to);


/*
 */
Node getFromNode(g, e)

  
/*
 */  
Node getToNode(g, e);


/*
 */
Info getEdgeInfo(g, e);


/*
 */
void setEdgeInfo(g, e, info);


/*
 */
void removeEdge(g,e);


/*
 */
bool isAdjacent(Graph g, Node from, Node to);


/* 
   Adiciona 'a lista "nosAdjacentes" os nos adjacentes 'a "node".
 */
void adjacentNodes(g, node, nosAdjacentes);


/*
   Adiciona 'a lista "arestaAdjacentes" as arestas (x,y), tal que,
   x == node.
 */
void adjacentEdges(g, node, Lista arestasAdjacentes);


/*
   Insere na lista "nomesNodes" os nomes atribuidos aos nos do grafo.
 */
void  getNodeNames(Graph g, Lista nomesNodes);


/*
   Insere na lista "arestas", as arestas de g.
 */
void getEdges(g, Lista arestas);


/*
   Faz percurso em profundidade sobre  g, a partir do no' node, classificando 
   as arestas do grafo, invocando a respectiva funcao.
      A busca em profundidade, eventualmente, pode produzir uma floresta.
   newTree e' invocada sempre que o percurso for retomado.
 */  
bool dfs(g, node, procEdge treeEdge, forwardEdge, returnEdge,
	 crossEdge, newTree, void *extra);


/*
   Percorre o grafo g em largura, a partir do no' node. discoverNode e' usada
   para a aresta (x,y) usada para "descobrir" o y.
 */
bool bfs(g, node, discoverNode, void *extra);


/*
   Destroi o grafo "g".
 */
void killDG(Graph g);

#endif
