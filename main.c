/**
 * @main.c
 * @brief main.c da solução que irá representar o funcionamento das funções desenvolvidas
 *
 * @author Rogério Picas
 * @date 07-05-2025
 * @version 1.0
 *
 */
#define _CRT_SECURE_NO_WARNINGS // fopen_s warning
#include <stdio.h>
#include <stdlib.h>
#include "funcoes.h"
#include <stdbool.h>

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
	inserirAntenaGrafo(grafoA, 'A', 2, 5);
	inserirAntenaGrafo(grafoA, 'A', 4, 6);
	inserirAntenaGrafo(grafoA, 'A', 19, 17);

	//Frequencia - B
	inserirAntenaGrafo(grafoB, 'B', 3, 6);
	inserirAntenaGrafo(grafoB, 'B', 5, 7);
	inserirAntenaGrafo(grafoB, 'B', 13, 6);
	inserirAntenaGrafo(grafoB, 'B', 6, 3);
	//Teste duplicada
	inserirAntenaGrafo(grafoB, 'B', 6, 3);

	//Conectar antenas na frequência A
	conectarVertices(grafoA, 2, 5, 4, 6);
	conectarVertices(grafoA, 19, 17, 4, 6);
	conectarVertices(grafoA, 19, 17, 2, 5);
	// Teste conexão
	conectarVertices(grafoA, 19, 17, 5, 5);

	//Conectar antenas na frequência B
	conectarVertices(grafoB, 3, 6, 5, 7);
	conectarVertices(grafoB, 6, 3, 5, 7);
	conectarVertices(grafoB, 6, 3, 3, 6);
	conectarVertices(grafoB, 6, 3, 13, 6);
	conectarVertices(grafoB, 13, 6, 5, 7);
	conectarVertices(grafoB, 13, 6, 3, 6);

	// Teste de conexão entre vertices
	bool conexao = existeConexaoEntreVertices(grafoA, 2, 5, 4, 6);
	if (!conexao)
	{
		printf("Nao existe conexao!\n");
	}
	else printf("Existe conexao!\n");

	// BFT
	int count = 0;

	BFT(grafoB, 3, 6, &count);

	VERTICE* origem = encontrarVertice(grafoA, 2, 5);
	VERTICE* destino = encontrarVertice(grafoA, 19, 17);

	//if (origem == NULL || destino == NULL) {
	//	printf("Origem ou destino não encontrados.\n");
	//}
	//else {
	//	int totalPaths = countPaths(origem, destino);
	//	printf("Total de caminhos possiveis de (%d,%d) para (%d,%d): %d\n",
	//		origem->infoAntenas->x, origem->infoAntenas->y,
	//		destino->infoAntenas->x, destino->infoAntenas->y,
	//		totalPaths);
	//}


	mostrarRedeGrafos(rede);

	// Utilização do DFS
	printf("DFS -> Antenas alcancadas a partir de (3, 6)\n");
	DFS(grafoB, 3, 6);

	printf("\nAntenas visitadas na BFT: \n");
	VERTICE* v = grafoB->vertices;
	while (v) {
		if (v->visitado) {
			printf("  (%d, %d), frequencia: %c\n",
				v->infoAntenas->x,
				v->infoAntenas->y,
				v->infoAntenas->frequencia);
		}
		v = v->prox;
	}

	// Função para destruir o grafo
	//destruirGrafo(grafoA);

	// Listagem de grafos pela frequencia e numero de vertices
	GRAFO* listarGrafos = rede->listaGrafos;
	while (listarGrafos != NULL) {
		printf("Grafo com frequencia '%c', Numero de Vertices: %d\n",
			listarGrafos->frequencia,
			listarGrafos->numVertices);

		VERTICE* v = listarGrafos->vertices;
		while (v != NULL) {
			if (v->infoAntenas != NULL) {
				printf("  Vertice coord: (%d, %d)\n",
					v->infoAntenas->x,
					v->infoAntenas->y);
			}
			v = v->prox;
		}
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