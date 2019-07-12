#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define linhas 402
#define colunas 402

clock_t tempoInicial;

int i,j,k;
double red[linhas][colunas],black[linhas][colunas], chapaReal[linhas][colunas];

void printarBonito(double m[linhas][colunas]){
	
	for(i=0;i<linhas;i++){
		for(j=0;j<colunas;j++){
			printf("  [%6.4f]  ", m[i][j]);
		}
		printf("\n\n");
	}	
}

void inicializacaoRed(){
	
	//Zerando bordas
	for(i=0;i<linhas;i++){
		red[0][i]=0;
	}
	for(i=0;i<linhas;i++){
		red[i][0]=0;
	}
	for(i=0;i<linhas;i++){
		red[linhas-1][i]=0;
	}
	for(i=0;i<linhas;i++){
		red[i][colunas-1]=0;
	}
	
}

void setRed(){
	//sets
	red[75][75]=-10;
	red[75][175]=25;
	red[75][275]=0;
	red[190][75]=20;
	red[190][175]=-20;
	red[190][275]=10;
	red[305][75]=10;
	red[305][175]=30;
	red[305][275]=40;
}

void inicializacaoBlack(){
	
	//Zerando bordas
	for(i=0;i<linhas;i++){
		black[0][i]=0;
	}
	for(i=0;i<linhas;i++){
		black[i][0]=0;
	}
	for(i=0;i<linhas;i++){
		black[linhas-1][i]=0;
	}
	for(i=0;i<linhas;i++){
		black[i][colunas-1]=0;
	}
}

void setBlack(){
	//sets
	black[75][75]=-10;
	black[75][175]=25;
	black[75][275]=0;
	black[190][75]=20;
	black[190][175]=-20;
	black[190][275]=10;
	black[305][75]=10;
	black[305][175]=30;
	black[305][275]=40;
}

int main (){
	double dif;
	int iteracao=1, parar;
	
	//Zerando bordas
	inicializacaoRed();
	inicializacaoBlack();
	//Inicializações ignorando as bordas!
	for(i=1;i<linhas-1;i++){
		for(j=1;j<colunas-1;j++){
			red[i][j]=0;
			black[i][j]=0;
		}
	}
	//setando pontos constantes
	setRed();
	setBlack();
	
	printf("\nMatriz RED\n");
	//printarBonito(red);
	printf("\nMatriz BLACK\n");
	//printarBonito(black);
	
	tempoInicial = clock();
	do{
		//printf("\nIte: %d\n", iteracao);
		if( (iteracao%2) == 0){
			//red recebe ignorando as bordas
			for(i=1;i<linhas-1;i++){
				for(j=1;j<colunas-1;j++){
					red[i][j] = (black[i][j] + black[i-1][j] + black[i][j+1] + black[i+1][j] + black[i][j-1])/5;
				}
			}
				
		}
		else{
			//black recebe ignorando as bordas
			for(i=1;i<linhas-1;i++){
				for(j=1;j<colunas-1;j++){
					black[i][j] = (red[i][j] + red[i-1][j] + red[i][j+1] + red[i+1][j] + red[i][j-1])/5;
				}
			}
			
		}
		
		//Arrumar os valores constantes
		setRed();
		setBlack();
		
		//Assume-se que nesta iteração i iremos parar
		parar=1;
		//calcula a diferença dos pontos para checar o critério de parada
		for(i=1;i<linhas-1;i++){
			for(j=1;j<colunas-1;j++){
				dif = red[i][j] - black[i][j];
				//printf("\n DIF:%f \n",dif);
				//Se algum ponto das matrizes tiver diferença >= 0.01, então NÃO PARA
				if( fabs(dif) >= 0.01){
					//printf("\n DIF:%f \n",dif);
					parar = 0;
				}
			}
		}
		
		iteracao++;	
	}while(parar==0);
	
	if( (iteracao%2) == 0){
		for(i=1;i<linhas-1;i++){
			for(j=1;j<colunas-1;j++){
				chapaReal[i][j] = red[i][j];				
			}
		}
		
	}else{
		for(i=1;i<linhas-1;i++){
			for(j=1;j<colunas-1;j++){
				chapaReal[i][j] = black[i][j];				
			}
		}
	}
	
	tempoInicial = clock() - tempoInicial;
	
	printf("\nNúmero de iterações: %d\n", iteracao-1);
	printf("\nTempo de execução:%6.4f ms\n", ((double)tempoInicial) / ((CLOCKS_PER_SEC/1000)) );
	//printf("\nMatriz RED\n");
	//printarBonito(red);
	//printf("\nMatriz BLACK\n");
	//printarBonito(black);
	
	printf("\nChapa real final:\n");
	//printarBonito(chapaReal);
	
	return 0;
}