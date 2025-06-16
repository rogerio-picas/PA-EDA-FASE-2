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
  * @brief Cria e inicializa uma nova antena com frequ�ncia e coordenadas definidas
  *
  * Esta fun��o aloca mem�ria para uma nova estrutura ANTENAS, define a frequ�ncia
  * e as coordenadas (x, y) da antena e retorna um apontador para a estrutura criada.
  *
  * @param freqAntena caracter que representa a frequ�ncia da antena.
  * @param x coordenada X da antena.
  * @param y coordenada Y da antena.
  *
  * @return apontador para a nova estrutura ANTENAS criada, ou NULL se a aloca��o de mem�ria falhar.
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
 * @brief Cria um novo v�rtice contendo a informa��o de alguma antena criada
 *
 * Esta fun��o aloca mem�ria para um novo v�rtice do grafo, inicializa os seus
 * campos com valores apropriados e associa o apontador da estrutura de antena
 * fornecida ao campo "infoAntenas" do v�rtice.
 *
 * @param antena -> apontador para a estrutura ANTENAS que ser� associada ao v�rtice.
 *
 * @return um novo v�rtice criado, ou NULL se a aloca��o de mem�ria falhar.
 */
VERTICE* criarVertice(ANTENAS* antena) {

	VERTICE* novoVertice = (VERTICE*)malloc(sizeof(VERTICE));
	if (!novoVertice) return NULL;

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
 * @param rede apontador para a estrutura REDE onde o grafo ser� criado ou procurado.
 * @param freq frequ�ncia associada ao grafo a ser criado.
 *
 * @return apontador para o grafo correspondente � frequ�ncia especificada,
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
 * @return apontador para a nova estrutura REDE criada, ou NULL se a aloca��o de mem�ria falhar.
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
 * @brief Insere uma nova antena como v�rtice no grafo, se n�o existir j� uma igual.
 *
 * Verifica se j� existe uma antena com a mesma frequ�ncia e coordenadas (x, y) no grafo.
 * Caso contr�rio, cria uma nova antena e o v�rtice correspondente e adiciona no in�cio da lista de v�rtices.
 *
 * @param[in] grafo - apontador para o grafo onde a antena ser� inserida.
 * @param[in] frequencia - caractere que representa a frequ�ncia da antena.
 * @param[in] x - coordenada x da antena.
 * @param[in] y - coordenada y da antena.
 *
 * @return @c 0 se a inser��o foi bem-sucedida,
 *         @c 1 em caso de erro (grafo nulo, coordenadas inv�lidas, antena j� existente ou falha na cria��o).
 */
int inserirAntenaGrafo(GRAFO* grafo, char frequencia, int x, int y) {

	if (!grafo)
	{
		return 1;
	}
	if (x >= MAX_DIM || y >= MAX_DIM)
	{
		return 1;
	}
	// Verificar se antena existe na lista
	VERTICE* atual = grafo->vertices;

	while (atual) {

		if (atual && atual->infoAntenas &&
			atual->infoAntenas->frequencia == frequencia &&
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
 * @brief Cria uma liga��o bidirecional entre dois v�rtices no grafo.
 *
 * Esta fun��o liga os v�rtices com coordenadas (x1, y1) e (x2, y2) adicionando arestas
 * nas listas de adjac�ncia de ambos os v�rtices, permitindo assim a navega��o em ambas dire��es.
 *
 * @param[in] grafo - apontador para o grafo onde os v�rtices est�o inseridos.
 * @param[in] x1 - coordenada x do primeiro v�rtice.
 * @param[in] y1 - coordenada y do primeiro v�rtice.
 * @param[in] x2 - coordenada x do segundo v�rtice.
 * @param[in] y2 - coordenada y do segundo v�rtice.
 *
 * @return @c 0 se a liga��o foi criada com sucesso,
 *         @c 1 em caso de erro (grafo ou v�rtices inexistentes ou falha na aloca��o).
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

	return 0;
}

/**
 * @brief Mostra visualmente a rede de grafos numa matriz 2D.
 *
 * Esta fun��o cria uma matriz de dimens�o MAX_DIM x MAX_DIM e preenche-a com
 * '.' inicialmente. Depois, para cada v�rtice de cada grafo na rede, coloca na
 * posi��o correspondente as coordenadas (x,y) o caractere da frequ�ncia desse v�rtice.
 * Finalmente imprime a matriz no ecr�.
 *
 * @param[in] redeGrafos - apontador para a estrutura REDE que cont�m a lista de grafos.
 *
 * @return @c 0 em caso de sucesso, @c 1 se o apontador da rede for NULL.
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

	return 0;
}

/**
 * @brief Carrega a informa��o de um grafo a partir de um ficheiro de texto.
 *
 * Esta fun��o l� caractere a caractere de um ficheiro de texto usando @c fgetc
 * (em vez de um buffer), interpretando cada letra mai�scula como uma antena a ser
 * inserida na rede. A posi��o da antena � determinada pelas coordenadas da matriz
 * onde a letra foi lida.
 *
 * O grafo correspondente a cada letra � criado atrav�s da fun��o @c criarGrafo
 * e a antena � inserida usando @c inserirAntenaRedeGrafos. Linhas com quebras
 * de linha s�o devidamente tratadas durante a leitura.
 *
 * @param[in] - rede apontador para a estrutura de rede onde os grafos ser�o inseridos.
 * @param[in] - filename nome do ficheiro de texto a ser lido.
 *
 * @return 0 em caso de sucesso. Retorna -1 se ocorrer erro na abertura do ficheiro ou na cria��o do grafo.
 */
int carregaGrafo(REDE* rede, char* filename) {
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
				inserirAntenaGrafo(grafo, ch, i, j);
			}
		}
		// Ignora eventual \n ap�s cada linha (caso n�o tenha sido lido dentro do loop)
		fgetc(fp);
	}
	fclose(fp);
	return 0;
}

/**
 * @brief Procura um v�rtice no grafo com coordenadas espec�ficas.
 *
 * Esta fun��o percorre a lista de v�rtices de um grafo e retorna o v�rtice
 * que possui as coordenadas @p x e @p y. A compara��o � feita com base nas
 * coordenadas da antena associada a cada v�rtice.
 *
 * @param[in] - grafo apontador para a estrutura do grafo.
 * @param[in] - x coordenada X a procurar.
 * @param[in] - y coordenada Y a procurar.
 *
 * @return Apontador para o v�rtice encontrado com as coordenadas correspondentes.
 * Retorna NULL se @p grafo for nulo ou se n�o for encontrado nenhum v�rtice com essas coordenadas.
 */
VERTICE* encontrarVertice(GRAFO* grafo, int x, int y) {
	if (grafo == NULL) return NULL;
	VERTICE* vertice = grafo->vertices;
	while (vertice != NULL) {
		if (vertice->infoAntenas &&
			vertice->infoAntenas->x == x &&
			vertice->infoAntenas->y == y)
		{
			return vertice;
		}
		vertice = vertice->prox;
	}
	return NULL;
}

/**
 * @brief Guarda num ficheiro bin�rio a estrutura de um grafo com v�rtices e arestas.
 *
 * Esta fun��o percorre uma lista ligada de v�rtices (estrutura @c VERTICE) e grava em ficheiro bin�rio
 * a informa��o de cada v�rtice e respetivas arestas adjacentes, utilizando as estruturas @c VerticeFICHEIRO
 * e @c ArestasFICHEIRO.
 *
 * @param[in] - head apontador para o in�cio da lista de v�rtices do grafo.
 * @param[in] - filename nome do ficheiro onde os dados bin�rios ser�o guardados.
 *
 * @return @c true em caso de sucesso. Retorna @c false se @p head for nulo, se ocorrer erro ao abrir o ficheiro,
 *         ou se falhar a escrita dos dados.
 */
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

/**
 * @brief Guarda as arestas de uma lista ligada num ficheiro bin�rio.
 *
 * Esta fun��o percorre uma lista ligada de arestas (estrutura @c ARESTA) e grava no ficheiro bin�rio
 * as informa��es de origem e destino de cada aresta, utilizando a estrutura @c ArestasFICHEIRO.
 *
 * @param[in] - head apontador para o in�cio da lista ligada de arestas.
 * @param[in] - fp apontador para o ficheiro bin�rio onde os dados ser�o gravados.
 * @param[in] - xOrigem coordenada X da antena de origem.
 * @param[in] - yOrigem coordenada Y da antena de origem.
 *
 * @return 0 em caso de sucesso. Retorna -1 se @p head ou @p fp forem nulos ou se ocorrer um erro na escrita no ficheiro.
 */
int guardarArestas(ARESTA* head, FILE* fp, int xOrigem, int yOrigem) {
	if (head == NULL) return -1;
	if (fp == NULL) return -1;

	ARESTA* aux = head;
	ArestasFICHEIRO auxFile;

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
 * @brief Valida a integridade de um grafo.
 *
 * Esta fun��o verifica se o grafo fornecido est� corretamente inicializado e se cont�m
 * um n�mero v�lido de v�rtices,e tamb�m confirma se cada v�rtice possui informa��o
 * de antena associada.
 *
 * @param[in] grafo - apontador para a estrutura do grafo a validar.
 *
 * @return @c true se o grafo for v�lido.
 *         @c false se for nulo, se n�o contiver v�rtices, se o n�mero de v�rtices estiver fora dos limites,
 *         ou se faltar informa��o essencial em algum dos v�rtices.
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

/**
 * @brief Marca todos os v�rtices de um grafo como n�o visitados.
 *
 * Esta fun��o percorre a lista ligada de v�rtices e define o campo
 * 'visitado' de cada v�rtice como 0 (n�o visitado). � �til ap�s
 * execu��es de algoritmos de busca, como BFS ou DFS, para garantir que
 * os estados de visitado dos vertices sejam reiniciados corretamente.
 *
 * @param grafo - apontador para o primeiro v�rtice do grafo.
 * @return VERTICE* - retorna o apontador para o primeiro v�rtice (inalterado).
 *
 */
VERTICE* resetarVisitados(VERTICE* grafo) {
	VERTICE* v = grafo;
	while (v) {
		v->visitado = 0;
		v = v->prox;
	}
	return grafo;
}

/**
 * @brief Procura um grafo na rede pela frequ�ncia especificada.
 *
 * Esta fun��o percorre a lista de grafos presentes na rede e retorna o grafo
 * que possui a frequ�ncia igual a freq.
 *
 * @param[in] rede - apontador para a estrutura da rede onde os grafos est�o armazenados.
 * @param[in] freq - frequ�ncia do grafo a procurar.
 *
 * @return Apontador para o grafo encontrado com a frequ�ncia indicada.
 *         Retorna NULL se a rede for nula, a lista de grafos estiver vazia,
 *         ou se nenhum grafo com a frequ�ncia especificada for encontrado.
 */
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

/**
 * @brief Liberta toda a mem�ria ocupada pela lista de arestas ligada a um v�rtice.
 *
 * Esta fun��o percorre a lista de arestas apontada por 'lista', libertando uma a uma
 * da mem�ria. No final, devolve NULL para indicar que a lista foi destru�da.
 *
 * @param lista apontador para o in�cio da lista de arestas.
 * @return NULL, indicando que a lista foi destru�da com sucesso.
 */
ARESTA* destruirArestas(ARESTA* lista) {
	ARESTA* atual;
	while (lista) {
		atual = lista;
		lista = lista->prox;
		free(atual);
	}
	return NULL;
}

/**
 * @brief Liberta todos os v�rtices de um grafo, incluindo as suas arestas e informa��o associada.
 *
 * Esta fun��o percorre a lista de v�rtices de um grafo. Para cada v�rtice:
 * - Liberta a sua lista de arestas com 'destruirArestas'
 * - Liberta a estrutura 'ANTENAS' associada
 * - Liberta o pr�prio v�rtice
 *
 * @param lista - apontador para o in�cio da lista de v�rtices.
 * @return - NULL, indicando que todos os v�rtices foram destru�dos.
 */
VERTICE* destruirVertices(VERTICE* lista) {
	VERTICE* aux;
	while (lista) {
		aux = lista->prox;
		lista->adjacentes = destruirArestas(lista->adjacentes);
		free(lista->infoAntenas);
		free(lista);
		lista = aux;
	}
	return NULL;
}

/**
 * @brief Liberta completamente um grafo da mem�ria, incluindo todos os seus v�rtices e arestas.
 *
 * Esta fun��o elimina todos os v�rtices do grafo com a fun��o 'destruirVertices'
 * e de seguida liberta o pr�prio grafo.
 *
 * @param grafo - apontador para o grafo a destruir.
 * @return - NULL, pois o grafo � completamente removido da mem�ria.
 */
GRAFO* destruirGrafo(GRAFO* grafo) {
	if (grafo == NULL) return;
	grafo->vertices = destruirVertices(grafo->vertices);
	free(grafo);
	return 0;
}

bool existeConexao(ARESTA* head, int xDestino, int yDestino) {
	if (head == NULL) return false;
	if (head->destino->infoAntenas->x == xDestino && head->destino->infoAntenas->y == yDestino)
		return true;
	return existeConexao(head->prox, xDestino, yDestino);
}

bool existeConexaoEntreVertices(GRAFO* grafo, int xOrigem, int yOrigem, int xDestino, int yDestino) {
	VERTICE* verticeOrigem = encontrarVertice(grafo, xOrigem, yOrigem);
	if (verticeOrigem == NULL) return false;

	return existeConexao(verticeOrigem->adjacentes, xDestino, yDestino);
}

#pragma endregion

#pragma region Fun��es Auxiliares para o BFS

/**
 * @brief Inicializa uma fila vazia.
 *
 * Define os campos 'frente' e 'tras' da fila como NULL, preparando a estrutura para uso.
 *
 * @param[in,out] fila - apontador para a fila a ser inicializada.
 *
 */
void criarFila(FILA* fila) {
	fila->frente = NULL;
	fila->tras = NULL;
}

/**
 * @brief Verifica se a fila est� vazia.
 *
 * Esta fun��o verifica se a fila tem elementos, retornando true se estiver vazia
 * (ou seja, se o campo frente for NULL), ou falso caso contr�rio.
 *
 * @param[in] fila - apontador para a fila a ser verificada.
 *
 * @return @c true se a fila estiver vazia,
 *         @c false caso contr�rio.
 */
bool filaVazia(FILA* fila) {
	return(fila->frente == NULL);
}

/**
 * @brief Adiciona um v�rtice ao final da fila.
 *
 * Esta fun��o cria um novo n� para a fila contendo o v�rtice indicado e insere-o no final.
 *
 * @param[in,out] fila - apontador para a fila onde o v�rtice ser� inserido.
 * @param[in] vertice - apontador para o v�rtice a ser enfileirado.
 *
 * @return @c true se a inser��o foi bem-sucedida,
 *         @c false caso n�o tenha sido poss�vel alocar mem�ria para o novo n�.
 */
bool enfilarVertice(FILA* fila, VERTICE* vertice) {
	NOFILA* novo = (NOFILA*)malloc(sizeof(NOFILA));
	if (!novo) return false;

	novo->vertice = vertice;
	novo->prox = NULL;

	if (filaVazia(fila)) {
		fila->frente = fila->tras = novo;
	}
	else {
		fila->tras->prox = novo;
		fila->tras = novo;
	}

	return true;
}

/**
 * @brief Remove e retorna o v�rtice no in�cio da fila.
 *
 * Esta fun��o retira o primeiro n� da fila, libera a mem�ria desse n�,
 * e retorna o v�rtice que ele continha. Se a fila estiver vazia, retorna NULL.
 *
 * @param[in,out] fila - apontador para a fila de onde ser� removido o v�rtice.
 *
 * @return Apontador para o v�rtice removido da fila,
 *         ou NULL se a fila estiver vazia.
 */
VERTICE* desenfilarVertice(FILA* fila) {
	if (filaVazia(fila)) return NULL;

	NOFILA* tmp = fila->frente;
	VERTICE* vertice = tmp->vertice;
	fila->frente = tmp->prox;
	free(tmp);

	if (fila->frente == NULL)
	{
		fila->tras = NULL;
	}

	return vertice;
}

/**
 * @brief Realiza uma busca em largura (Breadth-First Traversal) no grafo a partir do v�rtice dado.
 *
 * Inicia a travessia no v�rtice com coordenadas (x, y) e conta quantos v�rtices s�o visitados.
 * Marca os v�rtices visitados durante a travessia e no final limpa o estado visitado para todos.
 *
 * @param[in,out] grafo - apontador para o grafo onde ser� realizada a travessia.
 * @param[in] x - coordenada x do v�rtice inicial.
 * @param[in] y - coordenada y do v�rtice inicial.
 * @param[out] count - apontador para inteiro onde ser� armazenado o n�mero de v�rtices visitados.
 *
 * @return @c 0 se a travessia foi executada com sucesso,
 *         @c 1 se o grafo for inv�lido, estiver vazio ou o v�rtice inicial n�o for encontrado.
 */
int BFT(GRAFO* grafo, int x, int y, int* count) {
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

	resetarVisitados(grafo->vertices);

	return 0;
}
int DFS(GRAFO* grafo, int x, int y) {
	if (grafo == NULL) return 1;
	if (x > MAX_DIM || x < 0 || y > MAX_DIM || y < 0) return 2;

	VERTICE* vertice = encontrarVertice(grafo, x, y);
	if (!vertice) return 3;

	ARESTA* adj = vertice->adjacentes;
	vertice->visitado = 1;

	while (adj) {
		VERTICE* vizinho = adj->destino;
		if (!vizinho->visitado)
		{
			vizinho->visitado = 1;
			printf("Antena alcancada: (%d,%d)\n", vizinho->infoAntenas->x, vizinho->infoAntenas->y);
			DFS(grafo, vizinho->infoAntenas->x, vizinho->infoAntenas->y);
		}
		adj = adj->prox;
	}
	return 0;
}

int countPathsDFS(GRAFO* grafo, int xOrigem, int yOrigem, int xDestino, int yDestino) {
	if (grafo == NULL) return -1;

	VERTICE* origem = encontrarVertice(grafo, xOrigem, yOrigem);
	VERTICE* destino = encontrarVertice(grafo, xDestino, yDestino);
	if (!origem || !destino) return -1;

	return countPaths(origem, destino);
}

int countPaths(VERTICE* origem, VERTICE* destino) {
	if (origem == NULL) return 0;
	if (origem == destino) return 1;

	origem->visitado = 1;
	int totalPaths = 0;

	ARESTA* adj = origem->adjacentes;

	while (adj) {
		if (!adj->destino->visitado)
		{
			totalPaths = countPaths(adj->destino, destino) + totalPaths;
		}
		adj = adj->prox;
	}
	origem->visitado = 0;
	return totalPaths;
}

void inserirEfeitoNefasto(REDE* rede, NEFASTO** nefasto, char frequencia, int x, int y) {
	NEFASTO* novoNefasto = (NEFASTO*)malloc(sizeof(NEFASTO));
	if (novoNefasto == NULL) return NULL;

	VERTICE* vertice = encontrarVertice(rede->listaGrafos, x, y);
	if (vertice = NULL) {
		free(novoNefasto);
		return;
	}

	vertice->infoAntenas->frequencia = frequencia;
	novoNefasto->vertice = vertice;
	novoNefasto->prox = *nefasto;
	*nefasto = novoNefasto;
}

bool efeitoNefasto(REDE* rede, NEFASTO* nefasto) {

	if (nefasto == NULL) return false;

	rede = rede->listaGrafos;
	if (rede->listaGrafos == NULL) return false;

	NEFASTO* aux1;
	NEFASTO* aux2;
	for (aux1 = nefasto; aux1 != NULL; aux1 = aux1->prox) {
		for (aux2 = aux1->prox; aux2 != NULL; aux2 = aux2->prox) {
			if (aux1->vertice->infoAntenas == aux2->vertice->infoAntenas) {
				int x1 = aux1->vertice->infoAntenas->x;
				int x2 = aux2->vertice->infoAntenas->x;
				int y1 = aux1->vertice->infoAntenas->y;
				int y2 = aux2->vertice->infoAntenas->y;
				printf("Antena x1y1: (%d, %d) && Antena x2y2: (%d, %d)", x1, y1, x2, y2);

				if (x2 >= x1 + 2 || x1 >= x2 + 2 || x2 <= x1 - 2 || x1 <= x2 - 2)
				{
					int numPosX = abs(x2 - x1);
					int numPosY = abs(y2 - y1);

					int newX1, newX2, newY1, newY2;

					if (x1 < x2)
					{
						newX1 = x1 - numPosX;
						newX2 = x2 + numPosX;
					}
					else {
						newX1 = x1 + numPosX;
						newX2 = x2 - numPosX;
					}
					if (y1 < y2)
					{
						newY1 = y1 - numPosY;
						newY2 = y2 + numPosY;
					}
					else {
						newY1 = y1 + numPosY;
						newY2 = y2 - numPosY;
					}

					printf("\nPosicoes do efeito nefasto: x1y1: (%d, %d) | x2y2: (%d, %d)\n", newX1, newY1, newX2, newY2); // Mostrar as posi��es do efeito nefasto

					if (newX1 >= 0 && newX1 < MAX_DIM && newY1 >= 0 && newY1 < MAX_DIM) { // verificar se newX1 e newY1 est�o dentro dos limites v�lidos da cidade
						inserirEfeitoNefasto(rede, &nefasto, '#', newX1, newY1);
					}
					else
					{
						printf("\nEfeito nefasto fora de limite x1y1: (%d, %d)", newX1, newY1);
					}

					if (newX2 >= 0 && newX2 < MAX_DIM && newY2 >= 0 && newY2 < MAX_DIM) { // verificar se newX2 e newY2 est�o dentro dos limites v�lidos da cidade
						inserirEfeitoNefasto(rede, &nefasto, '#', newX2, newY2);
					}
					else
					{
						printf("\nEfeito nefasto fora de limite x2y2: (%d, %d)", newX2, newY2);
					}
				}
				else
				{
					printf("\nAntenas muito proximas!");
				}
			}
			else {
				printf("\nAntenas com frequencias diferentes!");
			}

			printf("\n --------------------------- \n");
		}
	}
	return true;
}
#pragma endregion
