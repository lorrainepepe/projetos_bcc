// Biblioteca de funções para Árvore AVL

#include<stdio.h>
#include<stdlib.h>

typedef struct nodeTree{
	int item;
	struct nodeTree *esq;
	struct nodeTree *dir;
}ARV;

typedef struct nodeAVL{
	int item;
	int fb;
	struct nodeAVL *esq;
	struct nodeAVL *dir;
}AVL;

// Altura-------------------------------------
int max(int a, int b){
	if (a > b)
		return a;
	else return b;
	
}

int alturaAVL(ARV* raiz){
	if(raiz == NULL)
		return -1; 
	else return 1 + max (alturaAVL(raiz->esq), alturaAVL(raiz->dir));
}
// Rotação LL (LEFT- LEFT)----------------------
AVL *rotLL(AVL* desb){
	AVL* aux = desb ->esq;
	desb->esq = aux->dir;
	aux->dir = desb;
	desb = aux;
	return desb;
}
//Rotação RR (RIGHT-RIGHT)-----------------------
AVL *rotRR(AVL* desb){
	AVL* aux = desb->dir;
	desb->dir = aux->esq;
	aux->esq = desb;
	desb = aux;
	return desb;
}
//Rotação LR ------------------------------------
AVL* rotLR(AVL* desb){
	desb->esq = rotRR(desb->esq);
	desb = rotLL(desb);
	return desb;
}
//Rotação RL -------------------------------------
AVL* rotRL(AVL* desb){
	desb->dir = rotLL(desb->dir);
	desb = rotRR(desb);
	return desb;
}
//Balanceamento a esquerda -------------------------
AVL* balancEsq(AVL *raiz, int *cresceu){
	
	AVL* Sae = raiz->esq;
	AVL* sSad = Sae->dir; // Subarvore direita da sub esquerda
	
	switch(Sae->fb){
		case -1: 
			Sae->fb = 0;
			raiz->fb = 0;
			raiz = rotLL(raiz);
			*cresceu = 0;
		break;
		case 1:
		 // Subarvore direita da sub esquerda
			switch(sSad->fb){
				case -1:
					raiz->fb = 1;
					Sae->fb = 0;
				 	sSad->fb = 0;
				break;
				case 1:
					Sae->fb = -1;
					raiz->fb = 0;
					sSad->fb = 0;
				break;
				case 0: Sae->fb = sSad->fb = raiz->fb = 0;				
			}
			raiz = rotLR(raiz);
			*cresceu = 0;
	}
	return raiz;
}
//Balanceamento à direita--------------------
AVL* balancDir(AVL* raiz, int *cresceu){
	AVL* Sad = raiz->dir;
	AVL* sSae = Sad->esq; // Subarvore direita da sub esquerda
	switch(Sad->fb){
		case 1: 
			Sad->fb = 0;
			raiz->fb = 0;
			raiz = rotRR(raiz);
			*cresceu = 0;
		break;
		case - 1:
			switch(sSae->fb){
				case 1:
					raiz->fb = -1;
					sSae->fb =0;
					Sad->fb = 0;
				break;
				case -1:
					Sad->fb = 1;
					raiz->fb = sSae->fb = 0;
				break;
				case 0: Sad->fb = sSae->fb = raiz->fb = 0;				
			}
			raiz = rotRL(raiz);
			*cresceu = 0;
	}
	return raiz;
		
}

//INSERE------------------------------------------

AVL* Insere(AVL* raiz, int elem, int* cresceu){
	
	if(raiz == NULL){
		
		AVL* novo = (AVL*)malloc(sizeof(AVL));
		novo->item = elem;
		novo->dir = NULL;
		novo->esq = novo->dir;
		novo->fb = 0;
		raiz = novo;
		*cresceu = 1;
	}
	else{
		if(elem < raiz ->item){
			raiz->esq = Insere(raiz->esq, elem, cresceu);
			if(*cresceu){
				switch(raiz->fb){
					case 0: raiz->fb = -1;
					break;
					case 1: raiz->fb = 0;
					*cresceu = 0;
					break;
					case -1: raiz = balancEsq(raiz,cresceu);
				}
			}
		}
		else if (elem > raiz->item){
			raiz->dir = Insere(raiz->dir, elem, cresceu);
			if(*cresceu){
				switch(raiz->fb){
					case 0: raiz->fb = 1;
					break;
					case -1: raiz->fb = 0;
					*cresceu = 0;
					break;
					case 1: raiz = balancDir(raiz,cresceu);
				}
			}
		}
		//else printf("Elemento repetido nao pode ser inserido!\n");
	}
	return raiz;
}

AVL* InsereAVL(AVL* raiz, int elem){
	int cresceu;
	raiz = Insere(raiz, elem,& cresceu);
	return raiz;
}

//Destroi-----------------------------------------------------------------------------------------
AVL *DestruirAVL(AVL *aux){
	
	if(aux != NULL){
		DestruirAVL(aux->dir);
		DestruirAVL(aux->esq);
		free(aux);
	}
	return NULL;
}

//Consulta Um No Especifico --------------------------------------------------

void BuscaAVL(AVL *aux, int x){
	if(aux!=NULL){
		if(aux->item==x) printf("\n  Numero Encontrado: %d\n", aux->item);
		else{
			if(aux->item < x) BuscaAVL(aux->dir, x);
			if(aux->item > x) BuscaAVL(aux->esq, x);
		}
	}
	else printf("Numero nao encontrado.\n");
}

//Remove ------------------------------

AVL *RemoveAVL(AVL *aux, int x){
	AVL *p, *q;
	
	if(aux!=NULL){
		if(aux->item==x){
			if(aux->esq==aux->dir){
				free(aux);
				return NULL;
			}
			else if(aux->esq==NULL){
				p=aux->dir;
				free(aux);
				return p;
			}
			else if(aux->dir==NULL){
				p=aux->esq;
				free(aux);
				return p;
			}
			else{
				q=aux->dir;
				p=aux->dir;
				while(p->esq!=NULL) p=p->esq;
				p->esq=aux->esq;
				free(aux);
				return q;
			}
		}
		else if(aux->item<x) aux->dir=RemoveAVL(aux->dir, x);
		else aux->esq=RemoveAVL(aux->esq, x);
	}
	return aux;
}

// Imprimir
/*Impressões: 3 tipos, I) Pré-ordem, II) Pós-ordem, III) Ordem simétrica.
 * I) Imprime a árvore de cima para baixo
 * II) Imprime a árvore de baixo para cima.
 * III) Imprime o lado esquerdo, a raiz e após a lado direito;
*/

//  (I) Pré

void ImprimeAvlPre(AVL* arv){

	if(arv != NULL){
		printf("%d\n", arv->item); //mostra a raiz (R)
		ImprimeAvlPre(arv->esq); //mostra sae      (E)
		ImprimeAvlPre(arv->dir); //mostra dir      (D)
	}
}

// (II) Pós

void ImprimeAvlPos(AVL* arv){

	if(arv != NULL){
	
		ImprimeAvlPos(arv->esq); //mostra sae     (E)
		ImprimeAvlPos(arv->dir); //mostra dir     (D)
		printf("%d\n", arv->item); //mostra raiz; (R)
	}
}

// (III) Ordem Simétrica

void ImprimeAvlSimetrica(AVL* arv){ 

	if(arv != NULL){

		ImprimeAvlSimetrica(arv->esq); //mostra sae     (E)
		printf("%d\n", arv->item); //mostra raiz  (R)
		ImprimeAvlSimetrica(arv->dir); //mostra dir     (D)
	}
	
}

