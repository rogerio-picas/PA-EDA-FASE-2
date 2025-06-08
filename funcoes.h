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

  /** @def MAX_ANTENAS
   *  @brief N�mero m�ximo de antenas.
   */
#define MAX_ANTENAS 50

#define MAX_VERTICES 20


#pragma region Estruturas

   /**
	* @struct Antenas
	* @brief Representa uma antena com frequ�ncia e posi��o na matriz.
	*/
typedef struct Antenas {
	char frequencia; /** Frequ�ncia da antena (A-Z) */
	int x;           /** Coordenada X na matriz */
	int y;           /** Coordenada Y na matriz */
} ANTENAS;

/**
 * @struct Aresta
 * @brief Representa uma aresta entre v�rtices (antenas).
 */
typedef struct Aresta {
	struct Vertice* destino;
	struct Aresta* prox;
} ARESTA;

/**
 * @struct Vertice
 * @brief Representa um v�rtice no grafo, contendo uma antena e as suas liga��es.
 */
typedef struct Vertice {
	ANTENAS* infoAntenas;    /** Informa��o da antena associada ao v�rtice */
	ARESTA* adjacentes;      /** Lista de arestas (liga��es a outras antenas) */
	int visitado;            /** Campo utilizado para marcar se o vertice foi visitado ou n�o */
	struct Vertice* prox;    /** Necess�rio para percorrer os v�rtices para procurar */
} VERTICE;


typedef struct Grafo {
	char frequencia;
	VERTICE* vertices;
	int maxVertices;         /** N�mero m�ximo de liga��es/v�rtices */
	int numVertices;         /** N�mero atual de v�rtices no grafo */
	struct Grafo* prox;
} GRAFO;

/**
 * @struct Fila
 * @brief  Implementa uma fila din�mica de v�rtices para buscas em grafos (BFS)
 *
 * A fila usa o campo 'prox' em cada VERTICE para encadear os v�rtices na estrutura
 */
typedef struct Fila {
	VERTICE* frente;  /**< Apontador para o v�rtice no in�cio da fila */
	VERTICE* tras;    /**< Apontador para o v�rtice no fim da fila */
} FILA;
/**
 * @struct Rede_Grafos
 * @brief Conjunto de grafos separados por frequ�ncia.
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
 * @brief Insere uma nova antena na rede de grafos conforme a frequ�ncia
 * @param rede Apontador para a rede
 * @param freqAntena Frequ�ncia da antena
 * @param x Coordenada X
 * @param y Coordenada Y
 * @return 0 se inser��o for bem-sucedida, 1 em caso de erro
 */
int inserirAntenaRedeGrafos(GRAFO* grafo, char frequencia, int x, int y);

/**
 * @brief Conecta dois v�rtices num grafo com uma aresta bidirecional
 * @param grafo Apontador para o grafo
 * @param origem �ndice do v�rtice de origem
 * @param destino �ndice do v�rtice de destino
 * @return 0 se bem-sucedido, 1 em caso de erro
 */
int conectarVertices(GRAFO* grafo, int x1, int y1, int x2, int y2);

/**
 * @brief Mostra a rede de grafos numa matriz, com base nas frequ�ncias especificadas
 * @param redeGrafos Apontador para a rede
 * @param freqsAntenas Vetor de frequ�ncias a considerar
 * @param numFreqs N�mero de frequ�ncias no vetor
 * @return 0 se bem-sucedido, 1 se erro
 */
int mostrarRedeGrafos(REDE* redeGrafos);

GRAFO* encontrarGrafoPorFrequencia(REDE* rede, char freq);

/**
 * @brief L� um ficheiro de texto com a matriz de antenas e carrega na rede
 * @param rede Apontador para a rede
 * @param ficheiroTexto Nome do ficheiro
 * @return 0 se bem-sucedido, 1 em caso de erro
 */
int lerFicheiroTexto(REDE* rede, char* filename);

VERTICE* encontrarVertice(GRAFO* grafo, int x, int y);


int guardarArestas(ARESTA* head, FILE* fp, int xOrigem, int yOrigem);


bool guardarGrafoBin(VERTICE* head, char* filename);


#pragma endregion

#pragma region Fun��es Auxiliares para BFS
/**
 * @brief      Inicializa uma fila vazia
 * @param[in]  fila   Apontador para a estrutura da fila que ser� inicializada
 */
void criarFila(FILA* fila);

/**
 * @brief      Verifica se a fila est� vazia
 * @param[in]  fila   Apontador para a fila a ser verificada
 * @return     true se a fila estiver vazia (frente == NULL), caso contr�rio false
 */
bool filaVazia(FILA* fila);

/**
 * @brief      Enfila um v�rtice na fila
 * @param[in]  fila      Apontador para a fila
 * @param[in]  vertice   Apontador para o v�rtice a inserir
 * @return     true se o v�rtice foi enfileirado com sucesso, false em caso de falha de aloca��o
 */
bool enfilarVertice(FILA* fila, VERTICE* vertice);

/**
 * @brief      Desenfileira o v�rtice que est� na frente da fila
 * @param[in]  fila   Apontador para a fila
 * @return     Apontador para o v�rtice removido, ou NULL se a fila estiver vazia
 */
VERTICE* desenfilarVertice(FILA* fila);

/**
 * @brief      Executa a busca em largura (BFS) a partir de uma antena inicial
 * @param[in]  grafo       Apontador para o grafo onde a BFS ser� realizada
 * @param[in]  x           Coordenada X da antena inicial
 * @param[in]  y           Coordenada Y da antena inicial
 * @param[out] count       Endere�o de um inteiro onde ser� armazenado o n�mero de v�rtices visitados.
 * @param[out] resultado   Vetor (tamanho MAX_ANTENAS) onde ser�o guardados os �ndices dos v�rtices visitados,
 *                         na ordem em que foram alcan�ados pela BFS
 * @return     0 em caso de sucesso (BFS executada), 1 em caso de grafo inv�lido ou antena inicial n�o encontrada
 */
int BFS(GRAFO* grafo, int x, int y, int* count);

/**
 * @brief      Verifica se um grafo � v�lido (n�o � NULL e cont�m pelo menos um v�rtice)
 * @param[in]  grafo   Apontador para o grafo a ser validado
 * @return     true se o grafo for v�lido (n�o NULL e numVertices > 0), caso contr�rio false
 */
bool validarGrafo(GRAFO* grafo);

#pragma endregion
