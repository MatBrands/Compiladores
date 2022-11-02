#include <stdio.h>
#include <stdlib.h>

// Global Variables
char token;
FILE *file;
int productions[100];
int p_count, prod, word = 0, aceito = 0;

// Functions
FILE *readFile(char *);
void start();
void lex();
void erro(int);
int S();
int N(); int G(); int M();
int E(); int X();
int C();
void start_tree();
void parse_tree(char *, int *, int);
void put_production(char *, int *, int);

int main(int argc, char **argv){
    if(argc == 2){ file = readFile(argv[1]); }
    else{ printf("Carregue o arquivo contendo os inputs.\n"); exit(1); }

    start();

    fclose(file); file = NULL;
	return(0);
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

void start(){
    if(!feof(file)){
        printf("Palavra %d: ", ++word);
        p_count = 0, aceito = 0;
        S();
        if(!aceito) { printf("\nPalavra aceita."); }
        printf("\nProducoes: "); for (int i = 0; i < p_count; i++) { printf("P%d", productions[i]); if(i<p_count-1){ printf(", "); } }
        printf("\nArvore: "); start_tree();
        if (!feof(file)) { printf("\n****************************************************************************************************************************************\n\n"); start(); }
    }
}

void lex(){
    do {
        if (token != '\n'){ printf("%c", token); }
        token = fgetc(file);
    }while(token == ' ');
}

void erro(int codigo){
    aceito = 1;
    if (token != '\n') { printf("\nErro %d. Token '%c' inesperado. Prosseguindo para proxima palavra.", codigo, token); }
    while((token != '\n')) {
        token = fgetc(file);
        if (feof(file)) { fclose(file); file = NULL; exit(0); }
    }
}

// P1: S -> M
// P2: S -> GM
// P3: S -> NGM
int S(){
    lex();
    if (token == 'm'){ productions[p_count++] = 1; M(); }
    else if (token == 'g'){ productions[p_count++] = 2; G(); M(); }
    else if (token == 'n'){ productions[p_count++] = 3; N(); G(); M(); }
    else { erro(0); }
    return 0;
}

// P4: N -> n(){ C; r(E); }
int N(){
    if (token == 'n') { lex();
        if (token == '(') { lex();
            if (token == ')') { lex();
                if (token == '{') { productions[p_count++] = 4; lex();
                    C();
                    if (token == ';') { lex();
                        if (token == 'r') { lex();
                            if (token == '(') { lex();
                                E();
                                if (token == ')') { lex();
                                    if (token == ';') { lex();
                                        if (token == '}') { lex();
                                        }
                                        else { erro(10); }
                                    }
                                    else { erro(11); }
                                }
                                else { erro(12); }
                            }
                            else { erro(13); }
                        }
                        else { erro(14); }
                    }
                    else { erro(15); }
                }
                else { erro(16); }
            }
            else { erro(17); }
        }
        else { erro(18); }
    }
    else { erro(19); }
    return 0;
}

// P5: G -> g(){ C; r(E); }
int G(){
    if (token == 'g') { lex();
        if (token == '(') { lex();
            if (token == ')') { lex();
                if (token == '{') { productions[p_count++] = 5; lex();
                    C();
                    if (token == ';') { lex();
                        if (token == 'r') { lex();
                            if (token == '(') { lex();
                                E();
                                if (token == ')') { lex();
                                    if (token == ';') { lex();
                                        if (token == '}') { lex();
                                        }
                                        else { erro(20); }
                                    }
                                    else { erro(21); }
                                }
                                else { erro(22); }
                            }
                            else { erro(23); }
                        }
                        else { erro(24); }
                    }
                    else { erro(25); }
                }
                else { erro(26); }
            }
            else { erro(27); }
        }
        else { erro(28); }
    }
    else { erro(29); }
    return 0;
}

// P6: M -> m() { C; r(E); }
int M(){
    if (token == 'm') { lex();
        if (token == '(') { lex();
            if (token == ')') { lex();
                if (token == '{') {productions[p_count++] = 6;  lex();
                    C();
                    if (token == ';') { lex();
                        if (token == 'r') { lex();
                            if (token == '(') { lex();
                                E();
                                if (token == ')') { lex();
                                    if (token == ';') { lex();
                                        if (token == '}') { lex();
                                        }
                                        else { erro(30); }
                                    }
                                    else { erro(31); }
                                }
                                else { erro(32); }
                            }
                            else { erro(33); }
                        }
                        else { erro(34); }
                    }
                    else { erro(35); }
                }
                else { erro(36); }
            }
            else { erro(37); }
        }
        else { erro(38); }
    }
    else { erro(39); }
    return 0;
}

// P7: E -> 0
// P8: E -> 1
// P9: E -> x
// P10: E -> y
// P11: E -> (EXE)
int E(){
    if (token == '0'){ productions[p_count++] = 7; lex(); }
    else if (token == '1'){ productions[p_count++] = 8; lex(); }
    else if (token == 'x'){ productions[p_count++] = 9; lex(); }
    else if (token == 'y'){ productions[p_count++] = 10; lex(); }
    else if (token == '('){ productions[p_count++] = 11; lex(); 
        E(); X(); E();
        if (token == ')'){ lex(); }
        else { erro(40); }
    }
    else { erro(41); }
    return 0;
}

// P12: X -> +
// P13: X -> -
// P14: X -> *
// P15: X -> /
int X(){
    if (token == '+'){ productions[p_count++] = 12; lex(); }
    else if (token == '-'){ productions[p_count++] = 13; lex(); }
    else if (token == '*'){ productions[p_count++] = 14; lex(); }
    else if (token == '/'){ productions[p_count++] = 15; lex(); }
    else { erro(50); }
}

// P16: C -> h=E
// P17: C -> i=E
// P18: C -> j=E
// P19: C -> k=E
// P20: C -> z=E
// P21: C -> (EXE)
// P22: C -> w(E){ C; }
// P23: C -> f(E){ C; }
// P24: C -> o(E;E;E){ C; }
int C(){
    if (token == 'h'){ productions[p_count++] = 16; lex(); 
        if (token == '=') { lex(); E(); }
        else { erro(60); }
    }
    else if (token == 'i'){ productions[p_count++] = 17; lex(); 
        if (token == '=') { lex(); E(); }
        else { erro(61); }
    }
    else if (token == 'j'){ productions[p_count++] = 18; lex(); 
        if (token == '=') { lex(); E(); }
        else { erro(62); }
    }
    else if (token == 'k'){ productions[p_count++] = 19; lex(); 
        if (token == '=') { lex(); E(); }
        else { erro(63); }
    }
    else if (token == 'z'){ productions[p_count++] = 20; lex(); 
        if (token == '=') { lex(); E(); }
        else { erro(64); }
    }
    else if (token == '('){ productions[p_count++] = 21; lex();
        E(); X(); E();
        if (token == ')'){ lex(); }
        else { erro(65); }
    }
    else if (token == 'w'){ lex();
        if (token == '('){ productions[p_count++] = 22; lex();
            E();
            if (token == ')'){ lex();
                if (token == '{'){ lex();
                    C();
                    if (token == ';'){ lex();
                        if (token == '}'){ lex(); }
                        else { erro(66); }
                    }
                    else { erro(67); }
                }
                else { erro(68); }
            }
            else { erro(69); }
        }
        else { erro(70); }
    }
    else if (token == 'f'){ lex();
        if (token == '('){ productions[p_count++] = 23; lex();
            E();
            if (token == ')'){ lex();
                if (token == '{'){ lex();
                    C();
                    if (token == ';'){ lex();
                        if (token == '}'){ lex();
                        }
                        else { erro(71); }
                    }
                    else { erro(72); }
                }
                else { erro(73); }
            }
            else { erro(74); }
        }
        else { erro(75); }
    }
    else if (token == 'o'){ lex();
        if (token == '('){ productions[p_count++] = 24; lex();
            E();
            if (token == ';'){ lex();
                E();
                if (token == ';'){ lex();
                    E();
                    if (token == ')'){ lex();
                        if (token == '{'){ lex();
                            C();
                            if (token == ';'){ lex();
                                if (token == '}'){ lex();
                                }
                                else { erro(76); }
                            }
                            else { erro(77); }
                        }
                        else { erro(78); }
                    }
                    else { erro(79); }
                }
                else { erro(80); }
            }
            else { erro(81); }
        }
        else { erro(82); }
    }
    else{ erro(83); }
    return 0;
}

void start_tree(){
    const int max = 50000;
    char tree[max];
    int visitados[max];
    prod = 0;
    for (int i = 0; i < max; i++) { tree[i] = ' '; visitados[i] = 0; }
    tree[0] = 'S';

    parse_tree(tree, visitados, 0);

    for (int i = 0; i< max; i++){
        if (tree[i] != ' ') { printf("%c[%d]\t", tree[i], i); }
    }

    printf("\n");
}

void parse_tree(char *tree, int *visitados, int atual){
    if (atual == 0 && visitados[atual] == 1) { return; }
    if (prod > p_count){ return; }
    // --------------------------------------------------------------------
    if (tree[4*atual+1] == ' ') { put_production(tree, visitados, atual); prod++; }
    // --------------------------------------------------------------------
    if (visitados[4*atual+1] == 0){ parse_tree(tree, visitados, 4*atual+1); }
    else if (visitados[4*atual+2] == 0){ parse_tree(tree, visitados, 4*atual+2); }
    else if (visitados[4*atual+3] == 0){ parse_tree(tree, visitados, 4*atual+3); }
    else if (visitados[4*atual+4] == 0){ parse_tree(tree, visitados, 4*atual+4); }
    else { visitados[atual] = 1; parse_tree(tree, visitados, 0); }
}

void put_production(char *tree, int *visitados, int atual){
    if (productions[prod] == 1){ 
        if (tree[atual] == 'S'){
            tree[4*atual + 1] = 'M'; tree[4*atual + 2] = ' '; tree[4*atual + 3] = ' '; tree[4*atual + 4] = ' ';
            visitados[4*atual+2] = 1; visitados[4*atual+3] = 1; visitados[4*atual+4] = 1;
        }
    }
    // P2: S -> GM
    else if(productions[prod] == 2) {
        if (tree[atual] == 'S'){
            tree[4*atual + 1] = 'G'; tree[4*atual + 2] = 'M'; tree[4*atual + 3] = ' '; tree[4*atual + 4] = ' ';
            visitados[4*atual+3] = 1; visitados[4*atual+4] = 1;
        }
    }
    // P3: S -> NGM
    else if(productions[prod] == 3) {
        if (tree[atual] == 'S'){
            tree[4*atual + 1] = 'N'; tree[4*atual + 2] = 'G'; tree[4*atual + 3] = 'M'; tree[4*atual + 4] = ' ';
            visitados[4*atual+4] = 1;
        }
    }
    // P4: N -> n(){ C; r(E); }
    else if(productions[prod] == 4) {
        if (tree[atual] == 'N'){
            tree[4*atual + 1] = 'C'; tree[4*atual + 2] = 'E'; tree[4*atual + 3] = ' '; tree[4*atual + 4] = ' ';
            visitados[4*atual+3] = 1; visitados[4*atual+4] = 1;
        }
    }
    // P5: G -> g(){ C; r(E); }
    else if(productions[prod] == 5) {
        if (tree[atual] == 'G'){
            tree[4*atual + 1] = 'C'; tree[4*atual + 2] = 'E'; tree[4*atual + 3] = ' '; tree[4*atual + 4] = ' ';
            visitados[4*atual+3] = 1; visitados[4*atual+4] = 1;
        }
    }
    // P6: M -> m() { C; r(E); }
    else if(productions[prod] == 6) {
        if (tree[atual] == 'M'){
            tree[4*atual + 1] = 'C'; tree[4*atual + 2] = 'E'; tree[4*atual + 3] = ' '; tree[4*atual + 4] = ' ';
            visitados[4*atual+3] = 1; visitados[4*atual+4] = 1;
        }
    }
    // P7: E -> 0
    // P8: E -> 1
    // P9: E -> x
    // P10: E -> y
    else if(productions[prod] >= 7 && productions[prod] <= 10) {
        if (tree[atual] == 'E'){
            if (productions[prod] == 7) tree[4*atual + 1] = '0';
            else if (productions[prod] == 8) { tree[4*atual + 1] = '1'; }
            else if (productions[prod] == 9) { tree[4*atual + 1] = 'x'; }
            else { tree[4*atual + 1] = 'y'; }
            tree[4*atual + 2] = ' '; tree[4*atual + 3] = ' '; tree[4*atual + 4] = ' ';
            visitados[4*atual+1] = 1; visitados[4*atual+2] = 1; visitados[4*atual+3] = 1; visitados[4*atual+4] = 1;
        }
    }
    // P11: E -> (EXE)
    else if(productions[prod] == 11) {
        if (tree[atual] == 'E'){
            tree[4*atual + 1] = 'E'; tree[4*atual + 2] = 'X'; tree[4*atual + 3] = 'E'; tree[4*atual + 4] = ' ';
            visitados[4*atual+4] = 1;
        }
    }
    // P12: X -> +
    // P13: X -> -
    // P14: X -> *
    // P15: X -> /
    else if(productions[prod] >= 12 && productions[prod] <= 15) {
        if (tree[atual] == 'X'){
            if(productions[prod] == 12) { tree[4*atual + 1] = '+'; }
            else if(productions[prod] == 13) { tree[4*atual + 1] = '-'; }
            else if(productions[prod] == 14) { tree[4*atual + 1] = '*'; }
            else { tree[4*atual + 1] = '/'; }
            tree[4*atual + 2] = ' '; tree[4*atual + 3] = ' '; tree[4*atual + 4] = ' ';
            visitados[4*atual+1] = 1; visitados[4*atual+2] = 1; visitados[4*atual+3] = 1; visitados[4*atual+4] = 1;
        }
    }
    // P16: C -> h=E
    // P17: C -> atual=E
    // P18: C -> j=E
    // P19: C -> k=E
    // P20: C -> z=E
    else if(productions[prod] >= 16 && productions[prod] <= 20) {
        if (tree[atual] == 'C'){
            tree[4*atual + 1] = 'E'; tree[4*atual + 2] = ' '; tree[4*atual + 3] = ' '; tree[4*atual + 4] = ' ';
            visitados[4*atual+2] = 1; visitados[4*atual+3] = 1; visitados[4*atual+4] = 1;
        }
    }
    // P21: C -> (EXE)
    else if(productions[prod] == 21) {
        if (tree[atual] == 'C'){
            tree[4*atual + 1] = 'E'; tree[4*atual + 2] = 'X'; tree[4*atual + 3] = 'E'; tree[4*atual + 4] = ' ';
            visitados[4*atual+4] = 1;
        }
    }
    // P22: C -> w(E){ C; }
    else if(productions[prod] == 22) {
        if (tree[atual] == 'C'){
            tree[4*atual + 1] = 'E'; tree[4*atual + 2] = 'C'; tree[4*atual + 3] = ' '; tree[4*atual + 4] = ' ';
            visitados[4*atual+3] = 1; visitados[4*atual+4] = 1;
        }
    }
    // P23: C -> f(E){ C; }
    else if(productions[prod] == 23) {
        if (tree[atual] == 'C'){
            tree[4*atual + 1] = 'E'; tree[4*atual + 2] = 'C'; tree[4*atual + 3] = ' '; tree[4*atual + 4] = ' ';
            visitados[4*atual+3] = 1; visitados[4*atual+4] = 1;
        }
    }
    // P24: C -> o(E;E;E){ C; }
    else if(productions[prod] == 24) {
        if (tree[atual] == 'C'){
            tree[4*atual + 1] = 'E'; tree[4*atual + 2] = 'E'; tree[4*atual + 3] = 'E'; tree[4*atual + 4] = 'C';
        }
    }
}