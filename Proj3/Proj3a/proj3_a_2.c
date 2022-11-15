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
int tam_syntax, fim_tree;

// 
FILE *readFile(char *);
void readSynt(Syntax *);
void start(Syntax *);
void abs_tree(Syntax *, Tree *, int);
void put_token(Syntax *, Tree *, int);

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
    while (syntax_tree[tokens].indice != tree[atual].hash_original*12+1){ tokens++; }
    // M
    if (tree[atual].token == 'M'){
        // m
        if (syntax_tree[tokens].token == 'm') {
            int valor;
            for (int i = tokens; i < tokens+6; i++){ if (syntax_tree[i].token == 'S'){ valor=i; break; } }
            tree[atual].token = syntax_tree[valor].token;
            tree[atual].hash_original = syntax_tree[valor].indice;   
            tree[atual].filhos = 1;
        }
        else{
            tree[atual].visitado = 1;
            return;
        }
    }
    // S
    else if (tree[atual].token == 'S'){
        // cX;S
        // iX;S
        if (syntax_tree[tokens].token == 'c' || syntax_tree[tokens].token == 'i'){
            int valores[2];
            for (int i = tokens; i < tokens+4; i++){
                if (syntax_tree[i].token == 'X'){ valores[0]=i; }
                else if (syntax_tree[i].token == 'S'){ valores[1]=i; break; }
            }
            tree[atual].token = syntax_tree[tokens].token;
            tree[atual].hash_original = syntax_tree[tokens].indice;
            tree[atual].filhos = 2;

            for (int i = 0; i < tree[atual].filhos; i++){
                tree[fim_tree].token = syntax_tree[valores[i]].token;
                tree[fim_tree].indice = 2*tree[atual].indice+(i+1);
                tree[fim_tree++].hash_original = syntax_tree[valores[i]].indice;
            }

        }
        // h=E;C
        // k=E;C
        // x=E;C
        // y=E;C
        // z=E;C
        else if (syntax_tree[tokens].token == 'h' || syntax_tree[tokens].token == 'k' || syntax_tree[tokens].token == 'x' || 
        syntax_tree[tokens].token == 'y' || syntax_tree[tokens].token == 'z'){
            int valores[2];
            for (int i = tokens; i < tokens+5; i++){
                if (syntax_tree[i].token == 'E'){ valores[0]=i; }
                else if (syntax_tree[i].token == 'C'){ valores[1]=i; break; }
            }
            tree[atual].token = syntax_tree[tokens+1].token;
            tree[atual].hash_original = syntax_tree[tokens+1].indice;
            tree[atual].filhos = 2;

            tree[fim_tree].token = syntax_tree[tokens].token;
            tree[fim_tree].indice = 2*tree[atual].indice+1;
            tree[fim_tree].visitado = 1;
            tree[fim_tree++].hash_original = syntax_tree[tokens].indice;


            tree[fim_tree].token = syntax_tree[valores[0]+1].token;
            tree[fim_tree].indice = 2*tree[atual].indice+2;
            tree[fim_tree].hash_original = syntax_tree[valores[0]+1].indice;
            tree[fim_tree].filhos = 2;
            atual = fim_tree++;

            for (int i = 0; i < tree[atual].filhos; i++){
                tree[fim_tree].token = syntax_tree[valores[i]].token;
                tree[fim_tree].indice = 2*tree[atual].indice+(i+1);
                tree[fim_tree++].hash_original = syntax_tree[valores[i]].indice;
            }
        }
        // w(E){C}C
        // f(E){C}C
        else if (syntax_tree[tokens].token == 'w' || syntax_tree[tokens].token == 'f'){
            int valores[3], test = 0;
            for (int i = tokens; i < tokens+9; i++){
                if (syntax_tree[i].token == 'E'){ valores[test++]=i; }
                else if (syntax_tree[i].token == 'C'){ valores[test++]=i; }
            }

            tree[atual].token = syntax_tree[tokens].token;
            tree[atual].hash_original = syntax_tree[tokens].indice;
            tree[atual].filhos = 2;

            tree[fim_tree].token = syntax_tree[valores[0]].token;
            tree[fim_tree].indice = 2*tree[atual].indice+1;
            tree[fim_tree++].hash_original = syntax_tree[valores[0]].indice;

            tree[fim_tree].token = syntax_tree[valores[1]].token;
            tree[fim_tree].indice = 2*tree[atual].indice+2;
            tree[fim_tree].hash_original = syntax_tree[valores[1]].indice;
            tree[fim_tree].filhos = 2;
            atual = fim_tree++;

            for (int i = 0; i < tree[atual].filhos; i++){
                tree[fim_tree].token = syntax_tree[valores[i+1]].token;
                tree[fim_tree].indice = 2*tree[atual].indice+(i+1);
                tree[fim_tree++].hash_original = syntax_tree[valores[i+1]].indice;
            }
        }

        // o(E;E;E){C}C
        else if (syntax_tree[tokens].token == 'o'){
            int valores[5], test = 0;
            for (int i = 0; i < tam_syntax; i++){
                if (syntax_tree[i].indice == tree[atual].hash_original*12+1){
                    for (int j = i; j < i+12; j++){
                        if (syntax_tree[j].token == 'E'){ valores[test++] = j; }
                        else if (syntax_tree[j].token == 'C'){ valores[test++] = j; }
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
            tree[fim_tree].token = syntax_tree[valores[1]].token;
            tree[fim_tree].hash_original = syntax_tree[valores[1]].indice;
            tree[fim_tree].indice = 2*tree[atual].indice+1;
            indice = tree[fim_tree++].indice;
            // E
            tree[fim_tree].token = syntax_tree[valores[1]].token;
            tree[fim_tree].hash_original = syntax_tree[valores[1]].indice;
            tree[fim_tree++].indice = 2*indice+1;
            // E1
            tree[fim_tree].token = syntax_tree[valores[0]].token;
            tree[fim_tree].hash_original = syntax_tree[valores[0]].indice;
            tree[fim_tree].indice = 2*tree[atual].indice+2;
            indice = tree[fim_tree++].indice;
            // E
            tree[fim_tree].token = syntax_tree[valores[0]].token;
            tree[fim_tree].hash_original = syntax_tree[valores[0]].indice;
            tree[fim_tree++].indice = 2*indice+1;
            // C1 pai
            tree[fim_tree].token = syntax_tree[valores[3]].token;
            tree[fim_tree].hash_original = syntax_tree[valores[3]].indice;
            tree[fim_tree].indice = 2*indice+2;
            tree[fim_tree].filhos = 2;
            indice = tree[fim_tree++].indice;
            // C1
            tree[fim_tree].token = syntax_tree[valores[3]].token;
            tree[fim_tree].hash_original = syntax_tree[valores[3]].indice;
            tree[fim_tree++].indice = 2*indice+1;
            // E3
            tree[fim_tree].token = syntax_tree[valores[2]].token;
            tree[fim_tree].hash_original = syntax_tree[valores[2]].indice;
            tree[fim_tree].indice = 2*indice+2;
            tree[fim_tree].filhos = 2;
            indice = tree[fim_tree++].indice;
            // E
            tree[fim_tree].token = syntax_tree[valores[2]].token;
            tree[fim_tree].hash_original = syntax_tree[valores[2]].indice;
            tree[fim_tree++].indice = 2*indice+1;
            // C2
            tree[fim_tree].token = syntax_tree[valores[4]].token;
            tree[fim_tree].hash_original = syntax_tree[valores[4]].indice;
            tree[fim_tree++].indice = 2*indice+2;
        }

        // r(E);
        else if (syntax_tree[tokens].token == 'r'){
            int valores;
            for (int i = tokens; i < tokens+4; i++){
                if (syntax_tree[i].token == 'E'){ valores=i; break; }
            }
            tree[atual].token = syntax_tree[tokens].token;
            tree[atual].hash_original = syntax_tree[tokens].indice;
            tree[atual].filhos = 1;

            tree[fim_tree].token = syntax_tree[valores].token;
            tree[fim_tree].indice = 2*tree[atual].indice+1;
            tree[fim_tree++].hash_original = syntax_tree[valores].indice;
        }

        else{
            tree[atual].visitado = 1;
            return;
        }
    }
    // E
    else if (tree[atual].token == 'E'){
        // (EBE)
        if (syntax_tree[tokens].token == '('){ tokens++;
            // B
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

        else if (syntax_tree[tokens].token == '-'){
            tree[atual].token = syntax_tree[tokens].token;
            tree[atual].hash_original = syntax_tree[tokens].indice;
            tree[atual].filhos = 1;

            tree[fim_tree].token = syntax_tree[tokens+1].token;
            tree[fim_tree].hash_original = syntax_tree[tokens+1].indice;
            tree[fim_tree++].indice = 2*tree[atual].indice+1;
        }

        else{
            tree[atual].visitado = 1;
            return;
        }
    }
    // X|B
    else if (tree[atual].token == 'X' || tree[atual].token == 'B'){
        if (tree[atual].token == 'X'){ tree[atual].visitado=1; }
        tree[atual].token = syntax_tree[tokens].token;
        tree[atual].hash_original = syntax_tree[tokens].indice;
    }
    // C
    else if (tree[atual].token == 'C'){
        // h=E;C
        // k=E;C
        // x=E;C
        // y=E;C
        // z=E;C
        if (syntax_tree[tokens].token == 'h' || syntax_tree[tokens].token == 'k' || syntax_tree[tokens].token == 'x' || 
        syntax_tree[tokens].token == 'y' || syntax_tree[tokens].token == 'z'){
            int valores[2];
            for (int i = tokens; i < tokens+5; i++){
                if (syntax_tree[i].token == 'E'){ valores[0]=i; }
                else if (syntax_tree[i].token == 'C'){ valores[1]=i; break; }
            }
            tree[atual].token = syntax_tree[tokens+1].token;
            tree[atual].hash_original = syntax_tree[tokens+1].indice;
            tree[atual].filhos = 2;

            tree[fim_tree].token = syntax_tree[tokens].token;
            tree[fim_tree].indice = 2*tree[atual].indice+1;
            tree[fim_tree].visitado = 1;
            tree[fim_tree++].hash_original = syntax_tree[tokens].indice;

            tree[fim_tree].token = syntax_tree[valores[0]+1].token;
            tree[fim_tree].indice = 2*tree[atual].indice+2;
            tree[fim_tree].hash_original = syntax_tree[valores[0]+1].indice;
            tree[fim_tree].filhos = 2;
            atual = fim_tree++;

            for (int i = 0; i < tree[atual].filhos; i++){
                tree[fim_tree].token = syntax_tree[valores[i]].token;
                tree[fim_tree].indice = 2*tree[atual].indice+(i+1);
                tree[fim_tree++].hash_original = syntax_tree[valores[i]].indice;
            }
        }
        // w(E){C}C
        // f(E){C}C
        else if (syntax_tree[tokens].token == 'w' || syntax_tree[tokens].token == 'f'){
            int valores[3], test = 0;
            for (int i = tokens; i < tokens+9; i++){
                if (syntax_tree[i].token == 'E'){ valores[test++]=i; }
                else if (syntax_tree[i].token == 'C'){ valores[test++]=i; }
            }

            tree[atual].token = syntax_tree[tokens].token;
            tree[atual].hash_original = syntax_tree[tokens].indice;
            tree[atual].filhos = 2;

            tree[fim_tree].token = syntax_tree[valores[0]].token;
            tree[fim_tree].indice = 2*tree[atual].indice+1;
            tree[fim_tree++].hash_original = syntax_tree[valores[0]].indice;

            tree[fim_tree].token = syntax_tree[valores[1]].token;
            tree[fim_tree].indice = 2*tree[atual].indice+2;
            tree[fim_tree].hash_original = syntax_tree[valores[1]].indice;
            tree[fim_tree].filhos = 2;
            atual = fim_tree++;

            for (int i = 0; i < tree[atual].filhos; i++){
                tree[fim_tree].token = syntax_tree[valores[i+1]].token;
                tree[fim_tree].indice = 2*tree[atual].indice+(i+1);
                tree[fim_tree++].hash_original = syntax_tree[valores[i+1]].indice;
            }
        }

        // o(E;E;E){C}C
        else if (syntax_tree[tokens].token == 'o'){
            int valores[5], test = 0;
            for (int i = 0; i < tam_syntax; i++){
                if (syntax_tree[i].indice == tree[atual].hash_original*12+1){
                    for (int j = i; j < i+12; j++){
                        if (syntax_tree[j].token == 'E'){ valores[test++] = j; }
                        else if (syntax_tree[j].token == 'C'){ valores[test++] = j; }
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
            tree[fim_tree].token = syntax_tree[valores[1]].token;
            tree[fim_tree].hash_original = syntax_tree[valores[1]].indice;
            tree[fim_tree].indice = 2*tree[atual].indice+1;
            indice = tree[fim_tree++].indice;
            // E
            tree[fim_tree].token = syntax_tree[valores[1]].token;
            tree[fim_tree].hash_original = syntax_tree[valores[1]].indice;
            tree[fim_tree++].indice = 2*indice+1;
            // E1
            tree[fim_tree].token = syntax_tree[valores[0]].token;
            tree[fim_tree].hash_original = syntax_tree[valores[0]].indice;
            tree[fim_tree].indice = 2*tree[atual].indice+2;
            indice = tree[fim_tree++].indice;
            // E
            tree[fim_tree].token = syntax_tree[valores[0]].token;
            tree[fim_tree].hash_original = syntax_tree[valores[0]].indice;
            tree[fim_tree++].indice = 2*indice+1;
            // C1 pai
            tree[fim_tree].token = syntax_tree[valores[3]].token;
            tree[fim_tree].hash_original = syntax_tree[valores[3]].indice;
            tree[fim_tree].indice = 2*indice+2;
            tree[fim_tree].filhos = 2;
            indice = tree[fim_tree++].indice;
            // C1
            tree[fim_tree].token = syntax_tree[valores[3]].token;
            tree[fim_tree].hash_original = syntax_tree[valores[3]].indice;
            tree[fim_tree++].indice = 2*indice+1;
            // E3
            tree[fim_tree].token = syntax_tree[valores[2]].token;
            tree[fim_tree].hash_original = syntax_tree[valores[2]].indice;
            tree[fim_tree].indice = 2*indice+2;
            tree[fim_tree].filhos = 2;
            indice = tree[fim_tree++].indice;
            // E
            tree[fim_tree].token = syntax_tree[valores[2]].token;
            tree[fim_tree].hash_original = syntax_tree[valores[2]].indice;
            tree[fim_tree++].indice = 2*indice+1;
            // C2
            tree[fim_tree].token = syntax_tree[valores[4]].token;
            tree[fim_tree].hash_original = syntax_tree[valores[4]].indice;
            tree[fim_tree++].indice = 2*indice+2;
        }

        // r(E);
        else if (syntax_tree[tokens].token == 'r'){
            int valores;
            for (int i = tokens; i < tokens+4; i++){
                if (syntax_tree[i].token == 'E'){ valores=i; break; }
            }
            tree[atual].token = syntax_tree[tokens].token;
            tree[atual].hash_original = syntax_tree[tokens].indice;
            tree[atual].filhos = 1;

            tree[fim_tree].token = syntax_tree[valores].token;
            tree[fim_tree].indice = 2*tree[atual].indice+1;
            tree[fim_tree++].hash_original = syntax_tree[valores].indice;
        }

        else if (syntax_tree[tokens].token == '.'){
            tree[atual].token = syntax_tree[tokens].token;
            tree[atual].hash_original = syntax_tree[tokens].indice;
            tree[atual].visitado=1;
        }

        else{
            tree[atual].visitado = 1;
            return;
        }
    }
}