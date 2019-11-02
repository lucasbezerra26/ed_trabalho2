#include <stdio.h>
#include <stdlib.h>
int t;

struct Grafo
{
	int eh_ponderado;
	int nro_vertices;
	int grau_max; //numero maximo de ligações
	int **aresta; //as conexões
	float **pesos;
	int *grau; //quantas aresta o vertice ja possue
	float valor_total;
};
typedef struct Grafo grafo;

struct Fila{
	int valor;
	struct Fila *prox;
};
typedef struct Fila fila;

struct Caminho{
	int *destino;
	int cidades;
	float peso;
};
typedef struct Caminho caminho;

fila *iniciaFila(){
	fila *f;
	f = (fila *) malloc(sizeof(fila));
	f->valor = -1;
	f->prox = NULL;
	return f;
}

caminho *iniciaCaminho(){
	caminho *c;
	c = (caminho *) malloc(sizeof(caminho));
	c->destino = (int *) malloc(sizeof(int));
	c->cidades = 0;
	c->peso = 0;
	return c;
}


grafo *cria_grafo(int nro_vertices, int grau_max, int eh_ponderado, float valor_total)
{
	grafo *gr = (grafo *)malloc(sizeof(grafo));
	int i;
	gr->nro_vertices = nro_vertices;
	gr->grau_max = grau_max;
	gr->valor_total = valor_total;
	gr->eh_ponderado = eh_ponderado;					 //só para garanatir que vai ser 0 ou 1
	gr->grau = (int *)calloc(nro_vertices, sizeof(int)); //criando a lista
	gr->aresta = (int **)malloc(sizeof(int *) * nro_vertices);
	for (int i = 0; i < nro_vertices; i++)
	{
		gr->aresta[i] = (int *)malloc(grau_max * sizeof(int)); //colocando para apontar cada posição da lista para outra lista

		if (gr->eh_ponderado)
		{
			gr->pesos = (float **)malloc(nro_vertices * sizeof(float *));
			for (int j = 0; j < nro_vertices; j++)
			{
				gr->pesos[j] = (float *)malloc(grau_max * sizeof(float)); //fazendo a mesma coisa com os pesos
			}
		}
	}
	return gr;
}

void libera_grafo(grafo *gr)
{
	if (gr != NULL)
	{
		for (int i = 0; i < gr->nro_vertices; i++)
		{
			free(gr->aresta[i]);
		}
		free(gr->aresta);
		if (gr->eh_ponderado)
		{
			for (int i = 0; i < gr->nro_vertices; i++)
			{
				free(gr->pesos[i]);
			}
			free(gr->pesos);
		}
		free(gr->grau);
		free(gr);
	}
}

int insereAresta(grafo *gr, int orig, int dest, int eh_digrafo, float peso)
{
	//inicio (verificando se tudo existe)
	if (gr == NULL)
		return 0;
	if (orig - 1 < 0 || orig - 1 >= gr->nro_vertices)
		return 0;
	if (dest - 1 < 0 || dest - 1 >= gr->nro_vertices)
		return 0;
	//fim;
	gr->aresta[orig - 1][gr->grau[orig - 1]] = dest - 1; //adicionando o destino a ultima posicao da lista
	if (gr->eh_ponderado)
		gr->pesos[orig - 1][gr->grau[orig - 1]] = peso; //se for ponderado faz a mesma coisa ao peso
	gr->grau[orig - 1]++;								// e incrementa o numero de ligações para aquele vertice

	if (eh_digrafo == 0)
	{
		insereAresta(gr, dest, orig, 1, peso); //se não for digrafo ele vai ligando o dest a orig, o 1 indicando que é digrafo é para ele repetir somente uma vez
	}
	return 1;
}

void imprimirGrafo(grafo *gr)
{
	for (int x = 0; x < gr->nro_vertices; x++)
	{
		for (int i = 0; i < gr->grau[x]; i++)
			printf("%d tem ligacao com %d\n", x + 1, gr->aresta[x][i] + 1);
	}
}

void enfileira(fila *f, int valor){
	fila *aux;
	aux = f;
	while(aux->valor != -1)
		aux = aux->prox;

	aux->valor = valor;
	aux->prox = (fila *) malloc(sizeof(fila));
	aux->prox->valor = -1;
	aux->prox->prox = NULL;
}

void desinfileira(fila *f){
	fila *aux;
	aux = f;
	f = f->prox;
	free(aux);
}

int inicio_fila(fila *f){
	return f->valor;
}

int marcado(caminho *c, int valor){
	for (int i = 0; i < c->cidades; i++){
		if (c->destino[i] == valor){
			return 1;
		}
	}
	return 0;
}

void busca(grafo *g, fila *f, int ini, int final, caminho *c){
	int visitados[g->nro_vertices];
			
	for(int i = 0; i < g->nro_vertices; i++){
		visitados[i] = 0;
	}
	
	int *fila_vertice, final_fila = 0, inicio_fila = 0, cont = 1, vertice_atual;
	fila_vertice = (int *) malloc(g->nro_vertices * sizeof(int));
	final_fila++;
	fila_vertice[final_fila] = ini-1;
	visitados[ini-1] = cont;
	
	while (inicio_fila != final_fila){
		inicio_fila = (inicio_fila + 1) % g->nro_vertices;
		vertice_atual = fila_vertice[inicio_fila];
		cont++;
		
		for (int i = 0; i < g->grau[vertice_atual]; i++){
			if(!visitados[g->aresta[vertice_atual][i]] && (g->valor_total >= c->peso + g->pesos[ini - 1][i])){
				final_fila = (final_fila + 1) % g->nro_vertices;
				fila_vertice[final_fila] = g->aresta[vertice_atual][i];
				visitados[g->aresta[vertice_atual][i]] = cont;
				c->cidades += 1;
				c->destino = (int *) realloc(c->destino, c->cidades*sizeof(int));
				c->destino[c->cidades-1] = g->aresta[ini-1][i];
				c->peso += g->pesos[ini-1][i];
			}
		}
	}
	free(fila_vertice);
}

void mostrarCaminho(caminho *c){
	printf("Numero de cidades: %d\n", c->cidades);
	printf("Dinheiro gasto: %f\n", c->peso);
	printf("Cidades: 1");
	for(int i = 0; i<c->cidades;i++)
		printf(", %d ", c->destino[i]+1);
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

	// for (int i = 0; i < g->nro_vertices; i++){
	// 	printf("cidade %d ", i+1);
	// 	for (int j = 0; j < g->grau[i]; j++){
	// 		printf("ligacao %d ", g->aresta[i][j] + 1);
	// 	}
	// 	printf("\n");
	// }
	
	fila *f = NULL;
	caminho *c = NULL;
	f = iniciaFila();
	c = iniciaCaminho();
	
	busca(g,f,1,4,c);

	mostrarCaminho(c);
	return 0;
}