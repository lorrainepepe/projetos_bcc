//Biblioteca com todas as fun��es necess�rias
#include "bpt_functions.h"

int main() {
	setlocale(LC_ALL, "Portuguese");
	//Raiz da �rvore B+
	node* raiz = NULL;
	
	// Prepara a leitura do arquivo do banco de dados (banco.dat)
	FILE* arq = fopen("banco.txt", "r");
	
	//Carregar o banco de dados do sistema
	if(arq != NULL) {
		raiz = carregar_banco(raiz, arq); //Carregar o banco de dados
		fclose(arq);
	}else{
		printf("O banco de dados n�o foi encontrado!\n\n");
		fclose(arq);
	}
	
	CLIENTE* cliente = NULL;
	int op = -1, cod;
	char acao; //controle: 'i' = inserir, 'd' = deletar
  	
	while (op != 0) {
		menu();
  		scanf("%d", &op);
		switch (op) {
			case 1: //Insere um novo cliente na �rvore B+ de acordo com o seu c�digo
				acao = 'i';
				puts("");
				cliente = novo_cliente();
				
				//Salvar no arquivo banco.txt
				salvar_cliente(arq, cliente, acao);
				raiz = insert(raiz, cliente);
				
				puts("\n");
				break;			
				
			case 2: //Remover um cliente
				puts("Insira o c�digo do cliente que deseja remover:");
				scanf("%d", &cod);
				raiz = delete(raiz, cod);
				print_tree(raiz);
				break;
				
			case 3: //Consultar Cliente
				scanf("%d", &cod);
				find_and_print(raiz, cod, 1);
				break;
			
			case 4: //Imprimir folhas
				
				puts("\nC�digo dos clientes cadastrados:\n");
				print_leaves(raiz);
				puts("");
				break;
	
			case 5: //Imprimir a �rvore
				print_tree(raiz);
				puts("\n");
				break;
	
			default:
				puts("Op��o inv�lida!\n");
				break;
			}
	}
	
	printf("\n\nSistema finalizado!\n");
	return 0;
}
