/**
 * @main.c
 * @brief main.c da solução que irá representar o funcionamento das funções desenvolvidas
 *
 * @author Rogério Picas
 * @date 07-05-2025
 * @version 1.0
 *
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "funcoes.h"

int main() {
	REDE* rede = criarRede();
	char* ficheiroTexto = "antenas.txt";
	char* ficheiroBin = "grafo.bin";

	// Carrega ficheiro (que pode criar grafos)
	carregaGrafo(rede, ficheiroTexto);

	// Criação de grafos por frequencia:
	GRAFO* grafoA = criarGrafo(rede, 'A');
	GRAFO* grafoB = criarGrafo(rede, 'B');

	// Inserção de antenas nos grafos por frequencia
	// Frequencia - A
	inserirAntenaRedeGrafos(grafoA, 'A', 2, 5);
	inserirAntenaRedeGrafos(grafoA, 'A', 4, 6);
	inserirAntenaRedeGrafos(grafoA, 'A', 19, 17);
	//Frequencia - B
	inserirAntenaRedeGrafos(grafoB, 'B', 3, 6);
	inserirAntenaRedeGrafos(grafoB, 'B', 5, 7);
	inserirAntenaRedeGrafos(grafoB, 'B', 13, 6);
	//Conectar antenas na frequência A
	conectarVertices(grafoA, 2, 5, 4, 6);
	conectarVertices(grafoA, 19, 17, 4, 6);
	conectarVertices(grafoA, 19, 17, 2, 5);
	//Conectar antenas na frequência B
	conectarVertices(grafoB, 3, 6, 5, 7);
	conectarVertices(grafoB, 13, 6, 5, 7);
	conectarVertices(grafoB, 13, 6, 3, 6);

	// BFS
	int count = 0;

	BFS(grafoA, 2, 5, &count);

	mostrarRedeGrafos(rede);

	printf("\nAntenas visitadas na BFS (freq 'A'):\n");
	VERTICE* v = grafoA->vertices;
	while (v) {
		if (v->visitado) {
			printf("  (%d, %d), frequencia: %c\n",
				v->infoAntenas->x,
				v->infoAntenas->y,
				v->infoAntenas->frequencia);
		}
		v = v->prox;
	}

	// Listagem de grafos pela frequencia e numero de vertices
	GRAFO* listarGrafos = rede->listaGrafos;
	while (listarGrafos != NULL) {
		printf("Grafo com frequencia '%c', Numero de Vertices: %d\n", listarGrafos->frequencia, listarGrafos->numVertices);
		listarGrafos = listarGrafos->prox;
	}

	// Guardar grafos para ficheiro binario
	bool binGrafoA = guardarGrafoBin(grafoA->vertices, ficheiroBin);
	bool binGrafoB = guardarGrafoBin(grafoB->vertices, ficheiroBin);
	if (!binGrafoA || !binGrafoB)
	{
		return false;
	}
	
	return 0;
}