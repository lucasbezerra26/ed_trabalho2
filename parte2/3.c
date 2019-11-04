#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

// Inicio da busca em profundida 

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
typedef struct Caminho caminho;
caminho *caminhoSuper;

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
	gr->aresta = (int**) malloc(sizeof(int*)*nro_vertices);
	for (int i = 0; i < nro_vertices; i++){
		gr->aresta[i] = (int*) malloc(grau_max * sizeof(int)); //colocando para apontar cada posição da lista para outra lista  
		
		if (gr->eh_ponderado){
			gr->pesos = (float**) malloc(nro_vertices * sizeof(float*));
			for (int j = 0; j < nro_vertices; j++){
				// printf("peso\n");
				gr->pesos[j] = (float*) malloc(grau_max  *sizeof(float)); //fazendo a mesma coisa com os pesos
			}
		}
	}
	// printf("terminei\n");
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

	// printf("Entrando antes do print: %d\n", orig);
	if(eh_digrafo == 0){
		// printf("Entrando: %d\n", orig);
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

caminho *iniciaCaminho(){
	caminho *aux;
	aux = (caminho *) malloc(sizeof(caminho));
	aux->destino = (int *) calloc(sizeof(int),1);
	aux->cidades = 0;
	aux->peso = (float) 0.0;
	return aux;
}

void mostrarCaminho(caminho *caminho){
	printf("Numero de cidades: %d\n", caminho->cidades);
	printf("Dinheiro gasto: %f\n", caminho->peso);
	printf("Cidades: %d",caminho->destino[0]+1);
	for(int i = 1; i<caminho->cidades;i++)
		printf(", %d ", caminho->destino[i]+1);
	printf("\n");
}


void buscando(grafo *gr,pilha *p, int ini, int ant, caminho *caminho){
	p = inserirNaPilha(p, ini-1);
	// printf("foi adcionado a pilha %d \n", ini-1);
	for (int i = 0; i < gr->grau[ini-1]; i++){
		// printf("bucado: %d; res: %d\n",gr->aresta[ini-1][i],buscarNaPilha(p, gr->aresta[ini-1][i]) );
		if (!buscarNaPilha(p, gr->aresta[ini-1][i]) && (gr->valor_total >= caminho->peso + gr->pesos[ini - 1][i])){
			caminho->cidades += 1;
			caminho->destino = (int *) realloc(caminho->destino, caminho->cidades*sizeof(int));
			caminho->destino[caminho->cidades-1] = gr->aresta[ini-1][i];
			caminho->peso += gr->pesos[ini-1][i];
			// printf("----aqui %d , %d \n", caminho->cidades, caminhoSuper->cidades);
			if (caminho->cidades > caminhoSuper->cidades){
				caminhoSuper->cidades = caminho->cidades;
				caminhoSuper->peso =caminho->peso ;
                caminhoSuper->destino = (int *)malloc((caminho->cidades ) * sizeof(int));
                for( int i =0; i< caminhoSuper->cidades; i++){
					caminhoSuper->destino[i] = caminho->destino[i];
					// printf("eita %d, peso %f\n", caminho->destino[i], caminhoSuper->peso);
				}
			}
			buscando(gr, p, gr->aresta[ini-1][i]+1, ini, caminho);
			caminho->cidades -= 1;
			caminho->destino = (int *)realloc(caminho->destino, caminho->cidades * sizeof(int));
			caminho->peso -= gr->pesos[ini-1][i];
		}
	}
	removerNaPilha(p);
}

void buscaProfundidade(grafo *gr,pilha *p, int ini, int ant, caminho *caminho){
    caminhoSuper->cidades = 1;
    caminho->cidades = 1;
    buscando(gr,p,ini,ant,caminho);
}

//fim do que se refere a busca em profundidade

//inicio busca em largura
struct Fila{
	int valor;
	struct Fila *prox;
};
typedef struct Fila fila;

fila *iniciaFila(){
	fila *f;
	f = (fila *) malloc(sizeof(fila));
	f->valor = -1;
	f->prox = NULL;
	return f;
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

int esta_no_caminho(caminho *c, int valor){
	for (int i = 0; i < c->cidades; i++){
		if(c->destino[i] == valor)
			return 1;
	}
	return 0;
}

int caminho_valido(grafo *g, caminho *c){
	int resultado = 1;
	for(int i= 0; i< c->cidades-1; i++){
		int x = 0;
		for(int j = 0; j < g->grau[c->destino[i]]; j++){
			if(g->aresta[c->destino[i]][j] == c->destino[i+1]){
				x = 1;
				break;
			}
		}
		if(x == 0){
			resultado = 0;
			break;
		}		
	}
	return resultado;
}

void buscaLargura(grafo *g, fila *f, int ini, int final, caminho *c, caminho *super){
	int visitados[g->nro_vertices];
			
	for(int i = 0; i < g->nro_vertices; i++){
		visitados[i] = 0;
	}
	
	int *fila_vertice, final_fila = 0, inicio_fila = 0, cont = 1, vertice_atual;
	fila_vertice = (int *) malloc(g->nro_vertices * sizeof(int));
	final_fila++;
	fila_vertice[final_fila] = ini-1;
	visitados[ini-1] = cont;	

	if(g->valor_total >= c->peso + g->pesos[ini - 1][0]){
		c->cidades += 1;
		c->destino = (int *) realloc(c->destino, c->cidades*sizeof(int));
		c->destino[c->cidades-1] = ini-1;
	}

	float ultimo_peso = 0;
	
	while (inicio_fila != final_fila){
		inicio_fila++;
		vertice_atual = fila_vertice[inicio_fila];
		cont++;
		
		for (int i = 0; i < g->grau[vertice_atual]; i++){
			if(!esta_no_caminho(c, g->aresta[vertice_atual][i]) && (g->valor_total >= c->peso + g->pesos[vertice_atual][i])){
				c->cidades += 1;
				c->destino = (int *) realloc(c->destino, c->cidades*sizeof(int));
				c->destino[c->cidades-1] = g->aresta[vertice_atual][i];
				c->peso += g->pesos[vertice_atual][i];
				ultimo_peso = g->pesos[vertice_atual][i];
				

				final_fila++;
				fila_vertice[final_fila] = g->aresta[vertice_atual][i];
				visitados[g->aresta[vertice_atual][i]] = cont;

			}
		}

		if(caminho_valido(g, c)){
			if(super->cidades < c->cidades){
				super->cidades = c->cidades;
				super->peso =c->peso ;
				super->destino = (int *)malloc(c->cidades * sizeof(int));
				for( int i =0; i< super->cidades; i++){
					super->destino[i] = c->destino[i];
				}
			}
		}else{
			c->cidades -= 1;
			c->peso -= ultimo_peso;
		}


	}
	free(fila_vertice);
}


//fim do que se refere a busca em largura

grafo **geraCasos(){
    grafo **casosGrafos = (grafo**) malloc(sizeof(grafo*)*10);
    casosGrafos[0] = cria_grafo(8, 8, 1, 10);
    casosGrafos[1] = cria_grafo(9, 9, 1, 15);
    casosGrafos[2] = cria_grafo(10, 10, 1, 10);
    casosGrafos[3] = cria_grafo(11, 11, 1, 12);
    casosGrafos[4] = cria_grafo(12, 12, 1, 8);
    casosGrafos[5] = cria_grafo(8, 8, 1, 5);
    casosGrafos[6] = cria_grafo(10, 10, 1, 18);
    casosGrafos[7] = cria_grafo(12, 12, 1, 8);
    casosGrafos[8] = cria_grafo(12, 12, 1, 18);
    casosGrafos[9] = cria_grafo(11, 11, 1, 18);

    //inicio: ligacoes do casosGrafos[0]
    insereAresta(casosGrafos[0], 1, 2, 0, 1);
    insereAresta(casosGrafos[0], 2, 3, 0, 2);
    insereAresta(casosGrafos[0], 3, 5, 0, 1);
    insereAresta(casosGrafos[0], 5, 6, 0, 5);
    insereAresta(casosGrafos[0], 6, 7, 0, 2);
    insereAresta(casosGrafos[0], 3, 8, 0, 1);
    // insereAresta(casosGrafos[0], 8, 9, 0, 1);
    insereAresta(casosGrafos[0], 1, 4, 0, 1);
    //fim

    //inicio: ligacoes do casosGrafos[1]
    insereAresta(casosGrafos[1], 1, 2, 0, 1);
    insereAresta(casosGrafos[1], 2, 3, 0, 2);
    insereAresta(casosGrafos[1], 3, 5, 0, 1);
    insereAresta(casosGrafos[1], 5, 6, 0, 5);
    insereAresta(casosGrafos[1], 6, 7, 0, 2);
    insereAresta(casosGrafos[1], 3, 8, 0, 1);
    insereAresta(casosGrafos[1], 7, 9, 0, 1);
    insereAresta(casosGrafos[1], 6, 4, 0, 1);
    //fim
    
    //inicio: ligacoes do casosGrafos[2]    
    insereAresta(casosGrafos[2], 1, 2, 0, 1);
    insereAresta(casosGrafos[2], 2, 3, 0, 2);
    insereAresta(casosGrafos[2], 3, 6, 0, 1);
    insereAresta(casosGrafos[2], 6, 8, 0, 1);
    insereAresta(casosGrafos[2], 8, 9, 0, 1);
    insereAresta(casosGrafos[2], 2, 4, 0, 1);
    insereAresta(casosGrafos[2], 3, 5, 0, 1);
    insereAresta(casosGrafos[2], 8, 7, 0, 1);
    insereAresta(casosGrafos[2], 8, 10, 0, 1);
    //fim

    //inicio: ligacoes do casosGrafos[3]    
    insereAresta(casosGrafos[3], 1, 2, 0, 1);
    insereAresta(casosGrafos[3], 2, 3, 0, 2);
    insereAresta(casosGrafos[3], 3, 6, 0, 1);
    insereAresta(casosGrafos[3], 6, 8, 0, 1);
    insereAresta(casosGrafos[3], 6, 11, 0, 1);
    insereAresta(casosGrafos[3], 8, 9, 0, 1);
    insereAresta(casosGrafos[3], 2, 4, 0, 1);
    insereAresta(casosGrafos[3], 8, 5, 0, 1);
    insereAresta(casosGrafos[3], 9, 7, 0, 4);
    insereAresta(casosGrafos[3], 3, 10, 0, 1);
    //fim

    //inicio: ligacoes do casosGrafos[4]    
    insereAresta(casosGrafos[4], 1, 2, 0, 1);
    insereAresta(casosGrafos[4], 2, 3, 0, 1);
    insereAresta(casosGrafos[4], 3, 6, 0, 1);
    insereAresta(casosGrafos[4], 6, 8, 0, 1);
    insereAresta(casosGrafos[4], 8, 9, 0, 1);
    insereAresta(casosGrafos[4], 6, 12, 0, 2);
    insereAresta(casosGrafos[4], 2, 4, 0, 1);
    insereAresta(casosGrafos[4], 6, 5, 0, 1);
    insereAresta(casosGrafos[4], 8, 7, 0, 2);
    insereAresta(casosGrafos[4], 9, 10, 0, 1);
    insereAresta(casosGrafos[4], 10, 11, 0, 1);
    //fim

    //inicio: ligacoes do casosGrafos[5]    
    insereAresta(casosGrafos[5], 1, 2, 0, 1);
    insereAresta(casosGrafos[5], 2, 3, 0, 2);
    insereAresta(casosGrafos[5], 3, 6, 0, 1);
    insereAresta(casosGrafos[5], 6, 8, 0, 3);
    insereAresta(casosGrafos[5], 2, 4, 0, 1);
    insereAresta(casosGrafos[5], 3, 5, 0, 1);
    insereAresta(casosGrafos[5], 6, 7, 0, 1);
    //fim

    //inicio: ligacoes do casosGrafos[6]
    insereAresta(casosGrafos[6], 1, 2, 0, 1);
    insereAresta(casosGrafos[6], 2, 3, 0, 1);
    insereAresta(casosGrafos[6], 3, 6, 0, 2);
    insereAresta(casosGrafos[6], 6, 8, 0, 7);
    insereAresta(casosGrafos[6], 2, 4, 0, 1);
    insereAresta(casosGrafos[6], 1, 5, 0, 3);
    insereAresta(casosGrafos[6], 6, 7, 0, 6);
    insereAresta(casosGrafos[6], 8, 10, 0, 1);
    //fim

    //inicio: ligacoes do casosGrafos[7]
    insereAresta(casosGrafos[7], 1, 2, 0, 1);
    insereAresta(casosGrafos[7], 2, 3, 0, 1);
    insereAresta(casosGrafos[7], 3, 6, 0, 3);
    insereAresta(casosGrafos[7], 6, 8, 0, 1);
    insereAresta(casosGrafos[7], 8, 9, 0, 2);
    insereAresta(casosGrafos[7], 6, 12, 0, 1);
    insereAresta(casosGrafos[7], 2, 4, 0, 1);
    insereAresta(casosGrafos[7], 3, 5, 0, 2);
    insereAresta(casosGrafos[7], 6, 7, 0, 1);
    insereAresta(casosGrafos[7], 8, 10, 0, 1);
    insereAresta(casosGrafos[7], 9, 11, 0, 2);
    //fim

    //inicio: ligacoes do casosGrafos[8]
    insereAresta(casosGrafos[8], 1, 2, 0, 1);
    insereAresta(casosGrafos[8], 2, 3, 0, 3);
    insereAresta(casosGrafos[8], 3, 6, 0, 1);
    insereAresta(casosGrafos[8], 6, 8, 0, 10);
    insereAresta(casosGrafos[8], 2, 4, 0, 4);
    insereAresta(casosGrafos[8], 3, 5, 0, 1);
    insereAresta(casosGrafos[8], 6, 7, 0, 10);
    insereAresta(casosGrafos[8], 8, 9, 0, 5);
    insereAresta(casosGrafos[8], 9, 10, 0, 1);
    insereAresta(casosGrafos[8], 10, 11, 0, 2);
    //fim

    //inicio: ligacoes do casosGrafos[9]
    insereAresta(casosGrafos[9], 1, 2, 0, 1);
    // insereAresta(casosGrafos[9], 1, 10, 0, 3);
    insereAresta(casosGrafos[9], 2, 3, 0, 3);
    insereAresta(casosGrafos[9], 3, 4, 0, 4);
    insereAresta(casosGrafos[9], 2, 5, 0, 1);
    insereAresta(casosGrafos[9], 4, 6, 0, 1);
    insereAresta(casosGrafos[9], 6, 7, 0, 10);
    insereAresta(casosGrafos[9], 4, 8, 0, 5);
    insereAresta(casosGrafos[9], 7, 9, 0, 5);
    insereAresta(casosGrafos[9], 9, 10, 0, 1);
    insereAresta(casosGrafos[9], 10, 11, 0, 9);
    //fim

    return casosGrafos;
}

long getMicrotime(){
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
}




int main(){
    grafo **casosGrafos;
    int ini,fim;
    casosGrafos = geraCasos();


    for(int i=0; i<10;i++){
        pilha *p = NULL;
        caminho *caminho = NULL;
        fila *f = NULL;
        p = iniciaPilha();
        caminho = iniciaCaminho();
        caminhoSuper = iniciaCaminho();
        f = iniciaFila();

        printf("%dº-------Profundidade------------\n", i);
        ini = getMicrotime();
        buscaProfundidade(casosGrafos[i], p, 1, 1, caminho);
        fim = getMicrotime();
        printf("Tempo: %d\n",fim-ini);
        mostrarCaminho(caminhoSuper);

        caminho = iniciaCaminho();
        caminhoSuper = iniciaCaminho();

        printf("%dº-------Largura------------\n", i);
        ini = getMicrotime();
        buscaLargura(casosGrafos[i], f, 1, 1, caminho, caminhoSuper);
        fim = getMicrotime();
        printf("Tempo: %d\n",fim-ini);
        mostrarCaminho(caminhoSuper);

        printf("\n");
        free(caminho);
        free(p);
        free(f);
    }
    for(int i=0; i<10;i++)
        libera_grafo(casosGrafos[i]);
    free(	casosGrafos);
    free(caminhoSuper);
    return 0;
}