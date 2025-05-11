#include <stdio.h>
#include <stdlib.h>
#include "funcoes.h"


// Listagem de funções para os grafos



// Função para criação de antena
ANTENAS* criarAntena(char freqAntena, int x, int y, int resultado) {
	resultado = 0;
	ANTENAS* auxVar;
	auxVar = (ANTENAS*)malloc(sizeof(ANTENAS));
	if (auxVar == NULL)
	{
		resultado = 0;
		return NULL;
	}
		auxVar->frequencia = freqAntena;
		auxVar->x = x;
		auxVar->y = y;

		resultado = 1;

		return auxVar;
}

// Função para inserir a antena na matriz de adjacência


VERTICE* criarVertice(ANTENAS* antena) {
	VERTICE* novoVertice = (VERTICE*)malloc(sizeof(VERTICE));
	if (!novoVertice)
	{
		return 0;
	}
	novoVertice->infoAntenas = antena;
	novoVertice->adjacentes = NULL;

	return novoVertice;
}

int inserirAntenaRedeGrafos(REDE* rede, char freqAntena, int x, int y){

	int resultado = 0;
	int indiceGrafo = freqAntena - 'A';

	if (indiceGrafo < 0 || indiceGrafo > MAX_FREQ)
	{
		resultado = 0;
		return 0;
	}

	// Caso não exista o grafo para a frequencia cria-lo

	if (rede->grafos[indiceGrafo]==NULL)
	{
		rede->grafos[indiceGrafo] = (GRAFO*)malloc(sizeof(GRAFO));
		if (rede->grafos[indiceGrafo] == NULL)
		{
			return 0;
		}
		rede->grafos[indiceGrafo]->numVertices = 0;
		rede->grafos[indiceGrafo]->maxLigacoes = MAX_DIM;
		rede->grafos[indiceGrafo]->vertices = (VERTICE**)malloc(sizeof(VERTICE*) * MAX_DIM);
	}

	GRAFO* grafo = rede->grafos[indiceGrafo];

	// Verifica se a antena já existe
	for (int i = 0; i < grafo->numVertices; i++) {
		if (grafo->vertices[i]->infoAntenas->x == x && grafo->vertices[i]->infoAntenas->y == y)
			return 0; // Já existe
	}

	ANTENAS* novaAntena = criarAntena(freqAntena, x, y, resultado);
	if (!novaAntena) 
	{
		return 0;
	}

	VERTICE* novoVertice = criarVertice(novaAntena);
	if (novoVertice == NULL)
	{
		free(novaAntena);
		return 0;
	}
	 
	if (grafo->vertices == NULL)
	{
		return 0;
	}

	grafo->vertices[grafo->numVertices++] = novoVertice;

	return 1;
    
}

GRAFO* criarGrafo(int maxLigacoes, int resultado) {
	resultado = 0;
	GRAFO* grafo = (GRAFO*)malloc(sizeof(GRAFO));
	if (grafo == NULL)
	{
		resultado = 0;
		return NULL;
	}
	grafo->vertices = (VERTICE**)malloc(sizeof(VERTICE*) * maxLigacoes);
	if (grafo->vertices == NULL)
	{
		resultado = 0;
		free(grafo);
		return NULL;
	}
	for (int i = 0; i < grafo->maxLigacoes; i++)
	{
		grafo->vertices[i] = NULL;
	}
	grafo->numVertices = 0;
	grafo->maxLigacoes = maxLigacoes;
	resultado = 1;
	
	return grafo;
}

int conectarVertices(GRAFO* grafo, int origem, int destino){
if (grafo == NULL)
{
	return 0;
}
if (origem < 0 || destino < 0 || origem >= grafo->numVertices || destino >= grafo->numVertices)
{
	return 0;
}
ARESTA* novaAresta = (ARESTA*)malloc(sizeof(ARESTA));
if (novaAresta == NULL)
{
	return 0;
}
novaAresta->destino = destino;
   novaAresta->prox = grafo->vertices[origem]->adjacentes; 
grafo->vertices[origem]->adjacentes = novaAresta; 

ARESTA* novaArestaInversa = (ARESTA*)malloc(sizeof(ARESTA));
if (novaArestaInversa == NULL)
{
	return 0;
}
novaArestaInversa->destino = origem;
novaArestaInversa->prox = grafo->vertices[destino]->adjacentes;
grafo->vertices[destino]->adjacentes = novaArestaInversa; 

return 1;
}


int antenaExiste(GRAFO* grafo, char freqAntena, int x, int y) {
	if (grafo->vertices == NULL)
	{
		return 0;
	}
	for (int i = 0; i < grafo->numVertices; i++)
	{
		ANTENAS* auxVar = grafo->vertices[i]->infoAntenas;
		if (auxVar->frequencia == freqAntena && auxVar->x == x && auxVar->y == y)
		{
			return 1;
		}
	}
	return 0;
}

REDE* criarRede(){
	REDE* rede = (REDE*)malloc(sizeof(REDE));
	if (!rede) return NULL;

	for (int i = 0; i < MAX_FREQ; i++)
		rede->grafos[i] = NULL;

	return rede;
}

int mostrarRedeGrafos(REDE* redeGrafos, char freqsAntenas[], int numFreqs) {

	if (redeGrafos == NULL)
	{
		return 0;
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
