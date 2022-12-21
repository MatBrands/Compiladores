#ifndef asa2np_h
#define asa2np_h

struct asa_Tree{
    char token;
    int indice;
}; typedef struct asa_Tree asa_Tree;

// 
void start_np();
void asa2np_(asa_Tree *, char *, char *, int);
// 

#endif