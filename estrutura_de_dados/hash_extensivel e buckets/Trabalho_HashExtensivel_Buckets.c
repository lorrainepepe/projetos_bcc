/* 		2º Trabalho de Laboratório de Estruturas de Dados II
 *  Grupo 3: Everton de Melo Camacho, Lorraine Maria Pepe, Gustavo Fernando Balbino, Vinicius Bernardi Vechiato
 *  Data de entrega proposta: 17/10/2017
 *  Versão Dev-C++: 5.11
 */

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <math.h>
#include <time.h>

//Estruturas Cliente ********************************************************************
typedef struct cliente {
	int conta;
	char nome[100];
	float limite;
	float saldo;
	int ordemCadastro;  //Útil na função fseek no momento de excluir um cliente
}CLIENTE;

struct auxiliarCliente { 
	int conta;
	char nome[100];
	float limite;
	float saldo;
	int ordemCadastro;
};

//Estrutura do Hash Extensível
typedef struct bucket{
	int conta[4];
	int linha[4];    //Linha indica em qual linha do arquivo estará o cliente.
	int tamBucket;  // Quantos clientes já foram cadastrados no bucket em questão
	int pl; 	   // Profundidade local
}BUCKET;

typedef struct HashExt{
	BUCKET* bucket;
	struct HashExt* prox;	
	int bin, dec; 		// Armazenar endereço de cada diretório em binário e em decimal.
}HASHEXT;

//Variáveis Globais ********************************************************************
int profundidadeGlobal = 2;
int tamDiretorio = 4;
int linhaArquivo = 0;

//								Funções
//CLIENTE ********************************************************************
CLIENTE* novoCliente(int, char*, float , float);
int cadastrarCliente(HASHEXT*, CLIENTE*);

//HASHING ********************************************************************
void CriaHashing(HASHEXT*);
void duplicarDiretorio(HASHEXT*);
void reorganizarBuckets(HASHEXT*, HASHEXT*);
	
//IMPRIME *********************************************************************
void imprimirHash(HASHEXT*);
void ImprimeUmCliente(CLIENTE*);

//SALVAR **********************************************************************
void salvarNoArquivo(FILE*, CLIENTE*);
void salvarTotalClientes(FILE*);
void CriaArquivoVisualizacao(CLIENTE*);
void geraArquivoEditado();

//CONSULTAS *******************************************************************
CLIENTE* consultarCliente(FILE*, int); //Procura um cliente pela sua conta e retorna o mesmo
CLIENTE* ConsultarClienteParaExclusao(HASHEXT *, int, FILE*); 
CLIENTE* verificarClientes(FILE*, int); 
void carregarDados(FILE*, HASHEXT*);

//PIN COUNT E DIRTY BIT *******************************************************
int PinCount();
int DirtyBit();

//CONVERTE *******************************************************
int ConvDec2Bin(int );

//REMOÇÃO
HASHEXT* removerCliente(HASHEXT*, CLIENTE*, FILE* arq);
	
int main(){
	
	srand(time(NULL));
	setlocale(LC_ALL, "Portuguese");
	
	// Arquivo do Banco de dados (banco.dat) para leitura dos dados
	FILE* arq = fopen("banco.dat", "rb");
		
	//Hashing Extensível: Lista encadeada com cabeça. O primeiro diretório estará em Hashing->prox.
	HASHEXT* Hashing = (HASHEXT*)malloc(sizeof(HASHEXT));
	Hashing->prox = NULL;
	
	//Cria Hashing
	CriaHashing(Hashing);
	
	//Variáveis de controle
	CLIENTE* Cliente = NULL;
	CLIENTE* Cli = NULL;
	int conta, op = 10, a = 0, i;
	float limite, saldo;
	char nome[100];
	
	//Carrega o banco de dados do sistema
	if(arq == NULL) {
		arq = fopen("banco.dat", "ab"); // Caso o arquivo de banco de dados não exista ele será criado
	}else {
		carregarDados(arq, Hashing);   //Carrega o banco de dados
	}
	
	fclose(arq); //Fecha o arquivo após a leitura
	
	while(op != 0){
		system("cls");
		printf("\n\t\t\t _______________Sistema de gerencia de clientes_____________\n");
		printf("\t\t\t| 1) CADASTRAR \t\t\t\t\t\t    |\n");
		printf("\t\t\t| 2) REMOVER \t\t\t\t\t\t    |\n");
		printf("\t\t\t| 3) CONSULTAR \t\t\t\t\t\t    |\n");
		printf("\t\t\t| 4) VISUALIZAR HASHING \t\t\t\t    |\n");
		printf("\t\t\t| 5) SAIR \t\t\t\t\t\t    |\n");
		printf("\t\t\t|___________________________________________________________|\n");
		
		printf("Escolha uma opção: \n");
		scanf("%d", &op);
	
		switch(op){
			case 1 : // Cadastrar um cliente
				printf("\t\t CADASTRO: \n");
				printf("Preencha os campos abaixo com os dados do cliente: \n");
				while(1){
					printf("  Conta: ");
					scanf("%d", &conta);
					Cliente = consultarCliente(arq, conta);
					if(Cliente) printf("\nA conta informada já está em uso. Digite outro valor de conta!\n");
					else break;
				}
				
				printf("  Nome: ");
				scanf("%*c%[^\n]%*c", nome);
				printf("  Limite: ");
				scanf("%f",&limite);
				printf("  Saldo: ");
				scanf("%f",&saldo);
				
				Cli = novoCliente(conta, nome, limite, saldo);
				int pass = cadastrarCliente(Hashing, Cli);		
				
				if(pass == 1){
					printf("\nA conta %d foi cadastrada com sucesso!\n", Cli->conta);
					salvarNoArquivo(arq, Cli);
					CriaArquivoVisualizacao(Cli);
				}else if (pass == 2){
					printf("\n\n ****** OVERFLOW ***************** OVERFLOW ************ OVERFLOW ***********\n\n");
				}
				system("pause");	
				break;
			
			case 2: // Remover um cliente
				i = 0;
				printf("\t\t REMOÇÃO DE CLIENTE: \n");
				
				while(1){
					printf("Insira a conta do cliente a ser removido: \n");
					printf("  Conta: ");
					scanf("%d", &conta);
					Cliente = consultarCliente(arq, conta);
					
					if(Cliente != NULL){
						printf("\nO cliente de conta %d foi encontrado com sucesso: \n\n", Cliente->conta);
						ImprimeUmCliente(Cliente);
						printf("\nRemovendo...\n");
						break;
					} else{
						puts("\nO cliente não foi encontrado!\n");
						printf("Sair? (1) Sim (0) Não\n");
						scanf("%d", &i);
						if(i == 1) break;
					}
				}
				while(1 && i != 1){
					int pin = PinCount();
					int dirty = DirtyBit();
					if(pin && dirty){
						printf("\n\nLiberada a remoção!\n");
						ConsultarClienteParaExclusao(Hashing, conta, arq);
						Hashing = removerCliente(Hashing, Cliente, arq);
						printf("\nRemoção realizada com sucesso!\n\n");
						break;
					}
					else if(!pin || !dirty){
						printf("\n\nRemoção NÃO liberada!\n");
					}
				}
				system("pause");
				break;
			
			case 3: // Consultar um cliente
				i = 0;
				printf("\t\t CONSULTA DE CLIENTE: \n");
				while(1){
					printf("Insira a conta do cliente a ser consultado: \n");
					printf("  Conta: ");
					scanf("%d",&conta);
					Cliente = consultarCliente(arq, conta);
					if(Cliente != NULL){
						printf("\nO cliente de conta %d foi encontrado com sucesso: \n", Cliente->conta);
						printf("\nVerificando sistema...\n");
						break;
					}
					else{
						puts("\nO cliente não foi encontrado!");
						printf("Sair? (1) Sim (0) Não\n");
						scanf("%d", &i);
						if(i == 1) break;
					}
				}
				while(1 && i != 1){
					int pin = PinCount();
					int dirty = DirtyBit();
					if(pin && dirty){
						printf("\nLiberada a consulta!\n");
						ImprimeUmCliente(Cliente);	
						break;
					}
					else if(!pin || !dirty){
						printf("\nConsulta NÃO liberada!\n\n");
					}
				}
				system("pause");	
				break;
			
			case 4: // Imprimir os clientes cadastrados
				puts("\n"); 
				imprimirHash(Hashing);
				system("pause");
				break;
			case 5: // Sair do sistema
				op = 0;
				system("pause");
				break; 
			default:{
				printf("\nOpção inválida!\n");
				system("pause");
				break;
			}
		}
	}
	printf("\n\tSistema Encerrado!\n");
}					

//Funções

//CONVERTE *******************************************************
int ConvDec2Bin(int decimal){
	char binario[12];
	itoa(decimal,binario,2);
	
	return atoi(binario);
}
	
//PIN COUNT E DIRTY BIT *******************************************************
int PinCount(){
	//srand(time(NULL));
	int i =  (rand() % 10) + 1;
	if(i <= 4){
		printf("\n\tPin Count:  0");
		 return 1; // pessoas acessando
	}
	else{
		i =  (rand() % 10) + 1;
		printf("\n\tPin Count:  %d",i);
	 	return 0; // ninguém acessando a pagina
	}
}

int DirtyBit(){
	//srand(time(NULL));
	int i =  (rand() % 10) + 1;
	if(i <= 4){
		printf("\n\tDirty Bit:  0");
		return 1; // bloqueado
	}
	else{
		printf("\n\tDirty Bit:  1");
	 	return 0; // não bloqueado
	}
}

//CLIENTE ********************************************************************
CLIENTE* novoCliente(int conta, char nome[], float limite, float saldo){
	
	CLIENTE* cli = (CLIENTE*) malloc(sizeof (CLIENTE));
	
	cli->conta = conta;
	strcpy(cli->nome, nome);
	cli->limite = limite;
	cli->saldo = saldo;
	cli->ordemCadastro = linhaArquivo;

	return cli;
}			

int cadastrarCliente(HASHEXT* Hashing, CLIENTE* cliente){

    int mod, k, duplicou = 0, controle = 0, atencao = 0;
	HASHEXT* aux = Hashing->prox; 
	
	mod = ConvDec2Bin(cliente->conta % tamDiretorio); // Função hash: usa o módulo com base no tamanho do diretório para encontrar o devido local de inserção

	while(aux != NULL){
		if(mod == aux->bin) break;
		else aux = aux->prox;
	}

	while(1){
		//Cadastra se achar um espaço com linha -1 no devido bucket
		for(k = 0; k < 4; k++){
			if((mod == aux->bin) && (aux->bucket->linha[k] == -1)){ //Espaço vazio = -1;
				aux->bucket->conta[k] = cliente->conta;
				aux->bucket->linha[k] = linhaArquivo;
				linhaArquivo++;
				aux->bucket->tamBucket += 1;
				return 1; //Cadastrou !
			}
		}
		//if (atencao == 1) return 0;		
		if(aux->bucket->pl < profundidadeGlobal && aux->bucket->tamBucket == 4){ //Quando precisar dividir o bucket sem duplicar o diretório
			reorganizarBuckets(Hashing, aux);
			//controle = 1;				
		}else if(aux->bucket->pl == profundidadeGlobal && profundidadeGlobal < 4 && aux->bucket->tamBucket == 4){
			duplicarDiretorio(Hashing);
			reorganizarBuckets(Hashing, aux); 
			duplicou = 1;
		}else if(aux->bucket->pl == 4 && aux->bucket->tamBucket){ // Overflow
			return 2;
		}			
			
		//Calcula o mod considerando o tamanho do novo diretório após duplicar
		if (duplicou == 1){
			mod = ConvDec2Bin(cliente->conta % tamDiretorio);
			aux = Hashing->prox;
					
			while(aux != NULL){ //Encontra o diretório correto
				if (aux->bin == mod) break;
				else aux = aux->prox;
			}
		}
		//atencao = 1;	
	}
}

//HASHING ********************************************************************
void duplicarDiretorio(HASHEXT* Hashing){
	int a, i, k;
	
	profundidadeGlobal += 1; //Incrementa a profundidade global
	
	//Novo tamanho do diretório
	tamDiretorio = pow(2, profundidadeGlobal);
	
	//Duplicar o diretório
	for(i = tamDiretorio/2; i < tamDiretorio ; i++){ 
		HASHEXT* aux = Hashing->prox;
		k = i;
		HASHEXT* novoDiretorio = (HASHEXT*) malloc(sizeof(HASHEXT));
		BUCKET* bucket = (BUCKET*)malloc(sizeof(BUCKET));
		
		//Inicia o bucket com 4 espaços 
		for(a = 0; a < 4; a++){
			bucket->conta[a] = -1; //-1 indica que apenas iniciou o bucket e não inseriu nenhum cliente ainda
			bucket->linha[a] = -1;
		}
		bucket->tamBucket = 0;
		novoDiretorio->bucket = bucket;
	
		//Refaz os apontamentos para os devidos buckets
		for(; k-3 != 1; k--){
			aux = aux->prox;
		}
		novoDiretorio->bucket = aux->bucket;
		
		//Faz a inicialização de um novo diretório
		novoDiretorio->prox = NULL;	
		novoDiretorio->bin = ConvDec2Bin(i);
		novoDiretorio->dec = i;
		
		aux = Hashing->prox;
		//Insere no final, apenas para ligar a lista de diretórios (da vertical)
		while(aux->prox != NULL){
			aux = aux->prox;
		}
			aux->prox = novoDiretorio;
	}
}

void reorganizarBuckets(HASHEXT* Hashing, HASHEXT* bucketCheio){
	
	HASHEXT* aux = Hashing->prox;
	HASHEXT* tmp = Hashing->prox;
	
	int a, bin, mod, pos, b, k = 0, trocou = 0;
	
	//Achar o primeiro diretório que aponta para o bucket cheio
	while(tmp != NULL){
		if(tmp->bucket == bucketCheio->bucket) break;
		else tmp = tmp->prox;
	}	
	
	pos = pow(2, (profundidadeGlobal-1));
	bin = ConvDec2Bin(tmp->dec + pos);
	
	//Alocar um novo bucket
	BUCKET* novoBucket = (BUCKET*)malloc(sizeof(BUCKET)); 
	
	for(a = 0; a < 4; a++){
		novoBucket->conta[a] = -1;
		novoBucket->linha[a] = -1;
	}
	novoBucket->tamBucket = 0;
	
	//Redistribuir as contas entre o bucket cheio e o novo bucket
	for(a = 0; a < 4; a++){
		if(bucketCheio->bucket->linha[a] != -1){
			mod = ConvDec2Bin(bucketCheio->bucket->conta[a] % tamDiretorio);
		} 	
		for(b = 0; b < 4; b++){
			if(bin == mod && novoBucket->linha[b] == -1){
				novoBucket->conta[b] = bucketCheio->bucket->conta[a];
				novoBucket->linha[b] = bucketCheio->bucket->linha[a];
				novoBucket->tamBucket += 1;
				bucketCheio->bucket->conta[a] = -1;	
				bucketCheio->bucket->linha[a] = -1;	
				bucketCheio->bucket->tamBucket -= 1;
				trocou = 1;
				break;
			}
		}
	}	
	
	aux = Hashing->prox;
	//Encontra o diretório que deve apontar para o novo bucket
	while(aux != NULL){
		if(aux->bin == bin) break;
		else aux = aux->prox;	
	}	

	if(trocou == 0){ //Se não conseguiu redistribuir os elementos  
		bucketCheio->bucket->pl +=1;
		novoBucket->pl = bucketCheio->bucket->pl;
		aux->bucket = novoBucket;
		return;
	}
	
	//Incrementa a profundidade local do bucket que está cheio
	bucketCheio->bucket->pl +=1;
	
	//A profundidade local do bucket criado é igual a pl do bucket cheio
    novoBucket->pl = bucketCheio->bucket->pl;
	
	aux->bucket = novoBucket;
}

void CriaHashing(HASHEXT* Hashing){
	int a, b;
	
	//Inicialmente a tabela hash terá 4 diretórios, onde cada um apontará para um bucket com 4 espaços
	for(a = 0; a < 4; a++){ 
		
		HASHEXT* novoDiretorio = (HASHEXT*) malloc(sizeof(HASHEXT));
		BUCKET* bucket = (BUCKET*)malloc(sizeof(BUCKET));
		
		//Faz a inicialização de um novo diretório
		novoDiretorio->prox = NULL;	
		novoDiretorio->bin = ConvDec2Bin(a); //Útil para mostrar os endereços dos diretórios em binário, no momento de Imprimir
		novoDiretorio->dec = a;
		
		//Inicia um bucket com 4 espaços
		for(b = 0; b < 4; b++){
			bucket->conta[b] = -1; //-1 indica que iniciou o bucket e não inseriu nenhum cliente ainda
			bucket->linha[b] = -1;
		}
		bucket->tamBucket = 0;
		bucket->pl = 2;
		
		novoDiretorio->bucket = bucket;
	
		if(Hashing->prox == NULL){
			Hashing->prox = novoDiretorio;
		}else{
			HASHEXT* aux = Hashing->prox;
			//Insere no final, apenas para ligar a lista de diretórios (da vertical)
			while(aux->prox != NULL){
				aux = aux->prox;
			}
			aux->prox = novoDiretorio;
		}
	}	
}

//IMPRIME ********************************************************************
void imprimirHash(HASHEXT* Hashing){
	HASHEXT* aux = Hashing->prox;
	int i;
	printf("\n\nPROFUNDIDADE GLOBAL: %d\n", profundidadeGlobal);
	while(aux != NULL){
		printf("----------------------------------------------------------------\n");
		if(profundidadeGlobal == 2) printf("Bucket: %.2d \n", aux->bin);
		else if(profundidadeGlobal == 3) printf("Bucket: %.3d \n", aux->bin);
		else if(profundidadeGlobal == 4) printf("Bucket: %.4d \n", aux->bin);
		
		for(i = 0; i < 4 ; i++){
			if(aux->bucket->linha[i] == -1)
				printf("\t\t\t\t >>> Conta: - | Linha: - \n");
			else
				printf("\t\t\t\t >>> Conta: %d | Linha: %d \n", aux->bucket->conta[i], aux->bucket->linha[i]);
		}
			printf("Profundidade local: %d \n", aux->bucket->pl);
		aux = aux->prox;
	}
	printf("----------------------------------------------------------------\n");
}

void ImprimeUmCliente(CLIENTE* cliente){
	printf("\nNumero da Conta: %d",cliente->conta);
	printf("\nNome: %s",cliente->nome);
	printf("\nLimite: %.2f",cliente->limite);
	printf("\nSaldo: %.2f\n",cliente->saldo);
}

//SALVAR ********************************************************************
void salvarNoArquivo(FILE* arq, CLIENTE* cli){

	struct auxiliarCliente cliente;
	
	cliente.conta = cli->conta;
	cliente.limite = cli->limite;
	cliente.saldo = cli->saldo;
	cliente.ordemCadastro = cli->ordemCadastro;
	
	strcpy(cliente.nome, cli->nome);
	
	arq = fopen("banco.dat", "ab");
	
	if(arq == NULL){
		printf("Erro na leitura do banco de dados!");
		system("pause");
		exit(1);
	}
	
	fwrite(&cliente, sizeof(struct auxiliarCliente), 1, arq);
	fclose (arq);
}

void CriaArquivoVisualizacao(CLIENTE* cli){
	
	FILE* arqVisualizacao = fopen("sgbd.txt", "r");

	if(arqVisualizacao == NULL){
		arqVisualizacao = fopen("sgbd.txt", "w");
	}else arqVisualizacao = fopen("sgbd.txt", "a");
	
	fprintf(arqVisualizacao, "Linha: %d|\t Conta: %d\t Nome: %s\t Limite: %.2f\t\t Saldo: %.2f\t \n", cli->ordemCadastro, cli->conta, cli->nome, cli->limite, cli->saldo);
	fclose(arqVisualizacao);

	fclose(arqVisualizacao);
}

void geraArquivoEditado(){
	
	struct auxiliarCliente cliente;
	
	FILE* arq = fopen("banco.dat", "rb");
	FILE* arq2 = fopen("sgbd.txt", "w");
	
	if(arq == NULL){
		printf("Erro na leitura do banco de dados!");
	}else{
		while(fread(&cliente, sizeof(struct auxiliarCliente), 1, arq) == 1){
			if(cliente.conta == -1){
				fprintf(arq2, "Linha: %d|\t *********************************** REMOVIDO *********************************** \n",cliente.ordemCadastro);
			}else{
				fprintf(arq2, "Linha: %d|\t Conta: %d\t Nome: %s\t Limite: %.2f\t\t Saldo: %.2f\t \n", cliente.ordemCadastro, cliente.conta, cliente.nome, cliente.limite, cliente.saldo);
			}
		}
	}
	fclose(arq);
	fclose(arq2);
}

//CONSULTAS ********************************************************************
CLIENTE* consultarCliente(FILE* arq, int conta){
	
	CLIENTE* cli = (CLIENTE*) malloc(sizeof (CLIENTE));
	struct auxiliarCliente cliente;
	
	arq = fopen("banco.dat", "rb");
	
	if(arq == NULL){
		printf("Erro na leitura do banco de dados!");
	}else{
		while(fread(&cliente, sizeof(struct auxiliarCliente), 1, arq) == 1){
			if(cliente.conta == conta){
				cli->conta = cliente.conta;
				cli->limite = cliente.limite;
				strcpy(cli->nome, cliente.nome);
				cli->saldo = cliente.saldo;
				cli->ordemCadastro = cliente.ordemCadastro;
				fclose(arq);
				return cli;
			}
		}
		return NULL;
	}
	
}

//CONSULTAS ********************************************************************
CLIENTE* verificarClientes(FILE* arq, int conta){
	
	CLIENTE* cli = (CLIENTE*) malloc(sizeof (CLIENTE));
	struct auxiliarCliente cliente;
	
	arq = fopen("banco.dat", "rb");
	
	if(arq == NULL){
		printf("Erro na leitura do banco de dados!");
	}else{
		while(fread(&cliente, sizeof(struct auxiliarCliente), 1, arq) == 1){
			if(cliente.conta == conta){
				cli->conta = cliente.conta;
				cli->limite = cliente.limite;
				strcpy(cli->nome, cliente.nome);
				cli->saldo = cliente.saldo;
				cli->ordemCadastro = cliente.ordemCadastro;
			}else if(cliente.conta == -1){
				cli->conta = cliente.conta;
				cli->limite = cliente.limite;
				strcpy(cli->nome, cliente.nome);
				cli->saldo = cliente.saldo;
				cli->ordemCadastro = cliente.ordemCadastro;
			}
		}
				fclose(arq);
		return cli;
	}
	
}

CLIENTE* ConsultarClienteParaExclusao(HASHEXT *Hashing, int conta, FILE* arq){
	HASHEXT* aux = Hashing->prox;
	CLIENTE* cliente = NULL;
	
	int i = 0;
	int contaBin = ConvDec2Bin(conta);
	int pos = contaBin%tamDiretorio;
	
	//Percorre o diretorio até achar a posição
	while(aux->dec != pos){
		aux = aux->prox;
	}
	
	for(i = 0; i < 4; i++){
		if(aux->bucket->conta[i] == conta){
			cliente = consultarCliente(arq, aux->bucket->conta[i]);
			aux->bucket->conta[i] = -1; 
			aux->bucket->linha[i] = -1;
			return cliente;
		}
	}
	
	return NULL;
}

void carregarDados(FILE* arq, HASHEXT* Hashing){
	
	CLIENTE* cli = (CLIENTE*)malloc(sizeof(CLIENTE));
		
	struct auxiliarCliente cliente;
	
	arq = fopen("banco.dat", "rb");
	
	while(fread(&cliente,sizeof(struct cliente),1,arq) == 1){
			if(cliente.conta != -1){ //Condição para ignorar a leitura de um cliente excluido, que possui atributo = -2.
				cli->conta = cliente.conta;
				cli->limite = cliente.limite;
				strcpy(cli->nome, cliente.nome);
				cli->saldo = cliente.saldo;
				cli->ordemCadastro = cliente.ordemCadastro;
				cadastrarCliente(Hashing, cli);
			}else{ //Incrementa a linha do arquivo para não perder o controle no uso da fseek
				linhaArquivo++;
			}
	}
	fclose (arq);
}

HASHEXT* removerCliente(HASHEXT* Hashing, CLIENTE* cli, FILE* arq){

	CLIENTE* cliente = (CLIENTE*)malloc(sizeof(CLIENTE));

	int ordemCadastro = cli->ordemCadastro;
	struct auxiliarCliente remover, teste;
	int i, linha = 0, conta, k ;
	FILE *arqVisualizar = NULL;
			
	//Localiza a posição do cliente no banco
	arq = fopen("banco.dat", "rb+");
	
	fseek(arq, ordemCadastro*sizeof(struct cliente), SEEK_CUR);
	fread(&remover, sizeof(struct cliente), 1, arq);
	
	//Reseta os atributos	
	remover.conta = -1;
	remover.limite = -1;
	strcpy(remover.nome, "REMOVIDO");
	remover.ordemCadastro = cli->ordemCadastro; //manter o controle de clientes
	remover.saldo = -1;
	
	fseek (arq, ordemCadastro*sizeof(struct cliente), SEEK_SET);
	fwrite(&remover, sizeof(struct cliente),1, arq); //Consolida a exclusão no banco.dat
	
	fclose(arq);

	/////// ATUALIZA O ARQUIVO sgbd.txt PARA MOSTRAR O CLIENTE REMOVIDO //////////////
	geraArquivoEditado();
	
	profundidadeGlobal = 2;
	tamDiretorio = 4;
	linhaArquivo = 0;	
	
	free(Hashing);
	HASHEXT* aux = (HASHEXT*)malloc(sizeof(HASHEXT));
	aux->prox = NULL;
	
	CriaHashing(aux);
	carregarDados(arq, aux);	
	return aux;
}
