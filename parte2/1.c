#include <stdio.h>
#include <stdlib.h>
int t;

struct Grafo{
	int eh_ponderado;
	int nro_vertices;
	int grau_max; //numero maximo de ligações
	int** aresta; //as conexões 
	float** pesos;
	int* grau; //quantas aresta o vertice ja possue
	float valor_total;
};
typedef struct Grafo grafo;

struct Caminho{
	int *destino;
	int cidades;
	float peso;
};
typedef struct Caminho Caminho;

struct Pilha{
	int valor;
	struct Pilha *prox;
	// int *memoria;
    // int topo;
};
typedef struct Pilha pilha;

grafo *cria_grafo(int nro_vertices, int grau_max, int eh_ponderado, float valor_total){
	grafo *gr = (grafo*) malloc(sizeof(grafo));
	int i;
	gr->nro_vertices = nro_vertices;
	gr->grau_max = grau_max;
	gr->valor_total = valor_total;
	gr->eh_ponderado = (eh_ponderado !=0 )?1:0;//só para garanatir que vai ser 0 ou 1
	gr->grau = (int*) calloc(nro_vertices,sizeof(int*)); //criando a lista
	gr->aresta = (int*) malloc(sizeof(int));
	for (int i = 0; i < nro_vertices; i++){
		gr->aresta[i] = (int*) malloc(grau_max * sizeof(int*)); //colocando para apontar cada posição da lista para outra lista  
	}
	if (gr->eh_ponderado){
		gr->pesos = (float**) malloc(nro_vertices * sizeof(float*));
		for (i = 0; i < nro_vertices; i++){
			printf("peso\n");
			gr->pesos[i] = (float*) malloc(grau_max  *sizeof(float)); //fazendo a mesma coisa com os pesos
		}
	}
	printf("terminei\n");
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

Caminho *iniciaCaminho(){
	Caminho *aux;
	aux = (Caminho *) malloc(sizeof(Caminho));
	aux->destino = (int *) malloc(sizeof(int));
	aux->cidades = 0;
	aux->peso = (float) 0.0;
	return aux;
}

pilha *iniciaPilha(){
	pilha *aux;
	aux = (pilha *) malloc(sizeof(pilha));
	aux->prox = NULL;
	return aux;
}

pilha *inserirNaPilha(pilha *listaPilha, int valor){
	pilha *novo = (pilha*) malloc(sizeof(pilha));
	novo->valor = valor;
	novo->prox = listaPilha;
	return novo;
}

pilha *removerNaPilha(pilha *listaPilha){
	if( listaPilha == NULL)
		return NULL;
	return listaPilha->prox;
}

int buscarNaPilha(pilha *listaPilha, int valor){
	if( listaPilha == NULL){
		return 0;
	}
	if( listaPilha->valor == valor)
		return 1;
	return buscarNaPilha(listaPilha->prox, valor);
}

int topoDaPilha(pilha *listaPilha){
	return listaPilha->valor;
}

void mostrarCaminho(Caminho *caminho){
	printf("Numero de cidades: %d", caminho->cidades);
	printf("Dinheiro gasto: %f", caminho->peso);
	printf("Cidades: ");
	for(int i = 0; i<caminho->cidades;i++)
		printf(" ,%d", caminho->destino[i]);
}

Caminho *caminhoSuper;

void busca(grafo *gr,pilha *p, int ini, int ant, Caminho *caminho){
	inserirNaPilha(p, ini);
	printf("tenho grau %d\n", gr->grau_max);
	printf("tenho grau %d\n", gr->grau[0]);
	for (int i = 0; i < gr->grau[ini]; i++){
		if (!buscarNaPilha(p, gr->aresta[ini][i]) && (gr->valor_total <= caminho->peso + gr->pesos[ant][ini])){
			caminho->cidades += 1; 
			caminho->destino = (int*) realloc(caminho->destino, caminho->cidades*sizeof(int));
			caminho->destino[caminho->cidades] = ini;
			caminho->peso += gr->pesos[ant][ini];
			if( caminho->cidades > caminhoSuper->cidades )
				caminhoSuper = caminho;
			busca(gr,p, gr->aresta[ini][i], ini, caminho);
		}
	}
	removerNaPilha(p);
}

int main(){
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

	printf("grafo=====\n");

	pilha *p = NULL;
	Caminho *caminho = NULL;

	p = iniciaPilha();
	caminho = iniciaCaminho();
	
	busca(g,p,1,1,caminho);
	mostrarCaminho(caminhoSuper);
	return 0;
}