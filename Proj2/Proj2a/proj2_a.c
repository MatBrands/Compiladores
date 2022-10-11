#include <stdio.h>
#include <stdlib.h>

int productions[100];
int p_count;

FILE *readFile(char *);
void parsing_table(FILE *);
void parsing_tree();

int main (int argc, char **argv){
    FILE *file;
    if(argc == 2){ file = readFile(argv[1]); }
    else{ printf("Carregue o arquivo contendo os inputs.\n"); exit(1); }

    int i = 0;
    while(!feof(file)){
        printf("Palavra %d:\n", ++i);
        parsing_table(file);
        if (p_count > 0) { printf("Arvore: "); parsing_tree(); }
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
    top++;
    stack[top] = 'S';
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
        printf("Producoes: ");
        for (int i = 0; i<p_count; i++) { printf("P%d   ", productions[i]); }
        printf("\n");
        goto tbl_fim; 
    }
    // Erro
    else {
        p_count = 0;
        printf("Erro.\n");
        tbl_loop:
        if(token != '\n' && !feof(file)){ token = fgetc(file); goto tbl_loop; }
        goto tbl_fim; 
    }
    goto tbl_q1;
    tbl_fim:
}

void parsing_tree(){
    const int max = 8096;
    char tree[max];
    int visitados[max];
    int i, top = 0;

    tree_q0:
    for (i = 0; i < max; i++) { tree[i] = ' '; visitados[i] = 0; }
    i = 0;
    tree[0] = 'S';
    tree_q1:

    if (i >= p_count) goto tree_fim;
    // P1: S -> M
    if (productions[i] == 1){ 
        if (tree[top] == 'S' && tree[4*top+1] == ' '){
            tree[4*top + 1] = 'M'; tree[4*top + 2] = ' '; tree[4*top + 3] = ' '; tree[4*top + 4] = ' ';
            i++;
            visitados[4*top+2] = 4; visitados[4*top+3] = 4; visitados[4*top+4] = 4;
        }
    }
    // P2: S -> GM
    else if(productions[i] == 2) {
        if (tree[top] == 'S' && tree[4*top+1] == ' '){
            tree[4*top + 1] = 'G'; tree[4*top + 2] = 'M'; tree[4*top + 3] = ' '; tree[4*top + 4] = ' ';
            i++;
            visitados[4*top+3] = 4; visitados[4*top+4] = 4;
        }
    }
    // P3: S -> NGM
    else if(productions[i] == 3) {
        if (tree[top] == 'S' && tree[4*top+1] == ' '){
            tree[4*top + 1] = 'N'; tree[4*top + 2] = 'G'; tree[4*top + 3] = 'M'; tree[4*top + 4] = ' ';
            i++;
            visitados[4*top+4] = 4;
        }
    }
    // P4: N -> n(){ C; r(E); }
    else if(productions[i] == 4) {
        if (tree[top] == 'N' && tree[4*top+1] == ' '){
            tree[4*top + 1] = 'C'; tree[4*top + 2] = 'E'; tree[4*top + 3] = ' '; tree[4*top + 4] = ' ';
            i++;
            visitados[4*top+3] = 4; visitados[4*top+4] = 4;
        }
    }
    // P5: G -> g(){ C; r(E); }
    else if(productions[i] == 5) {
        if (tree[top] == 'G' && tree[4*top+1] == ' '){
            tree[4*top + 1] = 'C'; tree[4*top + 2] = 'E'; tree[4*top + 3] = ' '; tree[4*top + 4] = ' ';
            i++;
            visitados[4*top+3] = 4; visitados[4*top+4] = 4;
        }
    }
    // P6: M -> m() { C; r(E); }
    else if(productions[i] == 6) {
        if (tree[top] == 'M' && tree[4*top+1] == ' '){
            tree[4*top + 1] = 'C'; tree[4*top + 2] = 'E'; tree[4*top + 3] = ' '; tree[4*top + 4] = ' ';
            i++;
            visitados[4*top+3] = 4; visitados[4*top+4] = 4;
        }
    }
    // P7: E -> 0
    // P8: E -> 1
    // P9: E -> x
    // P10: E -> y
    else if(productions[i] >= 7 && productions[i] <= 10) {
        if (tree[top] == 'E' && tree[4*top+1] == ' '){
            if (productions[i] == 7) tree[4*top + 1] = '0';
            else if (productions[i] == 8) { tree[4*top + 1] = '1'; }
            else if (productions[i] == 9) { tree[4*top + 1] = 'x'; }
            else { tree[4*top + 1] = 'y'; }
            tree[4*top + 2] = ' '; tree[4*top + 3] = ' '; tree[4*top + 4] = ' ';
            i++;
            visitados[4*top+1] = 4; visitados[4*top+2] = 4; visitados[4*top+3] = 4; visitados[4*top+4] = 4;
        }
    }
    // P11: E -> (EXE)
    else if(productions[i] == 11) {
        if (tree[top] == 'E' && tree[4*top+1] == ' '){
            tree[4*top + 1] = 'E'; tree[4*top + 2] = 'X'; tree[4*top + 3] = 'E'; tree[4*top + 4] = ' ';
            i++;
            visitados[4*top+4] = 4;
        }
    }
    // P12: X -> +
    // P13: X -> -
    // P14: X -> *
    // P15: X -> /
    else if(productions[i] >= 12 && productions[i] <= 15) {
        if (tree[top] == 'X' && tree[4*top+1] == ' '){
            if(productions[i] == 12) { tree[4*top + 1] = '+'; }
            else if(productions[i] == 13) { tree[4*top + 1] = '-'; }
            else if(productions[i] == 14) { tree[4*top + 1] = '*'; }
            else { tree[4*top + 1] = '/'; }
            tree[4*top + 2] = ' '; tree[4*top + 3] = ' '; tree[4*top + 4] = ' ';
            i++;
            visitados[4*top+1] = 4; visitados[4*top+2] = 4; visitados[4*top+3] = 4; visitados[4*top+4] = 4;
        }
    }
    // P16: C -> h=E
    // P17: C -> i=E
    // P18: C -> j=E
    // P19: C -> k=E
    // P20: C -> z=E
    else if(productions[i] >= 16 && productions[i] <= 20) {
        if (tree[top] == 'C' && tree[4*top+1] == ' '){
            tree[4*top + 1] = 'E'; tree[4*top + 2] = ' '; tree[4*top + 3] = ' '; tree[4*top + 4] = ' ';
            i++;
            visitados[4*top+2] = 4; visitados[4*top+3] = 4; visitados[4*top+4] = 4;
        }
    }
    // P21: C -> (EXE)
    else if(productions[i] == 21) {
        if (tree[top] == 'C' && tree[4*top+1] == ' '){
            tree[4*top + 1] = 'E'; tree[4*top + 2] = 'X'; tree[4*top + 3] = 'E'; tree[4*top + 4] = ' ';
            i++;
            visitados[4*top+4] = 4;
        }
    }
    // P22: C -> w(E){ C; }
    else if(productions[i] == 22) {
        if (tree[top] == 'C' && tree[4*top+1] == ' '){
            tree[4*top + 1] = 'E'; tree[4*top + 2] = 'C'; tree[4*top + 3] = ' '; tree[4*top + 4] = ' ';
            i++;
            visitados[4*top+3] = 4; visitados[4*top+4] = 4;
        }
    }
    // P23: C -> f(E){ C; }
    else if(productions[i] == 23) {
        if (tree[top] == 'C' && tree[4*top+1] == ' '){
            tree[4*top + 1] = 'E'; tree[4*top + 2] = 'C'; tree[4*top + 3] = ' '; tree[4*top + 4] = ' ';
            i++;
            visitados[4*top+3] = 4; visitados[4*top+4] = 4;
        }
    }
    // P24: C -> o(E;E;E){ C; }
    else if(productions[i] == 24) {
        if (tree[top] == 'C' && tree[4*top+1] == ' '){
            tree[4*top + 1] = 'E'; tree[4*top + 2] = 'E'; tree[4*top + 3] = 'E'; tree[4*top + 4] = 'C';
            i++;
        }
    }
    
    if (visitados[4*top+1] < 4){ top = 4*top+1; goto tree_q1; }
    if (visitados[4*top+2] < 4){ top = 4*top+2; goto tree_q1; }
    if (visitados[4*top+3] < 4){ top = 4*top+3; goto tree_q1; }
    if (visitados[4*top+4] < 4){ top = 4*top+4; goto tree_q1; }
    int aux = 0;
    if (visitados[4*top+1] == 4){ aux++; }
    if (visitados[4*top+2] == 4){ aux++; }
    if (visitados[4*top+3] == 4){ aux++; }
    if (visitados[4*top+4] == 4){ aux++; }
    visitados[top] = aux;
    if (visitados[top] == 4){ top = 0; }
    
    goto tree_q1;

    tree_fim:
    for (int i = 0; i< max; i++){
        if (tree[i] != ' ') { printf("%d, %c\t", i, tree[i]); }
    }

    printf("\n");
}