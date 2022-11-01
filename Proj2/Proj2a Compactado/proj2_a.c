#include <stdio.h>
#include <stdlib.h>

struct No{
    char token;
	int visitado;
    int indice;
    int filhos;
}; typedef struct No Tree;
const int max = 500;
int productions[100], prod, p_count, fim_vetor;

FILE *readFile(char *);
void parsing_table(FILE *);
void start_tree();
void parse_tree(Tree *, int);
void put_production(Tree *, int);

int main (int argc, char **argv){
    FILE *file;
    if(argc == 2){ file = readFile(argv[1]); }
    else{ printf("Carregue o arquivo contendo os inputs.\n"); exit(1); }

    int i = 0;
    while(!feof(file)){
        printf("Palavra %d:\n", ++i);
        parsing_table(file);
        printf("Arvore: "); start_tree();
        if (!feof(file)) printf("\n");
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

void parsing_table(FILE *file){
    // i | Qi | Token | Stack | Pi
    char token, stack[512];
    int i = 0, top = -1;
    p_count = 0;
    tbl_q0:
    token = fgetc(file);
    printf("i \t Qi \t Token \t Stack \t Pi\n");
    printf("%d \t Q0 \t %c \t %c \t -\n", i, token, stack[top]);
    stack[++top] = 'S';
    goto tbl_q1;
    
    tbl_q1:
    if (token == ' ') token = fgetc(file);
    // P1: S -> M
    else if (stack[top] == 'S' && token == 'm') { stack[top] = 'M'; printf("%d \t Q1 \t %c \t %c \t P1\n", ++i, token, stack[top]); productions[p_count++] = 1; }
    // P2: S -> GM
    else if (stack[top] == 'S' && token == 'g') { stack[top] = 'M'; stack[++top] = 'G'; printf("%d \t Q1 \t %c \t %c \t P2\n", ++i, token, stack[top]); productions[p_count++] = 2; }
    // P3: S -> NGM
    else if (stack[top] == 'S' && token == 'n') { stack[top] = 'M'; stack[++top] = 'G'; stack[++top] = 'N'; printf("%d \t Q1 \t %c \t %c \t P3\n", ++i, token, stack[top]); productions[p_count++] = 3; }
    // 
    else if (token == stack[top]){ printf("%d \t Q1 \t %c \t %c \t -\n", i, token, stack[--top]); token = fgetc(file); }
    // P4: N -> n(){ C; r(E); }
    else if (stack[top] == 'N' && token == 'n') { 
        stack[top] = '}';
        stack[++top] = ';';
        stack[++top] = ')';
        stack[++top] = 'E';
        stack[++top] = '(';
        stack[++top] = 'r';
        stack[++top] = ';';
        stack[++top] = 'C';
        stack[++top] = '{';
        stack[++top] = ')';
        stack[++top] = '(';
        token = fgetc(file);
        printf("%d \t Q1 \t %c \t %c \t P4\n", ++i, token, stack[top]);
        productions[p_count++] = 4;
    }  
    // P5: G -> g(){ C; r(E); }
    else if (stack[top] == 'G' && token == 'g') {
        stack[top] = '}';
        stack[++top] = ';';
        stack[++top] = ')';
        stack[++top] = 'E';
        stack[++top] = '(';
        stack[++top] = 'r';
        stack[++top] = ';';
        stack[++top] = 'C';
        stack[++top] = '{';
        stack[++top] = ')';
        stack[++top] = '(';
        token = fgetc(file);
        printf("%d \t Q1 \t %c \t %c \t P5\n", ++i, token, stack[top]);
        productions[p_count++] = 5;
    }   
    // P6: M -> m() { C; r(E); }
    else if (stack[top] == 'M' && token == 'm') {
        stack[top] = '}';
        stack[++top] = ';';
        stack[++top] = ')';
        stack[++top] = 'E';
        stack[++top] = '(';
        stack[++top] = 'r';
        stack[++top] = ';';
        stack[++top] = 'C';
        stack[++top] = '{';
        stack[++top] = ')';
        stack[++top] = '(';
        token = fgetc(file);
        printf("%d \t Q1 \t %c \t %c \t P6\n", ++i, token, stack[top]);
        productions[p_count++] = 6;
    }
    // P7: E -> 0
    else if (stack[top] == 'E' && token == '0') { token = fgetc(file); --top; printf("%d \t Q1 \t %c \t %c \t P7\n", ++i, token, stack[top]); productions[p_count++] = 7; }
    // P8: E -> 1
    else if (stack[top] == 'E' && token == '1') { token = fgetc(file); --top; printf("%d \t Q1 \t %c \t %c \t P8\n", ++i, token, stack[top]); productions[p_count++] = 8; }
    // P9: E -> x
    else if (stack[top] == 'E' && token == 'x') { token = fgetc(file); --top; printf("%d \t Q1 \t %c \t %c \t P9\n", ++i, token, stack[top]); productions[p_count++] = 9; }
    // P10: E -> y
    else if (stack[top] == 'E' && token == 'y') { token = fgetc(file); --top; printf("%d \t Q1 \t %c \t %c \t P10\n", ++i, token, stack[top]); productions[p_count++] = 10; }
    // P11: E -> (EXE)
    else if (stack[top] == 'E' && token == '(') {
        stack[top] = ')';
        stack[++top] = 'E';
        stack[++top] = 'X';
        stack[++top] = 'E';
        token = fgetc(file);
        printf("%d \t Q1 \t %c \t %c \t P11\n", ++i, token, stack[top]);
        productions[p_count++] = 11;
    }
    // P12: X -> +
    else if (stack[top] == 'X' && token == '+') { token = fgetc(file); --top; printf("%d \t Q1 \t %c \t %c \t P12\n", ++i, token, stack[top]); productions[p_count++] = 12; }
    // P13: X -> -
    else if (stack[top] == 'X' && token == '-') { token = fgetc(file); --top; printf("%d \t Q1 \t %c \t %c \t P13\n", ++i, token, stack[top]); productions[p_count++] = 13; }
    // P14: X -> *
    else if (stack[top] == 'X' && token == '*') { token = fgetc(file); --top; printf("%d \t Q1 \t %c \t %c \t P14\n", ++i, token, stack[top]); productions[p_count++] = 14; }
    // P15: X -> /
    else if (stack[top] == 'X' && token == '/') { token = fgetc(file); --top; printf("%d \t Q1 \t %c \t %c \t P15\n", ++i, token, stack[top]); productions[p_count++] = 15; }
    // P16: C -> h=E
    else if (stack[top] == 'C' && token == 'h') { 
        stack[top] = 'E'; 
        stack[++top] = '=';
        token = fgetc(file);
        printf("%d \t Q1 \t %c \t %c \t P16\n", ++i, token, stack[top]);
        productions[p_count++] = 16;
    }
    // P17: C -> i=E
    else if (stack[top] == 'C' && token == 'i') {
        stack[top] = 'E'; 
        stack[++top] = '=';
        token = fgetc(file);
        printf("%d \t Q1 \t %c \t %c \t P16\n", ++i, token, stack[top]);
        productions[p_count++] = 17;
    }
    // P18: C -> j=E
    else if (stack[top] == 'C' && token == 'j') {
        stack[top] = 'E'; 
        stack[++top] = '=';
        token = fgetc(file);
        printf("%d \t Q1 \t %c \t %c \t P16\n", ++i, token, stack[top]);
        productions[p_count++] = 18;
        }
    // P19: C -> k=E
    else if (stack[top] == 'C' && token == 'k') {
        stack[top] = 'E'; 
        stack[++top] = '=';
        token = fgetc(file);
        printf("%d \t Q1 \t %c \t %c \t P16\n", ++i, token, stack[top]);
        productions[p_count++] = 19;
    }
    // P20: C -> z=E
    else if (stack[top] == 'C' && token == 'z') {
        stack[top] = 'E'; 
        stack[++top] = '=';
        token = fgetc(file);
        printf("%d \t Q1 \t %c \t %c \t P16\n", ++i, token, stack[top]);
        productions[p_count++] = 20;
    }
    // P21: C -> (EXE)
    else if (stack[top] == 'C' && token == '(') {
        stack[top] = ')';
        stack[++top] = 'E';
        stack[++top] = 'X';
        stack[++top] = 'E';
        token = fgetc(file);
        printf("%d \t Q1 \t %c \t %c \t P21\n", ++i, token, stack[top]);
        productions[p_count++] = 21;
    }
    // P22: C -> w(E){ C; }
    else if (stack[top] == 'C' && token == 'w') {
        stack[top] = '}';
        stack[++top] = ';';
        stack[++top] = 'C';
        stack[++top] = '{';
        stack[++top] = ')';
        stack[++top] = 'E';
        stack[++top] = '(';
        token = fgetc(file);
        printf("%d \t Q1 \t %c \t %c \t P22\n", ++i, token, stack[top]); 
        productions[p_count++] = 22;
    }
    // P23: C -> f(E){ C; }
    else if (stack[top] == 'C' && token == 'f') { 
        stack[top] = '}';
        stack[++top] = ';';
        stack[++top] = 'C';
        stack[++top] = '{';
        stack[++top] = ')';
        stack[++top] = 'E';
        stack[++top] = '(';
        token = fgetc(file);
        printf("%d \t Q1 \t %c \t %c \t P23\n", ++i, token, stack[top]);
        productions[p_count++] = 23;
    }
    // P24: C -> o(E;E;E){ C; }
    else if (stack[top] == 'C' && token == 'o') { 
        stack[top] = '}';
        stack[++top] = ';';
        stack[++top] = 'C';
        stack[++top] = '{';
        stack[++top] = ')';
        stack[++top] = 'E';
        stack[++top] = ';';
        stack[++top] = 'E';
        stack[++top] = ';';
        stack[++top] = 'E';
        stack[++top] = '(';
        token = fgetc(file);
        printf("%d \t Q1 \t %c \t %c \t P24\n", ++i, token, stack[top]);
        productions[p_count++] = 24;
    }
    // Palavra aceita
    else if ((feof(file) || token == '\n') && top == -1 ) { 
        printf("Palavra aceita.\n");
        printf("Producoes: ");
        for (int i = 0; i<p_count; i++) { printf("P%d   ", productions[i]); }
        printf("\n");
        goto tbl_fim; 
    }
    // Erro
    else {
        printf("Erro.\n");
        printf("Producoes: ");
        for (int i = 0; i<p_count; i++) { printf("P%d   ", productions[i]); }
        printf("\n");
        tbl_loop:
        if(token != '\n' && !feof(file)){ token = fgetc(file); goto tbl_loop; }
        goto tbl_fim; 
    }
    goto tbl_q1;
    tbl_fim:
}

void start_tree(){
    Tree tree[max];
    for (int i = 0; i < max; i++) { tree[i].token = ' '; tree[i].visitado = 0; tree[i].indice = -1; tree[i].filhos = 0; }
    prod = 0; fim_vetor = 1; tree[0].token = 'S'; tree[0].indice = 0;

    parse_tree(tree, 0);

    for (int i = 0; i< fim_vetor; i++){ if (i<fim_vetor-1){ printf("[%d|%d| %c ], ", i, tree[i].indice, tree[i].token); } else { printf("[%d|%d| %c ]\n", i, tree[i].indice, tree[i].token); } }
}

void parse_tree(Tree * tree, int atual){
    if (prod > p_count || atual == 0 && tree[atual].visitado == 1) { return; }
    int inicio_vetor = -1;

    for (int i = 0; i < fim_vetor || (tree[i].indice == tree[atual].indice*12+1); i++){ if (tree[i].indice == tree[atual].indice*12+1) { inicio_vetor = i; } }
    
    if ( inicio_vetor == -1 ) { put_production(tree, atual); }
    else{
        for (int i = inicio_vetor; i < inicio_vetor+tree[inicio_vetor].filhos; i++){
            if (tree[i].visitado == 0){ parse_tree(tree, i); return; }
        }
        tree[atual].visitado = 1;
    }
    
    parse_tree(tree, 0);
}

void put_production(Tree * tree, int atual){
    // P1: S -> M
    if (productions[prod] == 1){
        if (tree[atual].token != 'S') { return; }
        tree[fim_vetor].token = 'M';
        tree[fim_vetor].filhos = 1;
    }
    // P2: S -> GM
    else if(productions[prod] == 2) {
        if (tree[atual].token != 'S') { return; }
        tree[fim_vetor].token = 'G'; tree[fim_vetor+1].token = 'M';
        tree[fim_vetor].filhos = 2;
    }
    // P3: S -> NGM
    else if(productions[prod] == 3) {
        if (tree[atual].token != 'S') { return; }
        tree[fim_vetor].token = 'N'; tree[fim_vetor+1].token = 'G'; tree[fim_vetor+2].token = 'M';
        tree[fim_vetor].filhos = 3;
    }
    // P4: N -> n(){ C; r(E); }
    // P5: G -> g(){ C; r(E); }
    // P6: M -> m() { C; r(E); }
    else if(productions[prod] >= 4 && productions[prod] <= 6) {
        for (int i = 0; i < 12; i++){ tree[fim_vetor+i].visitado = 1; }

        if (productions[prod] == 4) {
            if (tree[atual].token != 'N') { return; }
            tree[fim_vetor].token = 'n';
        }
        else if (productions[prod] == 5) {
            if (tree[atual].token != 'G') { return; }
            tree[fim_vetor].token = 'g';
        }
        else {
            if (tree[atual].token != 'M') { return; }
            tree[fim_vetor].token = 'm';
        }
        tree[fim_vetor+1].token = '('; tree[fim_vetor+2].token = ')'; tree[fim_vetor+3].token = '{';  tree[fim_vetor+4].token = 'C'; 
        tree[fim_vetor+5].token = ';'; tree[fim_vetor+6].token = 'r'; tree[fim_vetor+7].token = '('; tree[fim_vetor+8].token = 'E';
        tree[fim_vetor+9].token = ')'; tree[fim_vetor+10].token = ';'; tree[fim_vetor+11].token = '}';
        tree[fim_vetor+4].visitado = 0; tree[fim_vetor+8].visitado = 0;
        tree[fim_vetor].filhos = 12;
    }
    // P7: E -> 0
    // P8: E -> 1
    // P9: E -> x
    // P10: E -> y
    else if(productions[prod] >= 7 && productions[prod] <= 10) {
        if (tree[atual].token != 'E') { return; }
        if (productions[prod] == 7) { tree[fim_vetor].token = '0'; }
        else if (productions[prod] == 8) { tree[fim_vetor].token = '1'; }
        else if (productions[prod] == 9) { tree[fim_vetor].token = 'x'; }
        else { tree[fim_vetor].token = 'y'; }
        tree[fim_vetor].visitado = 1;
        tree[fim_vetor].filhos = 1;
    }
    // P11: E -> (EXE)
    // P21: C -> (EXE)
    else if(productions[prod] == 11 || productions[prod] == 21) {
        if(productions[prod] == 11) { if (tree[atual].token != 'E') { return; } }
        else { if (tree[atual].token != 'C') { return; } }
        tree[fim_vetor].token = '('; tree[fim_vetor+1].token = 'E'; tree[fim_vetor+2].token = 'X';
        tree[fim_vetor+3].token = 'E'; tree[fim_vetor+4].token = ')';
        tree[fim_vetor].visitado = 1; tree[fim_vetor+4].visitado = 1;
        tree[fim_vetor].filhos = 5;
    }
    // P12: X -> +
    // P13: X -> -
    // P14: X -> *
    // P15: X -> /
    else if(productions[prod] >= 12 && productions[prod] <= 15) {
        if (tree[atual].token != 'X') { return; }
        if(productions[prod] == 12) { tree[fim_vetor].token = '+'; }
        else if(productions[prod] == 13) { tree[fim_vetor].token = '-'; }
        else if(productions[prod] == 14) { tree[fim_vetor].token = '*'; }
        else { tree[fim_vetor].token = '/'; }
        tree[fim_vetor].visitado = 1;
        tree[fim_vetor].filhos = 1;
    }
    // P16: C -> h=E
    // P17: C -> i=E
    // P18: C -> j=E
    // P19: C -> k=E
    // P20: C -> z=E
    else if(productions[prod] >= 16 && productions[prod] <= 20) {
        if (tree[atual].token != 'C') { return; }
        if (productions[prod] == 16){ tree[fim_vetor].token = 'h'; }
        else if (productions[prod] == 17){ tree[fim_vetor].token = 'i'; }
        else if (productions[prod] == 18){ tree[fim_vetor].token = 'j'; }
        else if (productions[prod] == 19){ tree[fim_vetor].token = 'k'; }
        else if (productions[prod] == 20){ tree[fim_vetor].token = 'z'; }
        tree[fim_vetor+1].token = '='; tree[fim_vetor+2].token = 'E';
        tree[fim_vetor].visitado = 1;  tree[fim_vetor+1].visitado = 1;
        tree[fim_vetor].filhos = 3;
    }
    // P22: C -> w(E){ C; }
    // P23: C -> f(E){ C; }
    else if(productions[prod] == 22 || productions[prod] == 23) {
        if (tree[atual].token != 'C') { return; }

        for (int i = 0; i < 8; i++){ tree[fim_vetor+i].visitado = 1; }
        if (productions[prod] == 22) { tree[fim_vetor].token = 'w'; }
        else { tree[fim_vetor].token = 'f'; }
        tree[fim_vetor+1].token = '('; tree[fim_vetor+2].token = 'E'; tree[fim_vetor+3].token = ')'; tree[fim_vetor+4].token = '{';
        tree[fim_vetor+5].token = 'C'; tree[fim_vetor+6].token = ';'; tree[fim_vetor+7].token = '}';
        tree[fim_vetor+2].visitado = 0; tree[fim_vetor+5].visitado = 0;
        tree[fim_vetor].filhos = 8;
    }
    // P24: C -> o(E;E;E){ C; }
    else if(productions[prod] == 24) {
        if (tree[atual].token != 'C') { return; }

        for (int i = 0; i < 12; i++){ tree[fim_vetor+i].visitado = 1; }
        tree[fim_vetor].token = 'o'; tree[fim_vetor+1].token = '('; tree[fim_vetor+2].token = 'E'; tree[fim_vetor+3].token = ';';
        tree[fim_vetor+4].token = 'E'; tree[fim_vetor+5].token = ';'; tree[fim_vetor+6].token = 'E'; tree[fim_vetor+7].token = ')';
        tree[fim_vetor+8].token = '{'; tree[fim_vetor+9].token = 'C'; tree[fim_vetor+10].token = ';'; tree[fim_vetor+11].token = '}';
        tree[fim_vetor+2].visitado = 0; tree[fim_vetor+4].visitado = 0; tree[fim_vetor+6].visitado = 0; tree[fim_vetor+9].visitado = 0;
        tree[fim_vetor].filhos = 12;
    }
    
    for (int i = 0, j = 1; i < tree[fim_vetor].filhos; i++, j++) { tree[fim_vetor+i].indice = tree[atual].indice*12+j; }

    fim_vetor += tree[fim_vetor].filhos;
    prod++;
}