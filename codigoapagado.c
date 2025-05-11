
//VERTICE* criaVerticeAntena(char freqAntena, int x, int y, int resultado) {
//	resultado = 0;
//	VERTICE* novoVertice = (VERTICE*)malloc(sizeof(VERTICE));
//
//	if (novoVertice == NULL)
//	{
//		resultado = 0;
//		free(novoVertice);
//		return NULL;
//	}
//
//
//	ANTENAS* novaAntena = criarAntena(freqAntena, x, y, resultado);
//
//	if (novaAntena == NULL && resultado != 1)
//	{
//		resultado = 0;
//		free(novaAntena);
//		return NULL;
//	}
//
//	novoVertice->infoAntenas = novaAntena;
//	novoVertice->adjacentes = NULL;
//
//	resultado = 1;
//	return novoVertice;
//}