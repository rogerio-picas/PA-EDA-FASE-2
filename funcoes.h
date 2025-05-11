#pragma once

#define MAX_DIM 20 // Dimens�o da matriz adj
#define MAX_ANTENAS 50 // Numero maximo de antenas
#define MAX_FREQ 26 // N� de frequencias maximas
//Estrutura responsavel pela aloca��o dos dados das antenas

typedef struct Antenas {
	char frequencia;
	int x;
	int y;
}ANTENAS;


// Estrutura que ir� fazer as liga��es entre v�rtices         1� Aresta 2�Vertice 3�Grafo
typedef struct Aresta {
	int destino;
	struct Aresta* prox;
} ARESTA;

// Estrutura responsavel pela aloca��o das antenas num vertice

typedef struct Vertice {
	ANTENAS* infoAntenas;
	ARESTA* adjacentes; // Liga��es a outras antenas com mesma frequ�ncia
} VERTICE;

// Estrutura responsavel pela inser��o do grafo numa matriz de adjacencia

typedef struct Grafo {
	VERTICE** vertices;
	int maxLigacoes;
	int numVertices; // Contador de antenas presentes na matriz de adjacencia
}GRAFO;

// Estrutura respons�vel por gerir os diferentes grafos por frequencia

typedef struct Rede_Grafos {
	GRAFO* grafos[MAX_FREQ]; // Um grafo por frequencia
}REDE;




ANTENAS* criarAntena(char freqAntena, int x, int y, int resultado);


VERTICE* criarVertice(ANTENAS* antena);

int inserirAntenaRedeGrafos(REDE* rede, char freqAntena, int x, int y);

GRAFO* criarGrafo(int maxLigacoes, int resultado);

int conectarVertices(GRAFO* grafo, int origem, int destino);

int antenaExiste(GRAFO* grafo, char freqAntena, int x, int y);

REDE* criarRede();

int mostrarRedeGrafos(REDE* redeGrafos, char freqsAntenas[], int numFreqs);

int lerFicheiroTexto(REDE* rede, char* ficheiroTexto) {
	FILE* fp = fopen(ficheiroTexto, "r");
	if (fp == NULL)
	{
		return 0;
	}
	char linha[MAX_DIM + 2];
	for (int i = 0; i < MAX_DIM; i++)
	{
		if (fgets(linha, sizeof(linha), fp) == NULL) break;

		for (int j = 0; j < MAX_DIM; j++)
		{

		}
}

// Cada antena ser� um v�rtice
// Cada v�rtice cont�m uma antena e x e y
// S� dever�o estar interligadas antenas com a mesma frequencia