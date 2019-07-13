// Algoritmos de Ordenação e suas medições de tempo

#include<stdio.h>
#include<time.h>
#include<stdlib.h>

//Quantidade de elementos do vetor
long int  n = 550000;
int i;


int randomInteger (int low, int high){
    int k;
    double d;
    d = (double) rand () / ((double) RAND_MAX + 1);
    k = d * (high - low + 1);
    return low + k;
}

void VetorAleatorio(int* V, int n){ //SEM REPETIÇÃO DE ELEMENTOS

	for(i=0; i < n; i++){
		V[i] = randomInteger(0,100);
	}
}

void VetorCrescente(int *V, int n){
	for(i=0; i<n; i++)
		V[i] = i;
}

void VetorDecrescente(int *V, int n){
	for(i=n; i>0; i--)
		V[i] = i;
}

//Métodos de Ordenação
void InsertionSort(int* V, int n){
	int i, j, aux;
	for(i=1; i<n; i++){
		aux = V[i];
		for(j=i; j>0 && aux< V[j-1]; j--)
			V[j] = V[j-1];
		V[j] = aux;
	}
}

void BubbleSort(int *V, int n){
	int continua, aux, fim = n;
	do{
		continua = 0;
		for(i = 0; i<fim-1; i++){
			if(V[i]>V[i+1]){
				aux = V[i];
				V[i] = V[i+1];
				V[i+1] = aux;
				continua = i;
			}
		}
		fim--;
	}while(continua != 0);
}

void SelectionSort(int *V, int n){
  int i, j, min, aux;
  /* O minimo é o primeiro número não ordenado ainda */
  for (i = 0; i < (n - 1); i++) {
    min = i;
    for (j = i+1; j < n; j++) {
      if (V[j] < V[min]) 
		min = j;
    }
    /* Se o minimo for diferente do primeiro numero não ordenado ele faz a troca para ordena-los*/
    if (i != min) {
      aux = V[i];
      V[i] = V[min];
      V[min] = aux;
    }
  }
}


void main(){
	srand(time(NULL));
	double ini, fim, total;
	int V[n];
	
	VetorAleatorio(V, n);
	
	//Método de Ordenação
	ini = time(NULL);
	InsertionSort(V,n);
	fim = time(NULL);
	
	total = difftime(fim,ini);
	printf("Tempo p/ ordenar: %f segundos \n", total);
	//for(i=0; i<n; i++)
	//	printf("\nV[%d] = %d ", i, V[i]);
}
