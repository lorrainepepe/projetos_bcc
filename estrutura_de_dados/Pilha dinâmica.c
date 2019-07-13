/*
   Nome: Lorraine Maria Pepe
   Laboratório Estrutura de Dados I
   Compilador: Dev-C++ (versão): 5.11
   Data: 05/04/2017
   Enviar lista para: ines@ibilce.unesp.br

   Comentário: Ex 2: Idem Ex 1, mas agora uma pilha dinâmica. Além das funções descritas no ex 1, faça também uma função que ao 
   terminar a execução do programa, destrua a pilha inteira.
   
   
   // Falta retornar a qtidade de elementos, verificar se a desaloca está certa.
*/

#include<locale.h>
#include<stdio.h>
#include<stdlib.h>
#include <stdbool.h>
#define MAX 100 //PARA DECLARAR CONSTANTE GLOBAL

typedef struct Pilha{
	int num;
	struct Pilha *prox;
}pilha;

pilha *topo = NULL;
pilha *aux;
int qnt_elem=0;

bool Pilha_Cheia(){
	if(topo == MAX-1) 
		return true;
	else	return false;
}
	
bool pilha_Vazia(){
	if(topo==-1)
		return true;
	else
		return false;
}

int push(){ // Insere
	pilha *novo = (pilha *) malloc (sizeof(pilha));
	printf("Insira o valor: ");
	scanf("%d", &novo->num);
	novo->prox = topo;
	topo = novo;
	qnt_elem++;
	return qnt_elem;
}

int pop(){ // Remove
	if(topo==NULL)	
		printf("Pilha Vazia");
	else{
		aux=topo;
		topo=topo->prox;
		free(aux);
	}
	qnt_elem--;
	return qnt_elem;
}

void consulta_topo(){ // consulta valor do topo
	
	if(topo==NULL)
		printf("Pilha Vazia\n");
	
	else{
		aux = topo;
		while(aux != NULL){
			printf("%d", aux->num);
			aux = aux->prox;
		}
	}
}

void desaloca_pilha(){
	aux=topo;
	while(aux != NULL){
		free(topo);
		topo=aux;
		aux=aux->prox;
	}
	printf("Pilha desfeita! Topo = %d", topo);
}
	
void main(){
	setlocale(LC_ALL, "Portuguese");
	int op=1, valor;
	
	printf("Menu:\n\t1) Insere\n\t2)Remove \n\t3)Consulta \n\n\t\tPara desfazer a pilha tecle 0\n\n");
	
	while(op != 0){
		
		printf("\nInsira uma opção válida:  ");
		scanf("%d", &op);
		
		switch (op){
			case 1: 
				push(valor);
			break;
			case 2:
				printf("\tRemove elemento do topo:\n");
				pop();
			break;
			case 3:
				consulta_topo();
			break;
			case 0:
				printf("Desfaz pilha!\n");
				
				
			default: printf("\nVocê não escolheu uma opção válida!");
		}
	}
}




