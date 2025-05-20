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

    int opcao;
    do {
        printf("\n=== MENU ===\n");
        printf("1. Mostrar mapa de frequencias\n");
        printf("2. Inserir antena manualmente\n");
        printf("3. Conectar antenas manualmente (freq. A)\n");
        printf("4. Fazer BFS em frequencia A\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");

        if (scanf_s("%d", &opcao) != 1) {
            printf("Entrada invalida! Digite um número.\n");
            continue;
        }

        switch (opcao) {
        case 1: {
            char frequencias[MAX_FREQ] = { 'A', 'C', 'B', 'O' }; // Exemplo
            mostrarRedeGrafos(rede, frequencias, 4);
            break;
        }
        case 2: {
            char freq = lerFrequencia();
            int x = lerCoordenada("X");
            int y = lerCoordenada("Y");
            inserirAntenaRedeGrafos(rede, freq, x, y);
            printf("Antena inserida com sucesso!\n");
            break;
        }
        case 3: {
            // Qual é a frequencia do grafo a conectar os vertices?
            printf("Digite a frequencia do grafo a establecer conexao entre vertices:\n");
            char freq = lerFrequencia();
            GRAFO* grafoA = rede->grafos[freq - 'A'];
            if (!validarGrafo(grafoA)) {
                printf("Erro na construcao do grafo %c\n", freq);
                break;
            }
            conectarVerticesAuto(grafoA);
            if (conectarVerticesAuto(grafoA) == 0)
                printf("Vertices conectados com sucesso!\n");
            else
                printf("Erro ao conectar vertices.\n");
            break;
        }
        case 4: {
            GRAFO* grafoA = rede->grafos['A' - 'A'];
            if (!validarGrafo(grafoA)) {
                printf("Erro na construcao do grafo A.\n");
                break;
            }

            int x = 10, y = 5; // Ou pedir input ao utilizador
            int resultados[MAX_ANTENAS], count;

            BFS(grafoA, x, y, &count, resultados);
            printf("\nAntenas visitadas na BFS (freq 'A'):\n");
            for (int i = 0; i < count; i++) {
                VERTICE* v = grafoA->vertices[resultados[i]];
                printf("  (%d, %d), frequencia: %c\n", v->infoAntenas->x, v->infoAntenas->y, v->infoAntenas->frequencia);
            }
            break;
        }
        case 0:
            printf("A sair...\n");
            break;
        default:
            printf("Opcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;

}