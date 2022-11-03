#include <stdio.h>
#include <stdlib.h>

// Global Variables
struct No{
    char token;
	int visitado;
    int indice;
    int filhos;
    int hash_original;
}; typedef struct No Tree;

struct Node{
    char token;
    int indice;
}; typedef struct Node Syntax;

char token;
FILE *file;
const int max = 500;
int productions[100];
int p_count, prod, word = 0, aceito = 0;
int fim_synt_tree, fim_abs_tree;

// Functions
FILE *readFile(char *);
void start(); void lex();
void erro(int); int S();
int N(); int G(); int M();
int E(); int X(); int C();
void start_tree();
void parse_tree(Tree *, int);
void put_production(Tree *, int);
void start_abs(Syntax *);
void abs_tree(Syntax *, Tree *, int);
void put_token(Syntax *, Tree *, int);
// 

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
        printf("\nArvore Sintatica: "); start_tree();
        p_count = 0, aceito = 0;
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
    while(token != '\n') {
        token = fgetc(file);
        if (feof(file)) {
            fclose(file); file = NULL; exit(0);
        }
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
    Tree tree[max];
    for (int i = 0; i < max; i++) { tree[i].token = ' '; tree[i].visitado = 0; tree[i].indice = -1; tree[i].filhos = 0; }
    prod = 0; fim_synt_tree = 1; tree[0].token = 'S'; tree[0].indice = 0;

    parse_tree(tree, 0);

    Syntax syntax_tree[fim_synt_tree];
    for (int i = 0; i< fim_synt_tree; i++){
        syntax_tree[i].indice = tree[i].indice;
        syntax_tree[i].token = tree[i].token;
        if (i<fim_synt_tree-1){ printf("[%d|%d| %c ], ", i, syntax_tree[i].indice, syntax_tree[i].token); }
        else { printf("[%d|%d| %c ]\n", i, syntax_tree[i].indice, syntax_tree[i].token); } }

    printf("Arvore abstrata: "); start_abs(syntax_tree);
}

void parse_tree(Tree *tree, int atual){
    if (prod > p_count || atual == 0 && tree[atual].visitado == 1) { return; }
    int inicio_vetor = -1;

    for (int i = 0; i < fim_synt_tree || (tree[i].indice == tree[atual].indice*12+1); i++){ if (tree[i].indice == tree[atual].indice*12+1) { inicio_vetor = i; } }
    
    if ( inicio_vetor == -1 ) { put_production(tree, atual); }
    else{
        for (int i = inicio_vetor; i < inicio_vetor+tree[inicio_vetor].filhos; i++){
            if (tree[i].visitado == 0){ parse_tree(tree, i); return; }
        }
        tree[atual].visitado = 1;
    }
    
    parse_tree(tree, 0);
}

void put_production(Tree *tree, int atual){
    // P1: S -> M
    if (productions[prod] == 1){
        if (tree[atual].token != 'S') { return; }
        tree[fim_synt_tree].token = 'M';
        tree[fim_synt_tree].filhos = 1;
    }
    // P2: S -> GM
    else if(productions[prod] == 2) {
        if (tree[atual].token != 'S') { return; }
        tree[fim_synt_tree].token = 'G'; tree[fim_synt_tree+1].token = 'M';
        tree[fim_synt_tree].filhos = 2;
    }
    // P3: S -> NGM
    else if(productions[prod] == 3) {
        if (tree[atual].token != 'S') { return; }
        tree[fim_synt_tree].token = 'N'; tree[fim_synt_tree+1].token = 'G'; tree[fim_synt_tree+2].token = 'M';
        tree[fim_synt_tree].filhos = 3;
    }
    // P4: N -> n(){ C; r(E); }
    // P5: G -> g(){ C; r(E); }
    // P6: M -> m() { C; r(E); }
    else if(productions[prod] >= 4 && productions[prod] <= 6) {
        for (int i = 0; i < 12; i++){ tree[fim_synt_tree+i].visitado = 1; }

        if (productions[prod] == 4) {
            if (tree[atual].token != 'N') { return; }
            tree[fim_synt_tree].token = 'n';
        }
        else if (productions[prod] == 5) {
            if (tree[atual].token != 'G') { return; }
            tree[fim_synt_tree].token = 'g';
        }
        else {
            if (tree[atual].token != 'M') { return; }
            tree[fim_synt_tree].token = 'm';
        }
        tree[fim_synt_tree+1].token = '('; tree[fim_synt_tree+2].token = ')'; tree[fim_synt_tree+3].token = '{';  tree[fim_synt_tree+4].token = 'C'; 
        tree[fim_synt_tree+5].token = ';'; tree[fim_synt_tree+6].token = 'r'; tree[fim_synt_tree+7].token = '('; tree[fim_synt_tree+8].token = 'E';
        tree[fim_synt_tree+9].token = ')'; tree[fim_synt_tree+10].token = ';'; tree[fim_synt_tree+11].token = '}';
        tree[fim_synt_tree+4].visitado = 0; tree[fim_synt_tree+8].visitado = 0;
        tree[fim_synt_tree].filhos = 12;
    }
    // P7: E -> 0
    // P8: E -> 1
    // P9: E -> x
    // P10: E -> y
    else if(productions[prod] >= 7 && productions[prod] <= 10) {
        if (tree[atual].token != 'E') { return; }
        if (productions[prod] == 7) { tree[fim_synt_tree].token = '0'; }
        else if (productions[prod] == 8) { tree[fim_synt_tree].token = '1'; }
        else if (productions[prod] == 9) { tree[fim_synt_tree].token = 'x'; }
        else { tree[fim_synt_tree].token = 'y'; }
        tree[fim_synt_tree].visitado = 1;
        tree[fim_synt_tree].filhos = 1;
    }
    // P11: E -> (EXE)
    // P21: C -> (EXE)
    else if(productions[prod] == 11 || productions[prod] == 21) {
        if(productions[prod] == 11) { if (tree[atual].token != 'E') { return; } }
        else { if (tree[atual].token != 'C') { return; } }
        tree[fim_synt_tree].token = '('; tree[fim_synt_tree+1].token = 'E'; tree[fim_synt_tree+2].token = 'X';
        tree[fim_synt_tree+3].token = 'E'; tree[fim_synt_tree+4].token = ')';
        tree[fim_synt_tree].visitado = 1; tree[fim_synt_tree+4].visitado = 1;
        tree[fim_synt_tree].filhos = 5;
    }
    // P12: X -> +
    // P13: X -> -
    // P14: X -> *
    // P15: X -> /
    else if(productions[prod] >= 12 && productions[prod] <= 15) {
        if (tree[atual].token != 'X') { return; }
        if(productions[prod] == 12) { tree[fim_synt_tree].token = '+'; }
        else if(productions[prod] == 13) { tree[fim_synt_tree].token = '-'; }
        else if(productions[prod] == 14) { tree[fim_synt_tree].token = '*'; }
        else { tree[fim_synt_tree].token = '/'; }
        tree[fim_synt_tree].visitado = 1;
        tree[fim_synt_tree].filhos = 1;
    }
    // P16: C -> h=E
    // P17: C -> i=E
    // P18: C -> j=E
    // P19: C -> k=E
    // P20: C -> z=E
    else if(productions[prod] >= 16 && productions[prod] <= 20) {
        if (tree[atual].token != 'C') { return; }
        if (productions[prod] == 16){ tree[fim_synt_tree].token = 'h'; }
        else if (productions[prod] == 17){ tree[fim_synt_tree].token = 'i'; }
        else if (productions[prod] == 18){ tree[fim_synt_tree].token = 'j'; }
        else if (productions[prod] == 19){ tree[fim_synt_tree].token = 'k'; }
        else if (productions[prod] == 20){ tree[fim_synt_tree].token = 'z'; }
        tree[fim_synt_tree+1].token = '='; tree[fim_synt_tree+2].token = 'E';
        tree[fim_synt_tree].visitado = 1;  tree[fim_synt_tree+1].visitado = 1;
        tree[fim_synt_tree].filhos = 3;
    }
    // P22: C -> w(E){ C; }
    // P23: C -> f(E){ C; }
    else if(productions[prod] == 22 || productions[prod] == 23) {
        if (tree[atual].token != 'C') { return; }

        for (int i = 0; i < 8; i++){ tree[fim_synt_tree+i].visitado = 1; }
        if (productions[prod] == 22) { tree[fim_synt_tree].token = 'w'; }
        else { tree[fim_synt_tree].token = 'f'; }
        tree[fim_synt_tree+1].token = '('; tree[fim_synt_tree+2].token = 'E'; tree[fim_synt_tree+3].token = ')'; tree[fim_synt_tree+4].token = '{';
        tree[fim_synt_tree+5].token = 'C'; tree[fim_synt_tree+6].token = ';'; tree[fim_synt_tree+7].token = '}';
        tree[fim_synt_tree+2].visitado = 0; tree[fim_synt_tree+5].visitado = 0;
        tree[fim_synt_tree].filhos = 8;
    }
    // P24: C -> o(E;E;E){ C; }
    else if(productions[prod] == 24) {
        if (tree[atual].token != 'C') { return; }

        for (int i = 0; i < 12; i++){ tree[fim_synt_tree+i].visitado = 1; }
        tree[fim_synt_tree].token = 'o'; tree[fim_synt_tree+1].token = '('; tree[fim_synt_tree+2].token = 'E'; tree[fim_synt_tree+3].token = ';';
        tree[fim_synt_tree+4].token = 'E'; tree[fim_synt_tree+5].token = ';'; tree[fim_synt_tree+6].token = 'E'; tree[fim_synt_tree+7].token = ')';
        tree[fim_synt_tree+8].token = '{'; tree[fim_synt_tree+9].token = 'C'; tree[fim_synt_tree+10].token = ';'; tree[fim_synt_tree+11].token = '}';
        tree[fim_synt_tree+2].visitado = 0; tree[fim_synt_tree+4].visitado = 0; tree[fim_synt_tree+6].visitado = 0; tree[fim_synt_tree+9].visitado = 0;
        tree[fim_synt_tree].filhos = 12;
    }
    
    for (int i = 0, j = 1; i < tree[fim_synt_tree].filhos; i++, j++) { tree[fim_synt_tree+i].indice = tree[atual].indice*12+j; }
    fim_synt_tree += tree[fim_synt_tree].filhos;
    prod++;
}

void start_abs(Syntax *syntax_tree){
    Tree tree[fim_synt_tree];
    for (int i = 0; i < fim_synt_tree; i++) { tree[i].token = ' '; tree[i].visitado = 0; tree[i].indice = -1; tree[i].filhos = 1; tree[i].hash_original = 0; }
    fim_abs_tree = 1; tree[0].token = 'S'; tree[0].indice = 0;

    abs_tree(syntax_tree, tree, 0);

    for (int i = 0; i< fim_abs_tree; i++){ if (i<fim_abs_tree-1){ printf("[%d | %d | %c], ", i, tree[i].indice, tree[i].token); } else { printf("[%d | %d | %c]\n", i, tree[i].indice, tree[i].token); } }
}

void abs_tree(Syntax *syntax_tree, Tree *tree, int atual){
    if (atual == 0 && tree[atual].visitado == 1) { return; }
    int inicio_vetor = -1;

    for (int i = 0; i < fim_synt_tree || (tree[i].indice == tree[atual].indice*2+1); i++){ if (tree[i].indice == tree[atual].indice*2+1) { inicio_vetor = i; } }
    if ( inicio_vetor == -1) { put_token(syntax_tree, tree, atual); }
    else{
        for (int i = inicio_vetor; i < fim_abs_tree; i++){
            if(tree[i].indice == tree[atual].indice*2+1 || tree[i].indice == tree[atual].indice*2+2) {
                if (tree[i].visitado == 0){ abs_tree(syntax_tree, tree, i); return; }
            }
        }
        if (tree[atual].token == 'X') { put_token(syntax_tree, tree, atual); }
        tree[atual].visitado = 1;
    }
    abs_tree(syntax_tree, tree, 0);
}

void put_token(Syntax *syntax_tree, Tree *tree, int atual){
    // S
    if (tree[atual].token == 'S'){
        int tokens;
        for (int i = 0; i < fim_synt_tree; i++){
            if (syntax_tree[i].indice == tree[atual].hash_original*12+1){
                tokens = i;
                break;
            }
        }
        // M
        if (syntax_tree[tokens].token == 'M') {
            tree[atual].token = syntax_tree[tokens].token;
            tree[atual].hash_original = syntax_tree[tokens].indice;
        }
        //    S
        //   / \
        //  G   M
        else if (syntax_tree[tokens].token == 'G') {
            // S
            tree[atual].filhos = 2;
            // G, M
            for (int i = 0; i < tree[atual].filhos; i++){
                tree[fim_abs_tree].token = syntax_tree[tokens+i].token;
                tree[fim_abs_tree].hash_original = syntax_tree[tokens+i].indice;
                tree[fim_abs_tree].indice = tree[atual].indice*2+(i+1);
                tree[fim_abs_tree++].visitado = 0;
            }
        }
        //    S
        //   / \
        //  N   G
        //     / \
        //    G   M
        else if (syntax_tree[tokens].token == 'N') {
            tree[atual].filhos = 2;
            // N
            tree[fim_abs_tree].token = syntax_tree[tokens].token;
            tree[fim_abs_tree].hash_original = syntax_tree[tokens].indice;
            tree[fim_abs_tree].indice = 2*tree[atual].indice+1;
            tree[fim_abs_tree++].visitado = 0;
            // G pai
            tree[fim_abs_tree].token = syntax_tree[tokens+1].token;
            tree[fim_abs_tree].hash_original = syntax_tree[tokens+1].indice;
            tree[fim_abs_tree].indice = 2*tree[atual].indice+2;
            tree[fim_abs_tree].visitado = 0;
            int indice = tree[fim_abs_tree].indice;
            tree[fim_abs_tree++].filhos = 2;
            // G filho
            tree[fim_abs_tree].token = syntax_tree[tokens+1].token;
            tree[fim_abs_tree].hash_original = syntax_tree[tokens+1].indice;
            tree[fim_abs_tree].indice = 2*indice+1;
            tree[fim_abs_tree++].visitado = 0;
            // M
            tree[fim_abs_tree].token = syntax_tree[tokens+2].token;
            tree[fim_abs_tree].hash_original = syntax_tree[tokens+2].indice;
            tree[fim_abs_tree].indice = 2*indice+2;
            tree[fim_abs_tree++].visitado = 0;
        }

        else{
            tree[atual].visitado = 1;
            return;
        }
    }
    // NGM, GM, M
    else if (tree[atual].token == 'N' || tree[atual].token == 'G' || tree[atual].token == 'M'){
        int vetor[3];
        for (int i = 0; i < fim_synt_tree; i++){
            if (syntax_tree[i].indice == tree[atual].hash_original*12+1){
                for (int j = i; j < i+13; j++){
                    if (syntax_tree[j].token == 'C'){ vetor[0]=j; }
                    else if (syntax_tree[j].token == 'r'){ vetor[1]=j; }
                    else if (syntax_tree[j].token == 'E'){ vetor[2]=j; }
                }
                break;
            }
        }
        if (tree[atual].token == 'N'){ tree[atual].token = 'n'; }
        else if (tree[atual].token == 'G'){ tree[atual].token = 'g'; }
        else if (tree[atual].token == 'M') { tree[atual].token = 'm'; }
        else{
            tree[atual].visitado = 1;
            return;
        }
        tree[atual].filhos = 2;
        // C, r, E
        tree[fim_abs_tree].indice = 2*tree[atual].indice+1;
        tree[fim_abs_tree+1].indice = 2*tree[atual].indice+2;
        tree[fim_abs_tree+2].indice = 2*tree[fim_abs_tree+1].indice+1;
        for (int i = 0; i < 3; i++){
            tree[fim_abs_tree].token = syntax_tree[vetor[i]].token;
            tree[fim_abs_tree++].hash_original = syntax_tree[vetor[i]].indice;
        }
    }
    // E
    else if (tree[atual].token == 'E'){
        int tokens;
        for (int i = 0; i < fim_synt_tree; i++){
            if (syntax_tree[i].indice == tree[atual].hash_original*12+1){
                tokens = i;
                break;
            }
        }
        // (EXE)
        if (syntax_tree[tokens].token == '('){ tokens++;
            // X
            tree[atual].token = syntax_tree[tokens+1].token;
            tree[atual].hash_original = syntax_tree[tokens+1].indice;
            tree[atual].filhos = 2;
            tree[atual].visitado = 0;
            // E1
            tree[fim_abs_tree].token = syntax_tree[tokens].token;
            tree[fim_abs_tree].hash_original = syntax_tree[tokens].indice;
            tree[fim_abs_tree++].indice = 2*tree[atual].indice+1;
            // E2
            tree[fim_abs_tree].token = syntax_tree[tokens+2].token;
            tree[fim_abs_tree].hash_original = syntax_tree[tokens+2].indice;
            tree[fim_abs_tree++].indice = 2*tree[atual].indice+2;
        }
        // E -> 0, 1, x, y
        else if (syntax_tree[tokens].token == '0' || syntax_tree[tokens].token == '1' || 
        syntax_tree[tokens].token == 'x' || syntax_tree[tokens].token == 'y'){
            tree[atual].token = syntax_tree[tokens].token;
            tree[atual].hash_original = syntax_tree[tokens].indice;
            tree[atual].visitado = 1;
            tree[atual].filhos = 1;
        }

        else{
            tree[atual].visitado = 1;
            return;
        }
    }
    // X
    else if (tree[atual].token == 'X'){
        int tokens;
        for (int i = 0; i < fim_synt_tree; i++){
            if (syntax_tree[i].indice == tree[atual].hash_original*12+1){
                tokens = i;
                break;
            }
        }
        tree[atual].token = syntax_tree[tokens].token;
        tree[atual].hash_original = syntax_tree[tokens].indice;
        tree[atual].visitado = 1;
        tree[atual].filhos = 1;
    }
    // C
    else if (tree[atual].token == 'C'){
        int tokens;
        for (int i = 0; i < fim_synt_tree; i++){
            if (syntax_tree[i].indice == tree[atual].hash_original*12+1){
                tokens = i;
                break;
            }
        }
        // (EXE)
        if (syntax_tree[tokens].token == '('){ tokens++;
            // X
            tree[atual].token = syntax_tree[tokens+1].token;
            tree[atual].hash_original = syntax_tree[tokens+1].indice;
            tree[atual].filhos = 2;
            // E1
            tree[fim_abs_tree].token = syntax_tree[tokens].token;
            tree[fim_abs_tree].hash_original = syntax_tree[tokens].indice;
            tree[fim_abs_tree++].indice = 2*tree[atual].indice+1;
            // E2
            tree[fim_abs_tree].token = syntax_tree[tokens+2].token;
            tree[fim_abs_tree].hash_original = syntax_tree[tokens+2].indice;
            tree[fim_abs_tree++].indice = 2*tree[atual].indice+2;
        }
        // h, =, E
        // i, =, E
        // j, =, E
        // k, =, E
        // z, =, E
        else if (syntax_tree[tokens].token == 'h' || syntax_tree[tokens].token == 'i' || syntax_tree[tokens].token == 'j' ||
        syntax_tree[tokens].token == 'k' || syntax_tree[tokens].token == 'z'){
            // =
            tree[atual].token = syntax_tree[tokens+1].token;
            tree[atual].hash_original = syntax_tree[tokens+1].indice;
            tree[atual].filhos = 2;
            // h|i|j|k|z
            tree[fim_abs_tree].token = syntax_tree[tokens].token;
            tree[fim_abs_tree].hash_original = syntax_tree[tokens].indice;
            tree[fim_abs_tree].indice = 2*tree[atual].indice+1;
            tree[fim_abs_tree].filhos = 1;
            tree[fim_abs_tree++].visitado = 1;
            // E
            tree[fim_abs_tree].token = syntax_tree[tokens+2].token;
            tree[fim_abs_tree].hash_original = syntax_tree[tokens+2].indice;
            tree[fim_abs_tree].indice = 2*tree[atual].indice+2;
            tree[fim_abs_tree++].visitado = 0;
        }
            // w | f, E, C
        else if (syntax_tree[tokens].token == 'w' || syntax_tree[tokens].token == 'f'){
            int valores[2];
            for (int i = 0; i < fim_synt_tree; i++){
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
            tree[fim_abs_tree].token = syntax_tree[valores[0]].token;
            tree[fim_abs_tree].hash_original = syntax_tree[valores[0]].indice;
            tree[fim_abs_tree].indice = 2*tree[atual].indice+1;
            tree[fim_abs_tree++].visitado = 0;
            // C
            tree[fim_abs_tree].token = syntax_tree[valores[1]].token;
            tree[fim_abs_tree].hash_original = syntax_tree[valores[1]].indice;
            tree[fim_abs_tree].indice = 2*tree[atual].indice+2;
            tree[fim_abs_tree++].visitado = 0;
        }

        // C -> o, E, E, E, C
        else if (syntax_tree[tokens].token == 'o'){
            int valores[4], test = 0;
            for (int i = 0; i < fim_synt_tree; i++){
                if (syntax_tree[i].indice == tree[atual].hash_original*12+1){
                    for (int j = i; j < i+12; j++){
                        if (syntax_tree[j].token == 'E'){ valores[test++] = j; }
                        else if (syntax_tree[j].token == 'C'){ valores[test] = j; break; }
                    }
                    break;
                }
            }
            // o
            tree[atual].token = syntax_tree[tokens].token;
            tree[atual].hash_original = syntax_tree[tokens].indice;
            tree[atual].filhos = 2;
            // E2
            tree[fim_abs_tree].token = syntax_tree[valores[1]].token;
            tree[fim_abs_tree].hash_original = syntax_tree[valores[1]].indice;
            tree[fim_abs_tree].indice = 2*tree[atual].indice+1;
            tree[fim_abs_tree].filhos = 1;
            tree[fim_abs_tree++].visitado = 0;
            // E
            tree[fim_abs_tree].token = syntax_tree[valores[1]].token;
            tree[fim_abs_tree].hash_original = syntax_tree[valores[1]].indice;
            tree[fim_abs_tree].indice = 2*tree[fim_abs_tree-1].indice+1;
            tree[fim_abs_tree++].visitado = 0;
            // E1
            tree[fim_abs_tree].token = syntax_tree[valores[0]].token;
            tree[fim_abs_tree].hash_original = syntax_tree[valores[0]].indice;
            tree[fim_abs_tree].indice = 2*tree[atual].indice+2;
            tree[fim_abs_tree].filhos = 1;
            tree[fim_abs_tree++].visitado = 0;
            // E
            tree[fim_abs_tree].token = syntax_tree[valores[0]].token;
            tree[fim_abs_tree].hash_original = syntax_tree[valores[0]].indice;
            tree[fim_abs_tree].indice = 2*tree[fim_abs_tree-1].indice+1;
            tree[fim_abs_tree++].visitado = 0;
            // C
            tree[fim_abs_tree].token = syntax_tree[valores[3]].token;
            tree[fim_abs_tree].hash_original = syntax_tree[valores[3]].indice;
            tree[fim_abs_tree].indice = 2*tree[fim_abs_tree-2].indice+2;
            tree[fim_abs_tree].filhos = 2;
            tree[fim_abs_tree++].visitado = 0;
            // C
            tree[fim_abs_tree].token = syntax_tree[valores[3]].token;
            tree[fim_abs_tree].hash_original = syntax_tree[valores[3]].indice;
            tree[fim_abs_tree].indice = 2*tree[fim_abs_tree-1].indice+1;
            tree[fim_abs_tree++].visitado = 0;
            // E
            tree[fim_abs_tree].token = syntax_tree[valores[2]].token;
            tree[fim_abs_tree].hash_original = syntax_tree[valores[2]].indice;
            tree[fim_abs_tree].indice = 2*tree[fim_abs_tree-2].indice+2;
            tree[fim_abs_tree++].visitado = 0;
        }
        else{
            tree[atual].visitado = 1;
            return;
        }
    }
}