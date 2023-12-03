#include <stdio.h>
#include <string.h>
#include <conio2.h>
#include <ctype.h>
#include <stdlib.h>

struct tpTree {
	int freq, simbolo;
	struct tpTree *esq, *dir;
};
typedef struct tpTree Tree;

struct tpLista {
	Tree *raiz;
    struct tpLista *prox;
};
typedef struct tpLista TpLista;

struct tpBin {
    int simbolo, freq;
    char palavra[20], huffman[30];
};
typedef struct tpBin TpBin;

Tree *novaCaixaTree(int codPalavra, int freq, Tree *esq, Tree *dir) {
	Tree* novaCaixa = (Tree*)malloc(sizeof(Tree));
	
	novaCaixa->freq = freq;
	novaCaixa->simbolo = codPalavra;
	novaCaixa->esq = esq;
	novaCaixa->dir = dir;
	
	return novaCaixa;
}

TpLista* criaCaixaLista(Tree *folha) {
	TpLista* caixaLista = (TpLista*)malloc(sizeof(TpLista));
    
    caixaLista->raiz = folha;
    caixaLista->prox = NULL;
   	
    return caixaLista;
}


void insereHuffman(char huffman[30], int simbolo) {
	FILE *ptrArq = fopen("codigos.dat", "rb+");
    TpBin reg;
    fread(&reg, sizeof(TpBin), 1, ptrArq);
    while (!feof(ptrArq) && reg.simbolo != simbolo) {
        fread(&reg, sizeof(TpBin), 1, ptrArq);
    }
    strcpy(reg.huffman, huffman);
    fseek(ptrArq, ftell(ptrArq)-sizeof(TpBin), 0);
    fwrite(&reg, sizeof(TpBin), 1, ptrArq);
    fclose(ptrArq);
}

char palavraExiste(FILE *ptrArq, char palavra[30]) {
    TpBin reg;
    rewind(ptrArq);
    while (fread(&reg, sizeof(TpBin), 1, ptrArq) == 1) {
    		limparCaracteresEspeciais(reg.palavra);
        if (stricmp(reg.palavra, palavra) == 0) {
            return 1;
        }
    }
    return 0;
}

int contFreq(char word[30]) {
	char compara[30];
	int qtde = 0;
	FILE *ptrBusca = fopen("glimpse.txt", "r+");
	
	fscanf(ptrBusca, "%s ", compara);
	while(!feof(ptrBusca)) {
		limparCaracteresEspeciais(compara);
		if(stricmp(word, compara) == 0)
			qtde++;
			
		fscanf(ptrBusca, "%s ", compara);
	}
	fclose(ptrBusca);
//	printf("%d\n", qtde);
	return qtde;
}

void limparCaracteresEspeciais(char palavra[]) {
    int i = 0, j = 0;

    while (palavra[i]) {
        if (isalpha(palavra[i])) {
            palavra[j] = palavra[i];
            j++;
        }
        i++;
    }

    palavra[j] = '\0';
}

void insereLista(TpLista **lista, TpLista *caixaLista) {
    if (*lista == NULL || caixaLista->raiz->freq <= (*lista)->raiz->freq) {
	    caixaLista->prox = *lista;
        *lista = caixaLista;     
    }
	else {
        TpLista* atual = *lista;
        while (atual->prox != NULL && caixaLista->raiz->freq >= atual->prox->raiz->freq) {
	        atual = atual->prox;
        }

        caixaLista->prox = atual->prox;
        atual->prox = caixaLista;
    }
}

void montaLista(TpLista **inicio) {
	char word[30];
	TpBin reg;
	TpLista *palavraAtual;
	Tree *caixaArv;
	int codPalavra = 0;
	FILE *ptrArq = fopen("glimpse.txt", "r+");
	FILE *codDat = fopen("codigos.dat", "wb+");

	fscanf(ptrArq, "%s ", word);
	while(!feof(ptrArq)) {
		strlwr(word);
	
		limparCaracteresEspeciais(word);
	
		if(!palavraExiste(codDat, word))
		{
			//insere na lista
			reg.freq = contFreq(word);
			reg.simbolo = codPalavra;
			strcpy(reg.palavra, word);
			fseek(codDat, 0, SEEK_END);
			fwrite(&reg, sizeof(TpBin), 1, codDat);
			
			caixaArv = novaCaixaTree(codPalavra, reg.freq, NULL, NULL);
			palavraAtual = criaCaixaLista(caixaArv);
			insereLista(&*inicio, palavraAtual);
			
			codPalavra++;
		}
		
		fscanf(ptrArq, "%s ", word);
	}
	fclose(ptrArq);
	fclose(codDat);
}

Tree *novaTree(Tree *elem1, Tree *elem2) {
	Tree *novaCaixa = (Tree*)malloc(sizeof(Tree));
	
	novaCaixa->esq = elem1;
	novaCaixa->dir = elem2;
	novaCaixa->freq = elem1->freq + elem2->freq;
	novaCaixa->simbolo = -1;

	return novaCaixa;	
}

Tree* montaArv(TpLista **inicio) {
	Tree *raiz, *elem1, *elem2;
	TpLista *aux;
	Tree *caixaArv;
	TpLista *novaCaixaLista;
	int soma;
	
	while((*inicio)->prox != NULL) {
		elem1 = (*inicio)->raiz;
		aux = *inicio;
		*inicio = aux->prox;
		free(aux);
		
		elem2 = (*inicio)->raiz;
		aux = *inicio;
		*inicio = aux->prox;
		free(aux);
		
		caixaArv = novaCaixaTree(-1, elem1->freq + elem2->freq, elem1, elem2);
		novaCaixaLista = criaCaixaLista(caixaArv);
		insereLista(&*inicio, novaCaixaLista);
	}
	
	return (*inicio)->raiz;
}

void exibeLista(TpLista *pLista) {
	printf("Palavra    Cod    Quantidade\n");
	while(pLista != NULL) {
		
		printf("%d - %d\n", pLista->raiz->simbolo, pLista->raiz->freq);
		pLista = pLista->prox;
	}
}

void exibe(Tree *raiz){
	static int n=-1,i;
	if(raiz!=NULL)
	{
		n++;
		exibe(raiz->dir);
		for(i=0;i<5*n;i++)
			printf(" ");
		printf("(%d, %d)\n",raiz->simbolo,raiz->freq);
		exibe(raiz->esq);
		n--;	
	}			
}

void exibeArq() {
	TpBin reg;
	FILE *ptr = fopen("codigos.dat", "rb");
	
	fread(&reg, sizeof(TpBin), 1, ptr);
	while(!feof(ptr)) {
		printf("%s \t %d \t %d \t %s\n", reg.palavra, reg.simbolo, reg.freq, reg.huffman);
			fread(&reg, sizeof(TpBin), 1, ptr);
	}
	fclose(ptr);
}

void geraHuffman(Tree *raiz, char huffman[30]) {
    if (raiz != NULL) {
    	int len = strlen(huffman);
    	huffman[len] = '1';
    	huffman[len + 1] = '\0';
        geraHuffman(raiz->esq, huffman);    // Percorre a subárvore esquerda
        huffman[len] = '0';
    	huffman[len + 1] = '\0';
        geraHuffman(raiz->dir, huffman);     // Percorre a subárvore direita
    	if(raiz->simbolo != -1) {
			insereHuffman(huffman, raiz->simbolo);
		}
	}
}

void codificarFrase() {
	FILE* ptrArq = fopen("frasedecod.txt", "r");
	FILE* ptrArqBin = fopen("codigos.dat", "rb");
	FILE* ptrArqCod = fopen("frasecod.txt", "w+");
	TpBin reg;
	char palavra[30];
	while(!feof(ptrArq)) {
		fscanf(ptrArq, "%s ", &palavra);
		rewind(ptrArqBin);
		fread(&reg, sizeof(TpBin), 1, ptrArqBin);
		while(!feof(ptrArqBin) && stricmp(reg.palavra, palavra) != 0) {
			fread(&reg, sizeof(TpBin), 1, ptrArqBin);
		}
		fseek(ptrArqBin, ftell(ptrArqBin)-sizeof(TpBin), 0);
		fprintf(ptrArqCod, "%s", reg.huffman);
	}
}

int main() {
	TpLista *inicio = NULL;
	Tree *raiz = NULL;
	montaLista(&inicio);
	//exibeLista(inicio);
	montaArv(&inicio);
	raiz = montaArv(&inicio);
	char huffman[30];
	huffman[0] = '\0';
	geraHuffman(raiz, huffman);
	codificarFrase();
	printf("\n");
	exibeArq();
	//exibe(raiz);
	return 0;
}
