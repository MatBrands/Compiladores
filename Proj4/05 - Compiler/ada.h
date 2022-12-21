#ifndef ada_h
#define ada_h

struct Prod{
    char esquerda;
	char direita[20];
    int tam;
};typedef struct Prod Prod;

struct Tree{
    char token;
	int visitado;
    int indice;
    int filhos;
}; typedef struct Tree Tree;

void r_grammar();
void start_tree();
void parse_tree(Tree *, int, int *);
void put_production(Tree *, int, int *);

#endif