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
Caminho *caminhoSuper;

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
	gr->eh_ponderado = eh_ponderado;//só para garanatir que vai ser 0 ou 1
	gr->grau = (int*) calloc(nro_vertices,sizeof(int)); //criando a lista
	gr->aresta = (int**) malloc(sizeof(int*));
	for (int i = 0; i < nro_vertices; i++){
		gr->aresta[i] = (int*) malloc(grau_max * sizeof(int)); //colocando para apontar cada posição da lista para outra lista  
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
	if(orig-1 < 0 || orig-1 >= gr->nro_vertices)
		return 0;
	if(dest-1 < 0 || dest-1 >= gr->nro_vertices)
		return 0;
	//fim;
	gr->aresta[orig-1][gr->grau[orig-1]] = dest-1; //adicionando o destino a ultima posicao da lista
	if(gr->eh_ponderado)
		gr->pesos[orig-1][gr->grau[orig-1]] = peso; //se for ponderado faz a mesma coisa ao peso
	gr->grau[orig-1]++; // e incrementa o numero de ligações para aquele vertice 	

	printf("Entrando antes do print: %d\n", orig);
	if(eh_digrafo == 0){
		printf("Entrando: %d\n", orig);
		insereAresta(gr,dest,orig,1,peso); //se não for digrafo ele vai ligando o dest a orig, o 1 indicando que é digrafo é para ele repetir somente uma vez
	}
	return 1;
}

void imprimirGrafo(grafo *gr){
	for( int x=0; x<gr->nro_vertices; x++){
		for( int i=0; i< gr->grau[x]; i++)
			printf("%d tem ligacao com %d\n",x+1, gr->aresta[x][i]+1);
	}
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

Caminho *iniciaCaminho(){
	Caminho *aux;
	aux = (Caminho *) malloc(sizeof(Caminho));
	aux->destino = (int *) malloc(sizeof(int));
	aux->cidades = 0;
	aux->peso = (float) 0.0;
	return aux;
}

void mostrarCaminho(Caminho *caminho){
	printf("Numero de cidades: %d\n", caminho->cidades);
	printf("Dinheiro gasto: %f\n", caminho->peso);
	printf("Cidades: 1");
	for(int i = 0; i<caminho->cidades;i++)
		printf(", %d ", caminho->destino[i]+1);
}


void busca(grafo *gr,pilha *p, int ini, int ant, Caminho *caminho){
	p = inserirNaPilha(p, ini-1);
	printf("foi adcionado a pilha %d \n", ini-1);
	// printf("tenho grau %d\n", gr->grau_max);
	// printf("tenho grau %d\n", gr->grau[0]);
	// printf("ini %d\n", ini);
	for (int i = 0; i < gr->grau[ini-1]; i++){
		// printf("aresta %d, i=%d\n",gr->aresta[1][0],i);
		printf("bucado: %d; res: %d\n",gr->aresta[ini-1][i],buscarNaPilha(p, gr->aresta[ini-1][i]) );
		if (!buscarNaPilha(p, gr->aresta[ini-1][i]) && (gr->valor_total >= caminho->peso + gr->pesos[ini - 1][i])){
			caminho->cidades += 1;
			caminho->destino = (int *) realloc(caminho->destino, caminho->cidades*sizeof(int));
			caminho->destino[caminho->cidades-1] = gr->aresta[ini-1][i];
			caminho->peso += gr->pesos[ini-1][i];
			printf("----aqui %d , %d \n", caminho->cidades, caminhoSuper->cidades);
			if (caminho->cidades > caminhoSuper->cidades || ( (caminho->cidades == caminhoSuper->cidades) && (caminhoSuper->peso > caminho->peso) )){
				// if (caminho->cidades == caminhoSuper->cidades && caminhoSuper->peso > caminho->peso)
				caminhoSuper->cidades = caminho->cidades;
				caminhoSuper->peso =caminho->peso ;
				caminhoSuper->destino = (int *)malloc(caminho->cidades * sizeof(int));
				for( int i =0; i< caminhoSuper->cidades; i++){
					caminhoSuper->destino[i] = caminho->destino[i];
					printf("eita %d, peso %f\n", caminho->destino[i], caminhoSuper->peso);
				}
				// printf("adc\n");
			}
			// printf("a %d\n",gr->aresta[ini - 1][i]);
			// printf("eii,\n");
			busca(gr, p, gr->aresta[ini-1][i]+1, ini, caminho);
			caminho->cidades -= 1;
			caminho->destino = (int *)realloc(caminho->destino, caminho->cidades * sizeof(int));
			caminho->peso -= gr->pesos[ini-1][i];
		}
		
		// printf("eii\n");
		// printf("valor total %f\n", gr->valor_total);
		// printf("valor soma %f\n", caminho->peso + gr->pesos[ant - 1][ini - 1]);
		// if (!buscarNaPilha(p, gr->aresta[ini-1][i]) && (gr->valor_total >= caminho->peso + gr->pesos[ant-1][ini-1])){
		// 	caminho->cidades += 1; 
		// 	caminho->destino = (int*) realloc(caminho->destino, caminho->cidades*sizeof(int));
		// 	caminho->destino[caminho->cidades] = ini;
		// 	caminho->peso += gr->pesos[ant-1][ini-1];
		// 	if( caminho->cidades > caminhoSuper->cidades )
		// 		caminhoSuper = caminho;
		// 	printf("eii-%d",caminhoSuper->cidades);
		// 	busca(gr,p, gr->aresta[ini-1][i], ini, caminho);
		// }
		// caminho->cidades -= 1;
		// free(&caminho->destino[caminho->cidades]);
		// caminho->peso -= gr->pesos[ant - 1][ini - 1];
	}
	removerNaPilha(p);
}

int main(){
	// main

	grafo *g = NULL;
	int num_cidades;
	float caixa;

	// printf("Digite a quantidade de cidades: ");
	// scanf(" %d", &num_cidades);
	
	// printf("Qual o valor máximo para gasto: ");
	// scanf(" %f", &caixa);

	// g = cria_grafo(num_cidades, num_cidades, 1, caixa);
	g = cria_grafo(4, 4, 1, 15);

	// printf("Suas cidades são numeradas entre 1 e %d\n", num_cidades);

	int status = 1;
	int cidade_a, cidade_b;
	float peso;
	insereAresta(g, 1, 2, 0, 1);
	insereAresta(g, 2, 3, 0, 2);
	insereAresta(g, 2, 4, 0, 1);

	// insereAresta(g, 1, 5, 0, 1);
	// insereAresta(g, 1, 2, 0, 1);

	// while (status == 1){
	// 	printf("Digite as rodovias que ligam a cidade. \nEx.: 1 5 ligam as cidades 1 e 5 ida e volta\n");
	// 	scanf(" %d %d", &cidade_a, &cidade_b);
	// 	printf("Qual o valor do pedágio? ");
	// 	scanf(" %f", &peso);
	// 	insereAresta(g, cidade_a, cidade_b, 0, peso);
	// 	printf("Deseja inserir mais? 1-sim/2-nao \n");
	// 	scanf(" %d", &status);	
	// };

	printf("grafo=====\n");

	pilha *p = NULL;
	Caminho *caminho = NULL;

	p = iniciaPilha();
	caminho = iniciaCaminho();
	caminhoSuper = iniciaCaminho();

	// p = inserirNaPilha(p,1);
	// printf("Merda desgracada %d",buscarNaPilha(p,1));
	
	imprimirGrafo(g);
	busca(g,p,1,1,caminho);
	mostrarCaminho(caminhoSuper);
	printf("\n");
	return 0;
}