#include <stdio.h>
#include <stdlib.h>
#include "ada.h"
#define max 500

FILE *file;
Prod grammar[100];
int prod_gramatica, prod_count, tam_max_tree, prod_atual, fim_vetor;

void r_grammar(){
    FILE *file;
    file = fopen("../grammar", "r");
    if(file == NULL){
        printf("Erro ao abrir arquivo.\n");
        exit(-1);
    }
    char token; prod_gramatica = 0;
    token = fgetc(file);
    grammar[prod_gramatica].tam = 0; grammar[prod_gramatica].esquerda = token;
    while (!feof(file)){
        token = fgetc(file);
        if (token == '\n') {
            grammar[prod_gramatica].direita[grammar[prod_gramatica].tam+1] = '\0';
            prod_gramatica++;
            token = fgetc(file);
            grammar[prod_gramatica].tam = 0;
            grammar[prod_gramatica].esquerda = token;
        }
        else if (token != ' '){
            grammar[prod_gramatica].direita[grammar[prod_gramatica].tam++] = token;
        }
    }
    for (int i = 0; i < prod_gramatica; i++){ if (tam_max_tree < grammar[i].tam) { tam_max_tree = grammar[i].tam; } }
}

void start_tree(){
    r_grammar();
    FILE *file;
    file = fopen("productions", "r");    
    fscanf(file, "%d\n", &prod_count);
    fscanf(file, "%d\n", &tam_max_tree);
    int productions[prod_count];

    for (int i=0; i<prod_count; i++){
        if (i<prod_count-1){
            fscanf(file, "P%d\n", &productions[i]);
        }
        else{
            fscanf(file, "P%d", &productions[i]);
        }
    }
    fclose(file); file = NULL;


    Tree tree[max];
    FILE *saida;
    saida = fopen("ada_tree", "w");

    for (int i = 0; i < max; i++) { tree[i].token = ' '; tree[i].visitado = 0; tree[i].indice = 0; tree[i].filhos = 0; }
    prod_atual = 0; fim_vetor = 1;
    tree[0].token = grammar[0].esquerda;

    parse_tree(tree, 0, productions);

    fprintf(saida, "%d\n", fim_vetor);
    for (int i = 0; i < fim_vetor-1; i++){ fprintf(saida, "%d | %c\n", tree[i].indice, tree[i].token); printf("[%d|%d|%c], ", i, tree[i].indice, tree[i].token); }
    fprintf(saida, "%d | %c", tree[fim_vetor-1].indice, tree[fim_vetor-1].token);
    printf("[%d|%d|%c]\n", fim_vetor-1, tree[fim_vetor-1].indice, tree[fim_vetor-1].token);

    fclose(saida); saida = NULL;
}

void parse_tree(Tree * tree, int atual, int *productions){
    if (atual == 0 && tree[atual].visitado == 1) { return; }
    int inicio_vetor = -1;
    for (int i = 0; i < fim_vetor || (tree[i].indice == tree[atual].indice*tam_max_tree+1); i++){ if (tree[i].indice == tree[atual].indice*tam_max_tree+1) { inicio_vetor = i; } }
    
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
    for (int i = 0; i < prod_gramatica; i++){
        if (productions[prod_atual]-1 == i){
            if (tree[atual].token != grammar[i].esquerda) { return; }
            for (int j = 0; j < grammar[i].tam; j++){
                nao_term = 0;
                tree[fim_vetor + j].token = grammar[i].direita[j];
                for (int z = 0; z < prod_gramatica; z++){
                    if (tree[fim_vetor + j].token != grammar[z].esquerda) { nao_term++; }
                }
                if (nao_term == prod_gramatica) { tree[fim_vetor + j].visitado = 1; }
            }
            tree[fim_vetor].filhos = grammar[i].tam;
            for (int i = 0, j = 1; i < tree[fim_vetor].filhos; i++, j++) { tree[fim_vetor+i].indice = tree[atual].indice*tam_max_tree+j; }
            fim_vetor += grammar[i].tam;
            prod_atual++;
            return;
        }
    }
}