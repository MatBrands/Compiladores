#include <stdio.h>
#include <stdlib.h>
#define max 500

struct Tree{
    char token;
    int indice;
}; typedef struct Tree Tree;

FILE *file;
int tam_asa=0, tam_pn=0, tam_pnr=0;

// 
FILE *readFile(char *);
void start(Tree *);
void asa2np(Tree *, char *, char *, int);
// 

int main (int argc, char **argv){
    if(argc == 2){ file = readFile(argv[1]); }
    else{ printf("Carregue os arquivos contendo o input.\n"); exit(1); }

    fscanf(file, "%d\n", &tam_asa);
    Tree asa_tree[tam_asa];
    for (int i=0; i<tam_asa; i++){
        if (i<tam_asa-1){
            fscanf(file, "%d | %c\n", &asa_tree[i].indice, &asa_tree[i].token);
        }
        else{
            fscanf(file, "%d | %c", &asa_tree[i].indice, &asa_tree[i].token);
        }
    }

    fclose(file); file = NULL;

    start(asa_tree);

    return 0;
}

FILE *readFile(char *nome){
    FILE *file;
    file = fopen(nome, "r");
    if(file == NULL){
        printf("Erro ao abrir arquivo.\n");
        exit(-1);
    }
    return file;
}

void start(Tree *asa){
    char polish[tam_asa], reverse_polish[tam_asa];
    for (int i = 0; i < tam_asa; i++) { polish[i] = ' '; reverse_polish[i] = ' '; }

    asa2np(asa, polish, reverse_polish, 0);

    FILE *saida;
    saida = fopen("np", "w");
    fprintf(saida, "%d\n", tam_pn);
    printf("Notacao ponolesa: ");
    for (int i = 0; i < tam_pn-1; i++){ fprintf(saida, "%c\n", polish[i]); printf("[%c], ", polish[i]); }
    fprintf(saida, "%c", polish[tam_pn-1]); printf("[%c]\n", polish[tam_pn-1]);
    fclose(saida); saida = NULL;

    saida = fopen("np_r", "w");
    fprintf(saida, "%d\n", tam_pnr);
    printf("Notacao ponolesa reversa: ");
    for (int i = 0; i < tam_pnr-1; i++){ fprintf(saida, "%c\n", reverse_polish[i]); printf("[%c], ", reverse_polish[i]); }
    fprintf(saida, "%c", reverse_polish[tam_pnr-1]); printf("[%c]\n", reverse_polish[tam_pnr-1]);
    fclose(saida); saida = NULL;
}

void asa2np(Tree *asa, char *polish, char *reverse_polish, int atual){
    if (atual == tam_asa) { return; }
    
    if (!(asa[atual].token == '|')) { polish[tam_pn++] = asa[atual].token; }

    for (int i = atual; i < tam_asa; i++){
        if (asa[atual].indice*2+1 == asa[i].indice){
            asa2np(asa, polish, reverse_polish, i);
            break;
        }
    }

    if (!(asa[atual].token == '|')) { reverse_polish[tam_pnr++] = asa[atual].token; }

    if (atual < tam_asa && asa[atual].indice+1 == asa[atual+1].indice && asa[atual].token != 'm'){
        asa2np(asa, polish, reverse_polish, atual+1);
    }
}