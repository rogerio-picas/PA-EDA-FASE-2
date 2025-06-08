/**
 * @file funcoes.c
 * @brief Implementação de funções para gestão de grafos de antenas, incluindo criação, manipulação,
 *        procura em largura (BFS) e validação de grafos.
 *
 * @author Rogério Picas
 * @date 07-05-2025
 * @version 1.0
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "funcoes.h"
#include <stdbool.h>



#pragma region Funções de Criação

 /**
  * @brief Cria e inicializa uma nova antena com frequência e coordenadas definidas.
  *
  * Esta função aloca memória para uma nova estrutura ANTENAS, define a frequência
  * e as coordenadas (x, y) da antena e retorna um apontador para a estrutura criada.
  *
  * @param freqAntena Carácter que representa a frequência da antena.
  * @param x Coordenada X da antena.
  * @param y Coordenada Y da antena.
  *
  * @return Apontador para a nova estrutura ANTENAS criada, ou NULL se a alocação de memória falhar.
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
 * @brief Cria um novo vértice contendo uma antena.
 *
 * Esta função aloca memória para um novo vértice do grafo, inicializa os seus
 * campos com valores apropriados e associa o apontador da estrutura de antena
 * fornecida ao campo "infoAntenas" do vértice.
 *
 * @param antena Apontador para a estrutura ANTENAS que será associada ao vértice.
 *
 * @return Um novo vértice criado, ou NULL se a alocação de memória falhar.
 */
VERTICE* criarVertice(ANTENAS* antena) {

	VERTICE* novoVertice = (VERTICE*)malloc(sizeof(VERTICE));
	if (!novoVertice)
	{
		return NULL;
	}
	novoVertice->infoAntenas = antena;
	novoVertice->adjacentes = NULL;
	novoVertice->visitado = 0;
	novoVertice->prox = NULL;
	return novoVertice;
}

/**
 * @brief Cria um novo grafo para uma determinada frequência, ou retorna um existente.
 *
 * Esta função verifica se já existe um grafo com a frequência especificada dentro
 * da rede. Caso exista, retorna o apontador para esse grafo. Caso contrário, cria
 * um novo grafo, inicializa os seus campos e adiciona-o à lista de grafos da rede.
 *
 * @param rede Apontador para a estrutura REDE onde o grafo será criado ou procurado.
 * @param freq Frequência associada ao grafo a ser criado.
 *
 * @return Apontador para o grafo correspondente à frequência especificada,
 *         ou NULL se ocorrer erro na alocação de memória.
 */
GRAFO* criarGrafo(REDE* rede, char freq) {
	if (rede == NULL) return NULL;

	GRAFO* atual = rede->listaGrafos;
	while (atual != NULL) {
		if (atual->frequencia == freq) {
			return atual;
		}
		atual = atual->prox;
	}
	// Caso não exista a frequencia inserida

	GRAFO* novoGrafo = (GRAFO*)malloc(sizeof(GRAFO));
	if (novoGrafo == NULL) return NULL;

	novoGrafo->vertices = NULL;
	novoGrafo->numVertices = 0;
	novoGrafo->frequencia = freq;
	novoGrafo->prox = rede->listaGrafos;
	rede->listaGrafos = novoGrafo;

	return novoGrafo;
}


/**
 * @brief Cria e inicializa uma nova estrutura de rede.
 *
 * Esta função aloca memória para uma nova estrutura REDE, inicializando o campo
 * "listaGrafos" com NULL, indicando que ainda não existem grafos associados.
 *
 * @return Apontador para a nova estrutura REDE criada, ou NULL se a alocação de memória falhar.
 */
REDE* criarRede() {
	REDE* rede = (REDE*)malloc(sizeof(REDE));
	if (rede == NULL)
	{
		return NULL;
	}
	rede->listaGrafos = NULL;
	return rede;
}

#pragma endregion

#pragma region Funções de Manipulação

/**
 * @brief Insere uma nova antena num grafo, caso ainda não exista.
 *
 * Esta função verifica se já existe uma antena no grafo com a mesma frequência
 * e coordenadas. Se não existir, cria uma nova antena e um novo vértice, e insere
 * esse vértice na lista de vértices do grafo. A contagem de vértices é atualizada.
 *
 * @param grafo Apontador para o grafo onde a antena será inserida.
 * @param frequencia Carácter que representa a frequência da antena.
 * @param x Coordenada X da antena.
 * @param y Coordenada Y da antena.
 *
 * @return 0 se a inserção for bem-sucedida, ou 1 se a antena já existir,
 *         se ocorrer erro de alocação de memória, ou se o grafo for inválido.
 */
int inserirAntenaRedeGrafos(GRAFO* grafo, char frequencia, int x, int y) {

	if (!grafo)
	{
		return 1;
	}
	// Verificar se antena existe na lista

	VERTICE* atual = grafo->vertices;

	while (atual) {

		if (atual->infoAntenas && atual->infoAntenas->frequencia == frequencia &&
			atual->infoAntenas->x == x &&
			atual->infoAntenas->y == y) {
			return 1;
		}
		atual = atual->prox;
	}

	ANTENAS* novaAntena = criarAntena(frequencia, x, y);
	if (!novaAntena)
	{
		return 1;
	}

	VERTICE* novoVertice = criarVertice(novaAntena);
	if (!novoVertice)
	{
		free(novaAntena);
		return 1;
	}

	novoVertice->prox = grafo->vertices;
	grafo->vertices = novoVertice;
	grafo->numVertices++;

	return 0;
}

/**
 * @brief Conecta dois vértices de um grafo, criando uma conexão bidirecional entre dois vértices.
 *
 * Esta função procura dois vértices no grafo com base nas suas coordenadas
 * e cria arestas entre eles em ambas as direções, representando uma ligação
 * bidirecional. As arestas são inseridas nas listas de adjacência dos vértices.
 *
 * @param grafo Apontador para o grafo onde os vértices se encontram.
 * @param x1 Coordenada X do primeiro vértice.
 * @param y1 Coordenada Y do primeiro vértice.
 * @param x2 Coordenada X do segundo vértice.
 * @param y2 Coordenada Y do segundo vértice.
 *
 * @return 0 se a conexão for bem-sucedida, ou 1 se algum dos vértices não for encontrado,
 *         se o grafo for inválido, ou se ocorrer falha na alocação de memória.
 */
int conectarVertices(GRAFO* grafo, int x1, int y1, int x2, int y2) {
	if (grafo == NULL || grafo->vertices == NULL)
		return 1;

	VERTICE* v1 = encontrarVertice(grafo, x1, y1);
	VERTICE* v2 = encontrarVertice(grafo, x2, y2);

	if (!v1 || !v2) return 1;

	// Cria aresta de origem para destino
	ARESTA* a1 = (ARESTA*)malloc(sizeof(ARESTA));
	if (a1 == NULL) return 1;
	a1->destino = v2;
	a1->prox = v1->adjacentes;
	v1->adjacentes = a1;

	// Cria aresta de destino para origem
	ARESTA* a2 = (ARESTA*)malloc(sizeof(ARESTA));
	if (a2 == NULL) return 1;
	a2->destino = v1;
	a2->prox = v2->adjacentes;
	v2->adjacentes = a2;

	return 0; //Sucesso
}

/**
 * @brief      Mostra a rede de grafos numa matriz de dimensão MAX_DIM x MAX_DIM
 *             usando os caracteres das frequências para indicar antenas
 * @param[in]  redeGrafos     -> Apontador para a rede de grafos
 * @param[in]  freqsAntenas   -> Vetor de frequências a considerar (caracteres 'A'..'Z')
 * @param[in]  numFreqs       -> Número de frequências válidas em freqsAntenas[]
 * @return     0 caso sucesso, 1 em caso de redeGrafos NULL
 */
int mostrarRedeGrafos(REDE* redeGrafos) {

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

	GRAFO* grafoAtual = redeGrafos->listaGrafos;

	while (grafoAtual != NULL) {
		VERTICE* verticeAtual = grafoAtual->vertices;
		while (verticeAtual != NULL) {

			int x = verticeAtual->infoAntenas->x;
			int y = verticeAtual->infoAntenas->y;
			char freq = verticeAtual->infoAntenas->frequencia;

			if (x >= 0 && x < MAX_DIM && y >= 0 && y < MAX_DIM)
			{
				matriz[x][y] = freq;
			}
			verticeAtual = verticeAtual->prox;
		}
		grafoAtual = grafoAtual->prox;
	}

	for (int i = 0; i < MAX_DIM; i++) {
		for (int j = 0; j < MAX_DIM; j++) {
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
int lerFicheiroTexto(REDE* rede, char* filename) {
	FILE* fp = fopen(filename, "r");
	if (!fp) return -1;

	for (int i = 0; i < MAX_DIM; i++) {
		for (int j = 0; j < MAX_DIM; j++) {
			int ch = fgetc(fp);
			if (ch == EOF) {
				fclose(fp);
				return 0;
			}

			if (ch == '\n') {
				j--;
				continue;
			}

			if (ch >= 'A' && ch <= 'Z') {
				GRAFO* grafo = criarGrafo(rede, ch);
				if (!grafo) {
					fclose(fp);
					return -1;
				}
				inserirAntenaRedeGrafos(grafo, ch, i, j);
			}
		}
		// Ignora eventual \n após cada linha (caso não tenha sido lido dentro do loop)
		fgetc(fp);
	}
	fclose(fp);
	return 0;
}


/**
 * @brief Procura um vértice no grafo pelas coordenadas da antena.
 *
 * Esta função percorre a lista de vértices do grafo procurando um vértice
 * cuja antena tenha as coordenadas (x, y) especificadas.
 *
 * @param grafo Apontador para o grafo onde a pesquisa será realizada.
 * @param x Coordenada X da antena procurada.
 * @param y Coordenada Y da antena procurada.
 *
 * @return Apontador para o vértice encontrado, ou NULL se nenhum vértice com as
 *         coordenadas fornecidas existir no grafo.
 */
VERTICE* encontrarVertice(GRAFO* grafo, int x, int y) {
	VERTICE* vertice = grafo->vertices;
	while (vertice != NULL) {
		if (vertice->infoAntenas->x == x && vertice->infoAntenas->y == y)
		{
			return vertice;
		}
		vertice = vertice->prox;
	}
	return NULL;
}

bool guardarGrafoBin(VERTICE* head, char* filename) {

	if (head == NULL) return false;

	FILE* fp;
	fp = fopen(filename, "wb");
	if (fp == NULL) return false;

	VERTICE* aux = head;
	VerticeFICHEIRO auxFicheiro;	//estrutura de vertice para gravar em ficheiro

	while (aux != NULL) {

		auxFicheiro.frequencia = aux->infoAntenas->frequencia;
		auxFicheiro.x = aux->infoAntenas->x;
		auxFicheiro.y = aux->infoAntenas->y;

		if (fwrite(&auxFicheiro, sizeof(VerticeFICHEIRO), 1, fp) != 1) {
			fclose(fp);
			return false;
		}

		// Guarda as as conexões entre vértices no mesmo ficheiro
		if (aux->adjacentes) {
			if (!guardarArestas(aux->adjacentes, fp, aux->infoAntenas->x, aux->infoAntenas->y)) {
				fclose(fp);
				return false;
			}
		}
		aux = aux->prox;
	}
	fclose(fp);
	return true;
}

int guardarArestas(ARESTA* head, FILE* fp, int xOrigem, int yOrigem) {
	if (head == NULL) return -1;
	if (fp == NULL) return -1;
	
	ARESTA* aux = head;
	ArestasFICHEIRO auxFile; // Declare a local variable instead of using a pointer  

	while (aux) {
		auxFile.xOrigem = xOrigem;
		auxFile.yOrigem = yOrigem;
		auxFile.xDestino = aux->destino->infoAntenas->x;
		auxFile.yDestino = aux->destino->infoAntenas->y;

		if (fwrite(&auxFile, sizeof(ArestasFICHEIRO), 1, fp) != 1) {
			return -1;
		}
		aux = aux->prox;
	}

	return 0;
}

/**
 * @brief      Verifica se um grafo é válido (não NULL, vetor de vértices OK e valores de numVertices)
 * @param[in]  grafo  -> Apontador para o grafo a validar
 * @return     true se o grafo for válido, false caso contrário
 */
bool validarGrafo(GRAFO* grafo) {
	if (!grafo) return false;
	if (!grafo->vertices) return false;
	if (grafo->numVertices <= 0 || grafo->numVertices > grafo->maxVertices) return false;
	for (int i = 0; i < grafo->numVertices; i++) {
		if (!grafo->vertices) return false;
		if (!grafo->vertices->infoAntenas) return false;
	}
	return true;
}

GRAFO* encontrarGrafoPorFrequencia(REDE* rede, char freq) {
	if (rede == NULL || rede->listaGrafos == NULL)
		return NULL;

	GRAFO* atual = rede->listaGrafos;
	while (atual != NULL) {
		if (atual->frequencia == freq)
			return atual;
		atual = atual->prox;
	}

	return NULL;  // Grafo com a frequência não encontrado
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
		return NULL;
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
int BFS(GRAFO* grafo, int x, int y, int* count) {
	if (!grafo || grafo->numVertices == 0)
	{
		return 1;
	}
	*count = 0;

	VERTICE* inicio = encontrarVertice(grafo, x, y);
	if (!inicio) return 1;

	VERTICE* vertice = grafo->vertices;
	while (vertice) {
		vertice->visitado = 0;
		vertice = vertice->prox;
	}

	FILA fila;
	criarFila(&fila);

	inicio->visitado = 1;
	enfilarVertice(&fila, inicio);

	while (!filaVazia(&fila)) {
		VERTICE* atual = desenfilarVertice(&fila);
		(*count)++;

		ARESTA* adj = atual->adjacentes;
		while (adj) {
			VERTICE* vizinho = adj->destino;
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

