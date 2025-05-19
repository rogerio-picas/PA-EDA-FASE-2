/**
 * @main.c
 * @brief main.c da solu��o que ir� representar o funcionamento das fun��es desenvolvidas
 *
 * @author Rog�rio Picas
 * @date 07-05-2025
 * @version 1.0
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "funcoes.h"

int main() {

    // Cria a rede e carrega ficheiro de texto

    REDE* rede = criarRede();
    char* ficheiroTexto = "antenas.txt";
    lerFicheiroTexto(rede, ficheiroTexto);

    // Inserir manualmente  antenas de frequ�ncia 'A'

    inserirAntenaRedeGrafos(rede, 'A', 10, 5);
    inserirAntenaRedeGrafos(rede, 'A', 11, 6);
    inserirAntenaRedeGrafos(rede, 'A', 19, 17);

    // Obt�m o grafo de frequ�ncia 'A' (�ndice 0)

    GRAFO* grafoA = rede->grafos['A' - 'A'];

    if (!validarGrafo(grafoA)) {
        printf("Erro na constru��o do grafo!\n");
        return 1;
    }

    // Conectar aqueles tr�s v�rtices (que ficaram nos �ndices 0, 1, 2 em grafoA)

    conectarVertices(grafoA, 0, 1);
    conectarVertices(grafoA, 1, 2);
    conectarVertices(grafoA, 0, 2);

    if (!validarGrafo(grafoA)) {
        printf("Erro a efetuar conec��es no grafo!\n");
        return 1;
    }

    // Mostrar o �mapa� da frequ�ncia A

    char frequencias[MAX_FREQ] = { 'A', 'C', 'B', 'O' }; // S� para exemplo
    printf("Mapa da frequ�ncia A:\n");
    mostrarRedeGrafos(rede, frequencias, 4);

    // Fazer o BFS em grafoA, come�ando na antena em (10,5)

    int resultados[MAX_ANTENAS];
    int count;
    BFS(grafoA, 10, 5, &count, resultados);

    // Imprimir os v�rtices visitados (usando os �ndices guardados em resultados[])
    printf("\nAntenas visitadas na BFS (freq 'A'):\n");
    for (int i = 0; i < count; i++) {
        VERTICE* v = grafoA->vertices[resultados[i]];
        printf("  (%d, %d), frequ�ncia: %c\n",
            v->infoAntenas->x,
            v->infoAntenas->y,
            v->infoAntenas->frequencia);

    }

    return 0;

}