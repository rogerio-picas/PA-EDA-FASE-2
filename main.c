#include <stdio.h>
#include <stdlib.h>
#include "funcoes.h"

int main() {

    REDE* rede = criarRede();
    char frequencias[MAX_FREQ] = { 'A', 'C', 'B'};
    inserirAntenaRedeGrafos(rede, 'A', 10, 5);
    inserirAntenaRedeGrafos(rede, 'A', 11, 6);
    inserirAntenaRedeGrafos(rede, 'A', 19, 19);
    inserirAntenaRedeGrafos(rede, 'C', 6, 9);
    inserirAntenaRedeGrafos(rede, 'B', 2, 10);
    inserirAntenaRedeGrafos(rede, 'B', 14, 3);
    inserirAntenaRedeGrafos(rede, 'A', 7, 4);

    printf("Mapa da frequência A:\n");
    mostrarRedeGrafos(rede, frequencias, 3);

    return 0;

	return 0;
}