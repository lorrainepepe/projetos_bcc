/*
  ÁRVORE DE BUSCA BINÁRIA ( DIFERENTE DE ARVORE BINÁRIA)
*/

#include<stdio.h>
#include<stdlib.h>

typedef struct arv{
	int info;
	struct arv* esq;
	struct arv* dir;
}Arv;

//Cria árvores vazias

Arv* CriaArvVazia(){
	return NULL;
}

int VerificaArvVazia(Arv* arv){
	return arv==NULL; // Equivalente a: if(arv==NULL) return 1 else return 0;
}

Arv* CriaNovo(int v){ // sae: sub-árvore esquerda e sad é a direita
	Arv* novo = (Arv*) malloc(sizeof(Arv));
	novo->info = v;
	novo->esq = NULL;
	novo->dir = NULL;

	return novo;
}

/*Impressões: 3 tipos, I) Pré-ordem, II) Pós-ordem, III) Ordem simétrica.
 * I) Imprime a árvore de cima para baixo
 * II) Imprime a árvore de baixo para cima.
 * III) Imprime o lado esquerdo, a raiz e após a lado direito;
*/

//  (I) Pré

void ImprimeArvPre(Arv* arv){

	if(arv){
		printf("%d \n ", arv->info); //mostra a raiz (R)
		ImprimeArvPre(arv->esq); //mostra esq     (E)
		ImprimeArvPre(arv->dir); //mostra dir      (D)
	}
}

// (II) Pós

void ImprimeArvPos(Arv* arv){
	if(arv){
		ImprimeArvPos(arv->esq); //mostra esq     (E)
		ImprimeArvPos(arv->dir); //mostra dir     (D)
		printf("%d\n  ", arv->info); //mostra raiz; (R)
	}
}

// (III) Ordem Simétrica

void ImprimeArvSimetrica(Arv* arv){ 

	if(arv != NULL){
		ImprimeArvSimetrica(arv->esq); //mostra esq     (E)
		printf("%d\n ", arv->info); //mostra raiz  (R)
		ImprimeArvSimetrica(arv->dir); //mostra dir     (D)
	}
}

Arv* Insere_abb (Arv* a, Arv* novo){
	
	if(a==NULL)
		return novo;
	else{
		if(novo->info < a->info)
			a->esq = Insere_abb(a->esq, novo);
		else
			a->dir = Insere_abb(a->dir, novo);
	}
}

Arv* busca_abb (Arv* r, int v){
	if( r == NULL ) return NULL;
	else if(r->info > v ) return busca_abb(r->esq, v);
	else if(r->info < v) return busca_abb(r->dir, v);
	else return r;
}

Arv* remove_abb ( Arv * r, int v){
	//Remove o elemento v da árvore
	
	if (r == NULL)
		return NULL;
		
	else if (r->info > v) r->esq = remove_abb (r->esq, v);
	else if (r->info < v) r->dir = remove_abb (r->dir, v);
	else {
		if(r->esq == NULL && r-> dir == NULL){
			free(r);
			r = NULL;
		}
		else if(r->esq == NULL){
			Arv* t = r;
			r = r->dir;
			free(t);
		}
		else if(r->dir == NULL){
			Arv* t = r;
			r = r->esq;
			free(r);
		}
		else{
			Arv* f = r->esq;
			while (f->dir != NULL) f = f->dir;
			r->info = f->info;
			f->info = v;
			r->esq = remove_abb(r->esq, v);
		}
	}
	return r;
}

//Libera memória da Árvore
Arv* DesalocaArv(Arv* a){
	if(!VerificaArvVazia(a)){
		DesalocaArv(a->esq);      
		DesalocaArv(a->dir);      
		free(a);
	}
	return NULL;
}
