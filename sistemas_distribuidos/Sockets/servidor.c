//Exemplo (processo servidor)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>// define operações nos sockets
#include <netinet/in.h> // define constantes para internet
#include <netdb.h> // conversão de endereços IP
#include <arpa/inet.h> // define operações sobre internet
#include <time.h>
#include <unistd.h>
#include <math.h>

#define BILLION  1000000000.0; //Usado para calcular o tempo de execução

#define linhas 402
#define colunas 402

//variáveis gloabais
int i,j,k;
int novaLinhas, esperaClientes, iteracao=0;
double red[linhas][colunas],black[linhas][colunas], chapaReal[linhas][colunas];
double enviaLinha[colunas];
double recebeLinha[colunas];

int idCliente = 1;
int finalizado = 0;
int nos;
int parar;
double dif;

int sock, connected, bytes_recv, true = 1;
int connected1, connected2, connected3, connected4, connected5, connected6, connected7;
char send_data[10240] , recv_data[10240];
struct sockaddr_in server_addr,  client_addr;
int sin_size, parar=0;
int parar1=0, parar2=0, parar3=0, parar4=0, parar5=0, parar6=0, parar7=0;

//usado para armazenar dados dos clientes
typedef struct clientes{
	char ender[100];
	int ID;
	int porta;		
}CLI;

CLI c1,c2,c3,c4,c5,c6,c7;

//Função para pegar entradas necessárias
void perguntas(){
	printf("\nDigite o número de nós\n");
	__fpurge(stdin);
	scanf("%d",&nos);
	//fget(nos, sizeof(nos), stdin);
	printf("\n-------------------------------------------------------------------\n");
	__fpurge(stdin);
	switch (nos){
		case 2:
			novaLinhas=201;
			esperaClientes = 1;
			break;
		case 4:
			novaLinhas=101;
			esperaClientes = 3;
			CLI c1,c2;
			break;
		case 8:
			novaLinhas=51;
			esperaClientes = 7;
			break;	
		default:
			printf("\nNúmero de nós inválido!\n");
			__fpurge(stdin);
			exit(1);
			break;
	}
	
}

//Funções usadas no algoritmo red&black
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


int main(int argc, char *argv[]) {

	
	//Pega os inputs do usuário
	perguntas();
	//Medir o tempo de execução
	struct timespec start, end;
	
	//Cria socket e verifica se foi bem sucedido na operação
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Erro na criação do Socket");
		exit(1);
	}
	// Ajusta opções para o socket criado
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &true,sizeof(int)) == -1) {
		perror("Erro em Setsockopt");
		exit(1);
	}
	// Inicializa parâmetros para o socket. Note o endereço da porta (5000)
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(5000);  // host-endian to network-endian
	server_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(server_addr.sin_zero),8);
	
	// Liga a estrutura do socket criado ao socket físico real
	if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
		perror("binding não foi possível");
		exit(1);
	}
	// Define o tamanho da fila de entrada (7)
	if (listen(sock, 7) == -1) {
		perror("Erro na definição do tamanho da fila de entrada");
		exit(1);
	}
	printf("\nServidor TCP esperando pelos clientes na porta 5000.\n");
	printf("Servidor é o cliente 0.");
	__fpurge(stdin);
	
	// Laço infinito de existência do servidor
	while(true){
		//Subtrai um de esperaClientes quando um já conectou
		esperaClientes--;
		
		//Pega os dados dos clientes que se conectam
		switch (idCliente){
			case 1:
				sin_size = sizeof(struct sockaddr_in);
				connected1 = accept(sock, (struct sockaddr *)&client_addr, &sin_size);
				printf("\nconnected:%d\n",connected1);
				printf("\nCliente 1 Conexão recebida (Cliente: %s , Porta: %d)\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
				__fpurge(stdin);
				strcpy(c1.ender,inet_ntoa(client_addr.sin_addr));
				c1.ID = idCliente;
				break;
			case 2:
				sin_size = sizeof(struct sockaddr_in);
				connected2 = accept(sock, (struct sockaddr *)&client_addr, &sin_size);
				printf("\nconnected:%d\n",connected2);
				printf("\nConexão 2 recebida (Cliente: %s , Porta: %d)\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
				__fpurge(stdin);
				strcpy(c2.ender,inet_ntoa(client_addr.sin_addr));
				c2.ID = idCliente;
				break;
			case 3:
				sin_size = sizeof(struct sockaddr_in);
				connected3 = accept(sock, (struct sockaddr *)&client_addr, &sin_size);
				printf("\nconnected:%d\n",connected3);
				printf("\nConexão 3 recebida (Cliente: %s , Porta: %d)\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
				__fpurge(stdin);
				strcpy(c3.ender,inet_ntoa(client_addr.sin_addr));
				c3.ID = idCliente;
				break;
			case 4:
				sin_size = sizeof(struct sockaddr_in);
				connected4 = accept(sock, (struct sockaddr *)&client_addr, &sin_size);
				printf("\nconnected:%d\n",connected4);
				printf("\nConexão 4 recebida (Cliente: %s , Porta: %d)\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
				__fpurge(stdin);
				strcpy(c4.ender,inet_ntoa(client_addr.sin_addr));
				c4.ID = idCliente;
				break;
			case 5:
				sin_size = sizeof(struct sockaddr_in);
				connected5 = accept(sock, (struct sockaddr *)&client_addr, &sin_size);
				printf("\nconnected:%d\n",connected5);
				printf("\nConexão 5 recebida (Cliente: %s , Porta: %d)\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
				__fpurge(stdin);
				strcpy(c5.ender,inet_ntoa(client_addr.sin_addr));
				c5.ID = idCliente;
				break;
			case 6:
				sin_size = sizeof(struct sockaddr_in);
				connected6 = accept(sock, (struct sockaddr *)&client_addr, &sin_size);
				printf("\nconnected:%d\n",connected6);
				printf("\nConexão 6 recebida (Cliente: %s , Porta: %d)\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
				__fpurge(stdin);
				strcpy(c6.ender,inet_ntoa(client_addr.sin_addr));
				c6.ID = idCliente;
				break;	
			case 7:
				sin_size = sizeof(struct sockaddr_in);
				connected7 = accept(sock, (struct sockaddr *)&client_addr, &sin_size);
				printf("\nconnected:%d\n",connected7);
				printf("\nConexão 7 recebida (Cliente: %s , Porta: %d)\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
				__fpurge(stdin);
				strcpy(c7.ender,inet_ntoa(client_addr.sin_addr));
				c7.ID = idCliente;
				break;	
		}
		
		//Todos os clientes conectaram
		if(esperaClientes==0){
			//podem começar
			printf("\nTodos os clientes se conectaram.\n");
			__fpurge(stdin);
			
			//Inicialização red&black
			//Zerando bordas
			inicializacaoRed();
			inicializacaoBlack();
			//Inicializações ignorando as bordas!
			for(i=1;i<linhas-1;i++){
				for(j=1;j<colunas-1;j++){
					red[i][j]=0.0;
					black[i][j]=0.0;
				}
			}
			//setando pontos constantes
			setRed();
			setBlack();
			
			//Laço que dura enquanto o critério de parada do red&black não for atingido
			while(finalizado==0){
				//Seleciona qual o caso a ser executado com 2, 4 e 8
				switch (nos){
					case 2:
						if(iteracao==0){
							//Falar para c1 que ele pode começar
							strcpy(send_data,"OK");
							//O primeiro parâmetro é o ID do cliente!
							send(connected1, send_data, strlen(send_data), 0);
							printf("\nEnviei o OK.\n");
							__fpurge(stdin);
							
							printf("\nPressione ENTER para começar o RED&BLACK!\n");
							getchar();
							clock_gettime(CLOCK_REALTIME, &start);
							
						}else{
							
							//Quero enviar a minha iteração
							send(connected1, &iteracao, sizeof(iteracao), 0);
							printf("\nEnviei a iteracao %d que estou.\n", iteracao);
							
							if((iteracao%2)==0){
								//Coloca a última linha da black na variável de envio
								printf("\nEnviando black\n");
								for(i=1;i<401;i++){
									enviaLinha[i] = black[200][i];
								}
								//Envia a black
								send(connected1, &enviaLinha, sizeof(enviaLinha), 0);
								printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
								__fpurge(stdin);
								printf("\nRecebendo black\n");
								bytes_recv=recv(connected1,&recebeLinha,sizeof(recebeLinha),0);
								//Imprime a resposta 
								printf("\n LINHA RECEBIDA do c1 = [%.10f], [%.10f], ...[%.10f] ...[%.10f]",recebeLinha[1],recebeLinha[2],recebeLinha[200],recebeLinha[400]);
								__fpurge(stdin);
								for(i=1;i<401;i++){
									black[201][i] = recebeLinha[i];
								}
								
								//cálculos
								for(i=1;i<novaLinhas;i++){
									for(j=1;j<colunas-1;j++){
										red[i][j] = (black[i][j] + black[i-1][j] + black[i][j+1] + black[i+1][j] + black[i][j-1])/5;
									}
								}
								setRed();
								setBlack();
								
							}else{
								//Coloca a última linha da red na variável de envio
								printf("\nEnviando RED\n");
								for(i=1;i<401;i++){
									enviaLinha[i] = red[200][i];
								}
								//enviar a ultima linha da red
								send(connected1, &enviaLinha, sizeof(enviaLinha), 0);
								printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
								
								printf("\nRecebendo RED\n");
								bytes_recv=recv(connected1, &recebeLinha, sizeof(recebeLinha),0);
								// Imprime a resposta 
								printf("\n LINHA RECEBIDA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
								__fpurge(stdin);
								for(i=1;i<401;i++){
									red[201][i] = recebeLinha[i];
								}
								//cálculos
								for(i=1;i<novaLinhas;i++){
									for(j=1;j<colunas-1;j++){
										black[i][j] = (red[i][j] + red[i-1][j] + red[i][j+1] + red[i+1][j] + red[i][j-1])/5;
									}
								}
								setRed();
								setBlack();
								
							}
							
							parar=1;
							//calcula a diferença dos pontos para checar o critério de parada
							for(i=1;i<101;i++){
								for(j=1;j<colunas-1;j++){
									dif = red[i][j] - black[i][j];
									//Se algum ponto das matrizes tiver diferença >= 0.01, então NÃO PARA
									if( fabs(dif) >= 0.01){
										parar = 0;
									}
								}
							}
							//recebe do cliente se atingiu o critério
							bytes_recv=recv(connected1,&parar1,sizeof(parar1),0);
							if(parar==1 && parar1==1)
								finalizado = 1;
							
						}
						iteracao++;
						break;
					//4 nós
					case 4:
						if(iteracao==0){
							printf("\nPressione ENTER para começar o RED&BLACK!\n");
							getchar();
							clock_gettime(CLOCK_REALTIME, &start);
						}else{
							printf("\nEnviei a iteracao %d que estou.\n", iteracao);
							send(connected1, &iteracao, sizeof(iteracao), 0);
							send(connected2, &iteracao, sizeof(iteracao), 0);
							send(connected3, &iteracao, sizeof(iteracao), 0);
							//Lógica de envio e recebimento dos clientes
							//É usado para garantir a coordenação
							if((iteracao%2)==0){
								for(i=1;i<401;i++){
									enviaLinha[i] = black[100][i];
								}
								//Envia a black
								send(connected1, &enviaLinha, sizeof(enviaLinha), 0);
								printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
								__fpurge(stdin);
								
								
								bytes_recv=recv(connected1,&recebeLinha,sizeof(recebeLinha),0);
								for(i=1;i<401;i++){
									black[101][i] = recebeLinha[i];
								}
								
								bytes_recv=recv(connected1,&recebeLinha,sizeof(recebeLinha),0);
								for(i=1;i<401;i++){
									black[200][i] = recebeLinha[i];
								}
								
								bytes_recv=recv(connected2,&recebeLinha,sizeof(recebeLinha),0);
								for(i=1;i<401;i++){
									black[201][i] = recebeLinha[i];
								}
								
								bytes_recv=recv(connected2,&recebeLinha,sizeof(recebeLinha),0);
								for(i=1;i<401;i++){
									black[300][i] = recebeLinha[i];
								}
								
								bytes_recv=recv(connected3,&recebeLinha,sizeof(recebeLinha),0);
								for(i=1;i<401;i++){
									black[301][i] = recebeLinha[i];
								}								

								for(i=1;i<401;i++){
									enviaLinha[i] = black[201][i];
								}
								send(connected1, &enviaLinha, sizeof(enviaLinha), 0);
								printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
								__fpurge(stdin);
								
								for(i=1;i<401;i++){
									enviaLinha[i] = black[200][i];
								}
								send(connected2, &enviaLinha, sizeof(enviaLinha), 0);
								printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
								__fpurge(stdin);
								
								for(i=1;i<401;i++){
									enviaLinha[i] = black[301][i];
								}
								
								send(connected2, &enviaLinha, sizeof(enviaLinha), 0);
								printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
								__fpurge(stdin);
								
							
								for(i=1;i<401;i++){
									enviaLinha[i] = black[301][i];
								}
								//Envia a black
								send(connected3, &enviaLinha, sizeof(enviaLinha), 0);
								printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
								__fpurge(stdin);
								
								//cálculos
								for(i=1;i<101;i++){
									for(j=1;j<colunas-1;j++){
										red[i][j] = (black[i][j] + black[i-1][j] + black[i][j+1] + black[i+1][j] + black[i][j-1])/5;
									}
								}
								setRed();
								setBlack();	

							//caso com a matriz red	
							}else{
								for(i=1;i<401;i++){
									enviaLinha[i] = red[100][i];
								}
								send(connected1, &enviaLinha, sizeof(enviaLinha), 0);
								printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
								__fpurge(stdin);
								
								
								bytes_recv=recv(connected1,&recebeLinha,sizeof(recebeLinha),0);
								for(i=1;i<401;i++){
									red[101][i] = recebeLinha[i];
								}
								
								bytes_recv=recv(connected1,&recebeLinha,sizeof(recebeLinha),0);
								for(i=1;i<401;i++){
									red[200][i] = recebeLinha[i];
								}
								
								for(i=1;i<401;i++){
									enviaLinha[i] = red[201][i];
								}
								send(connected1, &enviaLinha, sizeof(enviaLinha), 0);
								printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
								__fpurge(stdin);
								
								bytes_recv=recv(connected2,&recebeLinha,sizeof(recebeLinha),0);
								for(i=1;i<401;i++){
									red[201][i] = recebeLinha[i];
								}
								
								bytes_recv=recv(connected2,&recebeLinha,sizeof(recebeLinha),0);
								for(i=1;i<401;i++){
									red[300][i] = recebeLinha[i];
								}
								
								bytes_recv=recv(connected3,&recebeLinha,sizeof(recebeLinha),0);
								for(i=1;i<401;i++){
									red[301][i] = recebeLinha[i];
								}
								
								for(i=1;i<401;i++){
									enviaLinha[i] = red[201][i];
								}
								send(connected1, &enviaLinha, sizeof(enviaLinha), 0);
								printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
								__fpurge(stdin);
								
								for(i=1;i<401;i++){
									enviaLinha[i] = red[200][i];
								}
								send(connected2, &enviaLinha, sizeof(enviaLinha), 0);
								printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
								__fpurge(stdin);
								
								for(i=1;i<401;i++){
									enviaLinha[i] = red[301][i];
								}
								send(connected2, &enviaLinha, sizeof(enviaLinha), 0);
								printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
								__fpurge(stdin);
								
								for(i=1;i<401;i++){
									enviaLinha[i] = red[300][i];
								}
								send(connected3, &enviaLinha, sizeof(enviaLinha), 0);
								printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
								__fpurge(stdin);
								
								//cálculos
								for(i=1;i<101;i++){
									for(j=1;j<colunas-1;j++){
										black[i][j] = (red[i][j] + red[i-1][j] + red[i][j+1] + red[i+1][j] + red[i][j-1])/5;
									}
								}
								setRed();
								setBlack();	

							}
							parar=1;
							//calcula a diferença dos pontos para checar o critério de parada
							for(i=1;i<101;i++){
								for(j=1;j<colunas-1;j++){
									dif = red[i][j] - black[i][j];
									//Se algum ponto das matrizes tiver diferença >= 0.01, então NÃO PARA
									if( fabs(dif) >= 0.01){
										parar = 0;
									}
								}
							}

							bytes_recv=recv(connected1,&parar1,sizeof(parar1),0);
							bytes_recv=recv(connected2,&parar2,sizeof(parar2),0);
							bytes_recv=recv(connected3,&parar3,sizeof(parar3),0);
							printf("\n parar0: %d\n", parar);
							__fpurge(stdin);
							printf("\n parar1: %d\n", parar1);
							__fpurge(stdin);
							printf("\n parar2: %d\n", parar2);
							__fpurge(stdin);
							printf("\n parar3: %d\n", parar3);
							__fpurge(stdin);


							if((parar==1 )&& (parar1==1) && (parar2==1) && (parar3==1) ){
								finalizado=1;
							}
							else{
								//Máximo
								if(iteracao==332)
									finalizado=1;
							}
						}
						
						
						
						iteracao++;
						break;	
					case 8:
						if(iteracao==0){
							printf("\nPressione ENTER para começar o RED&BLACK!\n");
							getchar();
							clock_gettime(CLOCK_REALTIME, &start);
						
						}else{
							printf("\nEnviei a iteracao %d que estou.\n", iteracao);
							__fpurge(stdin);
							send(connected1, &iteracao, sizeof(iteracao), 0);
							send(connected2, &iteracao, sizeof(iteracao), 0);
							send(connected3, &iteracao, sizeof(iteracao), 0);
							send(connected4, &iteracao, sizeof(iteracao), 0);
							send(connected5, &iteracao, sizeof(iteracao), 0);
							send(connected6, &iteracao, sizeof(iteracao), 0);
							send(connected7, &iteracao, sizeof(iteracao), 0);
							
							//Ações necessárias para coordenar a troca de mensagens
							if((iteracao%2)==0){
								
								for(i=1;i<401;i++){
									enviaLinha[i] = black[50][i];
								}
								//Envia a black
								send(connected1, &enviaLinha, sizeof(enviaLinha), 0);
								printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
								__fpurge(stdin);
								
								
								bytes_recv=recv(connected1,&recebeLinha,sizeof(recebeLinha),0);
								for(i=1;i<401;i++){
									black[51][i] = recebeLinha[i];
								}
								
								bytes_recv=recv(connected1,&recebeLinha,sizeof(recebeLinha),0);
								for(i=1;i<401;i++){
									black[100][i] = recebeLinha[i];
								}
								
								bytes_recv=recv(connected2,&recebeLinha,sizeof(recebeLinha),0);
								for(i=1;i<401;i++){
									black[101][i] = recebeLinha[i];
								}
								bytes_recv=recv(connected2,&recebeLinha,sizeof(recebeLinha),0);
								for(i=1;i<401;i++){
									black[150][i] = recebeLinha[i];
								}
								
								bytes_recv=recv(connected3,&recebeLinha,sizeof(recebeLinha),0);
								for(i=1;i<401;i++){
									black[151][i] = recebeLinha[i];
								}
								bytes_recv=recv(connected3,&recebeLinha,sizeof(recebeLinha),0);
								for(i=1;i<401;i++){
									black[200][i] = recebeLinha[i];
								}
								
								bytes_recv=recv(connected4,&recebeLinha,sizeof(recebeLinha),0);
								for(i=1;i<401;i++){
									black[201][i] = recebeLinha[i];
								}
								bytes_recv=recv(connected4,&recebeLinha,sizeof(recebeLinha),0);
								for(i=1;i<401;i++){
									black[250][i] = recebeLinha[i];
								}
								
								bytes_recv=recv(connected5,&recebeLinha,sizeof(recebeLinha),0);
								for(i=1;i<401;i++){
									black[251][i] = recebeLinha[i];
								}
								bytes_recv=recv(connected3,&recebeLinha,sizeof(recebeLinha),0);
								for(i=1;i<401;i++){
									black[300][i] = recebeLinha[i];
								}
								
								bytes_recv=recv(connected6,&recebeLinha,sizeof(recebeLinha),0);
								for(i=1;i<401;i++){
									black[301][i] = recebeLinha[i];
								}
								bytes_recv=recv(connected6,&recebeLinha,sizeof(recebeLinha),0);
								for(i=1;i<401;i++){
									black[350][i] = recebeLinha[i];
								}
								
								
								bytes_recv=recv(connected7,&recebeLinha,sizeof(recebeLinha),0);
								for(i=1;i<401;i++){
									black[351][i] = recebeLinha[i];
								}
								
								//Envios
								
								
								for(i=1;i<401;i++){
									enviaLinha[i] = black[101][i];
								}
								//Envia a black
								send(connected1, &enviaLinha, sizeof(enviaLinha), 0);
								
								for(i=1;i<401;i++){
									enviaLinha[i] = black[100][i];
								}
								//Envia a black
								send(connected2, &enviaLinha, sizeof(enviaLinha), 0);
								for(i=1;i<401;i++){
									enviaLinha[i] = black[151][i];
								}
								//Envia a black
								send(connected2, &enviaLinha, sizeof(enviaLinha), 0);
								
								for(i=1;i<401;i++){
									enviaLinha[i] = black[150][i];
								}
								//Envia a black
								send(connected3, &enviaLinha, sizeof(enviaLinha), 0);
								for(i=1;i<401;i++){
									enviaLinha[i] = black[201][i];
								}
								//Envia a black
								send(connected3, &enviaLinha, sizeof(enviaLinha), 0);
								
								for(i=1;i<401;i++){
									enviaLinha[i] = black[200][i];
								}
								//Envia a black
								send(connected4, &enviaLinha, sizeof(enviaLinha), 0);
								for(i=1;i<401;i++){
									enviaLinha[i] = black[251][i];
								}
								//Envia a black
								send(connected4, &enviaLinha, sizeof(enviaLinha), 0);
								
								for(i=1;i<401;i++){
									enviaLinha[i] = black[250][i];
								}
								//Envia a black
								send(connected5, &enviaLinha, sizeof(enviaLinha), 0);
								for(i=1;i<401;i++){
									enviaLinha[i] = black[301][i];
								}
								//Envia a black
								send(connected5, &enviaLinha, sizeof(enviaLinha), 0);
								
								for(i=1;i<401;i++){
									enviaLinha[i] = black[300][i];
								}
								//Envia a black
								send(connected6, &enviaLinha, sizeof(enviaLinha), 0);
								for(i=1;i<401;i++){
									enviaLinha[i] = black[351][i];
								}
								//Envia a black
								send(connected6, &enviaLinha, sizeof(enviaLinha), 0);
								
								for(i=1;i<401;i++){
									enviaLinha[i] = black[350][i];
								}
								//Envia a black
								send(connected7, &enviaLinha, sizeof(enviaLinha), 0);
								
								//cálculos
								for(i=1;i<novaLinhas;i++){
									for(j=1;j<colunas-1;j++){
										red[i][j] = (black[i][j] + black[i-1][j] + black[i][j+1] + black[i+1][j] + black[i][j-1])/5;
									}
								}
								setRed();
								setBlack();	
								
							}else{
								
								for(i=1;i<401;i++){
									enviaLinha[i] = red[50][i];
								}
								//Envia a red
								send(connected1, &enviaLinha, sizeof(enviaLinha), 0);
								printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
								__fpurge(stdin);
							
								bytes_recv=recv(connected1,&recebeLinha,sizeof(recebeLinha),0);
								for(i=1;i<401;i++){
									red[51][i] = recebeLinha[i];
								}
								
								bytes_recv=recv(connected1,&recebeLinha,sizeof(recebeLinha),0);
								for(i=1;i<401;i++){
									red[100][i] = recebeLinha[i];
								}
								
								bytes_recv=recv(connected2,&recebeLinha,sizeof(recebeLinha),0);
								for(i=1;i<401;i++){
									red[101][i] = recebeLinha[i];
								}
								
								bytes_recv=recv(connected2,&recebeLinha,sizeof(recebeLinha),0);
								for(i=1;i<401;i++){
									red[150][i] = recebeLinha[i];
								}
								
								bytes_recv=recv(connected3,&recebeLinha,sizeof(recebeLinha),0);
								for(i=1;i<401;i++){
									red[151][i] = recebeLinha[i];
								}
								
								
								bytes_recv=recv(connected3,&recebeLinha,sizeof(recebeLinha),0);
								for(i=1;i<401;i++){
									red[200][i] = recebeLinha[i];
								}
								
								bytes_recv=recv(connected4,&recebeLinha,sizeof(recebeLinha),0);
								for(i=1;i<401;i++){
									red[201][i] = recebeLinha[i];
								}
								
								bytes_recv=recv(connected4,&recebeLinha,sizeof(recebeLinha),0);
								for(i=1;i<401;i++){
									red[250][i] = recebeLinha[i];
								}
								
								
								bytes_recv=recv(connected5,&recebeLinha,sizeof(recebeLinha),0);
								for(i=1;i<401;i++){
									red[251][i] = recebeLinha[i];
								}
								
								bytes_recv=recv(connected5,&recebeLinha,sizeof(recebeLinha),0);
								for(i=1;i<401;i++){
									red[300][i] = recebeLinha[i];
								}
								
								bytes_recv=recv(connected6,&recebeLinha,sizeof(recebeLinha),0);
								for(i=1;i<401;i++){
									red[301][i] = recebeLinha[i];
								}
								
								bytes_recv=recv(connected6,&recebeLinha,sizeof(recebeLinha),0);
								for(i=1;i<401;i++){
									red[350][i] = recebeLinha[i];
								}
								
								bytes_recv=recv(connected7,&recebeLinha,sizeof(recebeLinha),0);
								for(i=1;i<401;i++){
									red[351][i] = recebeLinha[i];
								}
								
								for(i=1;i<401;i++){
									enviaLinha[i] = red[101][i];
								}
								
								//Envia a red
								send(connected1, &enviaLinha, sizeof(enviaLinha), 0);
								printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
								__fpurge(stdin);
								
								for(i=1;i<401;i++){
									enviaLinha[i] = red[100][i];
								}
								//Envia a red
								send(connected2, &enviaLinha, sizeof(enviaLinha), 0);
								printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
								__fpurge(stdin);
								
								for(i=1;i<401;i++){
									enviaLinha[i] = red[151][i];
								}
								//Envia a red
								send(connected2, &enviaLinha, sizeof(enviaLinha), 0);
								printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
								__fpurge(stdin);
								
								for(i=1;i<401;i++){
									enviaLinha[i] = red[150][i];
								}
								//Envia a red
								send(connected3, &enviaLinha, sizeof(enviaLinha), 0);
								printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
								__fpurge(stdin);
								
								for(i=1;i<401;i++){
									enviaLinha[i] = red[201][i];
								}
								//Envia a red
								send(connected3, &enviaLinha, sizeof(enviaLinha), 0);
								printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
								__fpurge(stdin);
								
								for(i=1;i<401;i++){
									enviaLinha[i] = red[200][i];
								}
								//Envia a red
								send(connected4, &enviaLinha, sizeof(enviaLinha), 0);
								printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
								__fpurge(stdin);
								
								for(i=1;i<401;i++){
									enviaLinha[i] = red[251][i];
								}
								//Envia a red
								send(connected4, &enviaLinha, sizeof(enviaLinha), 0);
								printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
								__fpurge(stdin);
								
								for(i=1;i<401;i++){
									enviaLinha[i] = red[250][i];
								}
								//Envia a red
								send(connected5, &enviaLinha, sizeof(enviaLinha), 0);
								printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
								__fpurge(stdin);
								
								
								for(i=1;i<401;i++){
									enviaLinha[i] = red[301][i];
								}
								//Envia a red
								send(connected5, &enviaLinha, sizeof(enviaLinha), 0);
								printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
								__fpurge(stdin);
								
								for(i=1;i<401;i++){
									enviaLinha[i] = red[300][i];
								}
								//Envia a red
								send(connected6, &enviaLinha, sizeof(enviaLinha), 0);
								printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
								__fpurge(stdin);
								
								for(i=1;i<401;i++){
									enviaLinha[i] = red[351][i];
								}
								//Envia a red
								send(connected6, &enviaLinha, sizeof(enviaLinha), 0);
								printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
								__fpurge(stdin);
								
								
								for(i=1;i<401;i++){
									enviaLinha[i] = red[350][i];
								}
								//Envia a red
								send(connected7, &enviaLinha, sizeof(enviaLinha), 0);
								printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
								__fpurge(stdin);
								
								
								//cálculos
								for(i=1;i<novaLinhas;i++){
									for(j=1;j<colunas-1;j++){
										black[i][j] = (red[i][j] + red[i-1][j] + red[i][j+1] + red[i+1][j] + red[i][j-1])/5;
									}
								}
								setRed();
								setBlack();	
						
							}
							
							parar=1;
							//calcula a diferença dos pontos para checar o critério de parada
							for(i=1;i<51;i++){
								for(j=1;j<colunas-1;j++){
									dif = red[i][j] - black[i][j];
									//Se algum ponto das matrizes tiver diferença >= 0.01, então NÃO PARA
									if( fabs(dif) >= 0.01){
										parar = 0;
									}
								}
							}
							

							bytes_recv=recv(connected1,&parar1,sizeof(parar1),0);
							bytes_recv=recv(connected2,&parar2,sizeof(parar2),0);
							bytes_recv=recv(connected3,&parar3,sizeof(parar3),0);
							bytes_recv=recv(connected4,&parar4,sizeof(parar4),0);
							bytes_recv=recv(connected5,&parar5,sizeof(parar5),0);
							bytes_recv=recv(connected6,&parar6,sizeof(parar6),0);
							bytes_recv=recv(connected7,&parar7,sizeof(parar7),0);
							
							printf("\n parar0: %d\n", parar);
							__fpurge(stdin);
							printf("\n parar1: %d\n", parar1);
							__fpurge(stdin);
							printf("\n parar2: %d\n", parar2);
							__fpurge(stdin);
							printf("\n parar3: %d\n", parar3);
							__fpurge(stdin);
							printf("\n parar4: %d\n", parar4);
							__fpurge(stdin);
							printf("\n parar5: %d\n", parar5);
							__fpurge(stdin);
							printf("\n parar6: %d\n", parar6);
							__fpurge(stdin);
							printf("\n parar7: %d\n", parar7);
							__fpurge(stdin);

							if((parar==1 )&& (parar1==1) && (parar2==1) && (parar3==1)&& (parar4==1)&& (parar5==1)&& (parar6==1)&& (parar7==1) ){
								finalizado=1;
								break;
							}
						}
						
						iteracao++;
						break;	
					default:
						printf("\nNúmero de nós inválido!\n");
						exit(1);
						break;
						
				}
				
				
			}
			clock_gettime(CLOCK_REALTIME, &end);
			printf("\nNúmero de iterações: %d\n", iteracao-1);
			double time_spent = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / BILLION;
			printf("\nTempo de execução: %f segundos\n", time_spent);
			exit(1);
			
		}else{
			printf("\nAguardando todos os nós, faltam:%d\n", esperaClientes);
			__fpurge(stdin);
		}
		
		idCliente++;
	}// final do laço infinito do servidor
	
	close(sock);
	return 0;
}