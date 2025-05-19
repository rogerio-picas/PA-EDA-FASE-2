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


 /** @def MAX_DIM
  *  @brief Dimens�o da matriz da cidade.
  */

#define MAX_DIM 20

  /** @def MAX_ANTENAS
   *  @brief N�mero m�ximo de antenas.
   */

#define MAX_ANTENAS 50

   /** @def MAX_FREQ
	*  @brief N�mero m�ximo de frequ�ncias (de A a Z).
	*/

#define MAX_FREQ 26


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
    int destino;             /** �ndice do v�rtice de destino */
    struct Aresta* prox;     /** Apontador para a pr�xima aresta (lista ligada) */
} ARESTA;

/**
 * @struct Vertice
 * @brief Representa um v�rtice no grafo, contendo uma antena e as suas liga��es.
 */
typedef struct Vertice {
    ANTENAS* infoAntenas;    /** Informa��o da antena associada ao v�rtice */
    ARESTA* adjacentes;      /** Lista de arestas (liga��es a outras antenas) */
} VERTICE;

/**
 * @struct Grafo
 * @brief Representa um grafo com v�rtices ligados por arestas.
 */
typedef struct Grafo {
    VERTICE** vertices;      /** Lista de apontadores para v�rtices */
    int maxLigacoes;         /** N�mero m�ximo de liga��es/v�rtices */
    int numVertices;         /** N�mero atual de v�rtices no grafo */
} GRAFO;

/**
 * @struct Rede_Grafos
 * @brief Conjunto de grafos separados por frequ�ncia.
 */
typedef struct Rede_Grafos {
    GRAFO* grafos[MAX_FREQ]; /** Vetor de grafos, um por frequ�ncia de 'A' a 'Z' */
} REDE;

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

GRAFO* criarGrafo(int maxLigacoes);

/**
 * @brief Cria uma nova rede de grafos inicializada a NULL.
 * @return Apontador para a rede criada.
 */

REDE* criarRede();

#pragma endregion

#pragma region Fun��es de Inser��o e Manipula��o

/**
 * @brief Insere uma nova antena na rede de grafos conforme a frequ�ncia.
 * @param rede Apontador para a rede.
 * @param freqAntena Frequ�ncia da antena.
 * @param x Coordenada X.
 * @param y Coordenada Y.
 * @return 0 se inser��o for bem-sucedida, 1 em caso de erro.
 */
int inserirAntenaRedeGrafos(REDE* rede, char freqAntena, int x, int y);

/**
 * @brief Conecta dois v�rtices num grafo com uma aresta bidirecional.
 * @param grafo Apontador para o grafo.
 * @param origem �ndice do v�rtice de origem.
 * @param destino �ndice do v�rtice de destino.
 * @return 0 se bem-sucedido, 1 em caso de erro.
 */
int conectarVertices(GRAFO* grafo, int origem, int destino);

/**
 * @brief Verifica se uma antena j� existe no grafo.
 * @param grafo Apontador para o grafo.
 * @param freqAntena Frequ�ncia da antena.
 * @param x Coordenada X.
 * @param y Coordenada Y.
 * @return 0 se existir, 1 se n�o existir.
 */
int antenaExiste(GRAFO* grafo, char freqAntena, int x, int y);

/**
 * @brief Mostra a rede de grafos numa matriz, com base nas frequ�ncias especificadas.
 * @param redeGrafos Apontador para a rede.
 * @param freqsAntenas Vetor de frequ�ncias a considerar.
 * @param numFreqs N�mero de frequ�ncias no vetor.
 * @return 0 se bem-sucedido, 1 se erro.
 */
int mostrarRedeGrafos(REDE* redeGrafos, char freqsAntenas[], int numFreqs);

/**
 * @brief Encontra o �ndice de um v�rtice no grafo com base nas coordenadas.
 * @param grafo Apontador para o grafo.
 * @param x Coordenada X.
 * @param y Coordenada Y.
 * @return �ndice do v�rtice ou -1 se n�o encontrado.
 */
int encontrarIndiceVertice(GRAFO* grafo, int x, int y);

#pragma endregion

/**
 * @brief L� um ficheiro de texto com a matriz de antenas e carrega na rede.
 * @param rede Apontador para a rede.
 * @param ficheiroTexto Nome do ficheiro.
 * @return 0 se bem-sucedido, 1 em caso de erro.
 */
int lerFicheiroTexto(REDE* rede, char* ficheiroTexto);