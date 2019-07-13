#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include "funcoes.h"

int main() {
	setlocale(LC_ALL, "portuguese");
	Arv* arvore = NULL;
	FILE* arquivo = fopen ("arquivo.txt", "r");
	int resp, opcao;
	
	
	if (arquivo == NULL) {
		puts("Não foi possível carregar os dados do arquivo.");
		return 1;
	}
	arvore = carregarArv(arvore, arquivo);
	
	fclose(arquivo);
	Arv* arvoreDePercurso;
	char novaResposta[100];
	char novaPergunta[100];
	
	do {
		system("cls");
		printf("\n\t\t\tAKINATOR - VERSÃO DESENHOS ANIMADOS\n\n");
		printf("************************************INSTRUÇÕES***************************************\n");
		printf("\n\t*Responda as perguntas escolhendo um número:\n\t\t1-SIM\t 0-NÃO;\n");
		printf("\n\t*Pense sempre em um personagem de DESENHO ANIMADO;\n");
		printf("\n\t*As perguntas feitas devem ser de SIM ou NAO apenas;\n");
		printf("\n\t*NAO use acentos para digitar perguntas e respostas;\n");
		printf("\n*************************************************************************************\n");
		printf("\n Começando...\n");
		
		arvoreDePercurso = arvore;
		while (temSubArvores(arvoreDePercurso)) {
			fflush(stdin);
			printf("%s R: ", arvoreDePercurso -> info);
			fflush(stdin);
			scanf("%d", &resp);
			if (resp == 0) arvoreDePercurso = arvoreDePercurso -> esq;
			else if (resp == 1) arvoreDePercurso = arvoreDePercurso -> dir;
			fflush(stdin);
		}
		printf("\nVocê está pensando em  %s?  ", arvoreDePercurso -> info);
		fflush(stdin);
		scanf("%d", &resp);
		if (resp == 1) puts("\a\tEu ganhei! Adivinhei o desenho animado!");
		else if (resp == 0) {
			fflush(stdin);
			puts("\n\aParabéns, você me venceu! \t");
			puts("\nEm qual desenho animado estava pensando? \t");
			scanf("%[^\n]", &novaResposta);
			strupr(novaResposta);
			printf("\nInsira uma pergunta que é VÁLIDA PARA %s e NÃO PARA %s:\n\t", arvoreDePercurso -> info, novaResposta);
			fflush(stdin);
			scanf("%[^\n]", &novaPergunta);
			strupr(novaPergunta);
			fflush(stdin);
			arvoreDePercurso = inserirNovaResposta(arvoreDePercurso, novaResposta, novaPergunta);
			arquivo = fopen("arquivo.txt", "w");
			salvarArv(arvore, arquivo);
			fclose(arquivo);
		}
		printf("\n\nDeseja jogar novamente? 1-S ou 0-N\n ");
		scanf("%d", &opcao);
	} while (opcao == 1);
	desalocaArv(arvoreDePercurso);
	desalocaArv(arvore);
	return 0;
}
