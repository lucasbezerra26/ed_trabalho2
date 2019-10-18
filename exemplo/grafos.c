#include <stdio.h>
#include <stdlib.h.h>
struct Grafo{
	int eh_ponderado;
	int nro_vertices;
	int grau_max;
	int** aresta;
	float** pesos;
	int* grau;
};

typedef Grafo grafo;

grafo *cria_grafo(int nro_vertices, int grau_max, int eh_ponderado){
	grafo *gr = (grafo*) malloc(sizeof(struct Grafo));
	if ( gr != NULL){
	 	int i;
	 	gr->nro_vertices = nro_vertices
	 	gr->grau_max = grau_max;
	 	gr->eh_ponderado = (eh_ponderado !=0 )?1:0;//só para garanatir que vai ser 0 ou 1
	 	gr->grau = (int*) calloc(nro_vertices,sizeof(int*)); //criando a lista
	 	for (int i = 0; i < nro_vertices; i++){
	 		gr->aresta[i] = (int*) malloc(grau_max*sizeof(int*)); //colocando para apontar cada posição da lista para outra lista  
	 		if (gr->eh_ponderado){
	 			gr->pesos = (float**) malloc(nro_vertices*sizeof(float*));
	 			for (i = 0; i < nro_vertices; i++){
	 				gr->pesos[i] = (float*) malloc(grau_max*sizeof(float)); //fazendo a mesma coisa com os pesos
	 			}
	 		}
	 	}
	} 
}

void libera_grafo(garfo* gr){
	if ( gr != NULL){
		for (int i = 0; i < gr->nro_vertices; i++){
			free(gr->aresta[i]);
		}
		free(gr->aresta);
		if (gr->eh_ponderado){
			for (int i = 0; i < gr->nro_vertices; i++){
				free(gr->pesos[i]);
			}
			free(gr->pesos);
		}
		free(gr->grau);
		free(gr);
	}
}

int insereAresta(grafo* gr, int orig, int dest, int eh_digrafo, float peso){
	if (gr == NULL)
		return 0;
	if(orig < 0 || orig >= gr->nro_vertices)
		return 0;
	if(dest < 0 || dest >= gr->nro_vertices)
		return 0;

	gr->aresta[orig][gr->grau[orig]] = dest;
	if(gr->eh_ponderado)
		gr->pesos[orig][gr->grau[orig]] = peso;
	gr->grau[orig]++;

	if(eh_digrafo == 0)
		insereAresta(gr,dest,orig,1,peso);
	return 1;
}





int main(int argc, char const *argv[])
{
	
	return 0;
}