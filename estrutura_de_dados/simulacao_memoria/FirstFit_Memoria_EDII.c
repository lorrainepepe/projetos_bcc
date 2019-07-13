/**	Trabalho de Laboratório de Estruturas de Dado II
  * Prof. Dr. Leandro Alves Neves
  *
  * Grupo 3: Lorraine Maria Pepe, Gustavo Fernando Balbino, Vinícius Bernardi Vechiato, Everton de Melo Camacho
  * Data de entrega proposta: 12/09/2017
  * Versão Dev-C++: 5.11
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include "Biblio.h"

//Variáveis globais e seus valores iniciais
int n = 0;
int id = 0;
int para = 1;
int sorteio = 6; 

FILA_PROCESSOS* FilaProcess;
MEMORIA* MemTotal = NULL; 	//  cabeça da lista que representa toda a memória
int* espLivre = NULL; 		// Espaço livre total na memória

//Gera pcb aleatórios
PCB* GerarPCB(PCB* pcb, PROCESSO* processo) {
	srand(time(NULL));
	pcb->id = processo->mem->id;
	pcb->isDisp = -1; // indica que ainda não sabemos seu valor (se é bloqueado ou não)
	pcb->pc = (rand() % 1000) + 500;
	pcb->quantum = (rand() % 100) + 1;
	pcb->tempo_cpu = 0;
	return pcb;
}

// Cria o nó da fila de processos
PROCESSO* CriaNoFilaProc(MEMORIA* processo){ 
	PROCESSO* noFilaProc = (PROCESSO*)malloc(sizeof(PROCESSO));
	noFilaProc->mem = processo;
	noFilaProc->pcb = (PCB*)malloc(sizeof(PCB));
	noFilaProc->pcb = GerarPCB(noFilaProc->pcb, noFilaProc);
	return noFilaProc;
}

// MEMÓRIA: lista duplamente encadeada não circular
// Estado inicial da Memória
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

//Cria e Preenchem os processos na memória (nós da lista da memória)
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
	//Busca pela ID na memória para remover:
	MEMORIA* aux = MemTotal;
	while(aux){
		if(aux->id == processoMem->id) break;
		aux = aux->prox;
	}
	if(aux == MemTotal){ // se o nó removido for o 1º 
		if(aux->prox && aux->prox->status == 0){
			//Aumenta a memória disponível -> mantém o 1º nó como vazio e adiciona a ele o prox
			//pos_Ini continua a mesma
			aux->tamanho += aux->prox->tamanho;
			aux->id = -1;
			aux->status = 0;
			*espLivre += aux->tamanho; 
			// arruma os apontamentos para "juntar" os espaços em brancos (sumir com um nó vazio, o prox, já que ele foi incorporado ao aux)
			if((aux->prox)->prox != NULL)
				((aux->prox)->prox)->ant = aux;
			aux->prox = (aux->prox)->prox;
			free(aux->prox);
			printf("\n____________________\nMEMÓRIA ATUAL: \n\n");
			ImprimeMemoria(MemTotal);
			return;
		}
		else if(!aux->prox || aux->prox->status == 1){
			// O nó ficará representando um nó vazio
			// O tamanho do processo é o tamanho do espaço que ficará vazio, logo, não se altera
			aux->id = -1;
			aux->status = 0;
			*espLivre += aux->tamanho; 
			//não necessita arrumar ponteiros
			//posIni já é 0
			printf("\n____________________\nMEMÓRIA ATUAL: \n\n");
			ImprimeMemoria(MemTotal);
			return;
		}
	}
	else{
		if(aux->ant->status == 0 && (!aux->prox ||aux->prox->status == 1)){
			//Conserva o espaço vazio mais a esquerda (o ant), soma o aux nele e remove o aux, logo a pos_Ini nao muda
			aux->ant->tamanho += aux->tamanho;
			*espLivre += aux->tamanho;
			aux->ant->prox = aux->prox;
			aux->prox->ant = aux->ant;
			free(aux);
			printf("\n____________________\nMEMÓRIA ATUAL: \n\n");
			ImprimeMemoria(MemTotal);
			return;
		}
		else if(aux->ant->status == 1 && (!aux->prox ||aux->prox->status == 1)){ 
			// o aux ficará como sendo o espaço vazio, logo o tamanho do nó não muda, nem os apontamentos
			*espLivre += aux->tamanho;
			aux->id = -1;
			aux->status = 0;
			printf("\n____________________\nMEMÓRIA ATUAL: \n\n");
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
		else{ // ambos os espaços ao lado de aux estão vazios, no pior caso
			//mantemos o mais a esquerda para não mexermos com a posição e somamos o aux e o aux->prox nele (aux->ant)
			aux->ant->tamanho += aux->tamanho;
			aux->ant->tamanho +=aux->prox->tamanho;
			*espLivre += aux->tamanho;
			// não somamos o aux->prox ao espLivre pois se ele está vazio significa que já está contabilizado no total o seu tamanho
			//arruma-se os apontamentos
			(aux->ant)->prox = aux->prox;
			if((aux->prox)->ant)
				(aux->prox)->ant = aux->ant;
			free(aux->prox);
			free(aux);
			printf("\n____________________\nMEMÓRIA ATUAL: \n\n");
			ImprimeMemoria(MemTotal);
			return;
		}
	}
}

void InsereFirstFit(MEMORIA* processoMem){
	MEMORIA* aux = MemTotal;
	while(aux != NULL){ // procuraremos um espaço vazio
		if(aux->status == 1) aux = aux->prox;
		else{
			if(aux->tamanho < processoMem->tamanho) // Se não cabe o processo
				aux = aux->prox;
			else{ // Se couber:
				if(aux->tamanho == processoMem->tamanho){ // Independente dos nós ao lados, se for o "espaço perfeito", basta alocá-lo
					processoMem->ant = aux->ant;
					processoMem->prox = aux->prox;
					aux->ant = processoMem;
					aux->prox = processoMem;
					free(aux);
					processoMem->pos_ini = processoMem->ant->tamanho;
					//Diminuir a memória disponível
					*espLivre -= processoMem->tamanho;
					printf("\n\n________________________\nPROCESSO ALOCADO NA MEMÓRIA:\n");
					ImprimeProcesso(processoMem);
					printf("\n____________________\nMEMÓRIA ATUAL: \n\n");
					ImprimeMemoria(MemTotal);
					//Colocar o processo da memória também na fila de processos a serem executados
					PROCESSO *noProcess = CriaNoFilaProc(processoMem);
					printf("\n\n________________________\nPROCESSO INSERIDO NA FILA DE PROCESSOS:");
					FilaProcess = InsereFilaProcessos(FilaProcess, noProcess); // insere na fila
					printf("\n\n________________________\nFILA DE PROCESSOS: \n");
					ImprimeFilaProcessos(FilaProcess, 1);
					printf("\n");
					return;
				}
				//Se NÃO for "espaço perfeito", então o espaço é mais que suficiente e ainda sobra memória vazia
				else{
					int tam = aux->tamanho;
					// A posição aux da memória, agora é o processo, e o nó processoMem se transformou no espaço vazio que sobrou.
					aux->tamanho = processoMem->tamanho;
					aux->id = processoMem->id;
					//aux->pos_ini = aux->ant->tamanho;
					aux->status = 1; // ESPAÇO CHEIO, COM DADOS
					processoMem->status = 0;    // ESPAÇO VAZIO
					processoMem->pos_ini = processoMem->tamanho; // pois a posição da cabeça vai de 0 a tamanho-1
					processoMem->tamanho = tam - aux->tamanho;
					processoMem->id = -1;
					//Arrumar os ponteiros:
					processoMem->ant = aux;
					processoMem->prox = aux->prox;
					if(aux->prox != NULL)
						(aux->prox)->ant = processoMem;
					aux->prox = processoMem;
					printf("\n\n________________________\nPROCESSO ALOCADO NA MEMÓRIA:\n");
					ImprimeProcesso(aux);
					printf("\n____________________\nMEMÓRIA ATUAL: \n\n");
					ImprimeMemoria(MemTotal);
					//Diminuir a memória disponível
					*espLivre -= aux->tamanho;
					//Colocar o processo da memória também na fila de processos a serem executados
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
	// Se não retornou ainda é porque não foi alocado:
	printf("\nPROCESSO NÃO PODE SER ALOCADO! MEMÓRIA INSUFICIENTE!\nDADOS DO PROCESSO GERADO: \n");
	ImprimeProcesso(processoMem);
}

void Round_Robin(FILA_PROCESSOS * bloqueados){
	/*
	- SUPOMOS QUE O QUANTUM DA MÁQUINA SEJA DE 20s
	- Somente um processo é executado por vez, que é o do início da fila de processos, mas desde que esse esteja com recursos disponíveis.
		Se não estiver, então o processo irá para a fila de bloqueados, e só retornará à fila de processos(ao seu fim) quanto puder 
		ser atendido.
	- O contador de programa (PC) diz respeito ao local(endereço ou linha de código) em que deve iniciar a execução de um 
	 processo a cada ciclo que este passar pela CPU. Quando em questão processo for totalmente executado, isto é, quantum = 0, seu contador
	 de programa também se anula, visto que não há mais dados desse para ser analisado.  
	*/
	
	int tempo_sobrado = 0; // Quando o quantum da máquina é maior que o tamanho do processo e esse é encerrado.
	int tempo_CPU_total = 0;
	PROCESSO* proc = NULL;
	PROCESSO* bloq = NULL;
	do{
		printf("\n\nAnalisando memória...");
		printf("\n___________________________\n");
		
		printf("\n\nFILA DE PROCESSOS   -    ANTES\n");
		ImprimeFilaProcessos(FilaProcess, 1);
		
		proc = RemoveFila(FilaProcess);
		
		printf("\n\nFILA DE BLOQUEADOS    -    ANTES\n");
		ImprimeFilaProcessos(bloqueados, 1);
		
		//Se é a primeira execução do processo, isDisp = -1. Se a fila de processos está vazia, proc = NULL
		//Se o aleatório gerado for <= 3, o processo vai para a fila de dispositivos
		while((proc->pcb->isDisp <= 3 || proc->pcb->isDisp == -1) && proc != NULL){
			proc->pcb->isDisp = (rand() % 10) + 1;
			if(proc->pcb->isDisp <= 3){
				bloqueados = InsereFilaProcessos(bloqueados, proc);
				printf("\nO processo %d não possui os recursos necessários.\n", proc->pcb->id);
				printf("Foi transferido para a fila de bloqueados até que os recursos estejam disponíveis.\n");
				printf("\n___________________________\n");
				puts("\nTentaremos executar o próximo da fila de processos se houver...");
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
				printf("INICIANDO EXECUÇÃO...", n);
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
						printf("\n% Processo %d foi ENCERRADO. REMOVENDO DA MEMÓRIA...\n", proc -> pcb -> id);
						printf("\n____________________\nMEMÓRIA - ANTES: \n\n");
						ImprimeMemoria(MemTotal);
						RemoveFirstFit(proc->mem);
						printf("\nRESULTADOS DA EXECUÇÃO: \n");
						ImprimeProcesso(proc->mem);
						ImprimePCB(proc->pcb, 1);
						free(proc);
						printf("\n");
						system("pause");
						// Se encerrou então pode chegar um ou mais na memória, dependendo de sua lotação, controlada pelo GerenciaMemoria
						free(proc);
						return;
					}
					else {
						FilaProcess = InsereFilaProcessos(FilaProcess, proc);
						printf("\n\nProcesso não concluído...", n);
						proc->pcb->pc -= (rand() % 40) + 1; // Executa um número aleatório de 1 a 40 instruções no quantum decaido.
						printf("\n___________________________\n");
						printf("Retornando processo ao fim da fila de processos\n\n");
						printf("\nFILA DE PROCESSOS    -    PÓS EXECUÇÃO\n");
						ImprimeFilaProcessos(FilaProcess, 0);
						printf("\n____________________MEMÓRIA PERMANECE IGUAL:  \n\n");
						ImprimeMemoria(MemTotal);
						printf("\n___________________________\n");
						system("pause");
						system("cls");
					}
				}
				else {
					tempo_CPU_total += proc->pcb->quantum;
					tempo_sobrado += (20 - proc -> pcb -> quantum); // acumula o tempo não usado pelo processo 
					proc->pcb->tempo_cpu += proc->pcb->quantum;
					proc->pcb->quantum = 0;
					printf("\nProcesso %d foi ENCERRADO. REMOVENDO DA MEMÓRIA...\n", proc -> pcb -> id);
					proc->pcb->pc = 0;
					
					printf("\nRESULTADOS DA EXECUÇÃO: \n");
					ImprimeProcesso(proc->mem);
					ImprimePCB(proc->pcb, 1);
					printf("\n____________________\nMEMÓRIA - ANTES: \n\n");
					ImprimeMemoria(MemTotal);
					RemoveFirstFit(proc->mem);
					free(proc);
					printf("\n");
					system("pause");
					return;
					// Se encerrou então pode chegar um ou mais na memória, dependendo de sua lotação, controlada pelo GerenciaMemoria
				}
			}
		}
		if (!VerificaFilaVazia(bloqueados)){
			int aleatorio = 4;
			while(!VerificaFilaVazia(bloqueados) || aleatorio < 5){ // Pode ser que saia mais de um processo de uma vez
				aleatorio = (rand() % 10) + 1; // verifica se já pode sair da fila de bloqueados
				if (aleatorio >= 5) {
					bloq = RemoveFila(bloqueados);
					printf("\n___________________________\n");
					printf("O processo %d cumpriu suas necessidades...\n", bloq->pcb->id);
					bloq->pcb->isDisp = aleatorio;
					FilaProcess = InsereFilaProcessos(FilaProcess, bloq);
					printf("Retornando processo ao fim da fila de processos\n\n");
					printf("\nFILA DE PROCESSOS    -    PÓS EXECUÇÃO\n");
					ImprimeFilaProcessos(FilaProcess, 0);
					printf("\n___________________________\n");
					system("pause");
				}
			}
		}
		system("cls");
	}while(!VerificaFilaVazia(FilaProcess) || !VerificaFilaVazia(bloqueados));
	
	if(VerificaFilaVazia(FilaProcess)){
		puts("\n\tNão há processos na fila de processos para serem executados.\n");
		printf("\nTEMPO TOTAL PARA EXECUÇÃO DOS PROCESSOS FOI DE:  %d segundos\n\n", tempo_CPU_total);
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
	//Criando a Memória total:   é a lista que representa toda a memória
	MemTotal = (MEMORIA*)malloc(sizeof(MEMORIA));
	MemTotal = CriaMemoriaTotal(MemTotal);
	espLivre = (int*)malloc(sizeof(int));
	*espLivre = MemTotal->tamanho;
	
	printf("\n________________________\nMEMÓRIA CRIADA\n");
	ImprimeProcesso(MemTotal);
	
	printf("\n\n________________________\nCOMEÇANDO A GERAR PROCESSOS\n");
	while(sair != 0){
		para = 1;
		sorteio = 5;
		while(para){
			if(sorteio <= 7){
				if(*espLivre > 0){
					processoMem = CriaProcessosMem(processoMem);
					if(processoMem->tamanho > *espLivre){
						printf("PROCESSO NÃO PODE SER ALOCADO! MEMÓRIA INSUFICIENTE!\nDADOS DO PROCESSO GERADO: \n");
						ImprimeProcesso(processoMem);
						printf("\n____________________\nMEMÓRIA ATUAL: \n\n");
						ImprimeMemoria(MemTotal);
						sorteio = (rand() % 10) + 1; // Verifica se deve gerar outro processo ou não.
					}
					else{
						// condição inicial
						if(MemTotal->tamanho == 512 && MemTotal->status == 0 && !MemTotal->prox && !MemTotal->ant){
							// A posição inicial da memória, cabeça da lista, agora é o 1º processo, e o nó processoMem
						   // se transformou no espaço vazio que sobrou.
							MemTotal->tamanho = processoMem->tamanho;
							MemTotal->id = processoMem->id;
							MemTotal->pos_ini = 0;
							MemTotal->status = 1; // ESPAÇO CHEIO, COM DADOS
							processoMem->ant = MemTotal; //   liga os nós
							MemTotal->prox = processoMem; // liga os nós
							processoMem->status = 0;    // ESPAÇO VAZIO
							processoMem->tamanho = 512 - MemTotal->tamanho;
							processoMem->pos_ini = MemTotal->tamanho; // pois a posição da cabeça vai de 0 a tamanho-1
							processoMem->id = -1;
							printf("____________________\nMEMÓRIA ATUAL: \n\n");
							ImprimeMemoria(MemTotal);
							// A partir de agora a memória terá 2 nós
							// cria um nó da fila de processos com memória e pcb
							PROCESSO *noProcess = CriaNoFilaProc(MemTotal);
							printf("\n\n________________________\nPROCESSO INSERIDO NA FILA DE PROCESSOS:");
							FilaProcess = InsereFilaProcessos(FilaProcess, noProcess); // insere na fila
							printf("\n\n________________________\nFILA DE PROCESSOS: \n");
							ImprimeFilaProcessos(FilaProcess, 1);
							printf("\n");
							sorteio = (rand() % 10) + 1; // Verifica se deve gerar outro processo ou não.
						}
						else{ // já haverão pelo menos 2 nós ( a cabeça que já é um processo e um nó vazio)
							// Interrupção por parte do usuário

							printf("\nInterromper geração de processos? 1- SIM ou 0- NÃO:   ");
							scanf("%d", &para);
							if(para){
								para = 0;
								printf("\nPROCESSO INTERROMPIDO NÃO FOI ALOCADO!\nDADOS DO PROCESSO GERADO: \n");
								ImprimeProcesso(processoMem);
								printf("\n____________________\nMEMÓRIA ATUAL: \n\n");
								ImprimeMemoria(MemTotal);
								break;
							}
							InsereFirstFit(processoMem);
							printf("\n");
							//sorteio = (rand() % 10) + 1; // Verifica se deve gerar outro processo ou não.
						}
					}
				}
				sorteio = (rand() % 10) + 1; // Verifica se deve gerar outro processo ou não.
				if(sorteio > 7) break;	
			}
		}
		printf("\n");
		system("pause");
		para = 1;
		printf("\n________________________\nDADA ORDEM PARA EXECUTAR PROCESSOS\n"); // ou aleatório ou interrupção
		Round_Robin(FilaBloq); // se o para for 0 saiu do roudin por escolha do usuario
		// Encerrar o programa
		printf("\nDeseja encerrar o sistema? 1- SIM ou 0- NÃO:  ");
		scanf("%d", &sair);
		if(sair) sair = 0;
		else sair = 1;
	}
}
