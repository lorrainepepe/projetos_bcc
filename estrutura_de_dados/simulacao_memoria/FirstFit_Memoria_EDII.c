/**	Trabalho de Laborat�rio de Estruturas de Dado II
  * Prof. Dr. Leandro Alves Neves
  *
  * Grupo 3: Lorraine Maria Pepe, Gustavo Fernando Balbino, Vin�cius Bernardi Vechiato, Everton de Melo Camacho
  * Data de entrega proposta: 12/09/2017
  * Vers�o Dev-C++: 5.11
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include "Biblio.h"

//Vari�veis globais e seus valores iniciais
int n = 0;
int id = 0;
int para = 1;
int sorteio = 6; 

FILA_PROCESSOS* FilaProcess;
MEMORIA* MemTotal = NULL; 	//  cabe�a da lista que representa toda a mem�ria
int* espLivre = NULL; 		// Espa�o livre total na mem�ria

//Gera pcb aleat�rios
PCB* GerarPCB(PCB* pcb, PROCESSO* processo) {
	srand(time(NULL));
	pcb->id = processo->mem->id;
	pcb->isDisp = -1; // indica que ainda n�o sabemos seu valor (se � bloqueado ou n�o)
	pcb->pc = (rand() % 1000) + 500;
	pcb->quantum = (rand() % 100) + 1;
	pcb->tempo_cpu = 0;
	return pcb;
}

// Cria o n� da fila de processos
PROCESSO* CriaNoFilaProc(MEMORIA* processo){ 
	PROCESSO* noFilaProc = (PROCESSO*)malloc(sizeof(PROCESSO));
	noFilaProc->mem = processo;
	noFilaProc->pcb = (PCB*)malloc(sizeof(PCB));
	noFilaProc->pcb = GerarPCB(noFilaProc->pcb, noFilaProc);
	return noFilaProc;
}

// MEM�RIA: lista duplamente encadeada n�o circular
// Estado inicial da Mem�ria
MEMORIA* CriaMemoriaTotal() {
	MEMORIA* m = (MEMORIA*)malloc(sizeof(MEMORIA));
	m -> id = id;
	m -> pos_ini = -1;
	m -> tamanho = 512;
	m -> status = 0;
	m -> prox = NULL;
	m -> ant = NULL;
	return m;
}

//Cria e Preenchem os processos na mem�ria (n�s da lista da mem�ria)
MEMORIA* CriaProcessosMem(MEMORIA* m) {
	srand(time(NULL));
	m = (MEMORIA*)malloc(sizeof(MEMORIA));
	m -> id = id;
	id++;
	m -> pos_ini = -1;
	m -> tamanho = (rand() % 120) + 8;
	m -> status = 1;
	m -> prox = NULL;
	m -> ant = NULL;
	return m;
}

void RemoveFirstFit(MEMORIA* processoMem){
	//Busca pela ID na mem�ria para remover:
	MEMORIA* aux = MemTotal;
	while(aux){
		if(aux->id == processoMem->id) break;
		aux = aux->prox;
	}
	if(aux == MemTotal){ // se o n� removido for o 1� 
		if(aux->prox && aux->prox->status == 0){
			//Aumenta a mem�ria dispon�vel -> mant�m o 1� n� como vazio e adiciona a ele o prox
			//pos_Ini continua a mesma
			aux->tamanho += aux->prox->tamanho;
			aux->id = -1;
			aux->status = 0;
			*espLivre += aux->tamanho; 
			// arruma os apontamentos para "juntar" os espa�os em brancos (sumir com um n� vazio, o prox, j� que ele foi incorporado ao aux)
			if((aux->prox)->prox != NULL)
				((aux->prox)->prox)->ant = aux;
			aux->prox = (aux->prox)->prox;
			free(aux->prox);
			printf("\n____________________\nMEM�RIA ATUAL: \n\n");
			ImprimeMemoria(MemTotal);
			return;
		}
		else if(!aux->prox || aux->prox->status == 1){
			// O n� ficar� representando um n� vazio
			// O tamanho do processo � o tamanho do espa�o que ficar� vazio, logo, n�o se altera
			aux->id = -1;
			aux->status = 0;
			*espLivre += aux->tamanho; 
			//n�o necessita arrumar ponteiros
			//posIni j� � 0
			printf("\n____________________\nMEM�RIA ATUAL: \n\n");
			ImprimeMemoria(MemTotal);
			return;
		}
	}
	else{
		if(aux->ant->status == 0 && (!aux->prox ||aux->prox->status == 1)){
			//Conserva o espa�o vazio mais a esquerda (o ant), soma o aux nele e remove o aux, logo a pos_Ini nao muda
			aux->ant->tamanho += aux->tamanho;
			*espLivre += aux->tamanho;
			aux->ant->prox = aux->prox;
			aux->prox->ant = aux->ant;
			free(aux);
			printf("\n____________________\nMEM�RIA ATUAL: \n\n");
			ImprimeMemoria(MemTotal);
			return;
		}
		else if(aux->ant->status == 1 && (!aux->prox ||aux->prox->status == 1)){ 
			// o aux ficar� como sendo o espa�o vazio, logo o tamanho do n� n�o muda, nem os apontamentos
			*espLivre += aux->tamanho;
			aux->id = -1;
			aux->status = 0;
			printf("\n____________________\nMEM�RIA ATUAL: \n\n");
			ImprimeMemoria(MemTotal);
			return;
		}
		else if(aux->ant->status == 1 && (!aux->prox ||aux->prox->status == 0)){
			aux->tamanho = aux->prox->tamanho;
			*espLivre += aux->tamanho;
			aux->id = -1;
			aux->status = 0;
			//apontamentos
			aux = (aux->prox)->prox;
			if((aux->prox)->prox != NULL)
				((aux->prox)->prox)->ant = aux;
			free(aux->prox);
			return;
		}
		else{ // ambos os espa�os ao lado de aux est�o vazios, no pior caso
			//mantemos o mais a esquerda para n�o mexermos com a posi��o e somamos o aux e o aux->prox nele (aux->ant)
			aux->ant->tamanho += aux->tamanho;
			aux->ant->tamanho +=aux->prox->tamanho;
			*espLivre += aux->tamanho;
			// n�o somamos o aux->prox ao espLivre pois se ele est� vazio significa que j� est� contabilizado no total o seu tamanho
			//arruma-se os apontamentos
			(aux->ant)->prox = aux->prox;
			if((aux->prox)->ant)
				(aux->prox)->ant = aux->ant;
			free(aux->prox);
			free(aux);
			printf("\n____________________\nMEM�RIA ATUAL: \n\n");
			ImprimeMemoria(MemTotal);
			return;
		}
	}
}

void InsereFirstFit(MEMORIA* processoMem){
	MEMORIA* aux = MemTotal;
	while(aux != NULL){ // procuraremos um espa�o vazio
		if(aux->status == 1) aux = aux->prox;
		else{
			if(aux->tamanho < processoMem->tamanho) // Se n�o cabe o processo
				aux = aux->prox;
			else{ // Se couber:
				if(aux->tamanho == processoMem->tamanho){ // Independente dos n�s ao lados, se for o "espa�o perfeito", basta aloc�-lo
					processoMem->ant = aux->ant;
					processoMem->prox = aux->prox;
					aux->ant = processoMem;
					aux->prox = processoMem;
					free(aux);
					processoMem->pos_ini = processoMem->ant->tamanho;
					//Diminuir a mem�ria dispon�vel
					*espLivre -= processoMem->tamanho;
					printf("\n\n________________________\nPROCESSO ALOCADO NA MEM�RIA:\n");
					ImprimeProcesso(processoMem);
					printf("\n____________________\nMEM�RIA ATUAL: \n\n");
					ImprimeMemoria(MemTotal);
					//Colocar o processo da mem�ria tamb�m na fila de processos a serem executados
					PROCESSO *noProcess = CriaNoFilaProc(processoMem);
					printf("\n\n________________________\nPROCESSO INSERIDO NA FILA DE PROCESSOS:");
					FilaProcess = InsereFilaProcessos(FilaProcess, noProcess); // insere na fila
					printf("\n\n________________________\nFILA DE PROCESSOS: \n");
					ImprimeFilaProcessos(FilaProcess, 1);
					printf("\n");
					return;
				}
				//Se N�O for "espa�o perfeito", ent�o o espa�o � mais que suficiente e ainda sobra mem�ria vazia
				else{
					int tam = aux->tamanho;
					// A posi��o aux da mem�ria, agora � o processo, e o n� processoMem se transformou no espa�o vazio que sobrou.
					aux->tamanho = processoMem->tamanho;
					aux->id = processoMem->id;
					//aux->pos_ini = aux->ant->tamanho;
					aux->status = 1; // ESPA�O CHEIO, COM DADOS
					processoMem->status = 0;    // ESPA�O VAZIO
					processoMem->pos_ini = processoMem->tamanho; // pois a posi��o da cabe�a vai de 0 a tamanho-1
					processoMem->tamanho = tam - aux->tamanho;
					processoMem->id = -1;
					//Arrumar os ponteiros:
					processoMem->ant = aux;
					processoMem->prox = aux->prox;
					if(aux->prox != NULL)
						(aux->prox)->ant = processoMem;
					aux->prox = processoMem;
					printf("\n\n________________________\nPROCESSO ALOCADO NA MEM�RIA:\n");
					ImprimeProcesso(aux);
					printf("\n____________________\nMEM�RIA ATUAL: \n\n");
					ImprimeMemoria(MemTotal);
					//Diminuir a mem�ria dispon�vel
					*espLivre -= aux->tamanho;
					//Colocar o processo da mem�ria tamb�m na fila de processos a serem executados
					PROCESSO *noProcess = CriaNoFilaProc(aux);
					printf("\n\n________________________\nPROCESSO INSERIDO NA FILA DE PROCESSOS:");
					FilaProcess = InsereFilaProcessos(FilaProcess, noProcess); // insere na fila
					printf("\n\n________________________\nFILA DE PROCESSOS: \n");
					ImprimeFilaProcessos(FilaProcess, 1);
					printf("\n");
					return;
				}
			}
		}
	}
	// Se n�o retornou ainda � porque n�o foi alocado:
	printf("\nPROCESSO N�O PODE SER ALOCADO! MEM�RIA INSUFICIENTE!\nDADOS DO PROCESSO GERADO: \n");
	ImprimeProcesso(processoMem);
}

void Round_Robin(FILA_PROCESSOS * bloqueados){
	/*
	- SUPOMOS QUE O QUANTUM DA M�QUINA SEJA DE 20s
	- Somente um processo � executado por vez, que � o do in�cio da fila de processos, mas desde que esse esteja com recursos dispon�veis.
		Se n�o estiver, ent�o o processo ir� para a fila de bloqueados, e s� retornar� � fila de processos(ao seu fim) quanto puder 
		ser atendido.
	- O contador de programa (PC) diz respeito ao local(endere�o ou linha de c�digo) em que deve iniciar a execu��o de um 
	 processo a cada ciclo que este passar pela CPU. Quando em quest�o processo for totalmente executado, isto �, quantum = 0, seu contador
	 de programa tamb�m se anula, visto que n�o h� mais dados desse para ser analisado.  
	*/
	
	int tempo_sobrado = 0; // Quando o quantum da m�quina � maior que o tamanho do processo e esse � encerrado.
	int tempo_CPU_total = 0;
	PROCESSO* proc = NULL;
	PROCESSO* bloq = NULL;
	do{
		printf("\n\nAnalisando mem�ria...");
		printf("\n___________________________\n");
		
		printf("\n\nFILA DE PROCESSOS   -    ANTES\n");
		ImprimeFilaProcessos(FilaProcess, 1);
		
		proc = RemoveFila(FilaProcess);
		
		printf("\n\nFILA DE BLOQUEADOS    -    ANTES\n");
		ImprimeFilaProcessos(bloqueados, 1);
		
		//Se � a primeira execu��o do processo, isDisp = -1. Se a fila de processos est� vazia, proc = NULL
		//Se o aleat�rio gerado for <= 3, o processo vai para a fila de dispositivos
		while((proc->pcb->isDisp <= 3 || proc->pcb->isDisp == -1) && proc != NULL){
			proc->pcb->isDisp = (rand() % 10) + 1;
			if(proc->pcb->isDisp <= 3){
				bloqueados = InsereFilaProcessos(bloqueados, proc);
				printf("\nO processo %d n�o possui os recursos necess�rios.\n", proc->pcb->id);
				printf("Foi transferido para a fila de bloqueados at� que os recursos estejam dispon�veis.\n");
				printf("\n___________________________\n");
				puts("\nTentaremos executar o pr�ximo da fila de processos se houver...");
				proc = RemoveFila(FilaProcess);
				if(proc)
					proc->pcb->isDisp = (rand() % 10) + 1;	
			}
		}
		if (proc){	
			if(proc->pcb->isDisp > 3){
				printf("\n___________________________\n");
				printf("\n\nFILA DE PROCESSOS    -    DEPOIS\n");
				ImprimeFilaProcessos(FilaProcess, 0);
				printf("\n\nFILA DE BLOQUEADOS    -    DEPOIS\n");
				ImprimeFilaProcessos(bloqueados, 0);
				printf("\n___________________________\n");
				printf("INICIANDO EXECU��O...", n);
				printf("\n___________________________\n");
				printf("\nDADOS DO PROCESSO A SER EXECUTADO: \n");
				ImprimeProcesso(proc->mem);
				ImprimePCB(proc->pcb, 1);
				printf("\n\nExecutando... \n", n);
				printf("___________________________\n");
				system("pause");
				if (proc -> pcb -> quantum >= 20) {
					tempo_CPU_total += 20;
					proc -> pcb -> quantum -= 20;
					proc -> pcb -> tempo_cpu += 20;
					if (proc -> pcb -> quantum == 0) {
						proc->pcb->pc = 0;
						printf("\n% Processo %d foi ENCERRADO. REMOVENDO DA MEM�RIA...\n", proc -> pcb -> id);
						printf("\n____________________\nMEM�RIA - ANTES: \n\n");
						ImprimeMemoria(MemTotal);
						RemoveFirstFit(proc->mem);
						printf("\nRESULTADOS DA EXECU��O: \n");
						ImprimeProcesso(proc->mem);
						ImprimePCB(proc->pcb, 1);
						free(proc);
						printf("\n");
						system("pause");
						// Se encerrou ent�o pode chegar um ou mais na mem�ria, dependendo de sua lota��o, controlada pelo GerenciaMemoria
						free(proc);
						return;
					}
					else {
						FilaProcess = InsereFilaProcessos(FilaProcess, proc);
						printf("\n\nProcesso n�o conclu�do...", n);
						proc->pcb->pc -= (rand() % 40) + 1; // Executa um n�mero aleat�rio de 1 a 40 instru��es no quantum decaido.
						printf("\n___________________________\n");
						printf("Retornando processo ao fim da fila de processos\n\n");
						printf("\nFILA DE PROCESSOS    -    P�S EXECU��O\n");
						ImprimeFilaProcessos(FilaProcess, 0);
						printf("\n____________________MEM�RIA PERMANECE IGUAL:  \n\n");
						ImprimeMemoria(MemTotal);
						printf("\n___________________________\n");
						system("pause");
						system("cls");
					}
				}
				else {
					tempo_CPU_total += proc->pcb->quantum;
					tempo_sobrado += (20 - proc -> pcb -> quantum); // acumula o tempo n�o usado pelo processo 
					proc->pcb->tempo_cpu += proc->pcb->quantum;
					proc->pcb->quantum = 0;
					printf("\nProcesso %d foi ENCERRADO. REMOVENDO DA MEM�RIA...\n", proc -> pcb -> id);
					proc->pcb->pc = 0;
					
					printf("\nRESULTADOS DA EXECU��O: \n");
					ImprimeProcesso(proc->mem);
					ImprimePCB(proc->pcb, 1);
					printf("\n____________________\nMEM�RIA - ANTES: \n\n");
					ImprimeMemoria(MemTotal);
					RemoveFirstFit(proc->mem);
					free(proc);
					printf("\n");
					system("pause");
					return;
					// Se encerrou ent�o pode chegar um ou mais na mem�ria, dependendo de sua lota��o, controlada pelo GerenciaMemoria
				}
			}
		}
		if (!VerificaFilaVazia(bloqueados)){
			int aleatorio = 4;
			while(!VerificaFilaVazia(bloqueados) || aleatorio < 5){ // Pode ser que saia mais de um processo de uma vez
				aleatorio = (rand() % 10) + 1; // verifica se j� pode sair da fila de bloqueados
				if (aleatorio >= 5) {
					bloq = RemoveFila(bloqueados);
					printf("\n___________________________\n");
					printf("O processo %d cumpriu suas necessidades...\n", bloq->pcb->id);
					bloq->pcb->isDisp = aleatorio;
					FilaProcess = InsereFilaProcessos(FilaProcess, bloq);
					printf("Retornando processo ao fim da fila de processos\n\n");
					printf("\nFILA DE PROCESSOS    -    P�S EXECU��O\n");
					ImprimeFilaProcessos(FilaProcess, 0);
					printf("\n___________________________\n");
					system("pause");
				}
			}
		}
		system("cls");
	}while(!VerificaFilaVazia(FilaProcess) || !VerificaFilaVazia(bloqueados));
	
	if(VerificaFilaVazia(FilaProcess)){
		puts("\n\tN�o h� processos na fila de processos para serem executados.\n");
		printf("\nTEMPO TOTAL PARA EXECU��O DOS PROCESSOS FOI DE:  %d segundos\n\n", tempo_CPU_total);
	}
	sorteio = 5;
}

void main(){
	srand(time(NULL));
	setlocale(LC_ALL, "portuguese");
	int sair = 1;
	FilaProcess = CriaFila();
	FILA_PROCESSOS* FilaBloq = CriaFila();
	MEMORIA* processoMem = NULL;
	//Criando a Mem�ria total:   � a lista que representa toda a mem�ria
	MemTotal = (MEMORIA*)malloc(sizeof(MEMORIA));
	MemTotal = CriaMemoriaTotal(MemTotal);
	espLivre = (int*)malloc(sizeof(int));
	*espLivre = MemTotal->tamanho;
	
	printf("\n________________________\nMEM�RIA CRIADA\n");
	ImprimeProcesso(MemTotal);
	
	printf("\n\n________________________\nCOME�ANDO A GERAR PROCESSOS\n");
	while(sair != 0){
		para = 1;
		sorteio = 5;
		while(para){
			if(sorteio <= 7){
				if(*espLivre > 0){
					processoMem = CriaProcessosMem(processoMem);
					if(processoMem->tamanho > *espLivre){
						printf("PROCESSO N�O PODE SER ALOCADO! MEM�RIA INSUFICIENTE!\nDADOS DO PROCESSO GERADO: \n");
						ImprimeProcesso(processoMem);
						printf("\n____________________\nMEM�RIA ATUAL: \n\n");
						ImprimeMemoria(MemTotal);
						sorteio = (rand() % 10) + 1; // Verifica se deve gerar outro processo ou n�o.
					}
					else{
						// condi��o inicial
						if(MemTotal->tamanho == 512 && MemTotal->status == 0 && !MemTotal->prox && !MemTotal->ant){
							// A posi��o inicial da mem�ria, cabe�a da lista, agora � o 1� processo, e o n� processoMem
						   // se transformou no espa�o vazio que sobrou.
							MemTotal->tamanho = processoMem->tamanho;
							MemTotal->id = processoMem->id;
							MemTotal->pos_ini = 0;
							MemTotal->status = 1; // ESPA�O CHEIO, COM DADOS
							processoMem->ant = MemTotal; //   liga os n�s
							MemTotal->prox = processoMem; // liga os n�s
							processoMem->status = 0;    // ESPA�O VAZIO
							processoMem->tamanho = 512 - MemTotal->tamanho;
							processoMem->pos_ini = MemTotal->tamanho; // pois a posi��o da cabe�a vai de 0 a tamanho-1
							processoMem->id = -1;
							printf("____________________\nMEM�RIA ATUAL: \n\n");
							ImprimeMemoria(MemTotal);
							// A partir de agora a mem�ria ter� 2 n�s
							// cria um n� da fila de processos com mem�ria e pcb
							PROCESSO *noProcess = CriaNoFilaProc(MemTotal);
							printf("\n\n________________________\nPROCESSO INSERIDO NA FILA DE PROCESSOS:");
							FilaProcess = InsereFilaProcessos(FilaProcess, noProcess); // insere na fila
							printf("\n\n________________________\nFILA DE PROCESSOS: \n");
							ImprimeFilaProcessos(FilaProcess, 1);
							printf("\n");
							sorteio = (rand() % 10) + 1; // Verifica se deve gerar outro processo ou n�o.
						}
						else{ // j� haver�o pelo menos 2 n�s ( a cabe�a que j� � um processo e um n� vazio)
							// Interrup��o por parte do usu�rio

							printf("\nInterromper gera��o de processos? 1- SIM ou 0- N�O:   ");
							scanf("%d", &para);
							if(para){
								para = 0;
								printf("\nPROCESSO INTERROMPIDO N�O FOI ALOCADO!\nDADOS DO PROCESSO GERADO: \n");
								ImprimeProcesso(processoMem);
								printf("\n____________________\nMEM�RIA ATUAL: \n\n");
								ImprimeMemoria(MemTotal);
								break;
							}
							InsereFirstFit(processoMem);
							printf("\n");
							//sorteio = (rand() % 10) + 1; // Verifica se deve gerar outro processo ou n�o.
						}
					}
				}
				sorteio = (rand() % 10) + 1; // Verifica se deve gerar outro processo ou n�o.
				if(sorteio > 7) break;	
			}
		}
		printf("\n");
		system("pause");
		para = 1;
		printf("\n________________________\nDADA ORDEM PARA EXECUTAR PROCESSOS\n"); // ou aleat�rio ou interrup��o
		Round_Robin(FilaBloq); // se o para for 0 saiu do roudin por escolha do usuario
		// Encerrar o programa
		printf("\nDeseja encerrar o sistema? 1- SIM ou 0- N�O:  ");
		scanf("%d", &sair);
		if(sair) sair = 0;
		else sair = 1;
	}
}
