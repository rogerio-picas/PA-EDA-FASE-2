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

    REDE* rede = criarRede();
    GRAFO* grafoAdj;
    char frequencias[MAX_FREQ] = { 'A', 'C', 'B', 'O'}; // Tipos de frequ�ncias presentes
    char* ficheiroTexto = "antenas.txt";
    lerFicheiroTexto(rede, ficheiroTexto);
    inserirAntenaRedeGrafos(rede, 'A', 10, 5);
    inserirAntenaRedeGrafos(rede, 'A', 11, 6);
    inserirAntenaRedeGrafos(rede, 'A', 19, 19);
    conectarVertices(grafoAdj, 0, 1);
    conectarVertices(grafoAdj, 1, 2);
    conectarVertices(grafoAdj, 0, 2);

    printf("Mapa da frequ�ncia A:\n");
    mostrarRedeGrafos(rede, frequencias, 4);

    return 0;

	return 0;
}