#include <stdio.h>
#include <stdlib.h>
#include "asa2np.h"
#define max 500

int tam_asa_, tam_pn=0, tam_pnr=0;

void start_np(){
    FILE *file;
    file = fopen("asa_tree", "r");
    fscanf(file, "%d\n", &tam_asa_);
    asa_Tree asa_tree[tam_asa_];
    for (int i=0; i<tam_asa_; i++){
        if (i<tam_asa_-1){
            fscanf(file, "%d | %c\n", &asa_tree[i].indice, &asa_tree[i].token);
        }
        else{
            fscanf(file, "%d | %c", &asa_tree[i].indice, &asa_tree[i].token);
        }
    }

    fclose(file); file = NULL;

    char polish[tam_asa_], reverse_polish[tam_asa_];
    for (int i = 0; i < tam_asa_; i++) { polish[i] = ' '; reverse_polish[i]; }

    asa2np_(asa_tree, polish, reverse_polish, 0);

    FILE *saida;
    saida = fopen("np", "w");
    fprintf(saida, "%d\n", tam_pn);
    printf("Notacao ponolesa: ");
    for (int i = 0; i < tam_pn-1; i++){ fprintf(saida, "%c\n", polish[i]); printf("[%c], ", polish[i]); }
    fprintf(saida, "%c", polish[tam_pn-1]); printf("[%c]\n", polish[tam_pn-1]);
    fclose(saida); saida = NULL;

    saida = fopen("np_r", "w");
    fprintf(saida, "%d\n", tam_pnr);
    printf("Notacao ponolesa reversa: ");
    for (int i = 0; i < tam_pnr-1; i++){ fprintf(saida, "%c\n", reverse_polish[i]); printf("[%c], ", reverse_polish[i]); }
    fprintf(saida, "%c", reverse_polish[tam_pnr-1]); printf("[%c]\n", reverse_polish[tam_pnr-1]);
    fclose(saida); saida = NULL;
}

void asa2np_(asa_Tree *asa, char *polish, char *reverse_polish, int atual){
    if (atual == tam_asa_) { return; }
    
    if (!(asa[atual].token == '|')) { polish[tam_pn++] = asa[atual].token; }

    for (int i = atual; i < tam_asa_; i++){
        if (asa[atual].indice*2+1 == asa[i].indice){
            asa2np_(asa, polish, reverse_polish, i);
            break;
        }
    }

    if (!(asa[atual].token == '|')) { reverse_polish[tam_pnr++] = asa[atual].token; }

    if (atual < tam_asa_ && asa[atual].indice+1 == asa[atual+1].indice && asa[atual].token != 'm'){
        asa2np_(asa, polish, reverse_polish, atual+1);
    }
}