/**
 * @funcoes.c
 * @brief Ficheiro que contém as funções desenvolvidas para efetuar operações num grafo
 *
 * @author Rogério Picas
 * @date 07-05-2025
 * @version 1.0
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "funcoes.h"
#include <stdbool.h>


#pragma region Funções de Criação

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

REDE* criarRede() {
	REDE* rede = (REDE*)malloc(sizeof(REDE));
	if (!rede) return NULL;

	for (int i = 0; i < MAX_FREQ; i++)
		rede->grafos[i] = NULL;

	return rede;
}

#pragma endregion

#pragma region Funções de Manipulação

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

#pragma endregion

int encontrarIndiceVertice(GRAFO* grafo, int x, int y) {

	for (int i = 0; i < grafo->numVertices; i++)
	{
		if (grafo->vertices[i]->infoAntenas->x == x && grafo->vertices[i]->infoAntenas->y) {
			return i;
		}
	}
	return -1;
}

// Queue functions 
bool insertQueue(int queue[], int* rear, int data, int* queueItemCount) {
	//testes...return false;
	queue[++(*rear)] = data;
	(*queueItemCount)++;
	return true;
}

int nextQueue(int queue[], int* queueItemCount, int* front) {
	(*queueItemCount)--;
	return queue[(*front)++];
}

bool isQueueEmpty(int queueItemCount) {
	return queueItemCount == 0;
}
