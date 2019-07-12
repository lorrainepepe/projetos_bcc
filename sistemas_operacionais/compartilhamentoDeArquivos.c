/*		Trabalho de Sistemas Operacionais
 *
 * Alunos: Lorraine M. Pepe e Everton Camacho
 * Vers�o 1.0  - 13/06/2018
 * Sistema de Compartilhamento de Arquivos
 *
 */


#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <dirent.h>
#include <windows.h>

/*********************************************************************************************************************************************************************
	Funcionamento do sistema.:
	
	A princ�pio h� dois usu�rios cadastrados no sistema, o que j� possibilita a simula��o do compartilhamento de arquivos por 
	meio das seguintes funcionalidades do sistema:
	<3> Compartilhar arquivo, <4> Compartilhar pasta, <5> Criar Arquivo, <6> Copiar Arquivo, <7> Editar Arquivo, <8> Deletar Arquivo, <9> Visualizar arquivo.
	
	Tamb�m � poss�vel realizar o cadastro de novos usu�rios, a partir da cria��o de diret�rios para representar os mesmos. 
	Assim, todas as funcionalidades de compartilhamento citadas anteriormente funcionam da mesma maneira para qualquer n�mero de usu�rios.
	
	Espefica��es das funcionalidades:
	
	<1> Exibir usu�rios: Exibe o diret�rio dos dois usu�rios j� cadastrados no sistema;
	
	<2> Cadastrar usu�rios: Possibilita a cria��o de novos diret�rios para representar um novo usu�rio no sistema. Tamb�m cria um arquivo .txt padr�o
	para cada novo usu�rio cadastrado; 
	
	<3> Compartilhar arquivo: Realiza o compartilhamento de um arquivo espec�fico entre dois ou mais usu�rios, representados pelo caminho dos seus diret�rios;
	
	<4> Compartilhar pasta: Realiza o comportilhamento de todos os arquivos de um diret�rio para determinado usu�rio.
	
	<5> Criar Arquivo: Apenas o propriet�rio do seu diret�rio pode criar um arquivo;
	
	<6> Copiar Arquivo: Determinado usu�rio pode copiar para o seu diret�rio um arquivo que est� sendo compartilhado com ele;
	
	<7> Editar Arquivo: Determinado usu�rio pode editar qualquer arquivo do seu diret�rio, incluindo arquivos que foram copiados por meio
	do compartilhamento;
	
	<8> Deletar Arquivo: Apenas um usu�rio propriet�rio pode deletar determinado arquivo do seu diret�rio;
	
	<9> Visualizar arquivo: Determinado usu�rio pode visualizar tanto um arquivo do seu pr�prio diret�rio como tamb�m os arquivos
	compartilhados (link);
	
	<0> Sair do programa: Finaliza o sistema.
**********************************************************************************************************************************************************************/

void menu_principal(){
	
	printf("**************************************************************************************************\n");
	printf("\t\t\tSistema de simula��o de arquivos compartilhados\n");
	printf("**************************************************************************************************\n");
	printf("\t\t\t   |--------------------------------------|\n");
	printf("\t\t\t   |                MENU                  |\n");
	printf("\t\t\t   |--------------------------------------|\n");
	printf("\t\t\t   |--------------------------------------|\n");
	printf("\t\t\t   | <1> Exibir usu�rios                  |\n");
	printf("\t\t\t   | <2> Cadastrar usu�rios               |\n");
	printf("\t\t\t   | <3> Compartilhar arquivo             |\n");
	printf("\t\t\t   | <4> Compartilhar pasta               |\n");
	printf("\t\t\t   | <5> Criar Arquivo                    |\n");
	printf("\t\t\t   | <6> Copiar Arquivo                   |\n");
	printf("\t\t\t   | <7> Editar Arquivo                   |\n");
	printf("\t\t\t   | <8> Deletar Arquivo                  |\n");
	printf("\t\t\t   | <9> Visualizar arquivo               |\n");
	printf("\t\t\t   |--------------------------------------|\n");
	printf("\t\t\t   | <0> Sair do programa                 |\n");
	printf("\t\t\t   |--------------------------------------|\n");
	printf("**************************************************************************************************\n");

}

/* Exibe os dois usu�rios j� cadastrados no sistema */
void usuariosSistema(){
	
	puts("\nUsu�rios cadastrados.: \n");
	puts("\t\t\tUsu�rio A -> Diret�rio: C:\\USUARIO_A\\");
	puts("\t\t\tUsu�rio B -> Diret�rio: C:\\USUARIO_B\\");
	
}

void removeSubstring(char *s,const char *toremove){
  while( s=strstr(s,toremove) )
    memmove(s,s+strlen(toremove),1+strlen(s+strlen(toremove)));
}

/* Exibe arquivos de um diret�rio */
void exibeArquivosDiretorio(char caminhoDiretorio[]){
	
	DIR *dir;
    struct dirent *lsdir;
    int k;

    dir = opendir(caminhoDiretorio);

    while ( ( lsdir = readdir(dir) ) != NULL ){
    	
		/* Exibe os arquivos que n�o s�o compartilhados  */
    	k = strstr(lsdir->d_name, "_LINK");
		if(k == 0 && strcmp(lsdir->d_name, ".") != 0 && strcmp(lsdir->d_name, "..") != 0){
        	printf ("%s\n", lsdir->d_name);
    	}
    }
    closedir(dir);
}

/* Exibe os arquivos compartilhados (link) */
void exibeArquivosCompartilhados(char caminhoDiretorio[]){
	
	DIR *dir;
    struct dirent *lsdir;
    int k;

    dir = opendir(caminhoDiretorio);

    while ((lsdir = readdir(dir) ) != NULL ){
    	
    	/* Verifica se � do tipo link para exibir */
    	k = strstr(lsdir->d_name, "_LINK");
    	if(k != 0){
        	printf ("%s\n", lsdir->d_name);
    	}
    }
    closedir(dir);
}


/* Acrescentar outra barra invertida no caminho quando esta aparecer */
char* TratarCaminhoArq(char*caminho){
	
	//Contadores;
	int i, j, k;
	//Armazena a posi��o da barra localizada
	int posBarra;
	
	//Analisa onde est�o as '\' do caminho passado para acrescentar outra e poder ser utilizada em posteriores fun��es de arquivo.
	for(i=0; i < strlen(caminho); i++){
		
		if(caminho[i] == 92){ // 92 = barra invertida na tabela ASCII
			char* caminAux = NULL;
			caminAux = (char*)malloc(500*sizeof(char)); 
			posBarra = i;
			
			for(j=i+1, k=0; j < strlen(caminho); j++, k++){
				caminAux[k] = caminho[j];
			}
			
			caminAux[k] = 00; // null
			caminho[posBarra+1] = 92; // '\'
			caminho[posBarra+2]= 00; //null
			
			strcat(caminho, caminAux);
			caminAux = NULL;
			free(caminAux);
			
			i = posBarra+2;
		}
	}
	
	return caminho;	
}

//Verifica se o caminho passado � um arquivo .txt
int IsFileTXT(char* caminho){
	if(caminho[(strlen(caminho)-4)] != '.') return 0;
	if(caminho[(strlen(caminho)-3)] != 't') return 0;
	if(caminho[(strlen(caminho)-2)] != 'x') return 0;
	if(caminho[(strlen(caminho)-1)] != 't') return 0;
	
	if((caminho[0] < 65 || caminho[0] > 90)) return 0;
	if(caminho[1] != ':') return 0;
	return 1;
}


int IsPasta(char* caminho){
	if(caminho[(strlen(caminho)-4)] != '.'){
		if((caminho[0] < 65 || caminho[0] > 90)) return 0;
			if(caminho[1] != ':') return 0;
				return 1;
	} 
	else return 0;
}


char* RecebeCaminho(char* caminho, int i){
	
	char* caminhoArqTratado = (char*)malloc(500*sizeof(char));  // Desalocar ao fim  *************** CORRE��ES
	while(1){
		scanf("%s",caminho); 
		
		strcpy(caminhoArqTratado,TratarCaminhoArq(caminho));
	    if(i == 1){	    
		    if(IsFileTXT(caminho)){
		    	return caminhoArqTratado;
			}
			else printf("\nCaminho inv�lido, n�o � um arquivo TXT! Tente novamente:\n> "); 
		}
		else if(i==0){
			if(IsPasta(caminho)){
		    	return caminhoArqTratado;
			}
			else printf("\nCaminho inv�lido, n�o � um arquivo TXT! Tente novamente:\n> "); 
		}
	}
}

void cadastraUsuario(){

	char* caminhoDiretorio = (char*)malloc(100*sizeof(char)); 
	char nomeArquivo[100];
	FILE *arquivo = NULL;
	
	printf("\nInforme o nome do diret�rio para o novo usu�rio - Ex: C:\\USUARIO_K\\ \n");
	caminhoDiretorio = RecebeCaminho(caminhoDiretorio, 0);
	
	/* Cria o diret�rio para o novo usu�rio */
	CreateDirectory (caminhoDiretorio, NULL);
	
	printf("\nO diret�rio foi criado com sucesso!\n\n");
	
	printf("A seguir, crie algum arquivo neste diret�rio para simular o compartilhamento posteriormente!\n\n");
		
	fflush(stdin);
	printf("Informe o nome do arquivo que deseja criar (sem o .txt):\n");
	scanf("%[^\n]", nomeArquivo);
	
	strcat(nomeArquivo, ".txt");
	strcat(caminhoDiretorio, nomeArquivo);
		
	arquivo = fopen(caminhoDiretorio, "w");
	
	if(arquivo == NULL){
		printf("\nN�o foi poss�vel criar o arquivo!\n");
	}else{
		fprintf(arquivo, "Quaisquer informa��es...");
		printf("\nArquivo criado com sucesso!\n");
	}
	fclose(arquivo);
}

/* Simula o compartilhamento. V�lido apenas para os usu�rios j� cadastrados, com os seus devidos diret�rios. */
void compartilhaArquivo(){
	
	char* caminhoDiretorio = (char*)malloc(100*sizeof(char));
	char* caminhoDestino = (char*)malloc(100*sizeof(char)); 
	char nomeArquivo[100];
	FILE* arq = NULL;

	printf("\nInforme o diret�rio do usu�rio que deseja acessar:\n");
	caminhoDiretorio = RecebeCaminho(caminhoDiretorio, 0);
		
	/* Exibe os arquivos */
	printf("\nArquivos deste diret�rio:\n\n");
	exibeArquivosDiretorio(caminhoDiretorio);
	
	printf("\nInforme o nome do arquivo (sem o .txt) a ser compartilhado: \n");
	fflush(stdin);
	scanf("%[^\n]", nomeArquivo);
	
	strcat(caminhoDiretorio, nomeArquivo); /* Caminho diret�rio j� tem o caminho completo do arquivo .txt*/
	strcat(caminhoDiretorio, ".txt");
			
	printf("\nInsira o caminho da pasta ao qual deseja compartilhar o arquivo: \n");
	caminhoDestino = RecebeCaminho(caminhoDestino, 0);
	
	/* Cria um arquivo link na pasta destino */
	strcat(nomeArquivo, "_LINK.txt"); 
	strcat(caminhoDestino, nomeArquivo); 
	
	arq = fopen(caminhoDestino, "w");
	if(arq == NULL){
		printf("\nN�o foi poss�vel criar o arquivo!");
	}else{
		fprintf(arq, caminhoDiretorio);
	}
	fclose(arq);
	
	printf("\nCompartilhamento de arquivo realizado com sucesso!\n\n");
}

/* Simula o compartilhamento de todos os arquivos de um diret�rio com determinado usu�rio */
void compartilhaDiretorio(){
	
	char* caminhoDiretorio = (char*)malloc(100*sizeof(char));
	char* caminhoDestino = (char*)malloc(100*sizeof(char)); 
	char caminhoDiretorioCopia[100], caminhoDestinoCopia[100], nomeArquivo[100];
	FILE* arq = NULL;
	DIR *dir;
    struct dirent *lsdir;
    int k;
	
	printf("\nInforme o diret�rio que deseja compartilhar:\n");
	caminhoDiretorio = RecebeCaminho(caminhoDiretorio, 0);
	
	printf("\nInsira o caminho da pasta do usu�rio ao qual deseja compartilhar este diret�rio:\n");
	caminhoDestino = RecebeCaminho(caminhoDestino, 0);
	
    dir = opendir(caminhoDiretorio);
    

    while ((lsdir = readdir(dir)) != NULL ){
    	strcpy(caminhoDestinoCopia, caminhoDestino);
    	strcpy(caminhoDiretorioCopia, caminhoDiretorio);
		
    	k = strstr(lsdir->d_name, "_LINK");
    	if(k == 0 && strcmp(lsdir->d_name, ".") != 0 && strcmp(lsdir->d_name, "..") != 0){
    		removeSubstring(lsdir->d_name, ".txt");
    		strcpy(nomeArquivo, lsdir->d_name);
			
			/* Cria um arquivo link na pasta destino */
			strcat(nomeArquivo, "_LINK.txt"); 
			strcat(caminhoDestinoCopia, nomeArquivo); 
			strcat(caminhoDiretorioCopia, nomeArquivo); /* Caminho diret�rio j� tem o caminho completo do arquivo .txt*/
			
			arq = fopen(caminhoDestinoCopia, "w");
			if(arq == NULL){
				printf("\nN�o foi poss�vel criar o arquivo!");
			}else{
				strcat(caminhoDestinoCopia, ".txt");
				fprintf(arq, caminhoDiretorioCopia);
			}
			fclose(arq);
			printf("\nCompartilhamento do arquivo %s realizado com sucesso!\n\n", lsdir->d_name);
    	}
    }
    closedir(dir);	
}

/* Cria arquivo: para o propriet�rio do diret�rio */
void criaArquivo(){
	
	char* caminhoDiretorio = (char*)malloc(100*sizeof(char));
	char nomeArquivo[100];
	FILE* arq = NULL;
	
	printf("\nInforme o diret�rio do usu�rio que deseja acessar:\n");
	caminhoDiretorio = RecebeCaminho(caminhoDiretorio, 0);	
		
	/* Exibe os arquivos */
	printf("\nArquivos deste diret�rio:\n\n");
	exibeArquivosDiretorio(caminhoDiretorio);
	
	printf("\nInforme o nome do arquivo que deseja criar (sem o .txt):\n");
	fflush(stdin);
	scanf("%[^\n]", nomeArquivo);
	
	strcat(nomeArquivo, ".txt");
	strcat(caminhoDiretorio, nomeArquivo);
	
	arq = fopen(caminhoDiretorio, "w");
	if(arq == NULL){
		printf("\nN�o foi poss�vel criar o arquivo!\n");
	}else{
		fprintf(arq, "Quaisquer informa��es...");
	}
	fclose(arq);
	
	printf("\nArquivo criado com  sucesso!\n\n");
}


/* Exibe os arquivos ("link") que um usu�rio tem compartilhado com outro, e possibilta que ele copie este arquivo para o seu diret�rio */
void copiaArquivo(){
	
	char* caminhoDiretorio = (char*)malloc(100*sizeof(char));
	char nomeArquivo[100], caminhoCompartilhado[100], caminhoDiretorioCopia[100];
	char k;
	FILE *arq1, *arq2;
	FILE* arq = NULL;
	
	printf("\nInforme o diret�rio do usu�rio que deseja acessar:\n");
	caminhoDiretorio = RecebeCaminho(caminhoDiretorio, 0);
	strcat(caminhoDiretorioCopia, caminhoDiretorio);
	
	printf("\nArquivos compartilhados neste diret�rio:\n\n");
	exibeArquivosCompartilhados(caminhoDiretorio);
	
	printf("\nInforme o nome do arquivo compartilhado (sem o .txt) que deseja copiar para este diret�rio:\n");
	fflush(stdin);
	scanf("%[^\n]", nomeArquivo);
	

	strcat(caminhoDiretorio, nomeArquivo);
	strcat(caminhoDiretorio, ".txt");
	
	arq = fopen(caminhoDiretorio, "r");
	if(arq == NULL)
			printf("\nN�o foi poss�vel acessar o arquivo!\n");
	else{
		fscanf(arq,"%s\n", caminhoCompartilhado);
	}
	fclose(arq);
	
	removeSubstring(nomeArquivo,"_LINK");
	strcat(caminhoDiretorioCopia, nomeArquivo);
	strcat(caminhoDiretorioCopia, ".txt");
	 
    arq1 = fopen(caminhoCompartilhado, "r");
    arq2 = fopen(caminhoDiretorioCopia, "w");
 
	while (1) {
	k = fgetc(arq1);
 
	if (k == EOF)
		break;
	else
		putc(k, arq2);
   }
   
   fclose(arq1);
   fclose(arq2);
 
   printf("\nO arquivo compartilhado foi copiado com sucesso!\n\n");
}

/* Permite que um usu�rio edite um arquivo do seu diret�rio. */
void editaArquivo(){
	
	char* caminhoDiretorio = (char*)malloc(100*sizeof(char));
	char nomeArquivo[100], infoEditar[500];
	FILE* arq = NULL;
	
	printf("\nInforme o diret�rio do usu�rio que deseja acessar:\n");
	caminhoDiretorio = RecebeCaminho(caminhoDiretorio, 0);
	
	printf("\nArquivos compartilhados neste diret�rio:\n\n");
	exibeArquivosDiretorio(caminhoDiretorio);
	
	printf("\nInforme o nome do arquivo (sem o .txt) que deseja editar:\n");
	fflush(stdin);
	scanf("%[^\n]", nomeArquivo);
	
	printf("\nInforme as novas informa��es para adicionar no arquivo:\n");
	fflush(stdin);
	scanf("%[^\n]", infoEditar);
	
	strcat(nomeArquivo, ".txt");
	strcat(caminhoDiretorio, nomeArquivo);
	
	arq = fopen(caminhoDiretorio, "a");
	if(arq == NULL)
			printf("\nN�o foi poss�vel acessar o arquivo!\n");
	else{
		fprintf(arq, "\n%s", infoEditar);
	}
	fclose(arq);
	
	printf("\nArquivo editado com sucesso!\n");
}

/* Permite que determinado usu�rio delete um arquivo do seu diret�rio */
void deletaArquivo(){
	
	char* caminhoDiretorio = (char*)malloc(100*sizeof(char));
	char nomeArquivo[100];
	int k;
	
	printf("\nInforme o diret�rio do usu�rio que deseja acessar:\n");
	caminhoDiretorio = RecebeCaminho(caminhoDiretorio, 0);
	
	printf("\nArquivos compartilhados neste diret�rio:\n\n");
	exibeArquivosDiretorio(caminhoDiretorio);
	
	printf("\nInforme o nome do arquivo (sem o .txt) que deseja deletar:\n");
	fflush(stdin);
	scanf("%[^\n]", nomeArquivo);
	
	strcat(caminhoDiretorio, nomeArquivo);
	strcat(caminhoDiretorio, ".txt");
	
	k = remove(caminhoDiretorio);
	
	if(k == 0){
		printf("\nArquivo deletado com sucesso!\n");
	}else{
		printf("\nN�o foi poss�vel deletar o arquivo. Verifique se o mesmo est� sendo usado e tente novamente!\n");
	}	
}

/* Permite a visualiza��o dos arquivos */
void visualizaArquivo(){
	
	char* caminhoDiretorio = (char*)malloc(100*sizeof(char));
	char nomeArquivo[100], infoArq[500], caminhoCompartilhado[100];
	FILE* arq = NULL;
	
	printf("\nInforme o diret�rio do usu�rio que deseja acessar:\n");
	caminhoDiretorio = RecebeCaminho(caminhoDiretorio, 0);
	
	printf("\nArquivos deste diret�rio:\n\n");
	exibeArquivosDiretorio(caminhoDiretorio);
	exibeArquivosCompartilhados(caminhoDiretorio);
	
	printf("\nInforme o nome do arquivo (sem o .txt) que deseja visualizar:\n");
	fflush(stdin);
	scanf("%[^\n]", nomeArquivo);
	
	strcat(nomeArquivo, ".txt");
	strcat(caminhoDiretorio, nomeArquivo);
	printf("\n");
	
	if(strstr(nomeArquivo, "_LINK") != 0){ /* Visualiza um arquivo compartilhado, tem que acessar no diret�rio original */
		
		arq = fopen(caminhoDiretorio, "r");
		if(arq == NULL){
			printf("\nN�o foi poss�vel acessar o arquivo!\n");
		}
		else{
			fscanf(arq,"%s\n", caminhoCompartilhado);
		}
		fclose(arq);
		
		arq = fopen(caminhoCompartilhado, "r");
		
		if(arq == NULL){
			printf("\nN�o foi poss�vel acessar o arquivo!\n");
		}
		else{
			while((fgets(infoArq, sizeof(infoArq), arq))!= NULL ){
				printf("%s", infoArq);
			}
		}
		fclose(arq);
		
	}else{ /* Visualiza um arquivo do pr�prio usu�rio*/
		
		arq = fopen(caminhoDiretorio, "r");
		if(arq == NULL){
			printf("\nN�o foi poss�vel acessar o arquivo!\n");
		}
		else{
			while((fgets(infoArq, sizeof(infoArq), arq))!= NULL ){
				printf("%s", infoArq);
			}
		}
		fclose(arq);
	}
	printf("\n");
}

void main(){
	//system("color EA"); -- Tema da copa para o Trabalho
	setlocale(LC_ALL, "Portuguese");
	SetConsoleTitle("Simulador de compartilhamento de arquivos - Grupo: Lorraine, Everton");
	
	int opcao;
	int loop = 1; 
	char caminhoDiretorio[100];
	FILE *arquivoA = NULL;
	FILE *arquivoB = NULL;
	
	/* Cria os diret�rios para os dois usu�rios j� cadastrados */
	CreateDirectory ("C:\\USUARIO_A\\", NULL);
	CreateDirectory ("C:\\USUARIO_B\\", NULL);
	
	/* Cria os arquivos para os dois usu�rios j� cadastrados */
	arquivoA = fopen("C:\\USUARIO_A\\INFO.txt","w");
	if(arquivoA == NULL){
		printf("\nN�o foi poss�vel criar o arquivo!\n");
	}else{
		fprintf(arquivoA, "Quaisquer informa��es...");
	}
	fclose(arquivoA);
	
	arquivoB = fopen("C:\\USUARIO_B\\CONTAS.txt","w");
	if(arquivoB == NULL){
		printf("\nN�o foi poss�vel criar o arquivo!\n");
	}else{
		fprintf(arquivoB, "Quaisquer informa��es...");
	}
	fclose(arquivoB);
	
	while(loop != -1){
		
		menu_principal();
		printf("Informe a op��o desejada: ");
		scanf("%d", &opcao);
	
		switch(opcao){
			case 1:
				usuariosSistema();
				break;
			case 2:
				cadastraUsuario();
				break;
			case 3:
				compartilhaArquivo();
				break;
			case 4:
				compartilhaDiretorio();
				break;
			case 5:
				criaArquivo();
				break;
			case 6:
				copiaArquivo();
				break;
			case 7:
				editaArquivo();
				break;
			case 8:
				deletaArquivo();
				break;
			case 9:
				visualizaArquivo();
				break;
			case 0:
				loop = -1;
				break;
			default: 
				puts("\nOp��o inv�lida!\n");
		}
		
		printf("\n\n");
		system("pause");
		system("cls");
	}
	printf("\n\n\NSistema finalizado!");	
}
