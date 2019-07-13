#include<stdio.h>
#include<stdlib.h>

typedef struct lista{ //lista simples e circular
	int info;
	struct lista *prox;
}No;

typedef struct listaDupla {
    struct listaDupla *ant;
    int info;
    struct listaDupla *prox;
}NoDuplo;

typedef struct pilha{
	No* topo; // NOTE QUE TOPO � DO TIPO LISTA
}PILHA;

typedef struct fila{
  No *ini;
  No *fim;
}Fila;

//Lista simples------------------------------------------------------

No* InsereInicio (No* L, int valor){
 	No *novo; 							// 1) Cria o novo
	novo = (No*) malloc(sizeof(No)); 	// 2) Aloca mem�ria p/ novo
	novo->info = valor; 				// 3) Adiciona o valor ao novo n�
 	novo->prox = L; 					// 4) Encadeia o novo � lista 
 	return novo;						// 5) Novo � o IN�CIO agora.
 }

int Compara(No *L, No *M){
	No *aux1, *aux2;
	aux1 = L;
  	aux2 = M;
  	while (aux1  && aux2){
    	if(aux1->info == aux2->info){
      		aux1 = aux1->prox;
      		aux2 = aux2->prox;
    	}
   		 else return 0;
  	}
	if(!aux1 && aux2 == NULL) return 1;
	
	return 0;
}

void Imprime (No *L){
	No *aux = L;
	while (aux != NULL){
		printf("|%d| -> ", aux->info);
		aux = aux->prox;
	}
	printf("NULL\n");
}
//Desaloca:
void DesfazLista(No*L){
	No* aux = L;
	while (aux != NULL) {
		No* aux2 = aux->prox; /* guarda refer�ncia para o pr�ximo elemento*/
		free(aux); /* libera a mem�ria apontada por aux */
		aux = aux2; /* faz aux apontar para o pr�ximo */
	}
}

No* Concatena(No*L, No* M){
	No* aux = L;
	while(aux->prox)
		aux = aux->prox;
	aux->prox = M;
	
	return L;
}

//**************************************************************************************************

//Lista Circular----------------------------------------------------
No *InsereCirc(No *LC, int v){ //Inser��o no fim, mas aponta o fim para o come�o
	
	No *novo, *aux;
	novo = (No*)malloc(sizeof(No));
	aux = LC;
	
  	if(LC == NULL){ // 1� elemento
    	novo->info = v;
    	LC = novo;
    	novo->prox = LC;
    	return LC;
  	}
  	//Se j� houver 1 elemento inserido
 	while(aux->prox != LC)
    	aux = aux->prox;
    	
	novo->info = v;
	aux->prox = novo;
  	novo->prox = LC; //Torna a lista circular
  	return LC;
}

No* RemoveCirc(No* LC){ //Remove no "in�cio", considerando que "L->prox" seja o in�cio (2� da fila)

    if(!LC){ // Se lista == NULL
        printf("\nLista Vazia!\n");
        return NULL;
    }
	No* aux = LC->prox;
    if(aux->prox == aux){ //Caso de s� haver um elemento
        LC = NULL;
        free(aux);
        printf("\nRemovido!\n");
        return LC;
    }
	//Para caso de haver mais de 1 elemento
    LC->prox = aux->prox;
    free(aux);
    return LC;
}

void ImprimeCirc(No* LC	){ 
  No *aux;
  aux = LC;
  if(aux == NULL){
  	printf("\nLista Vazia\n");
  	return;
  }
  printf("\nInicio|%d|-> ", (aux->prox)->info);
  while(aux->prox != LC){
    aux = aux->prox;
    printf("|%d|-> ", (aux->prox)->info);
  }
  printf("Inicio\n");
}

No* DesfazListaCirc(No* LC){
	No*aux=LC->prox;
	No*ant;
	
	while(aux->prox != LC){ // Enquanto aux n�o fizer todo o "circulo" e retornar a LC
		ant = aux;
		aux = aux->prox;
		free(ant);
	}
	LC = NULL;
	return LC;
}

//**************************************************************************************************

//Lista Duplamente Encadeada----------------------------------------------------
NoDuplo* InsereDuplo(NoDuplo* L, int valor){
	//Cria
   	NoDuplo* novo;
	novo = (NoDuplo*)malloc(sizeof(NoDuplo));
	novo->info = valor;
	
	//Para lista vazia
	if(L == NULL){
		novo->ant = NULL;
		novo->prox = NULL;
		L = novo;
		return L;
	}
	//Inicializa��o e Inser��o
	L->ant = novo;
	novo->ant = NULL;
	novo->prox = L;
	L = novo;
	
	return L;
}

NoDuplo* Busca(NoDuplo* L, int elem){
	NoDuplo* aux;
	for (aux=L; aux != NULL; aux = aux->prox)
		if (aux->info == elem)
			return aux;
	//N�o achou o elemento
	return NULL; 
}

NoDuplo* RemoveDuplo(NoDuplo* L, int elem){
	NoDuplo* aux = L;
	NoDuplo* aux2=NULL;
		
	if(!L){
		printf("Lista vazia!\n");
		return L;
	}
	
	//Percorre at� terminar ou encontrar
	while(aux->prox != NULL && aux->info != elem)
		aux = aux->prox;
	
	//Se n�o for remover
	if(aux->info != elem) 
		return L;
	
	//Se for o 1� elemento
	if(aux->ant == NULL){
		if(aux->prox != NULL){ // Se houver mais de 1 na lista
		aux = aux->prox;
		aux->ant = NULL;
		return aux;
		}
		//Caso tenha apena um na lista
		L = NULL;
		return L;
	}
	//Se for o �ltimo elemento
	if(aux->ant != NULL && aux->prox == NULL){
		aux = aux->ant;
		aux->prox = NULL;
		return L;
	}
	//Se n�o for nem �ltimo e nem primeiro:
	aux2 = aux->ant;
	aux2->prox = aux->prox;
	aux = aux->prox;
	aux->ant = aux2;
	return L;

}

void ImprimeDuplo(NoDuplo* L){
    while(L){
    	printf("|%d|-><- ", L->info);
    	L = L -> prox;
    }
    printf("NULL\n");
}

NoDuplo* DesfazListaDupla(NoDuplo* L){
	NoDuplo* aux = L;
	while (aux != NULL) {
		L= aux->prox ; /* guarda refer�ncia para o pr�ximo elemento*/
		free(aux); /* libera a mem�ria apontada por aux */
		aux= L; /* faz aux apontar para o pr�ximo */
	}
	L = NULL;
	return L;
}

//**************************************************************************************************

// Pilha-----------------------------------------------------------------------------------
PILHA *CriaPilha(){ // Aloca a pilha e aponto o topo para nulo
  PILHA *pilha = (PILHA*)malloc(sizeof(PILHA));
  pilha->topo = NULL;
  return pilha;
}
//Vazia
int VerificaPilhaVazia (PILHA *pilha){
	if(pilha->topo ==NULL)
		return 1;
	else return 0;
}
//Insere elemento na Pilha:
PILHA *Push(PILHA *pilha, int valor){ // Adiciona um novo elemento na pilha, sempre "em cima"
  No *novo = (No*)malloc(sizeof(No));
  novo->info = valor;
  novo->prox = pilha->topo;
  pilha->topo = novo;
  return pilha;
}
//Remove elemento:
int Pop (PILHA* pilha){
  No *aux;
  int valor;
  if(VerificaPilhaVazia(pilha))
    printf("\nPilha j� vazia.\n");
  else{
    aux = pilha->topo;
    valor = aux->info;
    pilha->topo = aux->prox;
    free(aux);
    aux = NULL;
    return valor;
  }
}
//RETORNA TOPO:
int Top (PILHA *pilha){
	return (pilha -> topo)->info;
}
//Destroi pilha
void DestruirPilha(PILHA *pilha){
	while(!VerificaPilhaVazia(pilha)){
		Pop(pilha);
	}
	free(pilha);
	pilha = NULL;
}
//Remove 1 Elemento
void removeElem(PILHA *pilha, int elem){
	PILHA *aux;
	aux = CriaPilha(); 
	// cria uma pilha auxiliar para empilhar os valores do topos 
	//removidos da "pilha" e poder volta-los para ela posteriormente
	while (!VerificaPilhaVazia(pilha)){
		if(Top(pilha) != elem)
			Push(aux, Pop(pilha));
		else{
			Pop(pilha);
			break;
 // tem que parar o while sen�o ele vai at� esvaziar a pilha. 
 //Sem o break, caso tivesse elemento repetido ele tbm removeria.
		}
	}
	while(!VerificaPilhaVazia(aux)){
		Push(pilha, Pop(aux));
	}
}
//IMPRIME PILHA
void ImprimePilha(PILHA *pilha){ //Imprime do �ltimo inserido para o primeiro
  No *aux;
  aux = pilha->topo;
  while(aux != NULL){
    printf("\t|%d|\n", aux->info);
    aux = aux->prox;
  }
  printf("\tFIM\n");
}

//**************************************************************************************************

//FILA--------------------------------------------------------
Fila *CriaFila(){
	Fila *fila = (Fila*)malloc(sizeof(Fila));
	fila->ini = NULL;
	fila->fim = NULL;
  	return fila;
}

int VerificaFilaVazia(Fila *fila){
  if(fila->ini == NULL)
    return 1;
  else
    return 0;
}

Fila *InsereFila(Fila*fila, int valor){ //Insere no fim da fila
	
	No *novo = (No*)malloc(sizeof(No));
	novo->info = valor;
	novo->prox = NULL;
  
	if(VerificaFilaVazia(fila)){
    	fila->ini = novo;
    	fila->fim = novo;
    	return fila;
	}
	//else
	(fila->fim)->prox = novo;
	fila->fim = novo;
    return fila;
}

int AtendeFila(Fila *fila){ //Remove do in�cio
	
  No *aux = fila->ini;
  int valor;
  valor = aux->info;
  fila->ini = aux->prox;
  free(aux);

  if(fila->ini == NULL)
    fila->fim == NULL;
  return valor;
}

int InicioFila(Fila *fila){ //cabe�a
  return (fila->ini)->info;
}

void ImprimeFila(Fila *fila){
  No* aux;
  aux = fila->ini;
  printf("\nInicio - ");
  while(aux != NULL){
    printf("%d - ", aux->info);
    aux = aux->prox;
  }
  printf(" Fim\n");
}
//Destruir fila:
void DestruirFila(Fila *fila){
	while(!VerificaFilaVazia(fila)){
		AtendeFila(fila);
	}
	free(fila);
	
	fila->fim = NULL;
	fila->ini = NULL;
}



