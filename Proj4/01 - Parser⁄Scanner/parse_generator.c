#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define max 500

struct Producao{
    char esquerda;
	char direita[20];
    int tam;
};typedef struct Producao Producao;

FILE *file;
Producao gramatica[100];
int productions[100], producoes_gramatica, tam_tree, p_count;

// 
FILE *readFile(char *);
void readGrammar(char *);
char *readPalavra(FILE *, char *);
void parsing_table(char *);
// 

int main (int argc, char **argv){
    if(argc == 3){ readGrammar(argv[1]); file = readFile(argv[2]); }
    else{ printf("Carregue os arquivos contendo as producoes e inputs.\n"); exit(1); }

    for (int i = 0; i < producoes_gramatica; i ++){
        printf("P%d: %c -> %s\n", i+1, gramatica[i].esquerda, gramatica[i].direita, gramatica[i].tam);
    }
    printf("Arvore tera %d filhos\n", tam_tree);

    printf("\n************************************************************************************\n\n");
    char palavra[100];
    do{
        readPalavra(file, palavra);
        printf("%s\n", palavra);
        parsing_table(palavra);
    }while(!feof(file));
    printf("\n");

    fclose(file); file = NULL;
    return 0;
}

void readGrammar(char *nome){
    FILE *arq;
    arq = fopen(nome, "r");
    if(arq == NULL){
        printf("Erro ao abrir arquivo.\n");
        exit(-1);
    }
    char token; producoes_gramatica = 0;
    token = fgetc(arq);
    gramatica[producoes_gramatica].tam = 0; gramatica[producoes_gramatica].esquerda = token;
    while (!feof(arq)){
        token = fgetc(arq);
        if (token == '\n') {
            gramatica[producoes_gramatica].direita[gramatica[producoes_gramatica].tam+1] = '\0';
            producoes_gramatica++;
            token = fgetc(arq);
            gramatica[producoes_gramatica].tam = 0;
            gramatica[producoes_gramatica].esquerda = token;
        }
        else if (token != ' '){
            gramatica[producoes_gramatica].direita[gramatica[producoes_gramatica].tam++] = token;
        }
    }
    for (int i = 0; i < producoes_gramatica; i++){ if (tam_tree < gramatica[i].tam) { tam_tree = gramatica[i].tam; } }
    fclose(arq);
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
        if (!feof(file)) { palavra[i++] = token; }
    }while(!feof(file));
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
        FILE *saida;
        printf("Palavra aceita.\n");
        printf("Producoes: ");
        saida = fopen("productions", "w");
        fprintf(saida, "%d\n", p_count);
        fprintf(saida, "%d\n", tam_tree);
        for (int i = 0; i<p_count; i++) {
            if (i<p_count-1){
                fprintf(saida, "P%d\n", productions[i]);
                printf("P%d\t", productions[i]);
            }
            else {
                fprintf(saida, "P%d", productions[i]);
                printf("P%d\n", productions[i]);
            }
        }

        fclose(saida); saida = NULL;

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
                        printf("%d \t Q1 \t %c \t %c \t P%d\n", ++indice, token, stack[top], j+1);
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
    }
    fim:
}