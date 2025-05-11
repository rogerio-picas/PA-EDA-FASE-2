#pragma once

#define MAX_DIM 20 // Dimensão da matriz adj
#define MAX_ANTENAS 50 // Numero maximo de antenas
#define MAX_FREQ 26 // Nº de frequencias maximas
//Estrutura responsavel pela alocação dos dados das antenas

typedef struct Antenas {
	char frequencia;
	int x;
	int y;
}ANTENAS;


// Estrutura que irá fazer as ligações entre vértices         1º Aresta 2ºVertice 3ºGrafo
typedef struct Aresta {
	int destino;
	struct Aresta* prox;
} ARESTA;

// Estrutura responsavel pela alocação das antenas num vertice

typedef struct Vertice {
	ANTENAS* infoAntenas;
	ARESTA* adjacentes; // Ligações a outras antenas com mesma frequência
} VERTICE;

// Estrutura responsavel pela inserção do grafo numa matriz de adjacencia

typedef struct Grafo {
	VERTICE** vertices;
	int maxLigacoes;
	int numVertices; // Contador de antenas presentes na matriz de adjacencia
}GRAFO;

// Estrutura responsável por gerir os diferentes grafos por frequencia

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

// Cada antena será um vértice
// Cada vértice contém uma antena e x e y
// Só deverão estar interligadas antenas com a mesma frequencia