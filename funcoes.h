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

  /** @def MAX_ANTENAS
   *  @brief Número máximo de antenas.
   */
#define MAX_ANTENAS 50

#define MAX_VERTICES 20


#pragma region Estruturas

   /**
	* @struct Antenas
	* @brief Representa uma antena com frequência e posição na matriz.
	*/
typedef struct Antenas {
	char frequencia; /** Frequência da antena (A-Z) */
	int x;           /** Coordenada X na matriz */
	int y;           /** Coordenada Y na matriz */
} ANTENAS;

/**
 * @struct Aresta
 * @brief Representa uma aresta entre vértices (antenas).
 */
typedef struct Aresta {
	struct Vertice* destino;
	struct Aresta* prox;
} ARESTA;

/**
 * @struct Vertice
 * @brief Representa um vértice no grafo, contendo uma antena e as suas ligações.
 */
typedef struct Vertice {
	ANTENAS* infoAntenas;    /** Informação da antena associada ao vértice */
	ARESTA* adjacentes;      /** Lista de arestas (ligações a outras antenas) */
	int visitado;            /** Campo utilizado para marcar se o vertice foi visitado ou não */
	struct Vertice* prox;    /** Necessário para percorrer os vértices para procurar */
} VERTICE;


typedef struct Grafo {
	char frequencia;
	VERTICE* vertices;
	int maxVertices;         /** Número máximo de ligações/vértices */
	int numVertices;         /** Número atual de vértices no grafo */
	struct Grafo* prox;
} GRAFO;

/**
 * @struct Fila
 * @brief  Implementa uma fila dinâmica de vértices para buscas em grafos (BFS)
 *
 * A fila usa o campo 'prox' em cada VERTICE para encadear os vértices na estrutura
 */
typedef struct Fila {
	VERTICE* frente;  /**< Apontador para o vértice no início da fila */
	VERTICE* tras;    /**< Apontador para o vértice no fim da fila */
} FILA;
/**
 * @struct Rede_Grafos
 * @brief Conjunto de grafos separados por frequência.
 */
typedef struct Rede_Grafos {
	GRAFO* listaGrafos;
} REDE;

typedef struct VerticeFicheiro {
	char frequencia;
	int x;
	int y;
}VerticeFICHEIRO;

typedef struct ArestasFicheiro {
	int xOrigem;
	int yOrigem;
	int xDestino;
	int yDestino;
}ArestasFICHEIRO;

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
 * @brief Insere uma nova antena na rede de grafos conforme a frequência
 * @param rede Apontador para a rede
 * @param freqAntena Frequência da antena
 * @param x Coordenada X
 * @param y Coordenada Y
 * @return 0 se inserção for bem-sucedida, 1 em caso de erro
 */
int inserirAntenaRedeGrafos(GRAFO* grafo, char frequencia, int x, int y);

/**
 * @brief Conecta dois vértices num grafo com uma aresta bidirecional
 * @param grafo Apontador para o grafo
 * @param origem Índice do vértice de origem
 * @param destino Índice do vértice de destino
 * @return 0 se bem-sucedido, 1 em caso de erro
 */
int conectarVertices(GRAFO* grafo, int x1, int y1, int x2, int y2);

/**
 * @brief Mostra a rede de grafos numa matriz, com base nas frequências especificadas
 * @param redeGrafos Apontador para a rede
 * @param freqsAntenas Vetor de frequências a considerar
 * @param numFreqs Número de frequências no vetor
 * @return 0 se bem-sucedido, 1 se erro
 */
int mostrarRedeGrafos(REDE* redeGrafos);

GRAFO* encontrarGrafoPorFrequencia(REDE* rede, char freq);

/**
 * @brief Lê um ficheiro de texto com a matriz de antenas e carrega na rede
 * @param rede Apontador para a rede
 * @param ficheiroTexto Nome do ficheiro
 * @return 0 se bem-sucedido, 1 em caso de erro
 */
int lerFicheiroTexto(REDE* rede, char* filename);

VERTICE* encontrarVertice(GRAFO* grafo, int x, int y);


int guardarArestas(ARESTA* head, FILE* fp, int xOrigem, int yOrigem);


bool guardarGrafoBin(VERTICE* head, char* filename);


#pragma endregion

#pragma region Funções Auxiliares para BFS
/**
 * @brief      Inicializa uma fila vazia
 * @param[in]  fila   Apontador para a estrutura da fila que será inicializada
 */
void criarFila(FILA* fila);

/**
 * @brief      Verifica se a fila está vazia
 * @param[in]  fila   Apontador para a fila a ser verificada
 * @return     true se a fila estiver vazia (frente == NULL), caso contrário false
 */
bool filaVazia(FILA* fila);

/**
 * @brief      Enfila um vértice na fila
 * @param[in]  fila      Apontador para a fila
 * @param[in]  vertice   Apontador para o vértice a inserir
 * @return     true se o vértice foi enfileirado com sucesso, false em caso de falha de alocação
 */
bool enfilarVertice(FILA* fila, VERTICE* vertice);

/**
 * @brief      Desenfileira o vértice que está na frente da fila
 * @param[in]  fila   Apontador para a fila
 * @return     Apontador para o vértice removido, ou NULL se a fila estiver vazia
 */
VERTICE* desenfilarVertice(FILA* fila);

/**
 * @brief      Executa a busca em largura (BFS) a partir de uma antena inicial
 * @param[in]  grafo       Apontador para o grafo onde a BFS será realizada
 * @param[in]  x           Coordenada X da antena inicial
 * @param[in]  y           Coordenada Y da antena inicial
 * @param[out] count       Endereço de um inteiro onde será armazenado o número de vértices visitados.
 * @param[out] resultado   Vetor (tamanho MAX_ANTENAS) onde serão guardados os índices dos vértices visitados,
 *                         na ordem em que foram alcançados pela BFS
 * @return     0 em caso de sucesso (BFS executada), 1 em caso de grafo inválido ou antena inicial não encontrada
 */
int BFS(GRAFO* grafo, int x, int y, int* count);

/**
 * @brief      Verifica se um grafo é válido (não é NULL e contém pelo menos um vértice)
 * @param[in]  grafo   Apontador para o grafo a ser validado
 * @return     true se o grafo for válido (não NULL e numVertices > 0), caso contrário false
 */
bool validarGrafo(GRAFO* grafo);

#pragma endregion
