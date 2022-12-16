#include <stdio.h>
#include <stdlib.h>
#define max 500

struct Tree{
    char token;
    int indice;
}; typedef struct Tree Tree;

FILE *file;
int tam_syntax=0, tam_pnr=0, tam_asa=0;

// 
FILE *readFile(char *);
void readSynt(Tree *);
void start_convert(Tree *);
void ada2rpn(Tree *, char *, int);
void rpn2asa(char *, Tree *, int);
// 

int main (int argc, char **argv){
    if(argc == 2){ file = readFile(argv[1]); }
    else{ printf("Carregue os arquivos contendo o input.\n"); exit(1); }

    fscanf(file, "%d\n", &tam_syntax);
    Tree synt_tree[tam_syntax];
    for (int i=0; i<tam_syntax; i++){
        if (i<tam_syntax-1){
            fscanf(file, "%d | %c\n", &synt_tree[i].indice, &synt_tree[i].token);
        }
        else{
            fscanf(file, "%d | %c", &synt_tree[i].indice, &synt_tree[i].token);
        }
    }
    fclose(file); file = NULL;

    start_convert(synt_tree);

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

void start_convert(Tree *syntax){
    char reverse_polish[tam_syntax];
    for (int i = 0; i < tam_syntax; i++) { reverse_polish[i] = ' '; }

    ada2rpn(syntax, reverse_polish, 0);

    printf("Notacao ponolesa reversa: ");
    for (int i = 0; i < tam_pnr; i++){
        printf("%c", reverse_polish[i]);
        if (i<tam_pnr-1){ printf(" | "); }
        else { printf("\n"); }
    }

    Tree abs[tam_pnr];
    for (int i = 0; i < tam_pnr; i++) { abs[i].token = ' '; abs[i].indice = 0; }

    rpn2asa(reverse_polish, abs, 0);

    // for (int i = 0; i< tam_asa; i++){
    //     printf("[%d | %d | %c]", i, abs[i].indice, abs[i].token);
    //     if (i<tam_asa-1){ printf(", "); }
    // }
}

void ada2rpn(Tree *syntax, char *reverse_polish, int atual){
    if (atual == tam_syntax) { return; }

    if (syntax[atual].token == 'S' || syntax[atual].token == 'N' || syntax[atual].token == 'G' || syntax[atual].token == 'M' ||
    syntax[atual].token == 'A' || syntax[atual].token == 'B' || syntax[atual].token == 'E' || syntax[atual].token == 'X' ||
    syntax[atual].token == 'C' || syntax[atual].token == 'D'){
        for (int i = atual; i < tam_syntax; i++){
            if (syntax[atual].indice*12+1 == syntax[i].indice){
                ada2rpn(syntax, reverse_polish, i);
                break;
            }
        }
    }

    else if (syntax[atual].token == 'n' || syntax[atual].token == 'g' || syntax[atual].token == 'm' || syntax[atual].token == 'r') { reverse_polish[tam_pnr++] = syntax[atual].token; }

    else if (syntax[atual].token == ';') { reverse_polish[tam_pnr++] = syntax[atual].token; }

    else if (syntax[atual].token == '0' || syntax[atual].token == '1' || syntax[atual].token == '2' || syntax[atual].token == '3' ||
    syntax[atual].token == '4' || syntax[atual].token == '5' || syntax[atual].token == '6' || syntax[atual].token == '7' ||
    syntax[atual].token == '8' || syntax[atual].token == '9' || syntax[atual].token == 'x' || syntax[atual].token == 'y') { reverse_polish[tam_pnr++] = syntax[atual].token; }

    else if (syntax[atual].token == '+' || syntax[atual].token == '-' || syntax[atual].token == '*' || syntax[atual].token == '/') { reverse_polish[tam_pnr++] = syntax[atual].token; }

    else if (syntax[atual].token == '=' || syntax[atual].token == 'h' || syntax[atual].token == 'i' || syntax[atual].token == 'j' ||
    syntax[atual].token == 'k' || syntax[atual].token == 'z' || syntax[atual].token == 'w' || syntax[atual].token == 'f' ||
    syntax[atual].token == 'o') { reverse_polish[tam_pnr++] = syntax[atual].token; }

    if (atual < tam_syntax && syntax[atual].indice+1 == syntax[atual+1].indice && syntax[atual].token != 'S'){
        ada2rpn(syntax, reverse_polish, atual+1);
    }
}

void rpn2asa(char *reverse_polish, Tree *abs, int atual){
    if (atual == tam_pnr) { return; }
    rpn2asa(reverse_polish, abs, atual+1);
}