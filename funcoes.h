#pragma once

/**
 * @funcoes.h
 * @brief Definições de estruturas e funções para a gestão de redes de antenas no grafo
 *
 * @author Rogério Picas
 * @date 07-05-2025
 * @version 1.0
 *
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdbool.h>

 /** @def MAX_DIM
  *  @brief Dimensão da matriz da cidade.
  */
#define MAX_DIM 20

#pragma region Estruturas

   /**
    * @struct Antenas
    * @brief Representa uma antena com frequência e posição na matriz da cidade.
    */
typedef struct Antenas {
    char frequencia; /**< Frequência da antena (A-Z) */
    int x;           /**< Coordenada X na matriz */
    int y;           /**< Coordenada Y na matriz */
} ANTENAS;

/**
 * @struct Aresta
 * @brief Representa uma aresta (conexão) entre vértices no grafo.
 */
typedef struct Aresta {
    struct Vertice* destino; /**< Apontador para o vértice destino da aresta */
    struct Aresta* prox;     /**< Apontador para a próxima aresta na lista ligada */
} ARESTA;

/**
 * @struct Vertice
 * @brief Representa um vértice no grafo, contendo a antena e as ligações.
 */
typedef struct Vertice {
    ANTENAS* infoAntenas; /**< Apontador para a antena associada ao vértice */
    ARESTA* adjacentes;   /**< Lista ligada das arestas (ligações) para outros vértices */
    int visitado;         /**< Indicador se o vértice foi visitado */
    struct Vertice* prox; /**< Apontador para o próximo vértice na lista do grafo */
} VERTICE;

/**
 * @struct Grafo
 * @brief Representa um grafo contendo uma lista de vértices e outras informações.
 */
typedef struct Grafo {
    char frequencia;     /**< Frequência comum a todas as antenas do grafo */
    VERTICE* vertices;   /**< Apontador para a lista de vértices do grafo */
    int maxVertices;     /**< Número máximo de vértices permitido no grafo */
    int numVertices;     /**< Número atual de vértices no grafo */
    struct Grafo* prox;  /**< Apontador para o próximo grafo na lista de grafos */
} GRAFO;

/**
 * @struct NoFila
 * @brief Nó da fila usado para BFT
 */
typedef struct NoFila {
    VERTICE* vertice;   /**< Apontador para o vértice armazenado no nó da fila */
    struct NoFila* prox;/**< Apontador para o próximo nó da fila */
} NOFILA;

/**
 * @struct Fila
 * @brief Estrutura para fila ligada (BFT)
 */
typedef struct Fila {
    NOFILA* frente; /**< Apontador para o nó da frente da fila */
    NOFILA* tras;   /**< Apontador para o nó do final da fila */
} FILA;

/**
 * @struct Rede_Grafos
 * @brief Representa uma rede de grafos, onde cada grafo está associado a uma frequência.
 */
typedef struct Rede_Grafos {
    GRAFO* listaGrafos; /**< Apontador para a lista ligada de grafos */
} REDE;

/**
 * @struct VerticeFicheiro
 * @brief Estrutura auxiliar para armazenar informações de vértices para ficheiro binario
 */
typedef struct VerticeFicheiro {
    char frequencia; /**< Frequência da antena */
    int x;           /**< Coordenada X do vértice */
    int y;           /**< Coordenada Y do vértice */
} VerticeFICHEIRO;

/**
 * @struct ArestasFicheiro
 * @brief Estrutura auxiliar para armazenar informações de arestas para o ficheiro binario
 */
typedef struct ArestasFicheiro {
    int xOrigem;  /**< Coordenada X do vértice origem da aresta */
    int yOrigem;  /**< Coordenada Y do vértice origem da aresta */
    int xDestino; /**< Coordenada X do vértice destino da aresta */
    int yDestino; /**< Coordenada Y do vértice destino da aresta */
} ArestasFICHEIRO;

#pragma endregion

#pragma region Funções de Criação

/**
 * @brief Cria uma nova antena com a frequência e coordenadas indicadas.
 * @param freqAntena Frequência da antena (letra de A-Z).
 * @param x Coordenada X.
 * @param y Coordenada Y.
 * @return Apontador para a antena criada.
 */

ANTENAS* criarAntena(char freqAntena, int x, int y);

/**
 * @brief Cria um novo vértice associado a uma antena.
 * @param antena Apontador para a antena.
 * @return Apontador para o vértice criado.
 */

VERTICE* criarVertice(ANTENAS* antena);

/**
 * @brief Cria um novo grafo com capacidade para um número definido de ligações.
 * @param maxLigacoes Número máximo de vértices que o grafo pode conter.
 * @return Apontador para o grafo criado.
 */

GRAFO* criarGrafo(REDE* rede, char freq);

/**
 * @brief Cria uma nova rede de grafos inicializada a NULL.
 * @return Apontador para a rede criada.
 */

REDE* criarRede();

#pragma endregion

#pragma region Funções de Inserção e Manipulação

/**
 * @brief - insere uma nova antena no grafo com base na frequência e coordenadas indicadas.
 *
 * @param - grafo: apontador para o grafo onde a antena será inserida.
 * @param - frequencia: frequência da antena (caracter A-Z).
 * @param - x: coordenada X da antena.
 * @param - y: coordenada Y da antena.
 * @return - 0 se a inserção for bem-sucedida, 1 em caso de erro.
 */
int inserirAntenaGrafo(GRAFO* grafo, char frequencia, int x, int y);

/**
 * @brief - conecta dois vértices no grafo com uma aresta bidirecional.
 *
 * @param - grafo: apontador para o grafo onde a conexão será feita.
 * @param - x1: coordenada X do primeiro vértice (origem).
 * @param - y1: coordenada Y do primeiro vértice (origem).
 * @param - x2: coordenada X do segundo vértice (destino).
 * @param - y2: coordenada Y do segundo vértice (destino).
 * @return - 0 se a conexão for bem-sucedida, 1 em caso de erro.
 */
int conectarVertices(GRAFO* grafo, int x1, int y1, int x2, int y2);

/**
 * @brief - mostra a rede de grafos numa matriz, baseada nas frequências presentes.
 *
 * @param - redeGrafos: apontador para a rede de grafos a mostrar.
 * @return - 0 se a operação for bem-sucedida, 1 em caso de erro.
 */
int mostrarRedeGrafos(REDE* redeGrafos);

/**
 * @brief - procura um grafo na rede pela sua frequência.
 *
 * @param - rede: apontador para a rede de grafos.
 * @param - freq: frequência do grafo a procurar.
 * @return - apontador para o grafo encontrado ou NULL se não existir.
 */
GRAFO* encontrarGrafoPorFrequencia(REDE* rede, char freq);

/**
 * @brief - carrega grafos a partir de um ficheiro de texto para a rede.
 *
 * @param - rede: apontador para a rede onde os grafos serão inseridos.
 * @param - filename: nome do ficheiro de texto a ler.
 * @return - 0 se a carga for bem-sucedida, 1 em caso de erro.
 */
int carregaGrafo(REDE* rede, char* filename);

/**
 * @brief - procura um vértice no grafo pela sua posição (x, y).
 *
 * @param - grafo: apontador para o grafo onde a procura será feita.
 * @param - x: coordenada X do vértice procurado.
 * @param - y: coordenada Y do vértice procurado.
 * @return - apontador para o vértice encontrado ou NULL se não existir.
 */
VERTICE* encontrarVertice(GRAFO* grafo, int x, int y);

/**
 * @brief - procura um vértice no grafo pela sua posição (x, y).
 *
 * @param - grafo: apontador para o grafo onde a procura será feita.
 * @param - x: coordenada X do vértice procurado.
 * @param - y: coordenada Y do vértice procurado.
 * @return - apontador para o vértice encontrado ou NULL se não existir.
 */
VERTICE* encontrarVertice(GRAFO* grafo, int x, int y);

/**
 * @brief - guarda as arestas de um vértice num ficheiro.
 *
 * @param - head: apontador para a lista de arestas a guardar.
 * @param - fp: apontador para o ficheiro aberto para escrita.
 * @param - xOrigem: coordenada X do vértice origem das arestas.
 * @param - yOrigem: coordenada Y do vértice origem das arestas.
 * @return - número de arestas guardadas com sucesso.
 */
int guardarArestas(ARESTA* head, FILE* fp, int xOrigem, int yOrigem);

/**
 * @brief - guarda o grafo num ficheiro binário.
 *
 * @param - head: apontador para a lista de vértices do grafo.
 * @param - filename: nome do ficheiro onde será guardado o grafo.
 * @return - true se a operação foi bem sucedida, false caso contrário.
 */
bool guardarGrafoBin(VERTICE* head, char* filename);

/**
 * @brief - verifica se existe uma conexão entre dois vértices especificados pelas suas coordenadas.
 *
 * @param - grafo: apontador para o grafo.
 * @param - xOrigem: coordenada X do vértice origem.
 * @param - yOrigem: coordenada Y do vértice origem.
 * @param - xDestino: coordenada X do vértice destino.
 * @param - yDestino: coordenada Y do vértice destino.
 * @return - true se existe conexão, false caso contrário.
 */
bool existeConexaoEntreVertices(GRAFO* grafo, int xOrigem, int yOrigem, int xDestino, int yDestino);

/**
 * @brief - verifica se uma aresta aponta para o destino especificado.
 *
 * @param - head: apontador para a lista de arestas.
 * @param - xDestino: coordenada X do vértice destino.
 * @param - yDestino: coordenada Y do vértice destino.
 * @return - true se existir uma aresta para o destino, false caso contrário.
 */
bool existeConexao(ARESTA* head, int xDestino, int yDestino);



#pragma endregion

#pragma region Funções de Eliminação

/**
 * @brief Liberta toda a memória associada a um grafo, incluindo os seus vértices e arestas.
 *
 * Esta função destrói um grafo completo, removendo todos os vértices e as respetivas listas de arestas,
 * libertando toda a memória ocupada.
 *
 * @param grafo - apontador para o grafo a destruir.
 * @return - NULL, indicando que o grafo foi destruído.
 */
GRAFO* destruirGrafo(GRAFO* grafo);

/**
 * @brief Liberta toda a memória associada à lista de vértices, incluindo as listas de arestas e as antenas.
 *
 * Para cada vértice da lista, esta função remove todas as arestas adjacentes, liberta a memória da antena
 * associada e do próprio vértice.
 *
 * @param lista - apontador para o início da lista de vértices a destruir.
 * @return - NULL, indicando que a lista foi destruída.
 */
VERTICE* destruirVertices(VERTICE* lista);

/**
 * @brief Liberta toda a memória associada à lista de arestas.
 *
 * Para cada aresta da lista, esta função liberta a memória ocupada pela estrutura da aresta.
 *
 * @param lista - apontador para o início da lista de arestas a destruir.
 * @return - NULL, indicando que a lista foi destruída.
 */
ARESTA* destruirArestas(ARESTA* lista);

#pragma endregion

#pragma region Funções Auxiliares para BFS

/**
 * @brief - cria uma fila vazia.
 *
 * @param - fila: apontador para a estrutura da fila a inicializar.
 */
void criarFila(FILA* fila);

/**
 * @brief - verifica se a fila está vazia.
 *
 * @param - fila: apontador para a fila a verificar.
 * @return - true se a fila estiver vazia (frente == NULL), caso contrário false.
 */
bool filaVazia(FILA* fila);

/**
 * @brief - enfileira um vértice na fila.
 *
 * @param - fila: apontador para a fila.
 * @param - vertice: apontador para o vértice a inserir.
 * @return - true se o vértice foi inserido com sucesso, false em caso de falha de alocação.
 */
bool enfilarVertice(FILA* fila, VERTICE* vertice);

/**
 * @brief - desenfileira o vértice da frente da fila.
 *
 * @param - fila: apontador para a fila.
 * @return - apontador para o vértice removido, ou NULL se a fila estiver vazia.
 */
VERTICE* desenfilarVertice(FILA* fila);

/**
 * @brief - executa uma busca em largura (BFT) no grafo a partir da antena inicial indicada.
 *
 * @param - grafo: apontador para o grafo onde será executada a busca.
 * @param - x: coordenada X da antena inicial.
 * @param - y: coordenada Y da antena inicial.
 * @param - count: apontador para inteiro onde será armazenado o número de vértices visitados.
 * @return - 0 em caso de sucesso, 1 se grafo inválido ou antena inicial não encontrada.
 */
int BFT(GRAFO* grafo, int x, int y, int* count);

/**
 * @brief - valida o grafo, verificando se não é NULL e se tem vértices.
 *
 * @param - grafo: apontador para o grafo a validar.
 * @return - true se o grafo for válido, false caso contrário.
 */
bool validarGrafo(GRAFO* grafo);

/**
 * @brief - reinicia o campo visitado de todos os vértices para 0 (não visitado).
 *
 * @param - grafo: apontador para a lista de vértices.
 * @return - apontador para a lista de vértices após reiniciar o campo visitado.
 */
VERTICE* resetarVisitados(VERTICE* grafo);

#pragma endregion
