/**
 * @file funcoes.c
 * @brief Implementa��o de fun��es para gest�o de grafos de antenas, incluindo cria��o, manipula��o,
 *        procura em largura (BFS) e valida��o de grafos.
 *
 * Este ficheiro cont�m:
 * - Cria��o de tipos ANTENAS, VERTICE, GRAFO e REDE
 * - Fun��es para inserir antenas e conectar v�rtices
 * - Leitura de ficheiro de texto para carregar antenas em grafos
 * - Implementa��o de fila din�mica para BFS
 * - Fun��o BFS que retorna �ndices de v�rtices visitados
 * - Valida��o de consist�ncia do grafo
 *
 * @author Rog�rio Picas
 * @date 07-05-2025
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include "funcoes.h"
#include <stdbool.h>


#pragma region Fun��es de Cria��o

 /**
  * @brief      Cria uma nova antena com frequ�ncia e coordenadas fornecidas
  * @param[in]  freqAntena  Caracter que representa a frequ�ncia da antena ('A' a 'Z')
  * @param[in]  x - Coordenada X da antena (0..MAX_DIM-1)
  * @param[in]  y - Coordenada Y da antena (0..MAX_DIM-1)
  * @return     Apontador para a antena alocada, ou NULL em caso de falha de malloc
  */
ANTENAS* criarAntena(char freqAntena, int x, int y) {

	ANTENAS* auxVar;
	auxVar = (ANTENAS*)malloc(sizeof(ANTENAS));
	if (auxVar == NULL)
	{
		return NULL;
	}
		auxVar->frequencia = freqAntena;
		auxVar->x = x;
		auxVar->y = y;

		return auxVar;
}

/**
 * @brief      Cria um novo v�rtice associado a uma antena
 * @param[in]  antena  Apontador para a estrutura ANTENAS criada anteriormente
 * @return     Apontador para o v�rtice alocado, ou NULL em caso de falha de malloc
 */
VERTICE* criarVertice(ANTENAS* antena) {

	VERTICE* novoVertice = (VERTICE*)malloc(sizeof(VERTICE));
	if (!novoVertice)
	{
		return NULL;
	}
	novoVertice->infoAntenas = antena;
	novoVertice->adjacentes = NULL;

	return novoVertice;
}

/**
 * @brief      Cria um grafo vazio com capacidade m�xima de v�rtices especificada
 * @param[in]  maxLigacoes  N�mero m�ximo de v�rtices
 * @return     Apontador para o grafo alocado, ou NULL em caso de falha de malloc
 */
GRAFO* criarGrafo(int maxLigacoes) {
	GRAFO* grafo = (GRAFO*)malloc(sizeof(GRAFO));
	if (grafo == NULL)
	{
		return NULL;
	}
	grafo->vertices = (VERTICE**)malloc(sizeof(VERTICE*) * maxLigacoes);
	if (grafo->vertices == NULL)
	{
		free(grafo);
		return NULL;
	}
	for (int i = 0; i < grafo->maxLigacoes; i++)
	{
		grafo->vertices[i] = NULL;
	}
	grafo->numVertices = 0;
	grafo->maxLigacoes = maxLigacoes;
	
	return grafo;
}

/**
 * @brief      Cria uma nova rede de grafos, inicializando todos os grafos a NULL
 * @return     Apontador para a rede alocada, ou NULL em caso de falha de malloc
 */
REDE* criarRede() {
	REDE* rede = (REDE*)malloc(sizeof(REDE));
	if (!rede) return NULL;

	for (int i = 0; i < MAX_FREQ; i++)
		rede->grafos[i] = NULL;

	return rede;
}

#pragma endregion

#pragma region Fun��es de Manipula��o

/**
 * @brief      Insere uma antena no grafo correspondente � frequ�ncia fornecida
 *             Se o grafo n�o existir, cria-o. Se j� existir antena nas mesmas coordenadas, retorna erro
 * @param[in]  rede        -> Apontador para a rede de grafos
 * @param[in]  freqAntena  -> Frequ�ncia da antena (caracter 'A'..'Z')
 * @param[in]  x           -> Coordenada X da antena
 * @param[in]  y           -> Coordenada Y da antena
 * @return     0 caso sucesso, 1 em caso de erro (frequ�ncia inv�lida, falha de malloc, ou antena duplicada)
 */
int inserirAntenaRedeGrafos(REDE* rede, char freqAntena, int x, int y){

	int indiceGrafo = freqAntena - 'A'; // Converte uma letra num indice num�rico de 0 a 25

	if (indiceGrafo < 0 || indiceGrafo > MAX_FREQ)
	{

		return 1;
	}

	// Caso n�o exista o grafo para a frequencia, cria-lo

	if (rede->grafos[indiceGrafo]==NULL)
	{
		rede->grafos[indiceGrafo] = (GRAFO*)malloc(sizeof(GRAFO));
		if (rede->grafos[indiceGrafo] == NULL)
		{
			return 1;
		}
		rede->grafos[indiceGrafo]->numVertices = 0;
		rede->grafos[indiceGrafo]->maxLigacoes = MAX_DIM;
		rede->grafos[indiceGrafo]->vertices = (VERTICE**)malloc(sizeof(VERTICE*) * MAX_DIM);
	}

	GRAFO* grafo = rede->grafos[indiceGrafo];

	// Verifica se a antena j� existe
	for (int i = 0; i < grafo->numVertices; i++) {
		if (grafo->vertices[i]->infoAntenas->x == x && grafo->vertices[i]->infoAntenas->y == y)
			return 1; // J� existe
	}

	ANTENAS* novaAntena = criarAntena(freqAntena, x, y);
	if (!novaAntena) 
	{
		return 1;
	}

	VERTICE* novoVertice = criarVertice(novaAntena);
	if (novoVertice == NULL)
	{
		free(novaAntena);
		return 1;
	}
	 
	if (grafo->vertices == NULL)
	{
		return 1;
	}
	// Inserir as antenas de forma ordenada atrav�s das coordenadas
	// Encontrar a posi��o correta
	int posicao = 0;
	while (posicao < grafo->numVertices) {
		int xExistente = grafo->vertices[posicao]->infoAntenas->x;
		int yExistente = grafo->vertices[posicao]->infoAntenas->y;

		if (x < xExistente || (x == xExistente && y < yExistente))
		{
			break;
		}
		posicao++;
	}

	// Deslocar os elementos para a direita
	for (int i = grafo->numVertices; i > posicao; i--)
	{
		grafo->vertices[i] = grafo->vertices[i - 1];
	}

	grafo->vertices[posicao] = novoVertice;
	grafo->numVertices++;

	return 0;
    
}

/**
 * @brief      Conecta dois v�rtices no grafo criando arestas bidirecionais
 * @param[in]  grafo   -> Apontador para o grafo
 * @param[in]  origem  -> �ndice do v�rtice de origem no vetor vertices[]
 * @param[in]  destino -> �ndice do v�rtice de destino no vetor vertices[]
 * @return     0 caso sucesso, 1 em caso de erro (grafo NULL ou �ndices inv�lidos ou falha de malloc)
 */
int conectarVertices(GRAFO* grafo, int origem, int destino){
if (grafo == NULL)
{
	return 1;
}
if (origem < 0 || destino < 0 || origem >= grafo->numVertices || destino >= grafo->numVertices)
{
	return 1;
}
// Cria aresta de origem para destino
ARESTA* novaAresta = (ARESTA*)malloc(sizeof(ARESTA));
if (novaAresta == NULL)
{
	return 1;
}
novaAresta->destino = destino;
novaAresta->prox = grafo->vertices[origem]->adjacentes; 
grafo->vertices[origem]->adjacentes = novaAresta; 

// Cria aresta de destino para origem
ARESTA* novaArestaInversa = (ARESTA*)malloc(sizeof(ARESTA));
if (novaArestaInversa == NULL)
{
	return 1;
}
novaArestaInversa->destino = origem;
novaArestaInversa->prox = grafo->vertices[destino]->adjacentes;
grafo->vertices[destino]->adjacentes = novaArestaInversa; 

return 0; //Sucesso
}

/**
 * @brief      Verifica se existe uma antena no grafo com mesma frequ�ncia e coordenadas
 * @param[in]  grafo       -> Apontador para o grafo
 * @param[in]  freqAntena  -> Frequ�ncia da antena a pesquisar
 * @param[in]  x           -> Coordenada X
 * @param[in]  y           -> Coordenada Y
 * @return     0 se existir (antena encontrada), 1 se n�o existir
 */
int antenaExiste(GRAFO* grafo, char freqAntena, int x, int y) {
	if (grafo->vertices == NULL)
	{
		return 1;
	}
	for (int i = 0; i < grafo->numVertices; i++)
	{
		ANTENAS* auxVar = grafo->vertices[i]->infoAntenas;
		if (auxVar->frequencia == freqAntena && auxVar->x == x && auxVar->y == y)
		{
			return 0;
		}
	}
	return 0;
}

/**
 * @brief      Mostra a rede de grafos numa matriz de dimens�o MAX_DIM x MAX_DIM
 *             usando os caracteres das frequ�ncias para indicar antenas
 * @param[in]  redeGrafos     -> Apontador para a rede de grafos
 * @param[in]  freqsAntenas   -> Vetor de frequ�ncias a considerar (caracteres 'A'..'Z')
 * @param[in]  numFreqs       -> N�mero de frequ�ncias v�lidas em freqsAntenas[]
 * @return     0 caso sucesso, 1 em caso de redeGrafos NULL
 */
int mostrarRedeGrafos(REDE* redeGrafos, char freqsAntenas[], int numFreqs) {

	if (redeGrafos == NULL)
	{
		return 1;
	}
	char matriz[MAX_DIM][MAX_DIM];

	for (int i = 0; i < MAX_DIM; i++) {
		for (int j = 0; j < MAX_DIM; j++) {
			matriz[i][j] = '.';
		}
	}
	for (int i = 0; i < numFreqs; i++) {
			char freq = freqsAntenas[i];
			int indiceGrafo = freq - 'A';
			GRAFO* grafo = redeGrafos->grafos[indiceGrafo];

			if (grafo == NULL) continue;

			for (int i = 0; i < grafo->numVertices; i++){
				int x = grafo->vertices[i]->infoAntenas->x;
				int y = grafo->vertices[i]->infoAntenas->y;
				char freq = grafo->vertices[i]->infoAntenas->frequencia;
				if (x >= 0 && x < MAX_DIM && y >= 0 && y < MAX_DIM)
				{
					matriz[x][y] = freq;
				}
			}
	}
		for (int i = 0; i < MAX_DIM; i++) {
			for (int j = 0; j < MAX_DIM; j++)
			{
				printf("%c", matriz[i][j]);
			}
			printf("\n");
		}

		return 1;
}

/**
 * @brief      L� um ficheiro de texto contendo uma matriz de caracteres que representa antenas
 *             em posi��es X,Y. Insere cada caracter 'A'..'Z' como nova antena no grafo respectivo
 * @param[in]  rede            -> Apontador para a rede de grafos
 * @param[in]  ficheiroTexto   -> Nome do ficheiro de texto a ler
 * @return     0 em sucesso, -1 em caso de falha ao abrir o ficheiro
 */
int lerFicheiroTexto(REDE* rede, char* ficheiroTexto) {
	FILE* fp = fopen(ficheiroTexto, "r");
	if (fp == NULL)
	{
		return -1; // Se retornar -1 existe erro ao abrir o ficheiro de texto
	}

	char linha[MAX_DIM + 2];
	int resultado;
	for (int i = 0; i < MAX_DIM; i++)
	{
		if (fgets(linha, sizeof(linha), fp) == NULL) break;

		for (int j = 0; j < MAX_DIM; j++)
		{
			if (linha[j] == '\n' || linha[j] == '\0') break;
			char frequencia = linha[j];
			int x = i;
			int y = j;
			// Assim que encontrada a frequencia os valores de x e y ser�o encaminhados para a fun��o de inserirAntenasRedeGrafos
			if (frequencia >= 'A' && frequencia <= 'Z')
			{
				inserirAntenaRedeGrafos(rede, frequencia, x, y);
			}
		}
	}

	fclose(fp);
	return 0;
}

/**
 * @brief      Encontra o �ndice de um v�rtice no grafo com base nas coordenadas
 * @param[in]  grafo  -> Apontador para o grafo
 * @param[in]  x      -> Coordenada X do v�rtice
 * @param[in]  y      -> Coordenada Y do v�rtice
 * @return     �ndice do v�rtice (0..numVertices-1) se encontrado, ou -1 caso contr�rio
 */
int encontrarIndiceVertice(GRAFO* grafo, int x, int y) {

	for (int i = 0; i < grafo->numVertices; i++)
	{
		if (grafo->vertices[i]->infoAntenas->x == x && grafo->vertices[i]->infoAntenas->y) {
			return i;
		}
	}
	return -1;
}

/**
 * @brief      Verifica se um grafo � v�lido (n�o NULL, vetor de v�rtices OK e valores de numVertices)
 * @param[in]  grafo  -> Apontador para o grafo a validar
 * @return     true se o grafo for v�lido, false caso contr�rio
 */
bool validarGrafo(GRAFO* grafo) {
	if (!grafo) return false;
	if (!grafo->vertices) return false;
	if (grafo->numVertices <= 0 || grafo->numVertices > grafo->maxLigacoes) return false;
	for (int i = 0; i < grafo->numVertices; i++) {
		if (!grafo->vertices[i]) return false;
		if (!grafo->vertices[i]->infoAntenas) return false;
	}
	return true;
}

#pragma endregion

#pragma region Fun��es Auxiliares para o BFS
/**
 * @brief      Inicializa a fila definindo frente e tr�s como NULL
 * @param[in]  fila  -> Apontador para a estrutura FILA a inicializar
 */
void criarFila(FILA* fila) {
	fila->frente = NULL;
	fila->tras = NULL;
}

/**
 * @brief      Verifica se a fila est� vazia (frente == NULL)
 * @param[in]  fila  -> Apontador para a fila
 * @return     true se estiver vazia, false caso contr�rio
 */
bool filaVazia(FILA* fila) {
	return(fila->frente == NULL);
	return true;
}

/**
 * @brief      Insere um v�rtice na fila
 *             Usa o campo vertice->prox para encadear
 * @param[in]  fila     -> Apontador para a fila
 * @param[in]  vertice  -> Apontador para o v�rtice a enfileirar
 * @return     true sempre que aloca e insere com sucesso, false em falha de malloc
 */
bool enfilarVertice(FILA* fila, VERTICE* vertice) {
	// Se a fila estiver vazia o primeiro vertice ir� ser o anterior e o proximo simultaneamente
	if (filaVazia(fila))
	{
		fila->frente = vertice;
		fila->tras = vertice;
	}
	else
	{
		fila->tras->prox = vertice;
		fila->tras = vertice;
	}
	return true;
}

/**
 * @brief      Remove o v�rtice da frente da fila devolve-o
 * @param[in]  fila  -> Apontador para a fila
 * @return     Apontador para o v�rtice removido, ou NULL se a fila estiver vazia
 */
VERTICE* desenfilarVertice(FILA* fila) {
	if (filaVazia(fila))
	{
		return false;
	}
	VERTICE* vertice;
	vertice = fila->frente;
	fila->frente = vertice->prox;
	if (fila->frente == NULL)
	{
		fila->tras = NULL;
	}
	vertice->prox = NULL;
	
	return vertice;
}

/**
 * @brief      Executa a busca em largura (BFS) no grafo, a partir da antena em (x,y)
 *             Preenche o array resultado[] com os �ndices dos v�rtices visitados, em ordem de alcance
 * @param[in]  grafo    -> Apontador para o grafo onde a BFS ser� realizada
 * @param[in]  x        -> Coordenada X da antena inicial
 * @param[in]  y        -> Coordenada Y da antena inicial
 * @param[out] count    -> Endere�o de inteiro onde ser� armazenado o n�mero de v�rtices visitados
 * @param[out] resultado  Array (tamanho MAX_ANTENAS) que ir� conter os �ndices dos v�rtices visitados
 * @return     0 caso sucesso; 1 se o grafo for NULL, vazio, ou se a antena inicial n�o existir
 */
int BFS(GRAFO* grafo, int x, int y, int* count, int resultado[MAX_ANTENAS]) {
	if (!grafo|| grafo->numVertices == 0)
	{
		return 1;
	}
	*count = 0;

	// � necess�rio encontra o �ndice do v�rtice
	int indice = encontrarIndiceVertice(grafo, x, y);

	if (indice == -1) {
		// v�rtice de partida n�o est� no grafo
		return 1;
	}

	FILA fila;
	criarFila(&fila);

	for (int i = 0; i < grafo->numVertices; i++)
	{
		grafo->vertices[i]->visitado = 0;
		// Para certificar que o campo proximo � nulo antes come�ar a procura
		grafo->vertices[i]->prox = NULL; 
	}

	VERTICE* inicio = grafo->vertices[indice];
	inicio->visitado = 1;
	enfilarVertice(&fila, inicio);

	while (!filaVazia(&fila)) {
		VERTICE* atual = desenfilarVertice(&fila);

		int indiceAtual = encontrarIndiceVertice(grafo, atual->infoAntenas->x, atual->infoAntenas->y);
		resultado[*count] = indiceAtual;
		(*count)++;

		ARESTA* adj = atual->adjacentes;

		while (adj) {
			VERTICE* vizinho = grafo->vertices[adj->destino];
			if (!vizinho->visitado)
			{
				vizinho->visitado = 1;
				enfilarVertice(&fila, vizinho);
			}
			adj = adj->prox;
		}

	}
	return 0;
}

#pragma endregion