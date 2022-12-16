#include <stdio.h>
#include <stdlib.h>
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
int producoes_gramatica, p_count, tam_tree, prod_atual, fim_vetor;

// 
FILE *readFile(char *);
void readGrammar();
void start_tree();
void parse_tree(Tree *, int, int *);
void put_production(Tree *, int, int *);
// 

int main (int argc, char **argv){
    if(argc == 2){ readGrammar(); file = readFile(argv[1]); }
    else{ printf("Carregue o arquivo contendo o input.\n"); exit(1); }

    fscanf(file, "%d\n", &p_count);
    fscanf(file, "%d\n", &tam_tree);
    int productions[p_count];

    for (int i=0; i<p_count; i++){
        if (i<p_count-1){
            fscanf(file, "P%d\n", &productions[i]);
        }
        else{
            fscanf(file, "P%d", &productions[i]);
        }
    }
    fclose(file); file = NULL;

    start_tree(productions);
    
    return 0;
}

void readGrammar(){
    FILE *file;
    file = fopen("../grammar", "r");
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

void start_tree(int *productions){
    Tree tree[max];
    FILE *saida;
    saida = fopen("ada_tree", "w");

    for (int i = 0; i < max; i++) { tree[i].token = ' '; tree[i].visitado = 0; tree[i].indice = 0; tree[i].filhos = 0; }
    prod_atual = 0; fim_vetor = 1;
    tree[0].token = gramatica[0].esquerda;

    parse_tree(tree, 0, productions);

    fprintf(saida, "%d\n", fim_vetor);
    for (int i = 0; i < fim_vetor-1; i++){ fprintf(saida, "%d | %c\n", tree[i].indice, tree[i].token); printf("[%d|%d|%c], ", i, tree[i].indice, tree[i].token); }
    fprintf(saida, "%d | %c", tree[fim_vetor-1].indice, tree[fim_vetor-1].token);
    printf("[%d|%d|%c]\n", fim_vetor-1, tree[fim_vetor-1].indice, tree[fim_vetor-1].token);
}

void parse_tree(Tree * tree, int atual, int *productions){
    if (atual == 0 && tree[atual].visitado == 1) { return; }
    int inicio_vetor = -1;
    for (int i = 0; i < fim_vetor || (tree[i].indice == tree[atual].indice*tam_tree+1); i++){ if (tree[i].indice == tree[atual].indice*tam_tree+1) { inicio_vetor = i; } }
    
    if ( inicio_vetor == -1 ) { put_production(tree, atual, productions); }
    else{
        for (int i = inicio_vetor; i < inicio_vetor+tree[inicio_vetor].filhos; i++){
            if (tree[i].visitado == 0){ parse_tree(tree, i, productions); return; }
        }
        tree[atual].visitado = 1;
    }
    parse_tree(tree, 0, productions);
}

void put_production(Tree * tree, int atual, int *productions){
    int nao_term;
    for (int i = 0; i < producoes_gramatica; i++){
        if (productions[prod_atual]-1 == i){
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
            prod_atual++;
            return;
        }
    }
}