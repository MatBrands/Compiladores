#include <stdio.h>
#include <stdlib.h>
#include "parse_generator.h"
#include "ada.h"
#include "ada2asa.h"
#include "asa2np.h"

FILE *readFile(char *);

int main (int argc, char **argv){
    FILE *file;
    if(argc == 2){ file = readFile(argv[1]); }
    else{ printf("Carregue o arquivo contendo o input.\n"); exit(1); }

    readGrammar(file);
    char palavra[100];
    readPalavra(file, palavra);
    fclose(file); file = NULL;

    printf("%s\n", palavra);

    if (!parsing_table(palavra)){ return 1; }

    printf("\n************************************************************************************\n\n");

    start_tree();
    remove("productions");

    printf("\n************************************************************************************\n\n");

    start_asa();
    remove("ada_tree");

    printf("\n************************************************************************************\n\n");

    start_np();
    remove("asa_tree");

    printf("\n************************************************************************************\n");

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