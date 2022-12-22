#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define max 500

struct Producao{
    char esquerda;
	char direita[20];
    int tam;
};typedef struct Producao Producao;

struct Tree{
    char token;
	int visitado;
    int indice;
    int filhos;
}; typedef struct Tree Tree;

FILE *file, *saida;
Producao gramatica[100];
int producoes_gramatica, prod, fim_vetor, tam_tree;
int productions[100], p_count;

// 
FILE *readFile(char *);
void readProd();
char *readPalavra(FILE *, char *);
void parsing_table(char *);
void start_tree();
void parse_tree(Tree *, int);
void put_production(Tree *, int);
// 

int main (int argc, char **argv){
    if(argc == 2){ readProd(); file = readFile(argv[1]); }
    else{ printf("Carregue os arquivos contendo as producoes e inputs.\n"); exit(1); }

    for (int i = 0; i < producoes_gramatica; i ++){
        printf("P%d: %c -> %s\n", i+1, gramatica[i].esquerda, gramatica[i].direita, gramatica[i].tam);
    }
    printf("Arvore tera %d filhos\n", tam_tree);

    saida = fopen("ada_tree", "w");

    printf("\n************************************************************************************\n\n");
    int i = 0;
    char palavra[100];
    while(!feof(file)){
        printf("Palavra %d:\n", ++i);
        readPalavra(file, palavra);
        printf("%s\n", palavra);
        parsing_table(palavra);
        if (!feof(file)) printf("\n");
    }

    fclose(file); file = NULL;
    fclose(saida); saida = NULL;
    return 0;
}

void readProd(){
    FILE *file;
    file = fopen("grammar", "r");
    if(file == NULL){
        printf("Erro ao abrir arquivo.\n");
        exit(-1);
    }
    char token; producoes_gramatica = 0;
    token = fgetc(file);
    gramatica[producoes_gramatica].tam = 0; gramatica[producoes_gramatica].esquerda = token;
    while (!feof(file)){
        token = fgetc(file);
        if (token == '\n') {
            gramatica[producoes_gramatica].direita[gramatica[producoes_gramatica].tam+1] = '\0';
            producoes_gramatica++;
            token = fgetc(file);
            gramatica[producoes_gramatica].tam = 0;
            gramatica[producoes_gramatica].esquerda = token;
        }
        else if (token != ' '){
            gramatica[producoes_gramatica].direita[gramatica[producoes_gramatica].tam++] = token;
        }
    }
    for (int i = 0; i < producoes_gramatica; i++){ if (tam_tree < gramatica[i].tam) { tam_tree = gramatica[i].tam; } }
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

char *readPalavra(FILE *file, char *palavra){
    int i = 0;
    char token;
    do{
        token = fgetc(file);
        if (token != '\n' && !feof(file)) { palavra[i++] = token; }
    }while(!feof(file) && token != '\n');
    palavra[i] = '\0';
}

void parsing_table(char *palavra){
    // i | Qi | Token | Stack | Pi
    char token, stack[512];
    int indice = 0, top = -1, ponteiro = 0;
    p_count = 0;
    q0:
    token = palavra[ponteiro++];
    printf("i \t Qi \t Token \t Stack \t Pi\n");
    printf("%d \t Q0 \t %c \t %c \t -\n", indice, token, stack[top]);
    stack[++top] = gramatica[0].esquerda;
    q1:
    if (token == '\0' && top == -1) {
        printf("Palavra aceita.\n");
        printf("Producoes: ");
        for (int i = 0; i<p_count-1; i++) { printf("P%d   ", productions[i]); }
        printf("P%d\n", productions[p_count-1]);
        fprintf(saida, "%s\n", palavra);
        printf("Arvore sintatica: "); start_tree();
        goto fim; 
    }
    if (token == ' ') { token = palavra[ponteiro++]; goto q1; }
    else{
        for(int i = 0; i < producoes_gramatica; i++){
            if (stack[top] == gramatica[i].esquerda && token == gramatica[i].direita[0]){
                printf("%d \t Q1 \t %c \t %c \t P%d\n", ++indice, token, stack[top], i+1);
                if (gramatica[i].tam == 1){ token = palavra[ponteiro++]; top--; }
                else{
                    for (int j = gramatica[i].tam-1; j > 0; j--){ stack[top++] = gramatica[i].direita[j]; }
                    stack[top] = gramatica[i].direita[0];
                }
                productions[p_count++] = i+1;
                goto q1;
            }
            else if (token == stack[top]){ token = palavra[ponteiro++]; printf("%d \t Q1 \t %c \t %c \t -\n", indice, token, stack[--top]);  goto q1; }
        }
        for(int i = 0; i < producoes_gramatica; i++){
            if (token == gramatica[i].direita[0]){
                for(int j = 0; j < producoes_gramatica; j++){
                    if(gramatica[i].esquerda == gramatica[j].direita[0]){
                        for (int z = gramatica[j].tam-1; z > 0; z--){ stack[top++] = gramatica[j].direita[z]; }
                        stack[top] = gramatica[j].direita[0];
                        productions[p_count++] = j+1;
                        goto q1;
                    }
                }
            }
        }
        printf("Error! Token = '%c' nao esperado.\n", token);
        printf("Producoes: ");
        for (int i = 0; i<p_count-1; i++) { printf("P%d   ", productions[i]); }
        printf("P%d\n", productions[p_count-1]);
        loop:
        if(token != '\n' && palavra[ponteiro] != '\0'){ token = palavra[ponteiro++]; goto loop; }
    }
    fim:
}

void start_tree(){
    Tree tree[max];
    for (int i = 0; i < max; i++) { tree[i].token = ' '; tree[i].visitado = 0; tree[i].indice = 0; tree[i].filhos = 0; }
    prod = 0; fim_vetor = 1;
    tree[0].token = gramatica[0].esquerda;

    parse_tree(tree, 0);

    fprintf(saida, "%d, ", fim_vetor);

    for (int i = 0; i < fim_vetor-1; i++){ fprintf(saida, "%d | %c, ", tree[i].indice, tree[i].token); printf("[%d|%d|%c], ", i, tree[i].indice, tree[i].token); }
    fprintf(saida, "%d | %c\n", tree[fim_vetor-1].indice, tree[fim_vetor-1].token);
    printf("[%d|%d|%c]\n", fim_vetor-1, tree[fim_vetor-1].indice, tree[fim_vetor-1].token);
}

void parse_tree(Tree * tree, int atual){
    if (atual == 0 && tree[atual].visitado == 1) { return; }
    int inicio_vetor = -1;
    for (int i = 0; i < fim_vetor || (tree[i].indice == tree[atual].indice*tam_tree+1); i++){ if (tree[i].indice == tree[atual].indice*tam_tree+1) { inicio_vetor = i; } }
    
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
    int nao_term;
    for (int i = 0; i < producoes_gramatica; i++){
        if (productions[prod]-1 == i){
            if (tree[atual].token != gramatica[i].esquerda) { return; }
            for (int j = 0; j < gramatica[i].tam; j++){
                nao_term = 0;
                tree[fim_vetor + j].token = gramatica[i].direita[j];
                for (int z = 0; z < producoes_gramatica; z++){
                    if (tree[fim_vetor + j].token != gramatica[z].esquerda) { nao_term++; }
                }
                if (nao_term == producoes_gramatica) { tree[fim_vetor + j].visitado = 1; }
            }
            tree[fim_vetor].filhos = gramatica[i].tam;
            for (int i = 0, j = 1; i < tree[fim_vetor].filhos; i++, j++) { tree[fim_vetor+i].indice = tree[atual].indice*tam_tree+j; }
            fim_vetor += gramatica[i].tam;
            prod++;
            return;
        }
    }
}