#include<stdlib.h>
#include<string.h>
#include<stdio.h>

struct arv {
	char info[100];
	struct arv* esq; // nao
	struct arv* dir; // sim
};

typedef struct arv Arv;

Arv* criarNo(char* info){
    Arv* novoNo = (Arv*) malloc(sizeof(Arv));
    strcpy(novoNo -> info, info);
    novoNo -> esq = NULL;
    novoNo -> dir = NULL;
    return novoNo;
}

int temSubArvores(Arv* a) {
	if (a -> dir == NULL && a -> esq == NULL) return 0;
	return 1;
}

Arv* inserirNovaResposta(Arv* arvore, char* respostaNova, char* pergunta) {
	char respostaAnterior[100];
	strcpy(respostaAnterior, arvore -> info);
	strcpy(arvore -> info, pergunta);
	arvore -> esq = criarNo(respostaNova);
	arvore -> dir = criarNo(respostaAnterior);
	return arvore;
}

Arv* carregarArv(Arv* arvore, FILE* arq) { // Lê a árvore do arquivo
	char info[100];
	int existeSubArv;
	fscanf(arq, "%[^-]-%d\n", info, &existeSubArv);
	arvore = criarNo(info);
	if (existeSubArv) {
		arvore -> esq = carregarArv(arvore -> esq, arq);
		arvore -> dir = carregarArv(arvore -> dir, arq);
	}
	return arvore;
}

void salvarArv(Arv* arvore, FILE* arq) { // Salva no arquivo
	if (temSubArvores(arvore)) {
		fprintf(arq, "%s-1\n", arvore -> info);
		salvarArv(arvore -> esq, arq);
		salvarArv(arvore -> dir, arq);
	}
	else {
		fprintf(arq, "%s-0\n", arvore -> info);
	}
}

Arv* desalocaArv(Arv* a){
	if(!a){
		desalocaArv(a->esq);      
		desalocaArv(a->dir);      
		free(a);
	}
	return NULL;
}
