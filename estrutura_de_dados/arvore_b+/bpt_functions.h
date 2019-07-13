
/* 							Trabalho de Laboratório de Estruturas de Dados II
 * Grupo 3: Lorraine Maria Pepe, Everton Camacho de Melo, Vinicius Bernardi Vechiato, Gustavo Fernando Balbino
 * Árvore B+ utilizada: http://www.amittai.com/prose/bpt.c  - Último acesso: 25/10/2017
 */
#define Version "1.14"
// Uncomment the line below if you are compiling on Windows.
#define WINDOWS
#include <stdio.h>
#include <stdlib.h>
#include<locale.h>
#include<string.h>
#include <stdbool.h>
#ifdef WINDOWS
#define bool char
#define false 0
#define true 1
#endif

// Order = 5
#define DEFAULT_ORDER 5

// Minimum order is necessarily 3.  We set the maximum
// order arbitrarily.  You may change the maximum order.
#define MIN_ORDER 3
#define MAX_ORDER 20

// Constants for printing part or all of the GPL license.
#define LICENSE_FILE "LICENSE.txt"
#define LICENSE_WARRANTEE 0
#define LICENSE_WARRANTEE_START 592
#define LICENSE_WARRANTEE_END 624
#define LICENSE_CONDITIONS 1
#define LICENSE_CONDITIONS_START 70
#define LICENSE_CONDITIONS_END 625

// TYPES.

/* Tipo representando a gravação a que se refere uma determinada chave.  Em um sistema de árvore B + real, o
 * registro manteria dados (em um banco de dados) ou um arquivo (em um sistema operacional) ou alguma outra
 * informação. Os usuários podem reescrever esta parte do código para alterar o tipo eo conteúdo do campo de valor. */


//Variável global
int linha_arquivo = 0; //Para controlar a inserção de clientes

int id_folha = 1;

typedef struct cliente{
	int codigo;
	char nome[40];
	char telefone[8];
	char endereco[40];
	int idade;
	unsigned int status; //0 ou 1
	int linha_arquivo; //Indica em que linha o cliente estará no arquivo
}CLIENTE;

typedef struct record {
	int value;
}record;

typedef struct node {
	void ** pointers;
	int * keys;
	struct node * parent;
	bool is_leaf;
	int num_keys;
	struct node * next; // Used for queue.
	char nome_folha[20];
	int id_folha;
	int dividiu;
} node;

CLIENTE* buscar_cliente(int codigo, node* folha){
	
	CLIENTE* cli = (CLIENTE*) malloc(sizeof (CLIENTE));
	
	char acao;
	int cod, idade;
	unsigned int status = 1; //Cliente ativo
	char nome[40], telefone[9], endereco[40];
	int linha;	
	
	FILE* arq = fopen("banco.txt", "r");
	
	if(arq == NULL){
		printf("Erro na leitura da folha!");
	}else{
		while(!feof(arq)){
			fscanf(arq,"%d;%c;%d;%[^;];%[^;];%[^;];%d;%d",&linha, &acao, &cod, nome, telefone, endereco, &idade, &status);
			if(cod == codigo){
				cli->codigo = cod;
				strcpy(cli->nome,nome);
				strcpy(cli->telefone,telefone);
				strcpy(cli->endereco,endereco);
				cli->idade = idade;
				cli->status = status;
				cli->linha_arquivo = linha;
				return cli;
			}
		}
		fclose(arq);
		return NULL; // Se não encontrar o cliente no devia folha retorna NULL
	}
}

void salvar_arquivo_folha(node* folha){
	int k;
	CLIENTE* cli = NULL;

	FILE* arq = fopen(folha->nome_folha, "w");
	for(k = 0; k < folha->num_keys; k++){
		cli = buscar_cliente(folha->keys[k], folha);
		fprintf(arq, "Linha: %d| Codigo: %d\t Nome: %s\t Telefone: %s\t Endereco: %s\t Idade: %d\t Status: %d\n", cli->linha_arquivo, cli->codigo ,cli->nome, cli->telefone, cli->endereco, cli->idade, cli->status);
	}
	
	fclose(arq);
}


/*  O tipo acima está representando um nó na árvore B +. Este tipo é geral o suficiente para servir para ambos:
 * a folha e o nó interno. O coração do nó é a matriz das chaves e a matriz de correspondência aos ponteiros. 
 * A relação entre as chaves e ponteiros difere entre folhas e nós internos. Em uma folha, o índice de cada chave
 * é igual ao índice do correspondente ponteiro, com um máximo de ordem - 1 pares de ponteiros-chaves. 
 * O último ponteiro aponta para o folha para a direita (ou NULL no caso da folha mais à direita).
 * Em um nó interno, o primeiro ponteiro refere-se a nós inferiores com chaves menores que a chave mais pequena 
 * na matriz de chaves. Então, com índices i a partir de 0, o ponteiro em 1 + 1 pontos para a subárvore com as chaves
 * maior ou igual à chave nessa nó no índice i. O campo num_keys é usado para manter faixa do número de chaves válidas.
 * Em um nó interno, o número de ponteiros é sempre num_keys + 1. Em uma folha, o número de ponteiros válidos
 * para dados é sempre num_keys. O ponteiro de última folha aponta para a próxima folha.
 */

// Variáveis globais.

/* ORDEM: A ordem determina o máximo e o mínimo número de entradas (chaves e ponteiros) em qualquer nó. 
 * Cada nó possui no máximo uma ordem - 1 chave e pelo menos (grosso modo) metade desse número.
 * Cada folha tem tantos ponteiros quanto dados como chaves, e cada nó interno tem mais um ponteiro 
 * para uma subárvore do que o número de teclas. Esta variável global é inicializada para o valor padrão. 
 * 
 * FILA: A fila é usada para imprimir a árvore em ordem de nível, a partir da raiz imprimir cada classificação 
 * completa em um linha, terminando com as folhas.
 */
 
int order = DEFAULT_ORDER; // ordem
node* queue = NULL; // fila

/* The user can toggle on and off the "verbose"
 * property, which causes the pointer addresses
 * to be printed out in hexadecimal notation
 * next to their corresponding keys.
 */
bool verbose_output = false;


// FUNCTION PROTOTYPES.

// Output and utility.

void license_notice( void );
void print_license( int licence_part );
void usage_1( void );
void usage_2( void );
void usage_3( void );
void enqueue( node * new_node );
node * dequeue( void );
int height( node * root );
int path_to_root( node * root, node * child );
void print_leaves( node * root );
void print_tree( node * root );
void find_and_print(node * root, int key, bool verbose); 
void find_and_print_range(node * root, int range1, int range2, bool verbose); 
int find_range( node * root, int key_start, int key_end, bool verbose,
		int returned_keys[], void * returned_pointers[]); 
node * find_leaf( node * root, int key, bool verbose );
record * find( node * root, int key, bool verbose );
int cut( int length );

// Inserção

record * make_record(int value);
node * make_node( void );
node * make_leaf( void );
int get_left_index(node * parent, node * left);
node * insert_into_leaf( node * leaf, int key, record * pointer );
node * insert_into_leaf_after_splitting(node * root, node * leaf, CLIENTE*, record * pointer);
node * insert_into_node(node * root, node * parent, int left_index, int key, node * right);
node * insert_into_node_after_splitting(node * root, node * parent, int left_index, int key, node * right);
node * insert_into_parent(node * root, node * left, int key, node * right);
node * insert_into_new_root(node * left, int key, node * right);
node * start_new_tree(int key, record * pointer);
node * insert( node * root, CLIENTE* cli);

// Remoção

int get_neighbor_index( node * n );
node * adjust_root(node * root);
node * coalesce_nodes(node * root, node * n, node * neighbor, int neighbor_index, int k_prime);
node * redistribute_nodes(node * root, node * n, node * neighbor, int neighbor_index, int k_prime_index, int k_prime);
node * delete_entry( node * root, node * n, int key, void * pointer );
node * delete( node * root, int key );


// Copyright and license notice to user at startup.
void license_notice( void ) {
	printf("bpt version %s -- Copyright (C) 2010  Amittai Aviram "
			"http://www.amittai.com\n", Version);
	printf("This program comes with ABSOLUTELY NO WARRANTY; for details "
			"type `show w'.\n"
			"This is free software, and you are welcome to redistribute it\n"
			"under certain conditions; type `show c' for details.\n\n");
}

//Routine to print portion of GPL license to stdout.
void print_license( int license_part ) {
	int start, end, line;
	FILE * fp;
	char buffer[0x100];

	switch(license_part) {
	case LICENSE_WARRANTEE:
		start = LICENSE_WARRANTEE_START;
		end = LICENSE_WARRANTEE_END;
		break;
	case LICENSE_CONDITIONS:
		start = LICENSE_CONDITIONS_START;
		end = LICENSE_CONDITIONS_END;
		break;
	default:
		return;
	}

	fp = fopen(LICENSE_FILE, "r");
	if (fp == NULL) {
		perror("print_license: fopen");
		exit(EXIT_FAILURE);
	}
	for (line = 0; line < start; line++)
		fgets(buffer, sizeof(buffer), fp);
	for ( ; line < end; line++) {
		fgets(buffer, sizeof(buffer), fp);
		printf("%s", buffer);
	}
	fclose(fp);
}

// First message to the user.
void usage_1( void ) {
	printf("Árvor B+ de ordem %d.\n", order);
    printf("To build a B+ tree of a different order, start again and enter the order\nas an integer argument:  bpt <order>\n");
	printf("(%d <= order <= %d).\n", MIN_ORDER, MAX_ORDER);
    printf("Para começar com a entrada de um arquivo de inteiros delimitados por nova linha, \ ncomece novamente e digite o pedido seguido pelo nome do arquivo: \ n bpt <order> <inputfile> .\n");
}

void usage_2( void ) {
	printf("Digite um dos seguintes comandos após o prompt> :\n"
	"\ti <k>  -- Inserção <k> (um inteiro) com ambos chave e valor).\n"
	"\tf <k>  -- Encontre o valor abaixo da chave <k>.\n"
	"\tp <k> -- Imprima o caminho da raiz para a chave k e é associado valor.\n"
	"\tr <k1> <k2> -- Imprima as chaves e valores encontrados no intervalo "
			"[<k1>, <k2>\n"
	"\td <k>  -- Eliminar chave <k> e seu valor associado.\n"
	"\tx --Destrua toda a árvore. Comece novamente com uma árvore vazia com a mesma ordem.\n"
	"\tt -- Imprimir a árvore toda.\n"
	"\tl -- Imprima as chaves das folhas (linha inferior da árvore).\n"
	"\tv -- Alternar a saída dos endereços do ponteiro (\ verbose \) na árvore e sai.\n"
	"\tq -- Sair. (Ou use Ctl-D.)\n"
	"\t? -- Mensagem de ajuda.\n");
}

void usage_3( void ) {
	printf("Usage: ./bpt [<order>]\n");
	printf("\tordem:  %d <= ordem <= %d .\n", MIN_ORDER, MAX_ORDER);
}

//Impressão
void enqueue( node * new_node ) {
	node * c;
	if (queue == NULL) {
		queue = new_node;
		queue->next = NULL;
	}
	else {
		c = queue;
		while(c->next != NULL) {
			c = c->next;
		}
		c->next = new_node;
		new_node->next = NULL;
	}
}

node * dequeue( void ) {
	node * n = queue;
	queue = queue->next;
	n->next = NULL;
	return n;
}
/* Imprime a linha inferior das chaves da árvore (com seus respectivos ponteiros, 
	se o sinalizador verbose_output estiver definido. */
void print_leaves( node * root ) {
	int i;
	node * c = root;
	if (root == NULL) {
		printf("Empty tree.\n");
		return;
	}
	while (!c->is_leaf)
		c = c->pointers[0];
	while (true) {
		for (i = 0; i < c->num_keys; i++) {
			if (verbose_output)
				printf("%lx ", (unsigned long)c->pointers[i]);
			printf("%d ", c->keys[i]);
		}
		if (verbose_output)
			printf("%lx ", (unsigned long)c->pointers[order - 1]);
		if (c->pointers[order - 1] != NULL) {
			printf(" | ");
			c = c->pointers[order - 1];
		}
		else
			break;
	}
	printf("\n");
}
/* Função de utilidade para dar a altura da árvore, cujo comprimento em número de ponteiros do caminho da raiz 
para qualquer folha.*/

int height( node * root ) {
	int h = 0;
	node * c = root;
	while (!c->is_leaf) {
		c = c->pointers[0];
		h++;
	}
	return h;
}

int path_to_root( node * root, node * child ) {
	int length = 0;
	node * c = child;
	while (c != root) {
		c = c->parent;
		length++;
	}
	return length;
}

/* Imprime a árvore B + no comando linha no nível (classificação) ordem, com as chaves em cada nó e o '|' símbolo
 * para separar nós. Com o conjunto de sinalizadores verbose_output os valores dos ponteiros correspondentes para 
 * as chaves também aparecem ao lado de suas respectivas chaves, em notação hexadecimal.*/
 
void print_tree( node * root ) {

	node * n = NULL;
	int i = 0;
	int rank = 0;
	int new_rank = 0;

	if (root == NULL) {
		printf("Empty tree.\n");
		return;
	}
	queue = NULL;
	enqueue(root);
	while( queue != NULL ) {
		n = dequeue();
		if (n->parent != NULL && n == n->parent->pointers[0]) {
			new_rank = path_to_root( root, n );
			if (new_rank != rank) {
				rank = new_rank;
				printf("\n");
			}
		}
		if (verbose_output) 
			printf("(%lx)", (unsigned long)n);
		for (i = 0; i < n->num_keys; i++) {
			if (verbose_output)
				printf("%lx ", (unsigned long)n->pointers[i]);
			printf("%d ", n->keys[i]);
		}
		if (!n->is_leaf)
			for (i = 0; i <= n->num_keys; i++)
				enqueue(n->pointers[i]);
		if (verbose_output) {
			if (n->is_leaf) 
				printf("%lx ", (unsigned long)n->pointers[order - 1]);
			else
				printf("%lx ", (unsigned long)n->pointers[n->num_keys]);
		}
		printf("| ");
	}
	printf("\n");
}

//Busca
// Localiza o registro sob uma determinada chave e imprime um mensagem apropriada para stdout.
void find_and_print(node * root, int key, bool verbose) {
	record * r = find(root, key, verbose);
	if (r == NULL)
		printf("Record not found under key %d.\n", key);
	else 
		printf("Record at %lx -- key %d, value %d.\n",
				(unsigned long)r, key, r->value);
}
/* Localiza e imprime as chaves, ponteiros e valores dentro de um intervalo das chaves entre key_start e key_end, 
 * incluindo ambos os limites. */
 
void find_and_print_range( node * root, int key_start, int key_end, bool verbose ) {
	int i;
	int array_size = key_end - key_start + 1;
	int returned_keys[array_size];
	void * returned_pointers[array_size];
	int num_found = find_range( root, key_start, key_end, verbose,
			returned_keys, returned_pointers );
	if (!num_found)
		printf("None found.\n");
	else {
		for (i = 0; i < num_found; i++)
			printf("Key: %d   Location: %lx  Value: %d\n",
					returned_keys[i],
					(unsigned long)returned_pointers[i],
					((record *)
					 returned_pointers[i])->value);
	}
}

/* Localiza as chaves e os ponteiros, se presentes, no intervalo especificado
  * por key_start e key_end, inclusive. Coloca estes nos arrays return_keys e devolvido_pointers, e retorna o número de
  * entradas encontradas. */

int find_range( node * root, int key_start, int key_end, bool verbose, int returned_keys[], void * returned_pointers[]) {
	int i, num_found;
	num_found = 0;
	node * n = find_leaf( root, key_start, verbose );
	if (n == NULL) return 0;
	for (i = 0; i < n->num_keys && n->keys[i] < key_start; i++) ;
	if (i == n->num_keys) return 0;
	while (n != NULL) {
		for ( ; i < n->num_keys && n->keys[i] <= key_end; i++) {
			returned_keys[num_found] = n->keys[i];
			returned_pointers[num_found] = n->pointers[i];
			num_found++;
		}
		n = n->pointers[order - 1];
		i = 0;
	}
	return num_found;
}

/* Traça o caminho da raiz para uma folha, procurando por chave. Exibe informações sobre o caminho se o 
sinalizador detalhado estiver definido. Retorna a folha contendo a chave dada. */
node * find_leaf( node * root, int key, bool verbose ) {
	int i = 0;
	node * c = root;
	if (c == NULL) {
		if (verbose) 
			printf("Empty tree.\n");
		return c;
	}
	while (!c->is_leaf) {
		if (verbose) {
			printf("[");
			for (i = 0; i < c->num_keys - 1; i++)
				printf("%d ", c->keys[i]);
			printf("%d] ", c->keys[i]);
		}
		i = 0;
		while (i < c->num_keys) {
			if (key >= c->keys[i]) i++;
			else break;
		}
		if (verbose)
			printf("%d ->\n", i);
		c = (node *)c->pointers[i];
	}
	if (verbose) {
		printf("Leaf [");
		for (i = 0; i < c->num_keys - 1; i++)
			printf("%d ", c->keys[i]);
		printf("%d] ->\n", c->keys[i]);
	}
	return c;
}
// Localiza e retorna o registro ao qual uma chave se refere.
record * find( node * root, int key, bool verbose ) {
	int i = 0;
	node * c = find_leaf( root, key, verbose );
	if (c == NULL) return NULL;
	for (i = 0; i < c->num_keys; i++)
		if (c->keys[i] == key) break;
	if (i == c->num_keys) 
		return NULL;
	else
		return (record *)c->pointers[i];
}

// Localiza o local apropriado para dividir um nó muito grande em 2.
int cut( int length ) {
	if (length % 2 == 0)
		return length/2;
	else
		return length/2 + 1;
}


// INSERÇÃO
// Cria um novo registro para manter o valor a que se refere uma chave.
record * make_record(int value) {
	record * new_record = (record *)malloc(sizeof(record));
	if (new_record == NULL) {
		perror("Record creation.");
		exit(EXIT_FAILURE);
	}
	else {
		new_record->value = value;
	}
	return new_record;
}
// Cria um novo nó geral, que pode ser adaptado para servir como uma folha ou um nó interno.
node * make_node( void ) {
	node * new_node;
	new_node = malloc(sizeof(node));
	if (new_node == NULL) {
		perror("Node creation.");
		exit(EXIT_FAILURE);
	}
	new_node->keys = malloc( (order - 1) * sizeof(int) );
	if (new_node->keys == NULL) {
		perror("New node keys array.");
		exit(EXIT_FAILURE);
	}
	new_node->pointers = malloc( order * sizeof(void *) );
	if (new_node->pointers == NULL) {
		perror("New node pointers array.");
		exit(EXIT_FAILURE);
	}
	new_node->is_leaf = false; //é raiz
	new_node->num_keys = 0;
	new_node->parent = NULL;
	new_node->next = NULL;
	new_node->dividiu = 0;
	return new_node;
}
// Cria uma nova folha criando um nó e, em seguida, adaptá-lo adequadamente.
node * make_leaf( void ) {
	char converte[10], nome_folha[20];
	
	node * leaf = make_node();
	leaf->is_leaf = true;
	leaf->id_folha = id_folha;
	
	//Manipular o nome do arquivo.txt
	itoa(leaf->id_folha,converte,10);
	strcpy(nome_folha,"folha_");
	strcat(nome_folha, converte);
	strcat(nome_folha,".txt");
	strcpy(leaf->nome_folha, nome_folha);
	
	id_folha++;
	return leaf;
}

/*Função auxiliar usada em insert_into_parent  para encontrar o índice do ponteiro dos pais para
  o nó à esquerda da chave a ser inserida.*/
int get_left_index(node * parent, node * left) {
	int left_index = 0;
	while (left_index <= parent->num_keys && 
			parent->pointers[left_index] != left)
		left_index++;
	return left_index;
}

/* Insere um novo ponteiro para um registro e é correspondente chave em uma folha. Retorna a folha alterada. */
node * insert_into_leaf( node * leaf, int key, record * pointer ) {
	int i, insertion_point;

	insertion_point = 0;
	while (insertion_point < leaf->num_keys && leaf->keys[insertion_point] < key)
		insertion_point++;

	for (i = leaf->num_keys; i > insertion_point; i--) {
		leaf->keys[i] = leaf->keys[i - 1];
		leaf->pointers[i] = leaf->pointers[i - 1];
	}
	leaf->keys[insertion_point] = key;
	leaf->pointers[insertion_point] = pointer;
	leaf->num_keys++;
	return leaf;
}

/* Insere uma nova chave e ponteiro para um novo registro em uma folha de modo a exceder
 * a ordem da árvore, fazendo com que a folha seja dividida ao meio.*/
node * insert_into_leaf_after_splitting(node * root, node * leaf, CLIENTE* cli, record * pointer) {
	node * new_leaf;
	int * temp_keys;
	void ** temp_pointers;
	int insertion_index, split, new_key, i, j;

	new_leaf = make_leaf();

	temp_keys = malloc( order * sizeof(int) );
	if (temp_keys == NULL) {
		perror("Temporary keys array.");
		exit(EXIT_FAILURE);
	}

	temp_pointers = malloc( order * sizeof(void *) );
	if (temp_pointers == NULL) {
		perror("Temporary pointers array.");
		exit(EXIT_FAILURE);
	}

	insertion_index = 0;
	while (insertion_index < order - 1 && leaf->keys[insertion_index] < cli->codigo)
		insertion_index++;

	for (i = 0, j = 0; i < leaf->num_keys; i++, j++) {
		if (j == insertion_index) j++;
		temp_keys[j] = leaf->keys[i];
		temp_pointers[j] = leaf->pointers[i];
	}

	temp_keys[insertion_index] = cli->codigo;
	temp_pointers[insertion_index] = pointer;

	leaf->num_keys = 0;
	leaf->dividiu = 1; //Flag para controlar a mudança de status ao dividir uma determinada folha
	
	split = cut(order - 1);

	for (i = 0; i < split; i++) {
		leaf->pointers[i] = temp_pointers[i];
		leaf->keys[i] = temp_keys[i];
		leaf->num_keys++;
	}

	for (i = split, j = 0; i < order; i++, j++) {
		new_leaf->pointers[j] = temp_pointers[i];
		new_leaf->keys[j] = temp_keys[i];
		new_leaf->num_keys++;
	}

	free(temp_pointers);
	free(temp_keys);

	new_leaf->pointers[order - 1] = leaf->pointers[order - 1];
	leaf->pointers[order - 1] = new_leaf;

	for (i = leaf->num_keys; i < order - 1; i++){
		leaf->pointers[i] = NULL;
	}
	for (i = new_leaf->num_keys; i < order - 1; i++){
		new_leaf->pointers[i] = NULL;
	}

	new_leaf->parent = leaf->parent;
	new_key = new_leaf->keys[0];
	return insert_into_parent(root, leaf, new_key, new_leaf);
}

/* Insere uma nova chave e ponteiro em um nó em que estes caibam
 * sem violar as propriedades da árvore B +.*/ 
node * insert_into_node(node * root, node * n, int left_index, int key, node * right) {
	int i;

	for (i = n->num_keys; i > left_index; i--) {
		n->pointers[i + 1] = n->pointers[i];
		n->keys[i] = n->keys[i - 1];
	}
	n->pointers[left_index + 1] = right;
	n->keys[left_index] = key;
	n->num_keys++;
	return root;
}
/* Insere uma nova chave e ponteiro, fazendo com que o tamanho do nó exceda
 * a ordem, e fazendo com que o nó se divide em dois.*/
node * insert_into_node_after_splitting(node * root, node * old_node, int left_index, int key, node * right) {

	int i, j, split, k_prime;
	node * new_node, * child;
	int * temp_keys;
	node ** temp_pointers;
	
/* Primeiro crie um conjunto temporário de chaves e ponteiros para manter tudo em ordem, incluindo
 * a nova chave e ponteiro, inseridos em lugares corretos.
 * Em seguida, crie um novo nó e copie metade do chaves e ponteiros para o nó antigo e
 * a outra metade para o novo.*/
 
	temp_pointers = malloc( (order + 1) * sizeof(node *) );
	if (temp_pointers == NULL) {
		perror("Temporary pointers array for splitting nodes.");
		exit(EXIT_FAILURE);
	}
	temp_keys = malloc( order * sizeof(int) );
	if (temp_keys == NULL) {
		perror("Temporary keys array for splitting nodes.");
		exit(EXIT_FAILURE);
	}

	for (i = 0, j = 0; i < old_node->num_keys + 1; i++, j++) {
		if (j == left_index + 1) j++;
		temp_pointers[j] = old_node->pointers[i];
	}

	for (i = 0, j = 0; i < old_node->num_keys; i++, j++) {
		if (j == left_index) j++;
		temp_keys[j] = old_node->keys[i];
	}

	temp_pointers[left_index + 1] = right;
	temp_keys[left_index] = key;

	/* Crie o novo nó e copie metade das chaves e ponteiros para o antigo e meio para o novo. */
	split = cut(order);
	new_node = make_node();
	old_node->num_keys = 0;
	for (i = 0; i < split - 1; i++) {
		old_node->pointers[i] = temp_pointers[i];
		old_node->keys[i] = temp_keys[i];
		old_node->num_keys++;
		printf("\n@@@kEY: %d\n", old_node->keys[i]);
	}
	old_node->pointers[i] = temp_pointers[i];
	k_prime = temp_keys[split - 1];
	for (++i, j = 0; i < order; i++, j++) {
		new_node->pointers[j] = temp_pointers[i];
		new_node->keys[j] = temp_keys[i];
		new_node->num_keys++;
		printf("\n@@@@kEY: %d\n", new_node->keys[i]);
	}
	new_node->pointers[j] = temp_pointers[i];
	free(temp_pointers);
	free(temp_keys);
	new_node->parent = old_node->parent;
	for (i = 0; i <= new_node->num_keys; i++) {
		child = new_node->pointers[i];
		child->parent = new_node;
	}

	/* Insira uma nova chave no pai dos dois nós resultantes da divisão, com
	 * o nó antigo à esquerda e o novo à direita */
	return insert_into_parent(root, old_node, k_prime, new_node);
}


/* Insere um novo nó (folha ou nó interno) na árvore B +.
 * Retorna a raiz da árvore após a inserção. */
node * insert_into_parent(node * root, node * left, int key, node * right) {

	int left_index;
	node * parent;

	parent = left->parent;

	// Caso: nova raiz . 
	if (parent == NULL)
		return insert_into_new_root(left, key, right);

	// Caso: folha ou nó -> resto do corpo da função 
	 
	// Encontra os pais dos ponteiros a esquerda do nó
	left_index = get_left_index(parent, left);

	// Caso simples: A nova chave se encaixa no nó. 
	if (parent->num_keys < order - 1)
		return insert_into_node(root, parent, left_index, key, right);

	// Caso mais difícil: dividir um nó em ordem para preservar as propriedades da árvore B +.
	return insert_into_node_after_splitting(root, parent, left_index, key, right);
}


// Cria uma nova raiz para duas subveres e insere a chave apropriada para a nova raiz.
node * insert_into_new_root(node * left, int key, node * right) {

	node * root = make_node();
	root->keys[0] = key;
	root->pointers[0] = left;
	root->pointers[1] = right;
	root->num_keys++;
	root->parent = NULL;
	left->parent = root;
	right->parent = root;

	return root;
}

// Primeira inserção: iniciar uma nova árvore.
node * start_new_tree(int key, record * pointer) {
	
	node * root = make_leaf();
	root->keys[0] = key;
	root->pointers[0] = pointer;
	root->pointers[order - 1] = NULL;
	root->parent = NULL;
	root->num_keys++;
	
	salvar_arquivo_folha(root);
	return root;	
}
/* Função de inserção mestre. Insere uma chave e um valor associado para a árvore B +, 
 * fazendo com que a árvore seja ajustada.*/
node * insert( node * root, CLIENTE* cli) {
	record * pointer;
	node * leaf;
	node* raiz = NULL;
	node* aux = NULL;
	node* raiz_antiga = root;
	int i;

	// A implementação atual ignora duplicatas.
	if (find(root, cli->codigo, false) != NULL)
		return root;

	// Crie um novo registro para o valor.
	pointer = make_record(cli->codigo);

	// Caso: a árvore ainda não existe. Inicie uma nova árvore.
	if (root == NULL) 
		return start_new_tree(cli->codigo, pointer);

	// Caso: a árvore já existe. (Resto do corpo da função.)
	leaf = find_leaf(root, cli->codigo, false);

	// Caso: a folha tem espaço para chave e ponteiro.
	if (leaf->num_keys < order - 1) {
		leaf = insert_into_leaf(leaf, cli->codigo, pointer);
		salvar_arquivo_folha(leaf);
		return root;
	}
	
	//Caso: a folha deve ser dividida.
	raiz = insert_into_leaf_after_splitting(root, leaf, cli, pointer);
	aux = raiz;
	
	//Verificar os novos apontamentos para salvar corretamente as folhas.
	while (!aux->is_leaf){
		aux = aux->pointers[0];
	}
	while (1) {
		if(aux) salvar_arquivo_folha(aux);
		
		if (aux->pointers[order - 1] != NULL) aux = aux->pointers[order - 1];
		else break;
	}
	
	return raiz;
}

// REMOÇÃO

 /* Função de utilidade para eliminação. Recupera o índice do vizinho mais próximo de um nó (irmão)
  * à esquerda, se houver. Caso contrário (o nó é o filho mais à esquerda), retorna -1 para significar este caso especial.*/

int get_neighbor_index( node * n ) {

	int i;
 	/* Retorna o índice da chave à esquerda do ponteiro no pai apontando para n.
  	 * Se n é o filho mais à esquerda, isso significa retornar -1 */
	for (i = 0; i <= n->parent->num_keys; i++)
		if (n->parent->pointers[i] == n)
			return i - 1;

	//Estado de erro
	printf("Search for nonexistent pointer to node in parent.\n");
	printf("Node:  %#lx\n", (unsigned long)n);
	exit(EXIT_FAILURE);
}


node * remove_entry_from_node(node * n, int key, node * pointer) {
	int i, num_pointers;

	//Remova a chave e mude outras chaves de acordo.
	i = 0;
	while (n->keys[i] != key)
		i++;
	for (++i; i < n->num_keys; i++)
		n->keys[i - 1] = n->keys[i];

	// Remova o ponteiro e mude outros ponteiros de acordo.
	// Primeiro determine o número de ponteiros.
	num_pointers = n->is_leaf ? n->num_keys : n->num_keys + 1;
	i = 0;
	while (n->pointers[i] != pointer)
		i++;
	for (++i; i < num_pointers; i++)
		n->pointers[i - 1] = n->pointers[i];


	// Uma chave a menos
	n->num_keys--;

	// Defina os outros ponteiros para NULL para arrumação.
	// Uma folha usa o último ponteiro para apontar para a próxima folha.
	if (n->is_leaf)
		for (i = n->num_keys; i < order - 1; i++)
			n->pointers[i] = NULL;
	else
		for (i = n->num_keys + 1; i < order; i++)
			n->pointers[i] = NULL;

	return n;
}


node * adjust_root(node * root) {

	node * new_root;

	// Caso: raiz não vazia. A chave eo ponteiro já foram excluídos, então não há nada a ser feito.

	if (root->num_keys > 0)
		return root;

	//Caso: raiz vazia.
	// Se tem um filho, promova o primeiro (único) filho como a nova raiz.

	if (!root->is_leaf) {
		new_root = root->pointers[0];
		new_root->parent = NULL;
	}

	// Se é uma folha (não tem filhos), então toda a árvore está vazia.

	else
		new_root = NULL;

	free(root->keys);
	free(root->pointers);
	free(root);

	return new_root;
}


	/*Junta(aglutina) um nó que se tornou muito pequeno após a eliminação com um nó vizinho que possa aceitar as entradas adicionais
   * sem exceder o máximo. */

node * coalesce_nodes(node * root, node * n, node * neighbor, int neighbor_index, int k_prime) {

	int i, j, neighbor_insertion_index, n_end;
	node * tmp;

	// Troque o vizinho com o nó se o nó estiver no extrema esquerda e vizinha está à direita.
	if (neighbor_index == -1) {
		tmp = n;
		n = neighbor;
		neighbor = tmp;
	}

	/* Ponto de partida no vizinho para copiar chaves e ponteiros de n.
	 * Lembre-se de que n e vizinho trocaram lugares  no caso especial de n ser um filho mais à esquerda.*/

	neighbor_insertion_index = neighbor->num_keys;

	/* Caso: nó de folha não orientada.
     * Anexe o k_prime e o seguinte ponteiro. Anexar todos os ponteiros e as chaves do vizinho.*/

	if (!n->is_leaf) {

		// Anexe o k_prime.

		neighbor->keys[neighbor_insertion_index] = k_prime;
		neighbor->num_keys++;

		n_end = n->num_keys;

		for (i = neighbor_insertion_index + 1, j = 0; j < n_end; i++, j++) {
			neighbor->keys[i] = n->keys[j];
			neighbor->pointers[i] = n->pointers[j];
			neighbor->num_keys++;
			n->num_keys--;
		}

		 //O número de ponteiros é sempre mais do que o número de chaves.

		neighbor->pointers[i] = n->pointers[j];

		//Todas os filhos agora devem apontar para o mesmo pai.

		for (i = 0; i < neighbor->num_keys + 1; i++) {
			tmp = (node *)neighbor->pointers[i];
			tmp->parent = neighbor;
		}
	}

	/* Em uma folha, anexe as chaves e os ponteiros de n para o vizinho.
	 * Defina o último ponteiro do vizinho para apontar para o que tinha sido o vizinho direito de n.*/

	else {
		for (i = neighbor_insertion_index, j = 0; j < n->num_keys; i++, j++) {
			neighbor->keys[i] = n->keys[j];
			neighbor->pointers[i] = n->pointers[j];
			neighbor->num_keys++;
		}
		neighbor->pointers[order - 1] = n->pointers[order - 1];
	}

	root = delete_entry(root, n->parent, k_prime, n);
	free(n->keys);
	free(n->pointers);
	free(n); 
	return root;
}

/* Redistribui entradas entre dois nós quando uma tornou-se muito pequena após a eliminação, mas o seu vizinho é muito grande
 * para acrescentar o entradas de nó pequeno sem exceder o máximo */
node * redistribute_nodes(node * root, node * n, node * neighbor, int neighbor_index, 
		int k_prime_index, int k_prime) {  

	int i;
	node * tmp;

	//Caso: n tem um vizinho à esquerda. Puxe o último par de chave-ponteiro do vizinho do lado direito do vizinho até o final esquerdo do n.

	if (neighbor_index != -1) {
		if (!n->is_leaf)
			n->pointers[n->num_keys + 1] = n->pointers[n->num_keys];
		for (i = n->num_keys; i > 0; i--) {
			n->keys[i] = n->keys[i - 1];
			n->pointers[i] = n->pointers[i - 1];
		}
		if (!n->is_leaf) {
			n->pointers[0] = neighbor->pointers[neighbor->num_keys];
			tmp = (node *)n->pointers[0];
			tmp->parent = n;
			neighbor->pointers[neighbor->num_keys] = NULL;
			n->keys[0] = k_prime;
			n->parent->keys[k_prime_index] = neighbor->keys[neighbor->num_keys - 1];
		}
		else {
			n->pointers[0] = neighbor->pointers[neighbor->num_keys - 1];
			neighbor->pointers[neighbor->num_keys - 1] = NULL;
			n->keys[0] = neighbor->keys[neighbor->num_keys - 1];
			n->parent->keys[k_prime_index] = n->keys[0];
		}
	}

	 /* Caso: n é o filho mais à esquerda. Pegue um par de chaveiro do vizinho para a direita.
	  * Mova o par de ponteiro-chave mais à esquerda do vizinho para a posição mais à direita de n. */

	else {  
		if (n->is_leaf) {
			n->keys[n->num_keys] = neighbor->keys[0];
			n->pointers[n->num_keys] = neighbor->pointers[0];
			n->parent->keys[k_prime_index] = neighbor->keys[1];
		}
		else {
			n->keys[n->num_keys] = k_prime;
			n->pointers[n->num_keys + 1] = neighbor->pointers[0];
			tmp = (node *)n->pointers[n->num_keys + 1];
			tmp->parent = n;
			n->parent->keys[k_prime_index] = neighbor->keys[0];
		}
		for (i = 0; i < neighbor->num_keys - 1; i++) {
			neighbor->keys[i] = neighbor->keys[i + 1];
			neighbor->pointers[i] = neighbor->pointers[i + 1];
		}
		if (!n->is_leaf)
			neighbor->pointers[i] = neighbor->pointers[i + 1];
	}

	// n agora tem mais uma chave e mais um ponteiro, o vizinho tem um menos de cada um.
	
	n->num_keys++;
	neighbor->num_keys--;

	return root;
}


/* Apaga uma entrada da árvore B +.  Remove o registro e sua chave e ponteiro a partir da folha, e depois faz todas as 
 *alterações apropriadas para preservar as propriedades da árvore B +. */
node * delete_entry( node * root, node * n, int key, void * pointer ) {

	int min_keys;
	node * neighbor;
	int neighbor_index;
	int k_prime_index, k_prime;
	int capacity;

// Remova a chave eo ponteiro do nó.

	n = remove_entry_from_node(n, key, pointer);

	//Caso: exclusão da raiz

	if (n == root) 
		return adjust_root(root);


	// Caso: exclusão de um nó abaixo da raiz. (Resto do corpo da função.)
	
	// Determine o tamanho mínimo permitido de nó, para ser preservado após a eliminação. */

	min_keys = n->is_leaf ? cut(order - 1) : cut(order) - 1;

	//Caso: o nó permanece em ou acima do mínimo. (O caso simples.)
	if (n->num_keys >= min_keys)
		return root;

	/* Caso: o nó cai abaixo do mínimo. Tanto aglutinação quanto redistribuição é preciso. */

	/* Encontre o nó vizinho apropriado com o qual unir-se. Também encontre a chave (k_prime) no pai
	 * entre o ponteiro para o nó n e o ponteiro para o vizinho. */

	neighbor_index = get_neighbor_index( n );
	k_prime_index = neighbor_index == -1 ? 0 : neighbor_index;
	k_prime = n->parent->keys[k_prime_index];
	neighbor = neighbor_index == -1 ? n->parent->pointers[1] : 
		n->parent->pointers[neighbor_index];

	capacity = n->is_leaf ? order : order - 1;

	// Junção (aglutinação).

	if (neighbor->num_keys + n->num_keys < capacity)
		return coalesce_nodes(root, n, neighbor, neighbor_index, k_prime);

	// Redistribuição:

	else
		return redistribute_nodes(root, n, neighbor, neighbor_index, k_prime_index, k_prime);
}



// Função de exclusão do mestre.
node * delete(node * root, int key) {

	node * key_leaf;
	record * key_record;

	key_record = find(root, key, false);
	key_leaf = find_leaf(root, key, false);
	if (key_record != NULL && key_leaf != NULL) {
		root = delete_entry(root, key_leaf, key, key_record);
		free(key_record);
	}
	return root;
}


void destroy_tree_nodes(node * root) {
	int i;
	if (root->is_leaf)
		for (i = 0; i < root->num_keys; i++)
			free(root->pointers[i]);
	else
		for (i = 0; i < root->num_keys + 1; i++)
			destroy_tree_nodes(root->pointers[i]);
	free(root->pointers);
	free(root->keys);
	free(root);
}


node * destroy_tree(node * root) {
	destroy_tree_nodes(root);
	return NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//*********************************************************************************************************************

void menu(){
	
	printf("|*****************************|\n");
	printf("|            MENU             |\n");
	printf("| 1) Cadastrar cliente        |\n");
	printf("| 2) Remover cliente          |\n");
	printf("| 3) Consultar cliente        |\n");
	printf("| 4) Exibir lista de clientes |\n");
	printf("|*****************************|\n");
	printf("| 0) Sair do programa         |\n");
	printf("|*****************************|\n");
	puts("\nInforme a opção desejada:");
	
}



CLIENTE* novo_cliente(){
	
	CLIENTE* novo_cliente = (CLIENTE*) malloc(sizeof (CLIENTE));
	CLIENTE* cliente = NULL;
	
	int codigo, idade;
	unsigned int status = 1; //Cliente ativo
	char nome[40], telefone[9], endereco[40];
	
	printf("Preencha os campos abaixo com os dados do cliente: \n");
	while(1){
		printf("\nCódigo do cliente: ");
		scanf("%d", &codigo);
		//cliente = buscar_cliente(codigo);
		//if(cliente) printf("\nO código informado já existe. Digite outro código!\n");
		break;
	}
	
	fflush(stdin);
	
	printf("\nNome: ");
	scanf("%[^\n]", nome);
	fflush(stdin);

	printf("\nTelefone: ");
	scanf("%[^\n]", telefone);
	fflush(stdin);
	
	printf("\nEndereço: ");
	scanf("%[^\n]", endereco);
	fflush(stdin);
	
	printf("\nInforme a idade do cliente: ");
	scanf("%d", &idade);	
		
	novo_cliente->codigo = codigo;
	strcpy(novo_cliente->nome,nome);
	strcpy(novo_cliente->telefone,telefone);
	strcpy(novo_cliente->endereco,endereco);
	novo_cliente->idade = idade;
	novo_cliente->status = status;
	novo_cliente->linha_arquivo = linha_arquivo;
	linha_arquivo++;
	
	return novo_cliente;
}

void salvar_cliente(FILE* arq, CLIENTE* cli, char acao){

	int codigo, idade;
	char nome[40], telefone[8], endereco[40];
	unsigned int status; //0 ou 1
		
	arq = fopen("banco.txt", "a");
	
	if(arq == NULL){
		printf("Erro!");
		system("pause");
		exit(1);
	}else{
		fprintf(arq, "%d;%c;%d;%s;%s;%s;%d\n", cli->linha_arquivo, acao, cli->codigo, cli->nome, cli->telefone, cli->endereco, cli->idade);
	}
	
	fclose(arq);	
}

//Carregar banco de dados
node* carregar_banco(node* root, FILE* arq){
	
	CLIENTE* novo_cliente = (CLIENTE*)malloc(sizeof(CLIENTE));
	int codigo, idade;
	char nome[40], telefone[8], endereco[40];
	unsigned int status; //0 ou 1
	char acao; //'i': inserir ou 'd' deletar
	
	printf("Carregando banco de dados...");
	sleep(1);
	system("cls");
		
	while(fscanf(arq,"%d;%c;%d;%[^;];%[^;];%[^;];%d;%d\n",&linha_arquivo, &acao, &codigo, nome, telefone, endereco, &idade, &status) != EOF){
			novo_cliente->codigo = codigo;
			strcpy(novo_cliente->nome, nome);
			strcpy(novo_cliente->telefone, telefone);
			strcpy(novo_cliente->endereco, endereco);
			novo_cliente->idade = idade; 
			novo_cliente->status = status;
			novo_cliente->linha_arquivo = linha_arquivo;
			linha_arquivo++;
			printf(">> Cliente linha %d carregado com sucesso! <<\n", novo_cliente->linha_arquivo);
			
			if(acao == 'i') {
				root = insert(root, novo_cliente);
			}/*else if(acao == 'd'){
				
			}*/
	}

	return root;
}

//Em teste
void criar_arq_visualizar(CLIENTE* cli){
	
	FILE* arq = fopen("todos_registros.txt", "r");

	if(arq  == NULL){
		arq  = fopen("todos_registros.txt", "w");
	}else arq  = fopen("todos_registros.txt", "a");
	
	fprintf(arq, "Linha: %d| Codigo: %d\n\tNome: %s\tTelefone: %s\tEndereco: %s\n\tIdade: %d\n\tStatus: %d\n", cli->linha_arquivo, cli->codigo ,cli->nome, cli->telefone, cli->endereco, cli->idade, cli->status);

	fclose(arq);
}


