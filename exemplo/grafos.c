#include <stdio.h>
#include <stdlib.h>

struct Grafo{
	int eh_ponderado;
	int nro_vertices;
	int grau_max; //numero maximo de liagações
	int** aresta; //as conexões 
	float** pesos;
	int* grau; //quantas aresta o vertice ja possue
	float valor_total;
};

typedef struct Grafo grafo;

grafo *cria_grafo(int nro_vertices, int grau_max, int eh_ponderado, float valor_total){
	grafo *gr = (grafo*) malloc(sizeof(grafo));
	if ( gr == NULL){
	 	int i;
	 	gr->nro_vertices = nro_vertices;
	 	gr->grau_max = grau_max;
		gr->valor_total = valor_total;
	 	gr->eh_ponderado = (eh_ponderado !=0 )?1:0;//só para garanatir que vai ser 0 ou 1
	 	gr->grau = (int*) calloc(nro_vertices,sizeof(int*)); //criando a lista
	 	for (int i = 0; i < nro_vertices; i++){
	 		gr->aresta[i] = (int*) malloc(grau_max * sizeof(int*)); //colocando para apontar cada posição da lista para outra lista  
	 		if (gr->eh_ponderado){
	 			gr->pesos = (float**) malloc(nro_vertices * sizeof(float*));
	 			for (i = 0; i < nro_vertices; i++){
	 				gr->pesos[i] = (float*) malloc(grau_max  *sizeof(float)); //fazendo a mesma coisa com os pesos
	 			}
	 		}
	 	}
	
	}
	return gr; 
}

void libera_grafo(grafo* gr){
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
	//inicio (verificando se tudo existe)
	if (gr == NULL)
		return 0;
	if(orig < 0 || orig >= gr->nro_vertices)
		return 0;
	if(dest < 0 || dest >= gr->nro_vertices)
		return 0;
	//fim;
	gr->aresta[orig][gr->grau[orig]] = dest; //adicionando o destino a ultima posicao da lista
	if(gr->eh_ponderado)
		gr->pesos[orig][gr->grau[orig]] = peso; //se for ponderado faz a mesma coisa ao peso
	gr->grau[orig]++; // e incrementa o numero de ligações para aquele vertice 	

	if(eh_digrafo == 0)
		insereAresta(gr,dest,orig,1,peso); //se não for digrafo ele vai ligando o dest a orig, o 1 indicando que é digrafo é para ele repetir somente uma vez
	return 1;
}

int removerAresta(grafo* gr, int orig, int dest, int eh_digrafo){
	//inicio (verificando se tudo existe)
	if (gr == NULL)
		return 0;
	if(orig < 0 || orig >= gr->nro_vertices)
		return 0;
	if(dest < 0 || dest >= gr->nro_vertices)
		return 0;
	//fim;

	int i=0;
	while(i<grau[orig] && gr->aresta[orig][i]!=dest)
		i++;
	if(i == gr->grau[orig]) //se n encontrou
		return 0;
	gr->grau[orig]--; //voltando um pq o grau fica com o numero de aresta mais 1.
	gr->aresta[orig][i] = gr->arestas[orig][gr->grau[orig]]; //jogando a ultima aresta na posição que quer excluir
	if(gr->eh_ponderado)
		gr->peso[orig][i]=gr->pesos[orig][gr->grau[orig]]; //fazendo a mesma coisa com peso
	if(eh_digrafo == 0)
		removerAresta(gr,dest,orig,1);
	return 1;
}

int main(int argc, char const *argv[]){
	// main

	grafo *g = NULL;
	int num_cidades;
	float caixa;

	printf("Digite a quantidade de cidades: ");
	scanf(" %d", &num_cidades);
	
	printf("Qual o valor máximo para gasto: ");
	scanf(" %f", &caixa);

	g = cria_grafo(num_cidades, num_cidades, 1, caixa);

	printf("Suas cidades são numeradas entre 1 e %d\n", num_cidades);

	int status = 1;
	int cidade_a, cidade_b;
	float peso;

	while (status == 1){
		printf("Digite as rodovias que ligam a cidade. \nEx.: 1 5 ligam as cidades 1 e 5 ida e volta\n");
		scanf(" %d %d", &cidade_a, &cidade_b);
		printf("Qual o valor do pedágio? ");
		scanf(" %f", &peso);
		insereAresta(g, cidade_a, cidade_b, 0, peso);
		printf("Deseja inserir mais? 1-sim/2-nao \n");
		scanf(" %d", &status);	
	};

	return 0;
}