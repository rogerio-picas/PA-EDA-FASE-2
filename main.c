/**
 * @main.c
 * @brief main.c da solução que irá representar o funcionamento das funções desenvolvidas
 *
 * @author Rogério Picas
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

    // Inserir manualmente  antenas de frequência 'A'

    inserirAntenaRedeGrafos(rede, 'A', 10, 5);
    inserirAntenaRedeGrafos(rede, 'A', 11, 6);
    inserirAntenaRedeGrafos(rede, 'A', 19, 17);

    // Obtém o grafo de frequência 'A' (índice 0)

    GRAFO* grafoA = rede->grafos['A' - 'A'];

    if (!validarGrafo(grafoA)) {
        printf("Erro na construção do grafo!\n");
        return 1;
    }

    // Conectar aqueles três vértices (que ficaram nos índices 0, 1, 2 em grafoA)

    conectarVertices(grafoA, 0, 1);
    conectarVertices(grafoA, 1, 2);
    conectarVertices(grafoA, 0, 2);

    if (!validarGrafo(grafoA)) {
        printf("Erro a efetuar conecções no grafo!\n");
        return 1;
    }

    // Mostrar o “mapa” da frequência A

    char frequencias[MAX_FREQ] = { 'A', 'C', 'B', 'O' }; // Só para exemplo
    printf("Mapa da frequência A:\n");
    mostrarRedeGrafos(rede, frequencias, 4);

    // Fazer o BFS em grafoA, começando na antena em (10,5)

    int resultados[MAX_ANTENAS];
    int count;
    BFS(grafoA, 10, 5, &count, resultados);

    // Imprimir os vértices visitados (usando os índices guardados em resultados[])
    printf("\nAntenas visitadas na BFS (freq 'A'):\n");
    for (int i = 0; i < count; i++) {
        VERTICE* v = grafoA->vertices[resultados[i]];
        printf("  (%d, %d), frequência: %c\n",
            v->infoAntenas->x,
            v->infoAntenas->y,
            v->infoAntenas->frequencia);

    }

    return 0;

}