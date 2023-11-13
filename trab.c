#include <stdio.h>
#include <string.h>
#include <conio2.h>
struct tpBin {
    int codPalavra;
    char palavra[20];
    int qtde;
};
typedef struct tpBin TpBin;

struct tpLista {
    struct tpLista *prox;
    int qtde;
    int codPalavra;
    char palavra[20];
};
typedef struct tpLista TpLista;

TpLista* criaCaixa(int codPalavra, int qtde, char palavra[20]) {
    TpLista* novaCaixa = (TpLista*)malloc(sizeof(TpLista));

    novaCaixa->prox = NULL;
    novaCaixa->qtde = qtde;
    strcpy(novaCaixa->palavra, palavra);
   	novaCaixa->codPalavra = codPalavra;
   	
    return novaCaixa;
}

void exibeArq(FILE *ptrArq) {
    TpBin Reg;
    fread(&Reg, sizeof(TpBin), 1, ptrArq);
    while (!feof(ptrArq)) {
        printf("%d - %s - %d\n", Reg.codPalavra, Reg.palavra, Reg.qtde);
        fread(&Reg, sizeof(TpBin), 1, ptrArq);
    }
}

void exibeLista(TpLista *pLista) {
	printf("Palavra    Cod    Quantidade\n");
	while(pLista) {
		printf("%s - %d - %d\n", pLista->palavra,pLista->codPalavra, pLista->qtde);
		pLista = pLista->prox;
	}
}

int retornaMaiorCod() {
	TpBin Reg;
	FILE *ptrArq = fopen("codigos.dat", "rb+");
	fseek(ptrArq, 0, SEEK_END);
	if(ftell(ptrArq) == 0)
		return 0;
	rewind(ptrArq);
	fread(&Reg, sizeof(TpBin), 1, ptrArq);
	while(!feof(ptrArq)) {
		fread(&Reg, sizeof(TpBin), 1, ptrArq);
	}
	fseek(ptrArq, 0, ftell(ptrArq) - sizeof(TpBin));
	fread(&Reg, sizeof(TpBin), 1, ptrArq);
	fclose(ptrArq);
	return Reg.codPalavra;
}

int palavraExiste(FILE *ptrArq, char palavra[20]) {
    TpBin Reg;
    rewind(ptrArq);
    while (fread(&Reg, sizeof(TpBin), 1, ptrArq) == 1) {
        if (stricmp(Reg.palavra, palavra) == 0) {
            return 1;
        }
    }
    return 0;
}

int main(void) {
	TpLista* lista = NULL;
	FILE *ptrArqPrincipal = fopen("glimpse.txt", "r+");
	FILE *ptrArq3 = fopen("codigos.dat", "w+");
	fclose(ptrArq3);
	int codPalavra = retornaMaiorCod() + 1, qtde;
    TpBin Reg;
    char stringPtPrincipal[20], stringPtSecundario[20];
    fscanf(ptrArqPrincipal, "%s ", stringPtPrincipal);
    while (!feof(ptrArqPrincipal)) {
    	qtde = 0;
        FILE *ptrArq = fopen("codigos.dat", "rb+");
        if (!palavraExiste(ptrArq, stringPtPrincipal)) {
        	FILE *ptrArqSecundario = fopen("glimpse.txt", "r+");
			fscanf(ptrArqSecundario, "%s ", stringPtSecundario);
			while(!feof(ptrArqSecundario)) {
				if(stricmp(stringPtPrincipal, stringPtSecundario) == 0)
					qtde++;
				fscanf(ptrArqSecundario, "%s ", stringPtSecundario);
			}
			fclose(ptrArqSecundario);
            fseek(ptrArq, 0, SEEK_END);
            Reg.codPalavra = codPalavra;
            strcpy(Reg.palavra, stringPtPrincipal);
            Reg.qtde = qtde;
            fwrite(&Reg, sizeof(TpBin), 1, ptrArq);
            
            TpLista* aux = lista;
		    TpLista* novaCaixa = criaCaixa(codPalavra, qtde, stringPtPrincipal);
		    if (lista == NULL || qtde >= lista->qtde) {
                novaCaixa->prox = lista;
                lista = novaCaixa;
            } else {
                TpLista* atual = lista;
                while (atual->prox != NULL && qtde < atual->prox->qtde) {
                    atual = atual->prox;
                }
                novaCaixa->prox = atual->prox;
                atual->prox = novaCaixa;
            }
            
            codPalavra++;
        }
        fclose(ptrArq);
        fscanf(ptrArqPrincipal, "%s ", stringPtPrincipal);
    }
    FILE *ptrArq = fopen("codigos.dat", "rb+");
    exibeLista(lista);
    printf("\n");
    exibeArq(ptrArq);
    fclose(ptrArqPrincipal);
    fclose(ptrArq);
}

