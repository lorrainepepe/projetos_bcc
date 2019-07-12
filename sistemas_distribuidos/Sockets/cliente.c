//Exemplo (processo cliente)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> // define operações nos sockets
#include <netinet/in.h> // define constantes para internet
#include <netdb.h> // conversão de endereços IP
#include <math.h>


#define linhas 402
#define colunas 402

//Variáveis globais
int i,j,k,l;
int novaLinhas;
double red[402][402],black[402][402], a;
double linhaCima[400], linhaBaixo[400];
double enviaLinha[colunas];
double recebeLinha[colunas];
double dif;

int idCliente, parar=0,nos,iteracao=0,finalizado=0,podeComecar=0;
int idVizinhoCima, idVizinhoBaixo;
int dado_ite[8];

int sock,sock2,sock4,sock6,bytes_recv, true = 1;
char send_data[10240],recv_data[10240];
struct hostent *host, *host1;
struct sockaddr_in server_addr, client_addr, sub_server, sub_cliente;
int sin_size;
int connectedCima=-1, connectedBaixo=-1, sockCima, sockBaixo,sock1;

typedef struct clientes{
	char ender[100];
	int ID;
	int porta;
}CLI;

CLI clienteCima,clienteBaixo;

void criterio(int fim){
//Assume-se que nesta iteração i iremos parar
	parar=1;
	//calcula a diferença dos pontos para checar o critério de parada
	for(i=l;i<fim;i++){
		for(j=1;j<colunas-1;j++){
			dif = red[i][j] - black[i][j];
			//Se algum ponto das matrizes tiver diferença >= 0.01, então NÃO PARA
			if( fabs(dif) >= 0.01){
				parar = 0;
			}
		}
	}
	printf("---------FIMMM: %d  PARAR:%d------------------",fim,parar);
	__fpurge(stdin);
	send(sock, &parar, sizeof(parar), 0);
}


void calculoBlack(int limite){
	for(i=l;i<limite;i++){
		for(j=1;j<colunas-1;j++){
			red[i][j] = (black[i][j] + black[i-1][j] + black[i][j+1] + black[i+1][j] + black[i][j-1])/5;
		}
	}

}

void calculoRed(int limite){
	for(i=l;i<limite;i++){
		for(j=1;j<colunas-1;j++){
			black[i][j] = (red[i][j] + red[i-1][j] + red[i][j+1] + red[i+1][j] + red[i][j-1])/5;
		}
	}

}

void perguntas(){
	//Usa o ID para saber qual é a faixa de linhas que ele deve fazer conta
	printf("\nDigite o ID:\n");
	__fpurge(stdin);
	scanf("%d",&idCliente);
	printf("\nID:%d\n", idCliente);
	__fpurge(stdin);
	printf("\nDigite o número de nós:\n");
	__fpurge(stdin);
	scanf("%d",&nos);
	
	switch (nos){
		case 2:
			//printf("2");
			novaLinhas=201;
			break;
		case 4:
			//printf("4");
			novaLinhas=101;
			break;
		case 8:
			//printf("8");
			novaLinhas=51;
			break;	
		default:
			printf("\nNúmero de nós inválido!\n");
			__fpurge(stdin);
			exit(1);
			break;
	}
}

//Funções do algoritmo Red&Black
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

int main(int argc, char *argv[]){

	//inicialização
	perguntas();
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
	
	// Le linha de comando para ter o nome (IP) do servidor
	host = gethostbyname(argv[1]);
	// Cria um socket
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Erro na criação do Socket");
		exit(1);
	}
	
	// Define informações sobre o socket do servidor
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(5000);
	server_addr.sin_addr = *((struct in_addr *)host->h_addr);
	bzero(&(server_addr.sin_zero),8);
	
	// Conecta com o servidor definido
	if (connect(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
		perror("Erro na conexão");
		exit(1);
	}
	
	//Ciclo que dura a existência do cliente
	while(finalizado==0){
			//Seleciona o caso de 2, 4 ou 8 nós
			switch (nos){
				case 2:
					if(podeComecar==1){
					//valor da coluna para os cálculos
					l = 201;
					printf("\nInicio da minha linha:%d\n",l);
					__fpurge(stdin);
					
					//Recebe a iteração depois do OK
					bytes_recv=recv(sock,dado_ite,sizeof(dado_ite),0);
					
					// Imprime a resposta 
					printf("\n Iteracao RECEBIDA = %d ",*dado_ite);
					__fpurge(stdin);
					iteracao = *dado_ite;
					printf("\nIteração de fato: %d\n", iteracao);
					__fpurge(stdin);
					
					//Recebe a ultima linha do servidor agora
					
					if( (iteracao%2) == 0 ){
						//receber black
						printf("\nRecebendo a BLACK.\n");
						__fpurge(stdin);
						bytes_recv=recv(sock,&recebeLinha,sizeof(recebeLinha),0);
						// Imprime a resposta 
						printf("\n LINHA RECEBIDA do server = [%.10f], [%.10f]...[%.10f] ",recebeLinha[1],recebeLinha[2],recebeLinha[400]);
						__fpurge(stdin);
						for(i=1;i<401;i++){
							black[200][i] = recebeLinha[i];
						}
						
						printf("\nEnviando a BLACK.\n");
						__fpurge(stdin);
						for(i=1;i<401;i++){
							enviaLinha[i] = black[201][i];
						}
											
						send(sock, &enviaLinha, sizeof(enviaLinha), 0);
						printf("\n LINHA ENVIADA para server = [%.10f], [%.10f]...[%.10f] ",enviaLinha[1],enviaLinha[2],enviaLinha[400]);
						__fpurge(stdin);
						//cálculos
						for(i=l;i<l+200;i++){
							for(j=1;j<colunas-1;j++){
								red[i][j] = (black[i][j] + black[i-1][j] + black[i][j+1] + black[i+1][j] + black[i][j-1])/5;
							}
						}
						setRed();
						setBlack();
						
					}else{
						//Receber a red
						printf("\nRecebendo a RED.\n");
						__fpurge(stdin);
						bytes_recv=recv(sock,&recebeLinha,sizeof(recebeLinha),0);
						// Imprime a resposta 
						printf("\n LINHA RECEBIDA do server = [%.10f], [%.10f]...[%.10f] ",recebeLinha[1],recebeLinha[2],recebeLinha[400]);
						__fpurge(stdin);
						for(i=1;i<401;i++){
							red[200][i] = recebeLinha[i];
						}
						
						printf("\nEnviando a RED.\n");
						__fpurge(stdin);
						for(i=1;i<401;i++){
							enviaLinha[i] = red[201][i];
						}
											
						send(sock, &enviaLinha, sizeof(enviaLinha), 0);
						printf("\n LINHA ENVIADA para server = [%.10f], [%.10f]...[%.10f] ",enviaLinha[1],enviaLinha[2],enviaLinha[400]);
						__fpurge(stdin);
						//cálculos
						for(i=l;i<l+200;i++){
							for(j=1;j<colunas-1;j++){
								black[i][j] = (red[i][j] + red[i-1][j] + red[i][j+1] + red[i+1][j] + red[i][j-1])/5;
							}
						}
						setRed();
						setBlack();
						
					}
					
					//Assume-se que nesta iteração i iremos parar
					parar=1;
					//calcula a diferença dos pontos para checar o critério de parada
					for(i=1;i<l+200;i++){
						for(j=1;j<colunas-1;j++){
							dif = red[i][j] - black[i][j];
							//Se algum ponto das matrizes tiver diferença >= 0.01, então NÃO PARA
							if( fabs(dif) >= 0.01){
								parar = 0;
							}
						}
					}
					if(parar==1){
						printf("\n------Iteracao que atingiu o critério:%d------------\n", iteracao);
						__fpurge(stdin);
						
					}
					send(sock, &parar, sizeof(parar), 0);	
					
					}else{
						//Primeiro recebe o OK do server
						bytes_recv=recv(sock,recv_data,10240,0);
						recv_data[bytes_recv] = '\0';
						// Imprime a resposta 
						printf("\n OK RECEBIDO = %s",recv_data);
						__fpurge(stdin);
						//Se o que recebeu foi OK, então podeComecar=1;
						if(strcmp(recv_data,"OK")==0){
							podeComecar=1;

						}
					break;
				case 4:
					l = 101 + (100*(idCliente-1) );
					printf("\nInicio da minha linha:%d\n",l);
					__fpurge(stdin);
						
						bytes_recv=recv(sock,dado_ite,sizeof(dado_ite),0);
						// Imprime a resposta 
						printf("\n Iteracao RECEBIDA = %d ",*dado_ite);
						__fpurge(stdin);
						iteracao = *dado_ite;
						printf("\nIteração de fato: %d\n", iteracao);
						__fpurge(stdin);
					//Seleciona qual é o cliente
					switch (idCliente){
						case 1:
							if((iteracao%2)==0){
								printf("\nRecebendo a BLACK.\n");
								__fpurge(stdin);
							//Recv
								bytes_recv=recv(sock,&recebeLinha,sizeof(recebeLinha),0);
								// Imprime a resposta 
								printf("\n LINHA RECEBIDA do server = [%.10f], [%.10f]...[%.10f] ",recebeLinha[1],recebeLinha[2],recebeLinha[400]);
								__fpurge(stdin);
								for(i=1;i<401;i++){
									black[100][i] = recebeLinha[i];
								}
								//Faz os dois envios necessários para o server
								for(i=1;i<401;i++){
									enviaLinha[i] = black[101][i];
								}
							//Send
								send(sock, &enviaLinha, sizeof(enviaLinha), 0);
								printf("\n LINHA ENVIADA para server = [%.10f], [%.10f]...[%.10f] ",enviaLinha[1],enviaLinha[2],enviaLinha[400]);
								__fpurge(stdin);

								for(i=1;i<401;i++){
									enviaLinha[i] = black[200][i];
								}
							//Send
								send(sock, &enviaLinha, sizeof(enviaLinha), 0);
								//Recebe a linha de baixo para fazer a conta
							//Recv
								bytes_recv=recv(sock,&recebeLinha,sizeof(recebeLinha),0);
								// Imprime a resposta 
								for(i=1;i<401;i++){
									black[201][i] = recebeLinha[i];
								}

								//cálculos
								for(i=l;i<201;i++){
									for(j=1;j<colunas-1;j++){
										red[i][j] = (black[i][j] + black[i-1][j] + black[i][j+1] + black[i+1][j] + black[i][j-1])/5;
									}
								}
								setRed();
								setBlack();

								
							}else{
								printf("\nRecebendo a red.\n");
								__fpurge(stdin);
								bytes_recv=recv(sock,&recebeLinha,sizeof(recebeLinha),0);
								// Imprime a resposta 
								printf("\n LINHA RECEBIDA do server = [%.10f], [%.10f]...[%.10f] ",recebeLinha[1],recebeLinha[2],recebeLinha[400]);
								__fpurge(stdin);
								for(i=1;i<401;i++){
									red[100][i] = recebeLinha[i];
								}
								//Faz os dois envios necessários para o server
								for(i=1;i<401;i++){
									enviaLinha[i] = red[101][i];
								}
								send(sock, &enviaLinha, sizeof(enviaLinha), 0);
								printf("\n LINHA ENVIADA para server = [%.10f], [%.10f]...[%.10f] ",enviaLinha[1],enviaLinha[2],enviaLinha[400]);
								__fpurge(stdin);

								for(i=1;i<401;i++){
									enviaLinha[i] = red[200][i];
								}
								send(sock, &enviaLinha, sizeof(enviaLinha), 0);

								//Recebe a linha de baixo para fazer a conta
								bytes_recv=recv(sock,&recebeLinha,sizeof(recebeLinha),0);
								// Imprime a resposta 
								for(i=1;i<401;i++){
									red[201][i] = recebeLinha[i];
								}

								//cálculos
								for(i=l;i<201;i++){
									for(j=1;j<colunas-1;j++){
										black[i][j] = (red[i][j] + red[i-1][j] + red[i][j+1] + red[i+1][j] + red[i][j-1])/5;
									}
								}
								setRed();
								setBlack();

							}
							//Assume-se que nesta iteração i iremos parar
							parar=1;
							//calcula a diferença dos pontos para checar o critério de parada
							for(i=l;i<201;i++){
								for(j=1;j<colunas-1;j++){
									dif = red[i][j] - black[i][j];
									//Se algum ponto das matrizes tiver diferença >= 0.01, então NÃO PARA
									if( fabs(dif) >= 0.01){
										parar = 0;
									}
								}
							}
							//Máximo
							if(iteracao>=329)
								parar = 1;
							printf("---------PARAR:%d------------------",parar);
							__fpurge(stdin);
							send(sock, &parar, sizeof(parar), 0);
							
							break;
						case 2:
							if((iteracao%2)==0){
								//Faz os dois envios necessários para o server
								for(i=1;i<401;i++){
									enviaLinha[i] = black[201][i];
								}
								send(sock, &enviaLinha, sizeof(enviaLinha), 0);
								printf("\n LINHA ENVIADA para server = [%.10f], [%.10f]...[%.10f] ",enviaLinha[1],enviaLinha[2],enviaLinha[400]);
								__fpurge(stdin);

								for(i=1;i<401;i++){
									enviaLinha[i] = black[300][i];
								}
								send(sock, &enviaLinha, sizeof(enviaLinha), 0);
								
								//Recebe a linha de baixo para fazer a conta
								bytes_recv=recv(sock,&recebeLinha,sizeof(recebeLinha),0);
								// Imprime a resposta 
								for(i=1;i<401;i++){
									black[200][i] = recebeLinha[i];
								}
								bytes_recv=recv(sock,&recebeLinha,sizeof(recebeLinha),0);
								// Imprime a resposta 
								for(i=1;i<401;i++){
									black[301][i] = recebeLinha[i];
								}

								//cálculos
								for(i=l;i<301;i++){
									for(j=1;j<colunas-1;j++){
										red[i][j] = (black[i][j] + black[i-1][j] + black[i][j+1] + black[i+1][j] + black[i][j-1])/5;
									}
								}
								setRed();
								setBlack();

							}else{
								//Faz os dois envios necessários para o server
								for(i=1;i<401;i++){
									enviaLinha[i] = red[201][i];
								}
								send(sock, &enviaLinha, sizeof(enviaLinha), 0);
								printf("\n LINHA ENVIADA para server = [%.10f], [%.10f]...[%.10f] ",enviaLinha[1],enviaLinha[2],enviaLinha[400]);
								__fpurge(stdin);

								for(i=1;i<401;i++){
									enviaLinha[i] = red[300][i];
								}
								send(sock, &enviaLinha, sizeof(enviaLinha), 0);

								//Recebe a linha de cima para fazer a conta
								bytes_recv=recv(sock,&recebeLinha,sizeof(recebeLinha),0);
								// Imprime a resposta 
								for(i=1;i<401;i++){
									red[200][i] = recebeLinha[i];
								}
								
								//Recebe a linha de baixo para fazer a conta
								bytes_recv=recv(sock,&recebeLinha,sizeof(recebeLinha),0);
								// Imprime a resposta 
								for(i=1;i<401;i++){
									red[301][i] = recebeLinha[i];
								}

								//cálculos
								for(i=l;i<301;i++){
									for(j=1;j<colunas-1;j++){
										black[i][j] = (red[i][j] + red[i-1][j] + red[i][j+1] + red[i+1][j] + red[i][j-1])/5;
									}
								}
								setRed();
								setBlack();

							}
							//Assume-se que nesta iteração i iremos parar
							parar=1;
							//calcula a diferença dos pontos para checar o critério de parada
							for(i=l;i<301;i++){
								for(j=1;j<colunas-1;j++){
									dif = red[i][j] - black[i][j];
									//Se algum ponto das matrizes tiver diferença >= 0.01, então NÃO PARA
									if( fabs(dif) >= 0.01){
										parar = 0;
									}
								}
							}
							//Máximo
							if(iteracao>=332)
								parar = 1;
							printf("---------PARAR:%d------------------",parar);
							__fpurge(stdin);
							send(sock, &parar, sizeof(parar), 0);
							
							break;
						case 3:
							if((iteracao%2)==0){
								for(i=1;i<401;i++){
									enviaLinha[i] = black[301][i];
								}
								send(sock, &enviaLinha, sizeof(enviaLinha), 0);
								printf("\n LINHA ENVIADA para server = [%.10f], [%.10f]...[%.10f] ",enviaLinha[1],enviaLinha[2],enviaLinha[400]);
								__fpurge(stdin);
								
								//Recebe a linha de baixo para fazer a conta
								bytes_recv=recv(sock,&recebeLinha,sizeof(recebeLinha),0);
								// Imprime a resposta 
								for(i=1;i<401;i++){
									black[300][i] = recebeLinha[i];
								}
								
								//cálculos
								for(i=l;i<401;i++){
									for(j=1;j<colunas-1;j++){
										red[i][j] = (black[i][j] + black[i-1][j] + black[i][j+1] + black[i+1][j] + black[i][j-1])/5;
									}
								}
								setRed();
								setBlack();
								
							}else{
								for(i=1;i<401;i++){
									enviaLinha[i] = red[301][i];
								}
								send(sock, &enviaLinha, sizeof(enviaLinha), 0);
								printf("\n LINHA ENVIADA para server = [%.10f], [%.10f]...[%.10f] ",enviaLinha[1],enviaLinha[2],enviaLinha[400]);
								__fpurge(stdin);
								
								//Recebe a linha de baixo para fazer a conta
								bytes_recv=recv(sock,&recebeLinha,sizeof(recebeLinha),0);
								// Imprime a resposta 
								for(i=1;i<401;i++){
									red[300][i] = recebeLinha[i];
								}
								
								//cálculos
								for(i=l;i<401;i++){
									for(j=1;j<colunas-1;j++){
										black[i][j] = (red[i][j] + red[i-1][j] + red[i][j+1] + red[i+1][j] + red[i][j-1])/5;
									}
								}
								setRed();
								setBlack();

							}
							//Assume-se que nesta iteração i iremos parar
							parar=1;
							//calcula a diferença dos pontos para checar o critério de parada
							for(i=l;i<401;i++){
								for(j=1;j<colunas-1;j++){
									dif = red[i][j] - black[i][j];
									//Se algum ponto das matrizes tiver diferença >= 0.01, então NÃO PARA
									if( fabs(dif) >= 0.01){
										parar = 0;
									}
								}
							}
							//Máximo
							if(iteracao>=335)
								parar = 1;
							printf("---------PARAR:%d------------------",parar);
							__fpurge(stdin);
							send(sock, &parar, sizeof(parar), 0);
							
							
							break;
							
					
					}
					
					break;
				case 8:
					l = 51 + (50*(idCliente-1) );
					bytes_recv=recv(sock,dado_ite,sizeof(dado_ite),0);
					// Imprime a resposta 
					printf("\n Iteracao RECEBIDA = %d ",*dado_ite);
					__fpurge(stdin);
					iteracao = *dado_ite;
					printf("\nIteração de fato: %d\n", iteracao);
					__fpurge(stdin);
						switch (idCliente){
							case 1:
								if((iteracao%2)==0){
									bytes_recv=recv(sock,&recebeLinha,sizeof(recebeLinha),0);
									for(i=1;i<401;i++){
										black[50][i] = recebeLinha[i];
									}
									__fpurge(stdin);
									
									for(i=1;i<401;i++){
										enviaLinha[i] = black[51][i];
									}
									//Envia a black
									send(sock, &enviaLinha, sizeof(enviaLinha), 0);
									printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
									__fpurge(stdin);
									
									for(i=1;i<401;i++){
										enviaLinha[i] = black[100][i];
									}
									//Envia a black
									send(sock, &enviaLinha, sizeof(enviaLinha), 0);
									printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
									__fpurge(stdin);
									
									bytes_recv=recv(sock,&recebeLinha,sizeof(recebeLinha),0);
									for(i=1;i<401;i++){
										black[101][i] = recebeLinha[i];
									}
									__fpurge(stdin);
									
									calculoBlack(101);
									setRed();
									setBlack();
								}else{
									bytes_recv=recv(sock,&recebeLinha,sizeof(recebeLinha),0);
									for(i=1;i<401;i++){
										red[50][i] = recebeLinha[i];
									}
									__fpurge(stdin);
									
									for(i=1;i<401;i++){
										enviaLinha[i] = red[51][i];
									}
									//Envia a red
									send(sock, &enviaLinha, sizeof(enviaLinha), 0);
									printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
									__fpurge(stdin);
									
									for(i=1;i<401;i++){
										enviaLinha[i] = red[100][i];
									}
									//Envia a red
									send(sock, &enviaLinha, sizeof(enviaLinha), 0);
									printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
									__fpurge(stdin);
									
									
									bytes_recv=recv(sock,&recebeLinha,sizeof(recebeLinha),0);
									for(i=1;i<401;i++){
										red[101][i] = recebeLinha[i];
									}
									__fpurge(stdin);
									
									calculoRed(101);
									setRed();
									setBlack();
								}
								//critério de parada
								criterio(101);
								
								break;
							case 2:
								if((iteracao%2)==0){
									for(i=1;i<401;i++){
										enviaLinha[i] = black[101][i];
									}
									//Envia a black
									send(sock, &enviaLinha, sizeof(enviaLinha), 0);
									printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
									__fpurge(stdin);
									for(i=1;i<401;i++){
										enviaLinha[i] = black[150][i];
									}
									//Envia a black
									send(sock, &enviaLinha, sizeof(enviaLinha), 0);
									printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
									__fpurge(stdin);
									bytes_recv=recv(sock,&recebeLinha,sizeof(recebeLinha),0);
									for(i=1;i<401;i++){
										black[100][i] = recebeLinha[i];
									}
									__fpurge(stdin);
									bytes_recv=recv(sock,&recebeLinha,sizeof(recebeLinha),0);
									for(i=1;i<401;i++){
										black[151][i] = recebeLinha[i];
									}
									__fpurge(stdin);
									
									calculoBlack(151);
									setRed();
									setBlack();
								}else{
									for(i=1;i<401;i++){
										enviaLinha[i] = red[101][i];
									}
									//Envia a red
									send(sock, &enviaLinha, sizeof(enviaLinha), 0);
									printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
									__fpurge(stdin);
									
									for(i=1;i<401;i++){
										enviaLinha[i] = red[150][i];
									}
									send(sock, &enviaLinha, sizeof(enviaLinha), 0);
									printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
									__fpurge(stdin);
									
									bytes_recv=recv(sock,&recebeLinha,sizeof(recebeLinha),0);
									for(i=1;i<401;i++){
										red[100][i] = recebeLinha[i];
									}
									__fpurge(stdin);
									bytes_recv=recv(sock,&recebeLinha,sizeof(recebeLinha),0);
									for(i=1;i<401;i++){
										red[151][i] = recebeLinha[i];
									}
									__fpurge(stdin);
									
									calculoRed(151);
									setRed();
									setBlack();
								}
								//critério de parada
								criterio(151);
								break;
							case 3:
								if((iteracao%2)==0){
									for(i=1;i<401;i++){
										enviaLinha[i] = black[151][i];
									}
									//Envia a black
									send(sock, &enviaLinha, sizeof(enviaLinha), 0);
									printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
									__fpurge(stdin);
									
									for(i=1;i<401;i++){
										enviaLinha[i] = black[200][i];
									}
									//Envia a black
									send(sock, &enviaLinha, sizeof(enviaLinha), 0);
									printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
									__fpurge(stdin);
									
									bytes_recv=recv(sock,&recebeLinha,sizeof(recebeLinha),0);
									for(i=1;i<401;i++){
										black[150][i] = recebeLinha[i];
									}
									__fpurge(stdin);
									bytes_recv=recv(sock,&recebeLinha,sizeof(recebeLinha),0);
									for(i=1;i<401;i++){
										black[201][i] = recebeLinha[i];
									}
									__fpurge(stdin);
									calculoBlack(201);
									setRed();
									setBlack();
								}else{
									for(i=1;i<401;i++){
										enviaLinha[i] = red[151][i];
									}
									//Envia a red
									send(sock, &enviaLinha, sizeof(enviaLinha), 0);
									printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
									__fpurge(stdin);
									
									for(i=1;i<401;i++){
										enviaLinha[i] = red[200][i];
									}
									//Envia a black
									send(sock, &enviaLinha, sizeof(enviaLinha), 0);
									printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
									__fpurge(stdin);
									bytes_recv=recv(sock,&recebeLinha,sizeof(recebeLinha),0);
									for(i=1;i<401;i++){
										red[150][i] = recebeLinha[i];
									}
									__fpurge(stdin);
									bytes_recv=recv(sock,&recebeLinha,sizeof(recebeLinha),0);
									for(i=1;i<401;i++){
										red[201][i] = recebeLinha[i];
									}
									__fpurge(stdin);
									
									calculoRed(201);
									setRed();
									setBlack();
								}
								//critério de parada
								criterio(201);
								break;
							case 4:
								if((iteracao%2)==0){
									for(i=1;i<401;i++){
										enviaLinha[i] = black[201][i];
									}
									//Envia a black
									send(sock, &enviaLinha, sizeof(enviaLinha), 0);
									printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
									__fpurge(stdin);
									
									
									for(i=1;i<401;i++){
										enviaLinha[i] = black[250][i];
									}
									//Envia a black
									send(sock, &enviaLinha, sizeof(enviaLinha), 0);
									printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
									__fpurge(stdin);
									
									bytes_recv=recv(sock,&recebeLinha,sizeof(recebeLinha),0);
									for(i=1;i<401;i++){
										black[200][i] = recebeLinha[i];
									}
									__fpurge(stdin);
									bytes_recv=recv(sock,&recebeLinha,sizeof(recebeLinha),0);
									for(i=1;i<401;i++){
										black[251][i] = recebeLinha[i];
									}
									__fpurge(stdin);
									calculoBlack(251);
									setRed();
									setBlack();
								}else{
									for(i=1;i<401;i++){
										enviaLinha[i] = red[201][i];
									}
									//Envia a black
									send(sock, &enviaLinha, sizeof(enviaLinha), 0);
									printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
									__fpurge(stdin);
									
									
									for(i=1;i<401;i++){
										enviaLinha[i] = red[250][i];
									}
									//Envia a black
									send(sock, &enviaLinha, sizeof(enviaLinha), 0);
									printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
									__fpurge(stdin);
									
				
									bytes_recv=recv(sock,&recebeLinha,sizeof(recebeLinha),0);
									for(i=1;i<401;i++){
										red[200][i] = recebeLinha[i];
									}
									__fpurge(stdin);
									bytes_recv=recv(sock,&recebeLinha,sizeof(recebeLinha),0);
									for(i=1;i<401;i++){
										red[251][i] = recebeLinha[i];
									}
									__fpurge(stdin);
									calculoRed(251);
									setRed();
									setBlack();
								}
								//critério de parada
								criterio(251);
								break;
							case 5:
								if((iteracao%2)==0){
									for(i=1;i<401;i++){
										enviaLinha[i] = black[251][i];
									}
									//Envia a black
									send(sock, &enviaLinha, sizeof(enviaLinha), 0);
									printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
									__fpurge(stdin);
									
									for(i=1;i<401;i++){
										enviaLinha[i] = black[300][i];
									}
									//Envia a black
									send(sock, &enviaLinha, sizeof(enviaLinha), 0);
									printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
									__fpurge(stdin);
									bytes_recv=recv(sock,&recebeLinha,sizeof(recebeLinha),0);
									for(i=1;i<401;i++){
										black[250][i] = recebeLinha[i];
									}
									__fpurge(stdin);
									
									bytes_recv=recv(sock,&recebeLinha,sizeof(recebeLinha),0);
									for(i=1;i<401;i++){
										black[301][i] = recebeLinha[i];
									}
									__fpurge(stdin);
									calculoBlack(301);
									setRed();
									setBlack();
								}else{
									for(i=1;i<401;i++){
										enviaLinha[i] = red[251][i];
									}
									//Envia a black
									send(sock, &enviaLinha, sizeof(enviaLinha), 0);
									printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
									__fpurge(stdin);
									
									for(i=1;i<401;i++){
										enviaLinha[i] = red[300][i];
									}
									//Envia a black
									send(sock, &enviaLinha, sizeof(enviaLinha), 0);
									printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
									__fpurge(stdin);
									
									bytes_recv=recv(sock,&recebeLinha,sizeof(recebeLinha),0);
									for(i=1;i<401;i++){
										red[250][i] = recebeLinha[i];
									}
									__fpurge(stdin);
									
									bytes_recv=recv(sock,&recebeLinha,sizeof(recebeLinha),0);
									for(i=1;i<401;i++){
										red[301][i] = recebeLinha[i];
									}
									__fpurge(stdin);
									calculoRed(301);
									setRed();
									setBlack();
								}
								//critério de parada
								criterio(301);
								break;
							case 6:
								if((iteracao%2)==0){
									for(i=1;i<401;i++){
										enviaLinha[i] = black[301][i];
									}
									//Envia a black
									send(sock, &enviaLinha, sizeof(enviaLinha), 0);
									printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
									__fpurge(stdin);
									for(i=1;i<401;i++){
										enviaLinha[i] = black[350][i];
									}
									//Envia a black
									send(sock, &enviaLinha, sizeof(enviaLinha), 0);
									printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
									__fpurge(stdin);
									
									bytes_recv=recv(sock,&recebeLinha,sizeof(recebeLinha),0);
									for(i=1;i<401;i++){
										black[300][i] = recebeLinha[i];
									}
									__fpurge(stdin);
									bytes_recv=recv(sock,&recebeLinha,sizeof(recebeLinha),0);
									for(i=1;i<401;i++){
										black[351][i] = recebeLinha[i];
									}
									calculoBlack(351);
									setRed();
									setBlack();
								}else{
									for(i=1;i<401;i++){
										enviaLinha[i] = red[301][i];
									}
									//Envia a black
									send(sock, &enviaLinha, sizeof(enviaLinha), 0);
									printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
									__fpurge(stdin);
									
									for(i=1;i<401;i++){
										enviaLinha[i] = red[350][i];
									}
									//Envia a black
									send(sock, &enviaLinha, sizeof(enviaLinha), 0);
									printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
									__fpurge(stdin);
									bytes_recv=recv(sock,&recebeLinha,sizeof(recebeLinha),0);
									for(i=1;i<401;i++){
										red[300][i] = recebeLinha[i];
									}
									
									bytes_recv=recv(sock,&recebeLinha,sizeof(recebeLinha),0);
									for(i=1;i<401;i++){
										red[351][i] = recebeLinha[i];
									}
									calculoRed(351);
									setRed();
									setBlack();
								}
								//critério de parada
								criterio(351);
								break;
							case 7:
								if((iteracao%2)==0){
									for(i=1;i<401;i++){
										enviaLinha[i] = black[350][i];
									}
									//Envia a black
									send(sock, &enviaLinha, sizeof(enviaLinha), 0);
									printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
									__fpurge(stdin);
									
									bytes_recv=recv(sock,&recebeLinha,sizeof(recebeLinha),0);
									for(i=1;i<401;i++){
										black[351][i] = recebeLinha[i];
									}
									calculoBlack(401);
									setRed();
									setBlack();
								}else{
									for(i=1;i<401;i++){
										enviaLinha[i] = red[350][i];
									}
									//Envia a black
									send(sock, &enviaLinha, sizeof(enviaLinha), 0);
									printf("\n LINHA ENVIADA para c1 = [%.10f], [%.10f], ... [%.10f], ...[%.10f]",enviaLinha[1],enviaLinha[2],enviaLinha[200],enviaLinha[400]);
									__fpurge(stdin);
									
									
									bytes_recv=recv(sock,&recebeLinha,sizeof(recebeLinha),0);
									for(i=1;i<401;i++){
										red[351][i] = recebeLinha[i];
									}
									calculoRed(401);
									setRed();
									setBlack();
								}
								//critério de parada
								criterio(401);
								break;	
						}
						
					break;	
			}
		
		}		
	}
	
	return 0;
}