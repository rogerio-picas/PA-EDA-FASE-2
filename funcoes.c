/**
 * @file funcoes.c
 * @brief Implementa��o de fun��es para gest�o de grafos de antenas, incluindo cria��o, manipula��o,
 *        procura em largura (BFS) e valida��o de grafos.
 *
 * @author Rog�rio Picas
 * @date 07-05-2025
 * @version 1.0
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "funcoes.h"
#include <stdbool.h>



#pragma region Fun��es de Cria��o

 /**
  * @brief Cria e inicializa uma nova antena com frequ�ncia e coordenadas definidas.
  *
  * Esta fun��o aloca mem�ria para uma nova estrutura ANTENAS, define a frequ�ncia
  * e as coordenadas (x, y) da antena e retorna um apontador para a estrutura criada.
  *
  * @param freqAntena Car�cter que representa a frequ�ncia da antena.
  * @param x Coordenada X da antena.
  * @param y Coordenada Y da antena.
  *
  * @return Apontador para a nova estrutura ANTENAS criada, ou NULL se a aloca��o de mem�ria falhar.
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
 * @brief Cria um novo v�rtice contendo uma antena.
 *
 * Esta fun��o aloca mem�ria para um novo v�rtice do grafo, inicializa os seus
 * campos com valores apropriados e associa o apontador da estrutura de antena
 * fornecida ao campo "infoAntenas" do v�rtice.
 *
 * @param antena Apontador para a estrutura ANTENAS que ser� associada ao v�rtice.
 *
 * @return Um novo v�rtice criado, ou NULL se a aloca��o de mem�ria falhar.
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
 * @brief Cria um novo grafo para uma determinada frequ�ncia, ou retorna um existente.
 *
 * Esta fun��o verifica se j� existe um grafo com a frequ�ncia especificada dentro
 * da rede. Caso exista, retorna o apontador para esse grafo. Caso contr�rio, cria
 * um novo grafo, inicializa os seus campos e adiciona-o � lista de grafos da rede.
 *
 * @param rede Apontador para a estrutura REDE onde o grafo ser� criado ou procurado.
 * @param freq Frequ�ncia associada ao grafo a ser criado.
 *
 * @return Apontador para o grafo correspondente � frequ�ncia especificada,
 *         ou NULL se ocorrer erro na aloca��o de mem�ria.
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
	// Caso n�o exista a frequencia inserida

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
 * Esta fun��o aloca mem�ria para uma nova estrutura REDE, inicializando o campo
 * "listaGrafos" com NULL, indicando que ainda n�o existem grafos associados.
 *
 * @return Apontador para a nova estrutura REDE criada, ou NULL se a aloca��o de mem�ria falhar.
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

#pragma region Fun��es de Manipula��o

/**
 * @brief Insere uma nova antena num grafo, caso ainda n�o exista.
 *
 * Esta fun��o verifica se j� existe uma antena no grafo com a mesma frequ�ncia
 * e coordenadas. Se n�o existir, cria uma nova antena e um novo v�rtice, e insere
 * esse v�rtice na lista de v�rtices do grafo. A contagem de v�rtices � atualizada.
 *
 * @param grafo Apontador para o grafo onde a antena ser� inserida.
 * @param frequencia Car�cter que representa a frequ�ncia da antena.
 * @param x Coordenada X da antena.
 * @param y Coordenada Y da antena.
 *
 * @return 0 se a inser��o for bem-sucedida, ou 1 se a antena j� existir,
 *         se ocorrer erro de aloca��o de mem�ria, ou se o grafo for inv�lido.
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
 * @brief Conecta dois v�rtices de um grafo, criando uma conex�o bidirecional entre dois v�rtices.
 *
 * Esta fun��o procura dois v�rtices no grafo com base nas suas coordenadas
 * e cria arestas entre eles em ambas as dire��es, representando uma liga��o
 * bidirecional. As arestas s�o inseridas nas listas de adjac�ncia dos v�rtices.
 *
 * @param grafo Apontador para o grafo onde os v�rtices se encontram.
 * @param x1 Coordenada X do primeiro v�rtice.
 * @param y1 Coordenada Y do primeiro v�rtice.
 * @param x2 Coordenada X do segundo v�rtice.
 * @param y2 Coordenada Y do segundo v�rtice.
 *
 * @return 0 se a conex�o for bem-sucedida, ou 1 se algum dos v�rtices n�o for encontrado,
 *         se o grafo for inv�lido, ou se ocorrer falha na aloca��o de mem�ria.
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
 * @brief      Mostra a rede de grafos numa matriz de dimens�o MAX_DIM x MAX_DIM
 *             usando os caracteres das frequ�ncias para indicar antenas
 * @param[in]  redeGrafos     -> Apontador para a rede de grafos
 * @param[in]  freqsAntenas   -> Vetor de frequ�ncias a considerar (caracteres 'A'..'Z')
 * @param[in]  numFreqs       -> N�mero de frequ�ncias v�lidas em freqsAntenas[]
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
 * @brief      L� um ficheiro de texto contendo uma matriz de caracteres que representa antenas
 *             em posi��es X,Y. Insere cada caracter 'A'..'Z' como nova antena no grafo respectivo
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
		// Ignora eventual \n ap�s cada linha (caso n�o tenha sido lido dentro do loop)
		fgetc(fp);
	}
	fclose(fp);
	return 0;
}


/**
 * @brief Procura um v�rtice no grafo pelas coordenadas da antena.
 *
 * Esta fun��o percorre a lista de v�rtices do grafo procurando um v�rtice
 * cuja antena tenha as coordenadas (x, y) especificadas.
 *
 * @param grafo Apontador para o grafo onde a pesquisa ser� realizada.
 * @param x Coordenada X da antena procurada.
 * @param y Coordenada Y da antena procurada.
 *
 * @return Apontador para o v�rtice encontrado, ou NULL se nenhum v�rtice com as
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

		// Guarda as as conex�es entre v�rtices no mesmo ficheiro
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
 * @brief      Verifica se um grafo � v�lido (n�o NULL, vetor de v�rtices OK e valores de numVertices)
 * @param[in]  grafo  -> Apontador para o grafo a validar
 * @return     true se o grafo for v�lido, false caso contr�rio
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

	return NULL;  // Grafo com a frequ�ncia n�o encontrado
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
 *             Preenche o array resultado[] com os �ndices dos v�rtices visitados, em ordem de alcance
 * @param[in]  grafo    -> Apontador para o grafo onde a BFS ser� realizada
 * @param[in]  x        -> Coordenada X da antena inicial
 * @param[in]  y        -> Coordenada Y da antena inicial
 * @param[out] count    -> Endere�o de inteiro onde ser� armazenado o n�mero de v�rtices visitados
 * @param[out] resultado  Array (tamanho MAX_ANTENAS) que ir� conter os �ndices dos v�rtices visitados
 * @return     0 caso sucesso; 1 se o grafo for NULL, vazio, ou se a antena inicial n�o existir
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

