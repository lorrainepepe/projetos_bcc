
/* MEDIÇÃO DE TEMPO EM INSERÇÃO E BUSCA DE ÁRVORE AVL, LISTA (INSERE ORDENADA) E ÁRVORE DE BUSCA BINÁRIA
 * Lorraine M. Pepe - Lab. de Ed.
 * Dev-C++ 5.11
 */
 
#include "ArvoreAVL.h"
#include "Lista.h"
#include "ArvBuscaBinaria.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int randomInteger (int low, int high){
    int k;
    double d;
    d = (double) rand () / ((double) RAND_MAX + 1);
    k = d * (high - low + 1);
    return low + k;
}

double calcMedia(double* med){
	int i;
	med[10] = 0;
	for(i=0; i < 10; i++){
		med[10]+= med[i];
	}
	return med[10]/10;
}

void main(){
	
	srand(time(NULL));
	FILE *arq; // para operações com arquivo
	
/*Cria uma lista com cem mil elementos aleatorios não repetidos 
para deixar a medição de tempo justa, visto que dependendo do elemento a árvore AVL aplica mais ou menos funções 
devido aos balanceamentos. E adiciona esses elementos em um arquivo.*/
	No* RandomList = NULL;
	int cont  = 0;
	int elem;
	arq = fopen("1milhãoElemNaoRepetidos","w"); 
	do{
		elem = randomInteger(0, 1000000); // pega elementos no intervalo fechado [0, 1000000] 
		
		if(!BuscaLista(RandomList, elem)){ // Se o num não estiver na lista, adiciona-o.
			InsereLista(RandomList, elem);
			cont++;
			fprintf(arq,"%d\n", elem);
		}
		
	}while(cont != 1000000);
	//Verifica se o arquivo está com os 100.000 elementos.
	printf("%d", cont);
	ImprimeLista(RandomList);
	LiberaLista(RandomList); // Libera a lista
	fclose(arq);
	arq = fopen("1milhãoElemNaoRepetidos","r"); 
 
 // Com o arquivo gerado: -----------------------------------------------------
 	FILE* media;
 	media = fopen("MediaDasMedicoes","w");
	fprintf(media,"\n\n:::::::::::::::::::::::::: MEDIA DAS MEDICOES DE TEMPO :::::::::::::::::::::::::::::::\n\n");
	
	long int contLinhas;
	clock_t  tempo;
	
	double temposLista[6]; 
	double temposABB[6];
	double temposAVL[6];
	
	double med[10]; // de 0 a 9 armazena o valor de cada laço, e med[10] armazena a media deles
	int repet=0;
	
    fprintf(media,"\n\n\t\t\t\t10 MIL ELEMENTOS: \n\n");
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::	
	AVL* Avl = NULL;
	AVL*aux = NULL;
//INSERÇÃO EM ARVORE AVL - 10.000 ELEMENTOS
	printf("\n\n:::::::::::::::::::AVL 10 MIL ELEMENTOS::::::::::::::::::::::\n");
    fprintf(media,"\n\t\tAVL______________ \n");
	contLinhas = 0;
	char *avl1 = malloc(sizeof(char)*10000);
	
	while(repet != 10){ // 0 a 9
		tempo = clock();
		while(!feof(arq) && contLinhas != 10000){
			fscanf(arq, "%s", avl1); // converte a string lida em inteiro
			Avl = InsereAVL(Avl, atoi(avl1));
			contLinhas++;
	    } 
		tempo = clock() - tempo; // final - inicial
    	med[repet] = (double)tempo/(CLOCKS_PER_SEC/1000); // Tempo final - inicial;
    	if (repet > 9)Avl = DestruirAVL(Avl);
    	repet++;
	}
	//ImprimeAvlSimetrica(Avl);
	temposAVL[0] = calcMedia(med);
	printf("\n\n\t INSERCAO: %.30lf milissegundos\n\n", temposAVL[0]);
	fprintf(media," Insercao:  %.10lf milisegundos \n", temposAVL[0]);

//BUSCA EM AVL - 10.000 ELEMENTOS
	// busca por 100001 pois esse elemento não existe nas estruturas, logo,  
	//veremos qual estrutura demora mais para buscar e dizer que não encontrou, ou seja, percorrer.
	repet = 0;
	aux = Avl;
	while(repet != 10){ // 0 a 49
		tempo = clock();
		BuscaAVL(Avl, 100001);
		tempo = clock() - tempo; // final - inicial
		med[repet] = (double)tempo/(CLOCKS_PER_SEC/1000); // Tempo final - inicial;
		repet ++;
	}
	temposAVL[1] = calcMedia(med);;
	Avl = DestruirAVL(Avl);
	aux = NULL;
	printf("\n\t BUSCA: %.30lf milissegundos\n", temposAVL[1]);
	fprintf(media," Busca:     %.10lf milisegundos \n", temposAVL[1]);
	avl1 = NULL;
	
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::	
	No* Lista = NULL;
	No* Busca;
	repet = 0;
//INSERÇÃO EM LISTA - 10.000 ELEMENTOS -  insere ordenada
	printf("\n\n:::::::::::::::::::LISTA 10 MIL ELEMENTOS::::::::::::::::::::::\n");
    fprintf(media,"\n\t\tLISTA______________ \n");
	contLinhas = 0;
	char *list1 = malloc(sizeof(char)*10000);
	
	while(repet != 10){ // 0 a 9
		tempo = clock();
		while(!feof(arq) && contLinhas != 10000){
			fscanf(arq, "%s", list1); // converte a string lida em inteiro
			Lista = InsereLista(Lista, atoi(list1));
			contLinhas++;
	    } 
		tempo = clock() - tempo; // final - inicial
    	med[repet] = (double)tempo/(CLOCKS_PER_SEC/1000); // Tempo final - inicial;
    	if (repet > 9){
    		LiberaLista(Lista);
    		Lista = NULL;
		}
    	repet ++;
	}
	//ImprimeLista(Lista);
	temposLista[0] = calcMedia(med);;
	printf("\n\n\t INSERCAO: %.30lf milissegundos\n\n", temposLista[0]);
	fprintf(media," Insercao:  %.10lf milisegundos \n", temposLista[0]);

//BUSCA EM LISTA - 10 MIL
	repet = 0;
	while(repet != 10){ // 0 a 9
		tempo = clock();
		Busca = BuscaLista(Lista, 100001);
		if(Busca != NULL) printf("\n Elemento encontrado na Lista\n");
		else printf("Elemento nao encontrado na lista!\n");
		tempo = clock() - tempo; // final - inicial
		med[repet] = (double)tempo/(CLOCKS_PER_SEC/1000); // Tempo final - inicial;
		repet ++;
		Busca = NULL;
	}
    
	temposLista[1] = calcMedia(med);
	LiberaLista(Lista);
	Lista = NULL;
	list1= NULL;
	printf("\n\t BUSCA: %.30lf milissegundos\n", temposLista[1]);
	fprintf(media," Busca:     %.10lf milisegundos \n", temposLista[1]);

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::	
	Arv* Abb = NULL;
	repet = 0;
//INSERÇÃO EM ABB - 10.000 ELEMENTOS
	printf("\n\n:::::::::::::::::::ABB 10 MIL ELEMENTOS::::::::::::::::::::::\n");
	fprintf(media,"\n\t\tABB______________ \n");
	contLinhas = 0;
	char *abb1 = malloc(sizeof(char)*10000);
		while(repet != 10){ // 0 a 9
		tempo = clock();
		while(!feof(arq) && contLinhas != 10000){
			fscanf(arq, "%s", abb1); // converte a string lida em inteiro
			Abb = Insere_abb(Abb, CriaNovo(atoi(abb1)));
			contLinhas++;
	    } 
		tempo = clock() - tempo; // final - inicial
    	med[repet] = (double)tempo/(CLOCKS_PER_SEC/1000); // Tempo final - inicial;
		if(repet > 9) Abb = DesalocaArv(Abb);
		repet++;
	}
	//ImprimeArvSimetrica(Abb);
	temposABB[0] = calcMedia(med);
	printf("\n\n\t INSERCAO: %.30lf milissegundos\n\n", temposABB[0]);
	fprintf(media," Insercao:  %.10lf milisegundos \n", temposABB[0]);

//BUSCA EM ABB - 10 MIL
	repet = 0;
	while(repet != 10){ // 0 a 10
		tempo = clock();
		if(busca_abb(Abb, 100001) != NULL) printf("\n Elemento encontrado na Arv BB\n");
		else printf("Elemento nao encontrado!\n");
		tempo = clock() - tempo; // final - inicial
		med[repet] = (double)tempo/(CLOCKS_PER_SEC/1000); // Tempo final - inicial;
		repet ++;
	}
    
	temposABB[1] = calcMedia(med);
	Abb = DesalocaArv(Abb);
	abb1= NULL;
	printf("\n\t BUSCA: %.30lf milissegundos\n", temposABB[1]);
	fprintf(media," Busca:     %.10lf milisegundos \n", temposABB[1]);

   fprintf(media,"\n\n\t\t\t\t50 MIL ELEMENTOS: \n\n");
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::	
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::	
	repet = 0;
	aux = NULL;
//INSERÇÃO EM ARVORE AVL - 50.000 ELEMENTOS
	printf("\n\n:::::::::::::::::::AVL 50 MIL ELEMENTOS::::::::::::::::::::::\n");
    fprintf(media,"\n\t\tAVL______________ \n");
	contLinhas = 0;
	char *avl2 = malloc(sizeof(char)*50000);
	
	while(repet != 10){ // 0 a 9
		tempo = clock();
		while(!feof(arq) && contLinhas != 50000){
			fscanf(arq, "%s", avl2); // converte a string lida em inteiro
			Avl = InsereAVL(Avl, atoi(avl2));
			contLinhas++;
	    } 
		tempo = clock() - tempo; // final - inicial
    	med[repet] = (double)tempo/(CLOCKS_PER_SEC/1000); // Tempo final - inicial;
		if (repet > 9)Avl = DestruirAVL(Avl);
    	repet++;
	}

	temposAVL[2] = calcMedia(med);
	printf("\n\n\t INSERCAO: %.30lf milissegundos\n\n", temposAVL[2]);
	fprintf(media," Insercao:  %.10lf milisegundos \n", temposAVL[2]);
	
//BUSCA EM AVL - 50.000 ELEMENTOS
	// busca por 100001 pois esse elemento não existe nas estruturas, logo,  
	//veremos qual estrutura demora mais para buscar e dizer que não encontrou, ou seja, percorrer.
	repet = 0;
	aux = Avl;
	while(repet != 10){ // 0 a 9
	
		tempo = clock();
		BuscaAVL(aux, 100001);
		tempo = clock() - tempo; // final - inicial
		med[repet] = (double)tempo/(CLOCKS_PER_SEC/1000); // Tempo final - inicial;
		repet ++;
	}

	temposAVL[3] = calcMedia(med);
	Avl = DestruirAVL(Avl);
	aux = NULL;
	printf("\n\t BUSCA: %.30lf milissegundos\n", temposAVL[3]);
	fprintf(media," Busca:     %.10lf milisegundos \n", temposAVL[3]);
	avl2 = NULL;
	
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::	
	
//INSERÇÃO EM LISTA - 50.000 ELEMENTOS -  insere no inicio
	printf("\n\n:::::::::::::::::::LISTA 50 MIL ELEMENTOS::::::::::::::::::::::\n");
    fprintf(media,"\n\t\tLISTA____________ \n");
	contLinhas = 0;
	repet = 0;
	char *list2 = malloc(sizeof(char)*50000);
	
	while(repet != 10){ // 0 a 9
		tempo = clock();
		while(!feof(arq) && contLinhas != 50000){
			fscanf(arq, "%s", list2); // converte a string lida em inteiro
			Lista = InsereLista(Lista, atoi(list2));
			contLinhas++;
	    } 
		tempo = clock() - tempo; // final - inicial
    	med[repet] = (double)tempo/(CLOCKS_PER_SEC/1000); // Tempo final - inicial;
    	if (repet > 9){
    		LiberaLista(Lista);
    		Lista = NULL;
		}
		repet++;
	}
	
	temposLista[2] = calcMedia(med);
	printf("\n\n\t INSERCAO: %.30lf milissegundos\n\n", temposLista[2]);
	fprintf(media," Insercao:  %.10lf milisegundos \n", temposLista[2]);

//BUSCA EM LISTA - 50 MIL
	repet = 0;
	while(repet != 10){ // 0 a 9
		tempo = clock();
		Busca = BuscaLista(Lista, 100001);
		if(Busca != NULL) printf("\n Elemento encontrado na Lista\n");
		else printf("Elemento nao encontrado na lista!\n");
		tempo = clock() - tempo; // final - inicial
		med[repet] = (double)tempo/(CLOCKS_PER_SEC/1000); // Tempo final - inicial;
		Busca = NULL;
		repet ++;
	}
    
	temposLista[3] = calcMedia(med);
	LiberaLista(Lista);
	Lista = NULL;
	list2= NULL;
	printf("\n\t BUSCA: %.30lf milissegundos\n", temposLista[3]);
	fprintf(media," Busca:     %.10lf milisegundos \n", temposLista[3]);

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::	
	Abb = NULL;
//INSERÇÃO EM ABB - 50.000 ELEMENTOS 
	printf("\n\n:::::::::::::::::::ABB 50 MIL ELEMENTOS::::::::::::::::::::::\n");
    fprintf(media,"\n\t\tABB______________ \n");
	contLinhas = 0;
	char *abb2 = malloc(sizeof(char)*50000);
	repet = 0;
	

	while(repet != 10){// 0 a 9
		tempo = clock();
		while(!feof(arq) && contLinhas != 50000){
			fscanf(arq, "%s", abb2); // converte a string lida em inteiro
			Abb = Insere_abb(Abb, CriaNovo(atoi(abb2)));
			contLinhas++;
	    } 
		tempo = clock() - tempo; // final - inicial
    	med[repet] = (double)tempo/(CLOCKS_PER_SEC/1000); // Tempo final - inicial;
    	if(repet > 9) Abb = DesalocaArv(Abb);
    	repet++;
	}	
	temposABB[2] = calcMedia(med);
	printf("\n\n\t INSERCAO: %.30lf milissegundos\n\n", temposABB[2]);
	fprintf(media," Insercao:  %.10lf milisegundos \n", temposABB[2]);

//BUSCA EM ABB - 50 mil
	repet = 0;
	while(repet != 10){ // 0 a 9
		tempo = clock();
		if(busca_abb(Abb, 100001) != NULL) printf("\n Elemento encontrado na Arv BB\n");
		else printf("Elemento nao encontrado!\n");
		tempo = clock() - tempo; // final - inicial
		med[repet] = (double)tempo/(CLOCKS_PER_SEC/1000); // Tempo final - inicial;
		repet ++;
	}
    
	temposABB[3] = calcMedia(med);
	Abb = DesalocaArv(Abb);
	abb2= NULL;
	printf("\n\t BUSCA: %.30lf milissegundos\n", temposABB[3]);
	fprintf(media," Busca:     %.10lf milisegundos \n", temposABB[3]);
 
    fprintf(media,"\n\n\t\t\t\t100 MIL ELEMENTOS: \n\n");
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::	
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::	
	aux = NULL;
	repet = 0;
//INSERÇÃO EM ARVORE AVL - 100.000 ELEMENTOS
	printf("\n\n:::::::::::::::::::AVL 100 MIL ELEMENTOS::::::::::::::::::::::\n");
    fprintf(media,"\n\t\tAVL______________ \n");
	contLinhas = 0;
	char *avl3 = malloc(sizeof(char)*100000);
	
	while(repet != 10){ // 0 a 9
		tempo = clock();
		while(!feof(arq) && contLinhas != 100000){
			fscanf(arq, "%s", avl3); // converte a string lida em inteiro
			Avl = InsereAVL(Avl, atoi(avl3));
			contLinhas++;
	    } 
		tempo = clock() - tempo; // final - inicial
    	med[repet] = (double)tempo/(CLOCKS_PER_SEC/1000); // Tempo final - inicial;
    	if (repet > 9)Avl = DestruirAVL(Avl);
    	repet++;
	}

	temposAVL[4] = calcMedia(med);
	printf("\n\n\t INSERCAO: %.30lf milissegundos\n\n", temposAVL[4]);
	fprintf(media," Insercao:  %.10lf milisegundos \n", temposAVL[4]);
	
//BUSCA EM AVL - 100.000 ELEMENTOS
	// busca por 100001 pois esse elemento não existe nas estruturas, logo,  
	//veremos qual estrutura demora mais para buscar e dizer que não encontrou, ou seja, percorrer.
	repet = 0;
	aux = Avl;
	while(repet != 10){ // 0 a 9
	
		tempo = clock();
		BuscaAVL(Avl, 100001);
		tempo = clock() - tempo; // final - inicial
		med[repet] = (double)tempo/(CLOCKS_PER_SEC/1000); // Tempo final - inicial;
		repet ++;
	}
	
	temposAVL[5] = calcMedia(med);
	Avl = DestruirAVL(Avl);
	aux = NULL;
	printf("\n\t BUSCA: %.30lf milissegundos\n", temposAVL[5]);
	fprintf(media," Busca:     %.10lf milisegundos \n", temposAVL[5]);
	avl3 = NULL;
	
	repet = 0;
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::	
//INSERÇÃO EM LISTA - 100.000 ELEMENTOS -  insere no inicio
	printf("\n\n:::::::::::::::::::LISTA 100 MIL ELEMENTOS::::::::::::::::::::::\n");
    fprintf(media,"\n\t\tLISTA____________ \n");
	contLinhas = 0;
	char *list3 = malloc(sizeof(char)*100000);
	
	while(repet != 10){ // 0 a 9
		tempo = clock();
		while(!feof(arq) && contLinhas != 100000){
			fscanf(arq, "%s", list3); // converte a string lida em inteiro
			Lista = InsereLista(Lista, atoi(list3));
			contLinhas++;
	    } 
		tempo = clock() - tempo; // final - inicial
    	med[repet] = (double)tempo/(CLOCKS_PER_SEC/1000); // Tempo final - inicial;
    	if (repet > 9){
    		LiberaLista(Lista);
    		Lista = NULL;
		}
		repet++;
	}
	
	temposLista[4] = calcMedia(med);
	printf("\n\n\t INSERCAO: %.30lf milissegundos\n\n", temposLista[4]);
	fprintf(media," Insercao:  %.10lf milisegundos \n", temposLista[4]);

//BUSCA EM LISTA - 100 MIL
	repet = 0;
	while(repet != 10){ // 0 a 9
		tempo = clock();
		Busca = BuscaLista(Lista, 100001);
		if(Busca != NULL) printf("\n Elemento encontrado na Lista\n");
		else printf("Elemento nao encontrado na lista!\n");
		tempo = clock() - tempo; // final - inicial
		med[repet] = (double)tempo/(CLOCKS_PER_SEC/1000); // Tempo final - inicial;
		repet ++;
		Busca = NULL;
	}
    
	temposLista[5] = calcMedia(med);
	LiberaLista(Lista);
	Lista = NULL;
	list3= NULL;
	printf("\n\t BUSCA: %.30lf milissegundos\n", temposLista[5]);
	fprintf(media," Busca:     %.10lf milisegundos \n", temposLista[5]);

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::	
	Abb = NULL;
	repet = 0;
//INSERÇÃO EM ABB - 100.000 ELEMENTOS 
	printf("\n\n:::::::::::::::::::ABB 100 MIL ELEMENTOS::::::::::::::::::::::\n");
    fprintf(media,"\n\t\tABB______________ \n");
	contLinhas = 0;
	char *abb3 = malloc(sizeof(char)*100000);
		while(repet != 10){ // 0 a 9
		tempo = clock();
		while(!feof(arq) && contLinhas != 100000){
			fscanf(arq, "%s", abb3); // converte a string lida em inteiro
			Abb = Insere_abb(Abb, CriaNovo(atoi(abb3)));
			contLinhas++;
	    } 
		tempo = clock() - tempo; // final - inicial
    	med[repet] = (double)tempo/(CLOCKS_PER_SEC/1000); // Tempo final - inicial;
    	if(repet > 9) Abb = DesalocaArv(Abb);
    	repet++;
	}

	temposABB[4] = calcMedia(med);
	printf("\n\n\t INSERCAO: %.30lf milissegundos\n\n", temposABB[4]);
	fprintf(media," Insercao:  %.10lf milisegundos \n", temposABB[4]);

//BUSCA EM ABB - 100 MIL
	repet = 0;
	while(repet != 10){ // 0 a 9
		tempo = clock();
		if(busca_abb(Abb, 100001) != NULL) printf("\n Elemento encontrado na Arv BB\n");
		else printf("Elemento nao encontrado!\n");
		tempo = clock() - tempo; // final - inicial
		med[repet] = (double)tempo/(CLOCKS_PER_SEC/1000); // Tempo final - inicial;
		repet ++;
	}
    
	temposABB[5] = calcMedia(med);
	Abb = DesalocaArv(Abb);
	abb3= NULL;
	printf("\n\t BUSCA: %.30lf milissegundos\n", temposABB[5]);
	fprintf(media," Busca:     %.10lf milisegundos \n", temposABB[5]);

	//Comparações:
	int i;
	printf("\n\n\tABRA O ARQUIVO CHAMADO MediaDasMedicoes PARA VER AS CONCLUSOES!\n\n ");
	fclose(media);
	fclose(arq);	
}

