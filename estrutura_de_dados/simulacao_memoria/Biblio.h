
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <locale.h>

typedef struct memoria {
	unsigned int id;
	int pos_ini;
	unsigned int tamanho;
	int status; // 1- espa�o cheio e 0- espa�o vazio
	struct memoria* prox;
	struct memoria* ant;
} MEMORIA;

typedef struct pcb {
	unsigned int id;
	int pc;
	int quantum;
	unsigned int tempo_cpu;
	int isDisp;
} PCB;

typedef struct processo {
	MEMORIA* mem;
	PCB* pcb;
	struct processo* prox;
} PROCESSO;

typedef struct fila_processos {
	PROCESSO* inicio;
	PROCESSO* fim;
} FILA_PROCESSOS;


//FILA DE PROCESSOS

// Fila de Processos
int VerificaFilaVazia(FILA_PROCESSOS* fila){                                 
	if(fila->inicio == NULL) return 1;
	else return 0;
}

FILA_PROCESSOS* CriaFila() {
	FILA_PROCESSOS* fila = (FILA_PROCESSOS*)malloc(sizeof(FILA_PROCESSOS));
	fila->inicio = NULL;
	fila->fim = NULL;
	return fila;
}

FILA_PROCESSOS* InsereFilaProcessos(FILA_PROCESSOS* fila, PROCESSO* novo){
	novo->prox = NULL;
	if(VerificaFilaVazia(fila)){
		fila->inicio = novo;
		fila->fim = novo;
	}
	else{//Se j� possui elemento insere "no fim"
		fila->fim->prox = novo;
		fila->fim = novo;
	}
	return fila;
}

PROCESSO* RemoveFila(FILA_PROCESSOS* fila){
	PROCESSO* aux = fila->inicio;
	if(aux == NULL){
		fila->inicio = NULL;
		fila->fim = NULL;
		return NULL;
	}
	else{
		fila->inicio = aux->prox;
		aux -> prox = NULL;
		return aux;
	}
}

//IMPRESS�O PROCESSO E PCB INDIVIDUALMENTE (MODO DE EXIBI��O)
void ImprimePCB(PCB* aux, int i){
	printf("*PCB* | ID:  %d | PC: %d | QUANTUM: %d | TEMPO CPU: %d |", aux->id, aux->pc, aux->quantum, aux->tempo_cpu);
	if(i){ // s� imprime o bloqueado se for na fila de pronto
		if(aux->isDisp <= 3)
	        printf("BLOQ.: SIM |\n");
	    else printf("BLOQ.: N�O |\n");
	}
}

void ImprimeProcesso(MEMORIA* aux){
	printf("NOME: P_%d | ID: %d | TAM.: %d w | LOCAL INI: %d |", aux->id, aux->id, aux->tamanho, aux->pos_ini);
	if(aux->status)
		printf(" CONTEUDO: OK | ", aux->status);
	else printf(" CONTEUDO: VAZIO | ");
}

// IMPRESS�O FILA DE PROCESSOS
void ImprimeFilaProcessos(FILA_PROCESSOS *fila, int i){
	if(!fila->inicio) printf("\tFILA VAZIA!\n\n");
	else{
		PROCESSO* aux = fila->inicio;
		
		printf("\n NOME \t\t|");
		while(aux){
			printf(" P_%d\t|",  aux->mem->id);
			aux = aux->prox;
		}
		
		aux = fila->inicio;
		printf("\n TAMANHO\t|");
		while(aux){
			printf(" %d w\t|",  aux->mem->tamanho);
			aux = aux->prox;
		}
		aux = fila->inicio;
		printf("\n LOCAL INI\t|"); // POSI��O INICIAL NA MEM�RIA
		while(aux){
			printf(" %d \t|",  aux->mem->pos_ini);
			aux = aux->prox;
		}
		aux = fila->inicio;
		printf("\n STATUS\t\t|");
		while(aux){
			if(aux->mem->status)
				printf(" OK\t|",  aux->mem->status);
			else printf(" VAZIO\t|");
			aux = aux->prox;
		}
		aux = fila->inicio;
		printf("\n ID\t\t|");
		while(aux){
			printf(" %d \t|",  aux->pcb->id);
			aux = aux->prox;
		}
		aux = fila->inicio;
		printf("\n PC\t\t|");
		while(aux){
			printf(" %d\t|",  aux->pcb->pc);
			aux = aux->prox;
		}
		aux = fila->inicio;
		printf("\n QUANTUM\t|");
		while(aux){
			printf(" %d\t|",  aux->pcb->quantum);
			aux = aux->prox;
		}
		aux = fila->inicio;
		printf("\n TEMPO CPU\t|"); 
		while(aux){
			printf(" %d\t|",  aux->pcb->tempo_cpu);
			aux = aux->prox;
		}
		if(i){ // Se 1 exibe, se 0 n�o
			aux = fila->inicio;
			printf("\n BLOQ.\t\t|");
			while(aux){
		      	if(aux->pcb->isDisp <= 3)
		      		printf(" SIM\t|");
		       	else printf(" N�O\t|");
		    	aux = aux->prox;
		   	}
		}
	}
}

//IMPRESS�O DA MEM�RIA (LISTA DUPLAMENTE ENCADEADA N�O CIRCULAR)
//-------------------
void ImprimeMemoria(MEMORIA* mem){
	MEMORIA* aux = mem;
	printf(" NOME \t\t|");
	while(aux){
		if(aux->id == (-1))
			printf(" VAZIO\t|",  aux->id);
		else 	printf(" P_%d\t|",  aux->id);
		aux = aux->prox;
	}
	aux = mem;
	printf("\n LOCAL INI \t|");
	while(aux){
		printf(" %d\t|",  aux->pos_ini);
		aux = aux->prox;
	}
	aux = mem;
	printf("\n TAMANHO\t|");
	while(aux){
		printf(" %d w\t|",  aux->tamanho);
		aux = aux->prox;
	}
	aux = mem;
	printf("\n STATUS \t|");
	while(aux){
		if(aux->status)
			printf(" OK\t|", aux->status);
		else printf(" VAZIO\t|");
		aux = aux->prox;
	}
}
