/* Funções para lista */

#include<stdio.h>
#include<stdlib.h>

//declaração Lista
struct node
{
	int info;
	struct node *next;
}list;

///Imprime
void *imprime(lista *l){
	while(l!=NULL)
	{
		printf("%d",l->info);
		l=l->info;
	}
}

// imprime recursiva
void *imprime_recursiva(lista*l){
    if (l==NULL)return;
    printf("%d\n",l->info);
	imprime_recursiva(l-prox);
}

// apagar lista 
lista* apagar_lista(lista *l){
	lista *p;
	if(l==NULL)return NULL;
	p=l;
	l=l->prox;
	free(p);
	apagar_lista(l);
}
// insere inicio
lista* insere_inicio(lista*l,int val){
	lista* novo;
	novo =(lista*) malloc(sizeof(lista));
	novo-> info =val;
	novo -> prox = l;
	return novo;// endereço do inicio da lista
}
//insere final
lista* insere_final(lista *l, int x){
	lista *novo,*p;
	novo=(lista*)malloc(sizeof(lista));
	novo-> info=x;
	novo->prox=NULL;//l->prox=NULL;
	if(l==NULL){
		l=novo;// l->prox=novo;
	}
	else{
		p=l;//p=l->prox;
		while(p->prox!=NULL)p=p->prox;
		p->prox=novo;
	}
	return l;
}
// busca lista
lista* Busca(lista *l,int val){
	lista *aux;
	aux=l;
	while(aux!=NULL){
		if(aux->info==val)return aux;
		aux= aux->prox;
	}
	return NULL;
}
// intercala 
lista* intercala(lsita *x, lista *y){
	lista *l,*x1,*y1;
	if(x==NULL)return y;
	if(y==NULL)return x;
	l=x;
while(x!=NULL && y!=NULL){
	x1=x->prox;
	y1=y->prox;
	x-> prox=y;
	if(x1!=NULL)y->prox=x1;
	x=x1;
	y=y1
}
return l;
}
//busca insere cab
lista* busca_inserecab(lista *l,int x,int y){
	lista *novo,*p,*ant;
	novo=(lista*)malloc(sizeof(lista));
	novo->info=x;
	if(l->prox==NULL){
		novo-> prox=NULL;
		l->prox= novo;
	}
	else{
		ant=l;
		p=l->prox;
		while(p->info!=y)
		{
			ant=p;
			p=p->prox;
		}
		novo->prox=p;
		ant->prox=novo;
	}
	return l;
} 
// busca remove cab
lista* busca_remove cab(lista *l,int x){
	lsita *ant, *p;
	if(l->prox==NULL)return l;
	ant=l;
	p=l->prox;
	while(p!=NULL && p->info!=x)
	{
		ant=p;
		p=p->prox;
	}
	if(p!=NULL){
		ant->prox = p->prox;
		free(p);
	}
	return l;
}
//busca insere
lista* busca_insere(lista *l,int x, int y){
	lista *novo,*ant,*p;
	novo=(lista*)malloc(sizeof(lista));
	novo->info=x;
	if(l==NULL){
		novo->prox=NULL;
		return (novo);
	}
	else{
		ant=NULL;
		p=l;
		while(p!=NULL && p->info!=y);{
			ant=p;
			p=p->prox;
		}
		if(ant==NULL){
			novo->prox=p;
			return (novo);
		}
		else{
			novo->prox=p;
			ant->prox=novo;
			return l;
		}
	}
}
//Função Busca e Remove
lista *busca_remove(lista *l, int x){
    lista *ant, *p,*novo;
    if(l==NULL) return l;   //verifica se a lista esta vazia
    ant=NULL;
    p=l;
    while(p!=NULL && p->info<valor)
    {
        ant=p;
        p=p->prox;
    }
    //A partir daqui o X foi encontrado na lista.
    if(ant==NULL)
    {
        ant=p;
        p=p->prox;
    }
    else
    {
        novo->prox=l;
        return novo;
    }
}    
// imprime recursiva
void imprime_recursiva(lista*l){
    if (l==NULL)return;
    printf("%d\n",l->info);
	imprime_recursiva(l-prox);
}
// apagar lista 
lista* apagar_lista(lista *l){
	lista *p;
	
	if(l==NULL)return NULL;
	p=l;
	
	l=l->prox;
	free(p);
	apagar_lista(l);
}
// insere inicio
lista* insere_inicio(lista*l,int val){
	lista* novo;
	novo =(lista*) malloc(sizeof(lista));
	novo-> info =val;
	novo -> prox = l;
	return novo;// endereço do inicio da lista
}
//insere final
ista *InsereFim(lista *L, int x){
	lista *novo, *aux;
	
	novo=(lista*)malloc(sizeof(lista));
	
	if(novo != NULL){
		novo->info=x;
		
		if(L==NULL){
			L=novo;// l->prox=novo;
			novo->prox=NULL;
			L= novo;
		}
		else{
			aux=L;
			while(aux->prox != NULL)
				aux=aux->prox;
				
			novo->prox=aux->prox;
			aux->prox=novo;
		}
	}
return L;
}
}
// busca lista
lista* Busca(lista *l,int val){
	lista *aux;
	aux=l;
	while(aux!=NULL){
		if(aux->info==val)return aux;
		aux= aux->prox;
	}
	return NULL;
}
// intercala 
lista* intercala(lsita *x, lista *y){
	lista *l,*x1,*y1;
	if(x==NULL)return y;
	if(y==NULL)return x;
	l=x;
while(x!=NULL && y!=NULL){
	x1=x->prox;
	y1=y->prox;
	x-> prox=y;
	if(x1!=NULL)y->prox=x1;
	x=x1;
	y=y1
}
return l;
}
//busca insere cab
lista* busca_inserecab(lista *l,int x,int y){
	lista *novo,*p,*ant;
	novo=(lista*)malloc(sizeof(lista));
	novo->info=x;
	if(l->prox==NULL){
		novo-> prox=NULL;
		l->prox= novo;
	}
	else{
		ant=l;
		p=l->prox;
		while(p->info!=y)
		{
			ant=p;
			p=p->prox;
		}
		novo->prox=p;
		ant->prox=novo;
	}
	return l;
} 
// busca remove cab
lista* busca_remove cab(lista *l,int x){
	lsita *ant, *p;
	if(l->prox==NULL)return l;
	ant=l;
	p=l->prox;
	while(p!=NULL && p->info!=x)
	{
		ant=p;
		p=p->prox;
	}
	if(p!=NULL){
		ant->prox = p->prox;
		free(p);
	}
	return l;
}
//busca insere
lista* busca_insere(lista *l,int x, int y){
	lista *novo,*ant,*p;
	novo=(lista*)malloc(sizeof(lista));
	novo->info=x;
	if(l==NULL){
		novo->prox=NULL;
		return (novo);
	}
	else{
		ant=NULL;
		p=l;
		while(p!=NULL && p->info!=y);{
			ant=p;
			p=p->prox;
		}
		if(ant==NULL){
			novo->prox=p;
			return (novo);
		}
		else{
			novo->prox=p;
			ant->prox=novo;
			return l;
		}
	}
}
//conta elementos
int Conta_elemento(lista *L){
	int cont=0;
	
	while(L != NULL){
		cont++;
		L=L->prox;
	}
	return cont;
	
}
//conta elementos recursiva
int Conta_recurs(lista *L){
	if(L==NULL)	return 0;
	return 1+Conta_recurs(L->prox);
}
