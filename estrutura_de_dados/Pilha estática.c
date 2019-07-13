/*
   Nome: Lorraine Maria Pepe
   Laborat�rio de Estruturas de Dados I
   Compilador: Dev-C++ (vers�o): 5.11
   Data: 05/04/2017
   Enviar lista para: ines@ibilce.unesp.br

   Coment�rio: 1) Implemente um programa em C que contenha as seguintes funcionalidades: Inserir um elemento em uma pilha est�tica,
   remove um elemento de uma pilha  est�tica e consulta o topo.
   
*/

#include<locale.h>
#include<stdio.h>
#include<stdlib.h>

#include <stdbool.h> //PARA USAR BOOL
#define MAX 100 //PARA DECLARAR CONSTANTE GLOBAL


int pilha[MAX];
int topo = -1;

bool Pilha_Cheia(){
	if(topo==MAX-1) 
		return true;
	else	return false;
}
	
bool Pilha_Vazia(){
	if(topo==-1)
		return true;
	else
		return false;
}

bool Push (int valor){ // Fun��o Insere
	if(Pilha_Cheia()) 
		return false;
	else {
	topo++;
	pilha[topo]=valor;
	return true;
	}
}

bool Pop(){ //Fun��o remove
	if (Pilha_Vazia())
		return false;
	else{
		topo--;
		return true;
	}
}

void consulta_topo(){
	if(topo != -1)
		printf("Pilha[%d] = %d\n",topo, pilha[topo]);
	else
		printf("A pilha est� vazia \n");
}


void main(){
	setlocale(LC_ALL, "Portuguese");
	int op=1, valor;
	
	printf("\nMenu:\n\t1) Insere\n\t2)Remove \n\t3)Consulta \n\n");
	
	while(op != 0){
		
		printf("\nInsira uma op��o v�lida:  ");
		scanf("%d", &op);
		
		switch (op){
			case 1: 
				printf("\tInsira um valor a ser inserido na pilha: ");
				scanf("%d", &valor);
				Push(valor);
			break;
			case 2:
				printf("\tRemove elemento do topo:\n");
				Pop();
			break;
			case 3:
				consulta_topo();
			break;
					
			default: printf("\nVoc� n�o escolheu uma op��o v�lida!");
		}
	}
}
		
				
				
		
		
		
	
