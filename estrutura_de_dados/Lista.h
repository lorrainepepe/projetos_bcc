// FUNÇÕES LISTA 

#include<stdlib.h>
#include<stdio.h>

typedef struct node{
	int info;
	struct node *prox;
}No;

/*No* InsereLista (No *l,int val){
	No* novo;
	novo =(No*)malloc(sizeof(No));
	novo->info = val;
	novo->prox = l;
	return novo;// endereço do inicio da lista
}*/

// insere inicio------------------------------------
No* InsereLista (No *l,int val){
	No* novo;
 	No* a = NULL; // ponteiro para elemento anterior 
	No* p = l; // ponteiro para percorrer a lista 
	 // procura posição de inserção 
	 while (p != NULL && p->info < val){ 
	 	a = p; p = p->prox; 
	 }
	 // cria novo elemento 
	 novo = (No*) malloc(sizeof(No));
	 novo->info = val;
	 // encadeia elemento
	 if (a == NULL){ // insere elemento no início 
	 	novo->prox = l; l = novo; 
	 }
	 else { // insere elemento no meio da lista 
		novo->prox = a->prox;
	 	a->prox = novo; 
	 }
	 return l;
}
// busca lista------------------------------------
No* BuscaLista (No* l,int val){
	No *aux;
	aux = l;
	while(aux != NULL){
		if(aux->info == val) return aux;
		aux = aux->prox;
	}
	return NULL;
}

// apagar lista------------------------------------
void LiberaLista(No* l){
	if(l != NULL){
		No *proxNode, *atual;
		
		atual = l->prox;
		while(atual != NULL){
			proxNode = atual->prox;
			free(atual);
			atual = proxNode;
		}
	}
}

//Conta Elementos------------------------------------
int Conta_elemento(No *L){
	int cont=0;	
	while(L != NULL){
		cont++;
		L=L->prox;
	}
	return cont;	
}

///Imprime------------------------------------
void ImprimeLista(No* l){
	if (l != NULL) {
 /* imprime primeiro elemento */
 	printf("info: %d\n",l->info);
 /* imprime sub-lista */
 	ImprimeLista(l->prox);
 }
}
