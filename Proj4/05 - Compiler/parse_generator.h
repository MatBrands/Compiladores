#ifndef parse_h
#define parse_h

struct Producao{
    char esquerda;
	char direita[20];
    int tam;
};typedef struct Producao Producao;

// 
void readGrammar(FILE *);
char *readPalavra(FILE *, char *);
int parsing_table(char *);
// 

#endif