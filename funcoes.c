/**
 * @file funcoes.c
 * @brief Implementação de funções para gestão de grafos de antenas, incluindo criação, manipulação,
 *        procura em largura (BFS) e validação de grafos.
 *
 * Este ficheiro contém:
 * - Criação de tipos ANTENAS, VERTICE, GRAFO e REDE
 * - Funções para inserir antenas e conectar vértices
 * - Leitura de ficheiro de texto para carregar antenas em grafos
 * - Implementação de fila dinâmica para BFS
 * - Função BFS que retorna índices de vértices visitados
 * - Validação de consistência do grafo
 *
 * @author Rogério Picas
 * @date 07-05-2025
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include "funcoes.h"
#include <stdbool.h>


#pragma region Funções de Criação

 /**
  * @brief      Cria uma nova antena com frequência e coordenadas fornecidas
  * @param[in]  freqAntena  Caracter que representa a frequência da antena ('A' a 'Z')
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
 * @brief      Cria um novo vértice associado a uma antena
 * @param[in]  antena  Apontador para a estrutura ANTENAS criada anteriormente
 * @return     Apontador para o vértice alocado, ou NULL em caso de falha de malloc
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
 * @brief      Cria um grafo vazio com capacidade máxima de vértices especificada
 * @param[in]  maxLigacoes  Número máximo de vértices
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

#pragma region Funções de Manipulação

/**
 * @brief      Insere uma antena no grafo correspondente à frequência fornecida
 *             Se o grafo não existir, cria-o. Se já existir antena nas mesmas coordenadas, retorna erro
 * @param[in]  rede        -> Apontador para a rede de grafos
 * @param[in]  freqAntena  -> Frequência da antena (caracter 'A'..'Z')
 * @param[in]  x           -> Coordenada X da antena
 * @param[in]  y           -> Coordenada Y da antena
 * @return     0 caso sucesso, 1 em caso de erro (frequência inválida, falha de malloc, ou antena duplicada)
 */
int inserirAntenaRedeGrafos(REDE* rede, char freqAntena, int x, int y){

	int indiceGrafo = freqAntena - 'A'; // Converte uma letra num indice numérico de 0 a 25

	if (indiceGrafo < 0 || indiceGrafo > MAX_FREQ)
	{

		return 1;
	}

	// Caso não exista o grafo para a frequencia, cria-lo

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

	// Verifica se a antena já existe
	for (int i = 0; i < grafo->numVertices; i++) {
		if (grafo->vertices[i]->infoAntenas->x == x && grafo->vertices[i]->infoAntenas->y == y)
			return 1; // Já existe
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
	// Inserir as antenas de forma ordenada através das coordenadas
	// Encontrar a posição correta
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
 * @brief      Conecta dois vértices no grafo criando arestas bidirecionais
 * @param[in]  grafo   -> Apontador para o grafo
 * @param[in]  origem  -> Índice do vértice de origem no vetor vertices[]
 * @param[in]  destino -> Índice do vértice de destino no vetor vertices[]
 * @return     0 caso sucesso, 1 em caso de erro (grafo NULL ou índices inválidos ou falha de malloc)
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
 * @brief      Verifica se existe uma antena no grafo com mesma frequência e coordenadas
 * @param[in]  grafo       -> Apontador para o grafo
 * @param[in]  freqAntena  -> Frequência da antena a pesquisar
 * @param[in]  x           -> Coordenada X
 * @param[in]  y           -> Coordenada Y
 * @return     0 se existir (antena encontrada), 1 se não existir
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
 * @brief      Mostra a rede de grafos numa matriz de dimensão MAX_DIM x MAX_DIM
 *             usando os caracteres das frequências para indicar antenas
 * @param[in]  redeGrafos     -> Apontador para a rede de grafos
 * @param[in]  freqsAntenas   -> Vetor de frequências a considerar (caracteres 'A'..'Z')
 * @param[in]  numFreqs       -> Número de frequências válidas em freqsAntenas[]
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
 * @brief      Lê um ficheiro de texto contendo uma matriz de caracteres que representa antenas
 *             em posições X,Y. Insere cada caracter 'A'..'Z' como nova antena no grafo respectivo
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
			// Assim que encontrada a frequencia os valores de x e y serão encaminhados para a função de inserirAntenasRedeGrafos
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
 * @brief      Encontra o índice de um vértice no grafo com base nas coordenadas
 * @param[in]  grafo  -> Apontador para o grafo
 * @param[in]  x      -> Coordenada X do vértice
 * @param[in]  y      -> Coordenada Y do vértice
 * @return     Índice do vértice (0..numVertices-1) se encontrado, ou -1 caso contrário
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
 * @brief      Verifica se um grafo é válido (não NULL, vetor de vértices OK e valores de numVertices)
 * @param[in]  grafo  -> Apontador para o grafo a validar
 * @return     true se o grafo for válido, false caso contrário
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

#pragma region Funções Auxiliares para o BFS
/**
 * @brief      Inicializa a fila definindo frente e trás como NULL
 * @param[in]  fila  -> Apontador para a estrutura FILA a inicializar
 */
void criarFila(FILA* fila) {
	fila->frente = NULL;
	fila->tras = NULL;
}

/**
 * @brief      Verifica se a fila está vazia (frente == NULL)
 * @param[in]  fila  -> Apontador para a fila
 * @return     true se estiver vazia, false caso contrário
 */
bool filaVazia(FILA* fila) {
	return(fila->frente == NULL);
	return true;
}

/**
 * @brief      Insere um vértice na fila
 *             Usa o campo vertice->prox para encadear
 * @param[in]  fila     -> Apontador para a fila
 * @param[in]  vertice  -> Apontador para o vértice a enfileirar
 * @return     true sempre que aloca e insere com sucesso, false em falha de malloc
 */
bool enfilarVertice(FILA* fila, VERTICE* vertice) {
	// Se a fila estiver vazia o primeiro vertice irá ser o anterior e o proximo simultaneamente
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
 * @brief      Remove o vértice da frente da fila devolve-o
 * @param[in]  fila  -> Apontador para a fila
 * @return     Apontador para o vértice removido, ou NULL se a fila estiver vazia
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
 *             Preenche o array resultado[] com os índices dos vértices visitados, em ordem de alcance
 * @param[in]  grafo    -> Apontador para o grafo onde a BFS será realizada
 * @param[in]  x        -> Coordenada X da antena inicial
 * @param[in]  y        -> Coordenada Y da antena inicial
 * @param[out] count    -> Endereço de inteiro onde será armazenado o número de vértices visitados
 * @param[out] resultado  Array (tamanho MAX_ANTENAS) que irá conter os índices dos vértices visitados
 * @return     0 caso sucesso; 1 se o grafo for NULL, vazio, ou se a antena inicial não existir
 */
int BFS(GRAFO* grafo, int x, int y, int* count, int resultado[MAX_ANTENAS]) {
	if (!grafo|| grafo->numVertices == 0)
	{
		return 1;
	}
	*count = 0;

	// É necessário encontra o índice do vértice
	int indice = encontrarIndiceVertice(grafo, x, y);

	if (indice == -1) {
		// vértice de partida não está no grafo
		return 1;
	}

	FILA fila;
	criarFila(&fila);

	for (int i = 0; i < grafo->numVertices; i++)
	{
		grafo->vertices[i]->visitado = 0;
		// Para certificar que o campo proximo é nulo antes começar a procura
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