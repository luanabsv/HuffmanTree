#include <stdio.h>
#include <string.h>
#include <conio2.h>
#include <ctype.h>
#include <stdlib.h>
#include<windows.h>

struct tpTree {
	int simbolo;
	struct tpTree *esq, *dir;
};
typedef struct tpTree Tree;

struct tpBin {
    int simbolo, freq;
    char palavra[20], huffman[30];
};
typedef struct tpBin TpBin;

void telacheia() {
	keybd_event ( VK_MENU, 0x38, 0, 0 );
	keybd_event ( VK_SPACE, 0x39, 0, 0 );
	keybd_event(0x58,0,0,0);
	keybd_event ( VK_MENU, 0x38, KEYEVENTF_KEYUP, 0 );
	keybd_event ( VK_SPACE, 0x39, KEYEVENTF_KEYUP, 0 );
	keybd_event(0x58,0,KEYEVENTF_KEYUP,0);
}

Tree *novaCaixaTree(int codPalavra, Tree *esq, Tree *dir) {
	Tree* novaCaixa = (Tree*)malloc(sizeof(Tree));

	novaCaixa->simbolo = codPalavra;
	novaCaixa->esq = esq;
	novaCaixa->dir = dir;
	
	return novaCaixa;
}


void exibeArvore(Tree* raiz, int espaco) {
	if(raiz) {
		int i;
		espaco += 5;
	    exibeArvore(raiz->dir, espaco);
	    printf("\n");
	    for (i = 5; i < espaco; i++) {
	        printf(" ");
	    }
	    printf("(%d)\n", raiz->simbolo);
	    exibeArvore(raiz->esq, espaco);	
	}
}

void criaHuffman(Tree **raiz) {
	FILE* ptrArqBin = fopen("codigos.dat", "rb+");
	int i;
	char huffman[10];
	TpBin reg;
	*raiz = novaCaixaTree(-1, NULL, NULL);
	while(!feof(ptrArqBin)) {
		fread(&reg, sizeof(TpBin), 1, ptrArqBin);
		strcpy(huffman, reg.huffman);
		Tree* aux = *raiz;
		i = 0;
		while(huffman[i] != '\0') {
			if(huffman[i] == '1') {
				if(!aux->dir) {
					if(huffman[i + 1] == '\0')
						aux->dir = novaCaixaTree(reg.simbolo, NULL, NULL);
					else
						aux->dir = novaCaixaTree(-1, NULL, NULL);
				}
				aux = aux->dir;
			}
			else {
				if(!aux->esq) {
					if(huffman[i + 1] == '\0')
						aux->esq = novaCaixaTree(reg.simbolo, NULL, NULL);
					else
						aux->esq = novaCaixaTree(-1, NULL, NULL);
				}
				aux = aux->esq;	
			}
			i++;
		}
	}
	fclose(ptrArqBin);
}

void retornaPalavra(int simbolo, char palavra[]) {
	FILE* ptrArqBin = fopen("codigos.dat", "rb");
	TpBin reg;
	fread(&reg, sizeof(TpBin), 1, ptrArqBin);
	while(reg.simbolo != simbolo) {
		fread(&reg, sizeof(TpBin), 1, ptrArqBin);
	}
	strcpy(palavra, reg.palavra);
}

void decodificarFrase(Tree *raiz) {
	FILE* ptrArq = fopen("frasecod.txt", "r");
	char huffman[300], palavra[30];
	int i = 0;
	fscanf(ptrArq, "%s\n", &huffman);
	printf("Frase codificada: %s\n", huffman);
	printf("Frase decodificada: ");
	Tree* aux = raiz;
	while(huffman[i] != '\0') {
		if(aux->simbolo == -1) {
			if(huffman[i] == '1')
				aux = aux->dir;
			else
				aux = aux->esq;
			i++;
		}
		else {
			retornaPalavra(aux->simbolo, palavra);
			printf("%s ", palavra);
			aux = raiz;
		}
	}
	retornaPalavra(aux->simbolo, palavra);
	printf("%s ", palavra);
	fclose(ptrArq);
}

int main(void) {
	telacheia();
	Sleep(1000);
	Tree* raiz = NULL;
	criaHuffman(&raiz);
	decodificarFrase(raiz);
	printf("\n\nEnter para a próxima parte\n\n");
	getch();
	printf("Arvore huffman: \n");
	exibeArvore(raiz, 0);
	
	
}
