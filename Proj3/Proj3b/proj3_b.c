#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Syntax{
    char token;
    int indice;
}; typedef struct Syntax Syntax;

struct Tree{
    char token;
    int visitado, filhos, indice, hash_original;
}; typedef struct Tree Tree;

// 
FILE *file;
int tam_syntax, fim_tree, tam_pnr, tam_pn;

// 
FILE *readFile(char *);
void readSynt(Syntax *);
void start(Syntax *);
void abs_tree(Syntax *, Tree *, int);
void put_token(Syntax *, Tree *, int);
void convert2np(Tree *);
void asa2np(Tree *, char *, char *, int);

int main (int argc, char **argv){
    if(argc == 2){ file = readFile(argv[1]); }
    else{ printf("Carregue os arquivos contendo os inputs.\n"); exit(1); }

    char palavra[100];
    while(!feof(file)){
        fgets(palavra, 100, file);
        printf("Arvore abstrata: %s", palavra);
        tam_syntax = 0;
        fscanf(file, "%d, ", &tam_syntax);
        Syntax synt_tree[tam_syntax];
        readSynt(synt_tree);
        start(synt_tree);
        if (!feof(file)){ printf("\n"); }
    }

    fclose(file); file = NULL;
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

void readSynt(Syntax *synt_tree){
    for (int i = 0; i < tam_syntax-1; i++){
        fscanf(file, "%d | %c, ", &synt_tree[i].indice, &synt_tree[i].token);
    }
    fscanf(file, "%d | %c\n", &synt_tree[tam_syntax-1].indice, &synt_tree[tam_syntax-1].token);
}

void start(Syntax *syntax_tree){
    Tree tree[tam_syntax];
    for (int i = 0; i < tam_syntax; i++) { tree[i].token = ' '; tree[i].visitado = 0; tree[i].indice = 0; tree[i].filhos = 0; tree[i].hash_original = 0; }
    fim_tree = 1; tree[0].token = syntax_tree[0].token;

    abs_tree(syntax_tree, tree, 0);

    for (int i = 0; i< fim_tree; i++){
        printf("[%d | %d | %c]", i, tree[i].indice, tree[i].token);
        if (i<fim_tree-1){ printf(", "); }
    }
    printf("\n");

    convert2np(tree);
}

void abs_tree(Syntax *syntax_tree, Tree *tree, int atual){
    if (atual == 0 && tree[atual].visitado == 1) { return; }
    int inicio_vetor = -1;

    for (int i = 0; i < tam_syntax || (tree[i].indice == tree[atual].indice*2+1); i++){ if (tree[i].indice == tree[atual].indice*2+1) { inicio_vetor = i; } }
    
    if ( inicio_vetor == -1 || tree[atual].token == 'X') { put_token(syntax_tree, tree, atual); }
    else{
        for (int i = inicio_vetor; i < fim_tree; i++){
            if(tree[i].indice == tree[atual].indice*2+1 || tree[i].indice == tree[atual].indice*2+2) {
                if (tree[i].visitado == 0){ abs_tree(syntax_tree, tree, i); return; }
            }
        }
        tree[atual].visitado = 1;
    }
    abs_tree(syntax_tree, tree, 0);
}

void put_token(Syntax *syntax_tree, Tree *tree, int atual){
    int tokens = 0;
    if (tree[atual].token == 'S' || tree[atual].token == 'N' || tree[atual].token == 'G' || tree[atual].token == 'M' || 
    tree[atual].token == 'E' || tree[atual].token == 'X'|| tree[atual].token == 'C'){
        while (syntax_tree[tokens].indice != tree[atual].hash_original*12+1){ tokens++; }
    }
    else if (tree[atual].token == 'r') {
        while (syntax_tree[tokens].indice != tree[atual].hash_original+2){ tokens++; }
    }
    // S
    if (tree[atual].token == 'S'){
        // M
        if (syntax_tree[tokens].token == 'M') {
            tree[atual].token = syntax_tree[tokens].token;
            tree[atual].hash_original = syntax_tree[tokens].indice;
        }
        //    S
        //   / \
        //  G   M
        else if (syntax_tree[tokens].token == 'G') {
            tree[atual].token = '|';
            tree[atual].filhos = 2;
            // G, M
            for (int i = 0; i < tree[atual].filhos; i++){
                tree[fim_tree].token = syntax_tree[tokens+i].token;
                tree[fim_tree].hash_original = syntax_tree[tokens+i].indice;
                tree[fim_tree++].indice = tree[atual].indice*2+(i+1);
            }
        }
        //    S
        //   / \
        //  N   G
        //     / \
        //    G   M
        else if (syntax_tree[tokens].token == 'N') { 
            tree[atual].token = '|';
            tree[atual].filhos = 2;
            int indice;
            // N
            tree[fim_tree].token = syntax_tree[tokens].token;
            tree[fim_tree].hash_original = syntax_tree[tokens].indice;
            tree[fim_tree++].indice = 2*tree[atual].indice+1;
            // G pai
            tree[fim_tree].token = '|';
            tree[fim_tree].hash_original = syntax_tree[tokens+1].indice;
            tree[fim_tree].indice = 2*tree[atual].indice+2;
            tree[fim_tree].visitado = 0;
            indice = tree[fim_tree].indice;
            tree[fim_tree++].filhos = 2;
            // G filho, M
            for (int i = 1; i <= 2; i++){
                tree[fim_tree].token = syntax_tree[tokens+i].token;
                tree[fim_tree].hash_original = syntax_tree[tokens+i].indice;
                tree[fim_tree++].indice = 2*indice+i;
            }
        }

        else{
            tree[atual].visitado = 1;
            return;
        }
    }
    // NGM, GM, M
    else if (tree[atual].token == 'N' || tree[atual].token == 'G' || tree[atual].token == 'M'){
        int valores[2];
        
        for (int i = tokens; i < tokens+13; i++){
            if (syntax_tree[i].token == 'C'){ valores[0]=i; }
            else if (syntax_tree[i].token == 'r'){ valores[1]=i; break; }
        }

        if (tree[atual].token == 'N'){ tree[atual].token = 'n'; }
        else if (tree[atual].token == 'G'){ tree[atual].token = 'g'; }
        else if (tree[atual].token == 'M') { tree[atual].token = 'm'; }
        else{
            tree[atual].visitado = 1;
            return;
        }
        tree[atual].filhos = 2;
        // C, r
        for (int i = 0; i < tree[atual].filhos; i++){
            tree[fim_tree].token = syntax_tree[valores[i]].token;
            tree[fim_tree].indice = 2*tree[atual].indice+(i+1);
            tree[fim_tree++].hash_original = syntax_tree[valores[i]].indice;
        }
    }
    // r
    else if (tree[atual].token == 'r'){
        tree[fim_tree].token = syntax_tree[tokens].token;
        tree[fim_tree].hash_original = syntax_tree[tokens].indice;
        tree[fim_tree++].indice = 2*tree[atual].indice+1;
    }
    // E
    else if (tree[atual].token == 'E'){
        // (EXE)
        if (syntax_tree[tokens].token == '('){ tokens++;
            // X
            tree[atual].token = syntax_tree[tokens+1].token;
            tree[atual].hash_original = syntax_tree[tokens+1].indice;
            tree[atual].filhos = 2;
            // E1
            tree[fim_tree].token = syntax_tree[tokens].token;
            tree[fim_tree].hash_original = syntax_tree[tokens].indice;
            tree[fim_tree++].indice = 2*tree[atual].indice+1;
            // E2
            tree[fim_tree].token = syntax_tree[tokens+2].token;
            tree[fim_tree].hash_original = syntax_tree[tokens+2].indice;
            tree[fim_tree++].indice = 2*tree[atual].indice+2;
        }
        // E -> 0, 1, x, y
        else if (syntax_tree[tokens].token == '0' || syntax_tree[tokens].token == '1' || 
        syntax_tree[tokens].token == 'x' || syntax_tree[tokens].token == 'y'){
            tree[atual].token = syntax_tree[tokens].token;
            tree[atual].hash_original = syntax_tree[tokens].indice;
            tree[atual].visitado = 1;
        }

        else{
            tree[atual].visitado = 1;
            return;
        }
    }
    // X
    else if (tree[atual].token == 'X'){
        tree[atual].token = syntax_tree[tokens].token;
        tree[atual].hash_original = syntax_tree[tokens].indice;
    }
    // C
    else if (tree[atual].token == 'C'){
        // (EXE)
        if (syntax_tree[tokens].token == '('){ tokens++;
            // X
            tree[atual].token = syntax_tree[tokens+1].token;
            tree[atual].hash_original = syntax_tree[tokens+1].indice;
            tree[atual].filhos = 2;
            // E1
            tree[fim_tree].token = syntax_tree[tokens].token;
            tree[fim_tree].hash_original = syntax_tree[tokens].indice;
            tree[fim_tree++].indice = 2*tree[atual].indice+1;
            // E2
            tree[fim_tree].token = syntax_tree[tokens+2].token;
            tree[fim_tree].hash_original = syntax_tree[tokens+2].indice;
            tree[fim_tree++].indice = 2*tree[atual].indice+2;
        }
        // h|i|j|k|z =, E
        else if (syntax_tree[tokens].token == 'h' || syntax_tree[tokens].token == 'i' || syntax_tree[tokens].token == 'j' ||
        syntax_tree[tokens].token == 'k' || syntax_tree[tokens].token == 'z'){
            // =
            tree[atual].token = syntax_tree[tokens+1].token;
            tree[atual].hash_original = syntax_tree[tokens+1].indice;
            tree[atual].filhos = 2;
            // h|i|j|k|z
            tree[fim_tree].token = syntax_tree[tokens].token;
            tree[fim_tree].hash_original = syntax_tree[tokens].indice;
            tree[fim_tree].indice = 2*tree[atual].indice+1;
            tree[fim_tree++].visitado = 1;
            // E
            tree[fim_tree].token = syntax_tree[tokens+2].token;
            tree[fim_tree].hash_original = syntax_tree[tokens+2].indice;
            tree[fim_tree++].indice = 2*tree[atual].indice+2;
        }
        // w|f, E, C
        else if (syntax_tree[tokens].token == 'w' || syntax_tree[tokens].token == 'f'){
            int valores[2];
            for (int i = 0; i < tam_syntax; i++){
                if (syntax_tree[i].indice == tree[atual].hash_original*12+1){
                    for (int j = i; j < i+7; j++){
                        if (syntax_tree[j].token == 'E'){ valores[0]=j; }
                        else if (syntax_tree[j].token == 'C'){ valores[1]=j; break; }
                    }
                    break;
                }
            }
            // w
            tree[atual].token = syntax_tree[tokens].token;
            tree[atual].hash_original = syntax_tree[tokens].indice;
            tree[atual].filhos = 2;
            // E
            tree[fim_tree].token = syntax_tree[valores[0]].token;
            tree[fim_tree].hash_original = syntax_tree[valores[0]].indice;
            tree[fim_tree++].indice = 2*tree[atual].indice+1;
            // C
            tree[fim_tree].token = syntax_tree[valores[1]].token;
            tree[fim_tree].hash_original = syntax_tree[valores[1]].indice;
            tree[fim_tree++].indice = 2*tree[atual].indice+2;
        }

        //    o
        //   / \
        // E2   E1
        // |   /  \
        // E  E    C
        //        / \
        //       C   E
        else if (syntax_tree[tokens].token == 'o'){
            int valores[4], test = 0;
            for (int i = 0; i < tam_syntax; i++){
                if (syntax_tree[i].indice == tree[atual].hash_original*12+1){
                    for (int j = i; j < i+12; j++){
                        if (syntax_tree[j].token == 'E'){ valores[test++] = j; }
                        else if (syntax_tree[j].token == 'C'){ valores[test] = j; break; }
                    }
                    break;
                }
            }
            int indice;
            // o
            tree[atual].token = syntax_tree[tokens].token;
            tree[atual].hash_original = syntax_tree[tokens].indice;
            tree[atual].filhos = 2;
            // E2
            tree[fim_tree].token = '|';
            tree[fim_tree].indice = 2*tree[atual].indice+1;
            indice = tree[fim_tree++].indice;
            // E
            tree[fim_tree].token = syntax_tree[valores[1]].token;
            tree[fim_tree].hash_original = syntax_tree[valores[1]].indice;
            tree[fim_tree++].indice = 2*indice+1;
            // E1
            tree[fim_tree].token = '|';
            tree[fim_tree].indice = 2*tree[atual].indice+2;
            indice = tree[fim_tree++].indice;
            // E
            tree[fim_tree].token = syntax_tree[valores[0]].token;
            tree[fim_tree].hash_original = syntax_tree[valores[0]].indice;
            tree[fim_tree++].indice = 2*indice+1;
            // C pai
            tree[fim_tree].token = '|';
            tree[fim_tree].indice = 2*indice+2;
            tree[fim_tree].filhos = 2;
            indice = tree[fim_tree++].indice;
            // C
            tree[fim_tree].token = syntax_tree[valores[3]].token;
            tree[fim_tree].hash_original = syntax_tree[valores[3]].indice;
            tree[fim_tree++].indice = 2*indice+1;
            // E
            tree[fim_tree].token = syntax_tree[valores[2]].token;
            tree[fim_tree].hash_original = syntax_tree[valores[2]].indice;
            tree[fim_tree++].indice = 2*indice+2;
        }
        
        else{
            tree[atual].visitado = 1;
            return;
        }
    }
}

void convert2np(Tree *asa){
    char polish[fim_tree], reverse_polish[fim_tree];
    tam_pnr=0, tam_pn=0;
    for (int i = 0; i < fim_tree; i++) { polish[i] = ' '; reverse_polish[i] = ' '; }

    asa2np(asa, polish, reverse_polish, 0);

    printf("Notacao ponolesa: ");
    for (int i = 0; i < tam_pn-1; i++){ printf("%c, ", polish[i]); }
    printf("%c\n", polish[tam_pn-1]);

    printf("Notacao ponolesa reversa: ");
    for (int i = 0; i < tam_pnr-1; i++){ printf("%c, ", reverse_polish[i]); }
    printf("%c\n", reverse_polish[tam_pnr-1]);
}

void asa2np(Tree *asa, char *polish, char *reverse_polish, int atual){
    if (atual == fim_tree) { return; }
    
    if (!(asa[atual].token == '|')) { polish[tam_pn++] = asa[atual].token; }

    for (int i = atual; i < fim_tree; i++){
        if (asa[atual].indice*2+1 == asa[i].indice){
            asa2np(asa, polish, reverse_polish, i);
            break;
        }
    }

    if (!(asa[atual].token == '|')) { reverse_polish[tam_pnr++] = asa[atual].token; }

    if (atual < fim_tree && asa[atual].indice+1 == asa[atual+1].indice && asa[atual].token != 'm'){
        asa2np(asa, polish, reverse_polish, atual+1);
    }
}