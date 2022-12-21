#ifndef ada2asa_h
#define ada2asa_h

struct Syntax{
    char token;
    int indice;
}; typedef struct Syntax Syntax;

struct Asa_Tree{
    char token;
    int visitado, filhos, indice, hash_original;
}; typedef struct Asa_Tree Asa_Tree;

void start_asa();
void ada2asa(Syntax *, Asa_Tree *, int);
void place_token(Syntax *, Asa_Tree *, int);

#endif