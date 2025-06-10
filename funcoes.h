#pragma once

/**
 * @funcoes.h
 * @brief Defini��es de estruturas e fun��es para a gest�o de redes de antenas no grafo
 *
 * @author Rog�rio Picas
 * @date 07-05-2025
 * @version 1.0
 *
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdbool.h>

 /** @def MAX_DIM
  *  @brief Dimens�o da matriz da cidade.
  */
#define MAX_DIM 20

#pragma region Estruturas

   /**
    * @struct Antenas
    * @brief Representa uma antena com frequ�ncia e posi��o na matriz da cidade.
    */
typedef struct Antenas {
    char frequencia; /**< Frequ�ncia da antena (A-Z) */
    int x;           /**< Coordenada X na matriz */
    int y;           /**< Coordenada Y na matriz */
} ANTENAS;

/**
 * @struct Aresta
 * @brief Representa uma aresta (conex�o) entre v�rtices no grafo.
 */
typedef struct Aresta {
    struct Vertice* destino; /**< Apontador para o v�rtice destino da aresta */
    struct Aresta* prox;     /**< Apontador para a pr�xima aresta na lista ligada */
} ARESTA;

/**
 * @struct Vertice
 * @brief Representa um v�rtice no grafo, contendo a antena e as liga��es.
 */
typedef struct Vertice {
    ANTENAS* infoAntenas; /**< Apontador para a antena associada ao v�rtice */
    ARESTA* adjacentes;   /**< Lista ligada das arestas (liga��es) para outros v�rtices */
    int visitado;         /**< Indicador se o v�rtice foi visitado */
    struct Vertice* prox; /**< Apontador para o pr�ximo v�rtice na lista do grafo */
} VERTICE;

/**
 * @struct Grafo
 * @brief Representa um grafo contendo uma lista de v�rtices e outras informa��es.
 */
typedef struct Grafo {
    char frequencia;     /**< Frequ�ncia comum a todas as antenas do grafo */
    VERTICE* vertices;   /**< Apontador para a lista de v�rtices do grafo */
    int maxVertices;     /**< N�mero m�ximo de v�rtices permitido no grafo */
    int numVertices;     /**< N�mero atual de v�rtices no grafo */
    struct Grafo* prox;  /**< Apontador para o pr�ximo grafo na lista de grafos */
} GRAFO;

/**
 * @struct NoFila
 * @brief N� da fila usado para BFT
 */
typedef struct NoFila {
    VERTICE* vertice;   /**< Apontador para o v�rtice armazenado no n� da fila */
    struct NoFila* prox;/**< Apontador para o pr�ximo n� da fila */
} NOFILA;

/**
 * @struct Fila
 * @brief Estrutura para fila ligada (BFT)
 */
typedef struct Fila {
    NOFILA* frente; /**< Apontador para o n� da frente da fila */
    NOFILA* tras;   /**< Apontador para o n� do final da fila */
} FILA;

/**
 * @struct Rede_Grafos
 * @brief Representa uma rede de grafos, onde cada grafo est� associado a uma frequ�ncia.
 */
typedef struct Rede_Grafos {
    GRAFO* listaGrafos; /**< Apontador para a lista ligada de grafos */
} REDE;

/**
 * @struct VerticeFicheiro
 * @brief Estrutura auxiliar para armazenar informa��es de v�rtices para ficheiro binario
 */
typedef struct VerticeFicheiro {
    char frequencia; /**< Frequ�ncia da antena */
    int x;           /**< Coordenada X do v�rtice */
    int y;           /**< Coordenada Y do v�rtice */
} VerticeFICHEIRO;

/**
 * @struct ArestasFicheiro
 * @brief Estrutura auxiliar para armazenar informa��es de arestas para o ficheiro binario
 */
typedef struct ArestasFicheiro {
    int xOrigem;  /**< Coordenada X do v�rtice origem da aresta */
    int yOrigem;  /**< Coordenada Y do v�rtice origem da aresta */
    int xDestino; /**< Coordenada X do v�rtice destino da aresta */
    int yDestino; /**< Coordenada Y do v�rtice destino da aresta */
} ArestasFICHEIRO;

#pragma endregion

#pragma region Fun��es de Cria��o

/**
 * @brief Cria uma nova antena com a frequ�ncia e coordenadas indicadas.
 * @param freqAntena Frequ�ncia da antena (letra de A-Z).
 * @param x Coordenada X.
 * @param y Coordenada Y.
 * @return Apontador para a antena criada.
 */

ANTENAS* criarAntena(char freqAntena, int x, int y);

/**
 * @brief Cria um novo v�rtice associado a uma antena.
 * @param antena Apontador para a antena.
 * @return Apontador para o v�rtice criado.
 */

VERTICE* criarVertice(ANTENAS* antena);

/**
 * @brief Cria um novo grafo com capacidade para um n�mero definido de liga��es.
 * @param maxLigacoes N�mero m�ximo de v�rtices que o grafo pode conter.
 * @return Apontador para o grafo criado.
 */

GRAFO* criarGrafo(REDE* rede, char freq);

/**
 * @brief Cria uma nova rede de grafos inicializada a NULL.
 * @return Apontador para a rede criada.
 */

REDE* criarRede();

#pragma endregion

#pragma region Fun��es de Inser��o e Manipula��o

/**
 * @brief - insere uma nova antena no grafo com base na frequ�ncia e coordenadas indicadas.
 *
 * @param - grafo: apontador para o grafo onde a antena ser� inserida.
 * @param - frequencia: frequ�ncia da antena (caracter A-Z).
 * @param - x: coordenada X da antena.
 * @param - y: coordenada Y da antena.
 * @return - 0 se a inser��o for bem-sucedida, 1 em caso de erro.
 */
int inserirAntenaGrafo(GRAFO* grafo, char frequencia, int x, int y);

/**
 * @brief - conecta dois v�rtices no grafo com uma aresta bidirecional.
 *
 * @param - grafo: apontador para o grafo onde a conex�o ser� feita.
 * @param - x1: coordenada X do primeiro v�rtice (origem).
 * @param - y1: coordenada Y do primeiro v�rtice (origem).
 * @param - x2: coordenada X do segundo v�rtice (destino).
 * @param - y2: coordenada Y do segundo v�rtice (destino).
 * @return - 0 se a conex�o for bem-sucedida, 1 em caso de erro.
 */
int conectarVertices(GRAFO* grafo, int x1, int y1, int x2, int y2);

/**
 * @brief - mostra a rede de grafos numa matriz, baseada nas frequ�ncias presentes.
 *
 * @param - redeGrafos: apontador para a rede de grafos a mostrar.
 * @return - 0 se a opera��o for bem-sucedida, 1 em caso de erro.
 */
int mostrarRedeGrafos(REDE* redeGrafos);

/**
 * @brief - procura um grafo na rede pela sua frequ�ncia.
 *
 * @param - rede: apontador para a rede de grafos.
 * @param - freq: frequ�ncia do grafo a procurar.
 * @return - apontador para o grafo encontrado ou NULL se n�o existir.
 */
GRAFO* encontrarGrafoPorFrequencia(REDE* rede, char freq);

/**
 * @brief - carrega grafos a partir de um ficheiro de texto para a rede.
 *
 * @param - rede: apontador para a rede onde os grafos ser�o inseridos.
 * @param - filename: nome do ficheiro de texto a ler.
 * @return - 0 se a carga for bem-sucedida, 1 em caso de erro.
 */
int carregaGrafo(REDE* rede, char* filename);

/**
 * @brief - procura um v�rtice no grafo pela sua posi��o (x, y).
 *
 * @param - grafo: apontador para o grafo onde a procura ser� feita.
 * @param - x: coordenada X do v�rtice procurado.
 * @param - y: coordenada Y do v�rtice procurado.
 * @return - apontador para o v�rtice encontrado ou NULL se n�o existir.
 */
VERTICE* encontrarVertice(GRAFO* grafo, int x, int y);

/**
 * @brief - procura um v�rtice no grafo pela sua posi��o (x, y).
 *
 * @param - grafo: apontador para o grafo onde a procura ser� feita.
 * @param - x: coordenada X do v�rtice procurado.
 * @param - y: coordenada Y do v�rtice procurado.
 * @return - apontador para o v�rtice encontrado ou NULL se n�o existir.
 */
VERTICE* encontrarVertice(GRAFO* grafo, int x, int y);

/**
 * @brief - guarda as arestas de um v�rtice num ficheiro.
 *
 * @param - head: apontador para a lista de arestas a guardar.
 * @param - fp: apontador para o ficheiro aberto para escrita.
 * @param - xOrigem: coordenada X do v�rtice origem das arestas.
 * @param - yOrigem: coordenada Y do v�rtice origem das arestas.
 * @return - n�mero de arestas guardadas com sucesso.
 */
int guardarArestas(ARESTA* head, FILE* fp, int xOrigem, int yOrigem);

/**
 * @brief - guarda o grafo num ficheiro bin�rio.
 *
 * @param - head: apontador para a lista de v�rtices do grafo.
 * @param - filename: nome do ficheiro onde ser� guardado o grafo.
 * @return - true se a opera��o foi bem sucedida, false caso contr�rio.
 */
bool guardarGrafoBin(VERTICE* head, char* filename);

/**
 * @brief - verifica se existe uma conex�o entre dois v�rtices especificados pelas suas coordenadas.
 *
 * @param - grafo: apontador para o grafo.
 * @param - xOrigem: coordenada X do v�rtice origem.
 * @param - yOrigem: coordenada Y do v�rtice origem.
 * @param - xDestino: coordenada X do v�rtice destino.
 * @param - yDestino: coordenada Y do v�rtice destino.
 * @return - true se existe conex�o, false caso contr�rio.
 */
bool existeConexaoEntreVertices(GRAFO* grafo, int xOrigem, int yOrigem, int xDestino, int yDestino);

/**
 * @brief - verifica se uma aresta aponta para o destino especificado.
 *
 * @param - head: apontador para a lista de arestas.
 * @param - xDestino: coordenada X do v�rtice destino.
 * @param - yDestino: coordenada Y do v�rtice destino.
 * @return - true se existir uma aresta para o destino, false caso contr�rio.
 */
bool existeConexao(ARESTA* head, int xDestino, int yDestino);



#pragma endregion

#pragma region Fun��es de Elimina��o

/**
 * @brief Liberta toda a mem�ria associada a um grafo, incluindo os seus v�rtices e arestas.
 *
 * Esta fun��o destr�i um grafo completo, removendo todos os v�rtices e as respetivas listas de arestas,
 * libertando toda a mem�ria ocupada.
 *
 * @param grafo - apontador para o grafo a destruir.
 * @return - NULL, indicando que o grafo foi destru�do.
 */
GRAFO* destruirGrafo(GRAFO* grafo);

/**
 * @brief Liberta toda a mem�ria associada � lista de v�rtices, incluindo as listas de arestas e as antenas.
 *
 * Para cada v�rtice da lista, esta fun��o remove todas as arestas adjacentes, liberta a mem�ria da antena
 * associada e do pr�prio v�rtice.
 *
 * @param lista - apontador para o in�cio da lista de v�rtices a destruir.
 * @return - NULL, indicando que a lista foi destru�da.
 */
VERTICE* destruirVertices(VERTICE* lista);

/**
 * @brief Liberta toda a mem�ria associada � lista de arestas.
 *
 * Para cada aresta da lista, esta fun��o liberta a mem�ria ocupada pela estrutura da aresta.
 *
 * @param lista - apontador para o in�cio da lista de arestas a destruir.
 * @return - NULL, indicando que a lista foi destru�da.
 */
ARESTA* destruirArestas(ARESTA* lista);

#pragma endregion

#pragma region Fun��es Auxiliares para BFS

/**
 * @brief - cria uma fila vazia.
 *
 * @param - fila: apontador para a estrutura da fila a inicializar.
 */
void criarFila(FILA* fila);

/**
 * @brief - verifica se a fila est� vazia.
 *
 * @param - fila: apontador para a fila a verificar.
 * @return - true se a fila estiver vazia (frente == NULL), caso contr�rio false.
 */
bool filaVazia(FILA* fila);

/**
 * @brief - enfileira um v�rtice na fila.
 *
 * @param - fila: apontador para a fila.
 * @param - vertice: apontador para o v�rtice a inserir.
 * @return - true se o v�rtice foi inserido com sucesso, false em caso de falha de aloca��o.
 */
bool enfilarVertice(FILA* fila, VERTICE* vertice);

/**
 * @brief - desenfileira o v�rtice da frente da fila.
 *
 * @param - fila: apontador para a fila.
 * @return - apontador para o v�rtice removido, ou NULL se a fila estiver vazia.
 */
VERTICE* desenfilarVertice(FILA* fila);

/**
 * @brief - executa uma busca em largura (BFT) no grafo a partir da antena inicial indicada.
 *
 * @param - grafo: apontador para o grafo onde ser� executada a busca.
 * @param - x: coordenada X da antena inicial.
 * @param - y: coordenada Y da antena inicial.
 * @param - count: apontador para inteiro onde ser� armazenado o n�mero de v�rtices visitados.
 * @return - 0 em caso de sucesso, 1 se grafo inv�lido ou antena inicial n�o encontrada.
 */
int BFT(GRAFO* grafo, int x, int y, int* count);

/**
 * @brief - valida o grafo, verificando se n�o � NULL e se tem v�rtices.
 *
 * @param - grafo: apontador para o grafo a validar.
 * @return - true se o grafo for v�lido, false caso contr�rio.
 */
bool validarGrafo(GRAFO* grafo);

/**
 * @brief - reinicia o campo visitado de todos os v�rtices para 0 (n�o visitado).
 *
 * @param - grafo: apontador para a lista de v�rtices.
 * @return - apontador para a lista de v�rtices ap�s reiniciar o campo visitado.
 */
VERTICE* resetarVisitados(VERTICE* grafo);

#pragma endregion
