#include <stdio.h>
#include <stdlib.h>
#define max 500

struct Syntax{
    char token;
    int indice;
}; typedef struct Syntax Syntax;

struct Tree{
    char token;
    int visitado, filhos, indice, hash_original;
}; typedef struct Tree Tree;

FILE *file;
int tam_syntax=0, tam_asa=1;

// 
FILE *readFile(char *);
void start(Syntax *);
void ada2asa(Syntax *, Tree *, int);
void place_token(Syntax *, Tree *, int);
// 

int main (int argc, char **argv){
    if(argc == 2){ file = readFile(argv[1]); }
    else{ printf("Carregue os arquivos contendo o input.\n"); exit(1); }

    fscanf(file, "%d\n", &tam_syntax);
    Syntax synt_tree[tam_syntax];
    for (int i=0; i<tam_syntax; i++){
        if (i<tam_syntax-1){
            fscanf(file, "%d | %c\n", &synt_tree[i].indice, &synt_tree[i].token);
        }
        else{
            fscanf(file, "%d | %c", &synt_tree[i].indice, &synt_tree[i].token);
        }
    }
    fclose(file); file = NULL;

    start(synt_tree);

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

void start(Syntax *syntax){
    Tree asa_tree[tam_syntax];
    for (int i = 0; i < tam_syntax; i++) { asa_tree[i].token = '|'; asa_tree[i].visitado = 0; asa_tree[i].indice = 0; asa_tree[i].filhos = 0; asa_tree[i].hash_original = 0; }
    asa_tree[0].token = syntax[0].token;

    ada2asa(syntax, asa_tree, 0);

    FILE *saida;
    saida = fopen("asa_tree", "w");
    fprintf(saida, "%d\n", tam_asa);
    for (int i = 0; i < tam_asa-1; i++){ fprintf(saida, "%d | %c\n", asa_tree[i].indice, asa_tree[i].token); printf("[%d|%d|%c], ", i, asa_tree[i].indice, asa_tree[i].token); }
    fprintf(saida, "%d | %c", asa_tree[tam_asa-1].indice, asa_tree[tam_asa-1].token);
    printf("[%d|%d|%c]\n", tam_asa-1, asa_tree[tam_asa-1].indice, asa_tree[tam_asa-1].token);

}

void ada2asa(Syntax *syntax, Tree *asa, int atual){
    if (atual == 0 && asa[atual].visitado == 1) { return; }
    int inicio_vetor = -1;

    for (int i = 0; i < tam_syntax || (asa[i].indice == asa[atual].indice*2+1); i++){ if (asa[i].indice == asa[atual].indice*2+1) { inicio_vetor = i; } }
    
    if ( inicio_vetor == -1 ) { place_token(syntax, asa, atual); }
    else{
        for (int i = inicio_vetor; i < tam_asa; i++){
            if(asa[i].indice == asa[atual].indice*2+1 || asa[i].indice == asa[atual].indice*2+2) {
                if (asa[i].visitado == 0){ ada2asa(syntax, asa, i); return; }
            }
        }
        asa[atual].visitado = 1;
    }
    ada2asa(syntax, asa, 0);
}

void place_token(Syntax *syntax, Tree *asa, int atual){
    int token_atual = 0;
    if (asa[atual].token == 'S' || asa[atual].token == 'N' || asa[atual].token == 'G' || asa[atual].token == 'M' || 
    asa[atual].token == 'A' || asa[atual].token == 'B' || asa[atual].token == 'E' || asa[atual].token == 'X' || 
    asa[atual].token == 'C' || asa[atual].token == 'D'){
        while (syntax[token_atual].indice != asa[atual].hash_original*12+1){ token_atual++; }
    }
    else if (asa[atual].token == 'r') { while (syntax[token_atual].indice != asa[atual].hash_original+2){ token_atual++; } }

    // S
    if (asa[atual].token == 'S'){
        // M
        while (syntax[token_atual].token != 'M') { token_atual++; }
        
        asa[atual].token = syntax[token_atual].token;
        asa[atual].hash_original = syntax[token_atual].indice;
    }
    
    // N, G, M
    else if (asa[atual].token == 'N' || asa[atual].token == 'G' || asa[atual].token == 'M'){
        int valores[2];
        
        for (int i = token_atual; i <= token_atual+12; i++){
            if (syntax[i].token == 'A'){ valores[0]=i; }
            else if (syntax[i].token == 'r'){ valores[1]=i; break; }
        }

        if (asa[atual].token == 'N'){ asa[atual].token = 'n'; }
        else if (asa[atual].token == 'G'){ asa[atual].token = 'g'; }
        else if (asa[atual].token == 'M') { asa[atual].token = 'm'; }
        asa[atual].filhos = 2;

        // A, r
        for (int i = 0; i < asa[atual].filhos; i++){
            asa[tam_asa].token = syntax[valores[i]].token;
            asa[tam_asa].indice = 2*asa[atual].indice+(i+1);
            asa[tam_asa++].hash_original = syntax[valores[i]].indice;
        }
    }
    
    // r
    else if (asa[atual].token == 'r'){
        asa[tam_asa].token = syntax[token_atual].token;
        asa[tam_asa].hash_original = syntax[token_atual].indice;
        asa[tam_asa++].indice = 2*asa[atual].indice+1;
    }

    // A
    else if (asa[atual].token == 'A'){
        int valores[2];
        
        for (int i = token_atual; i <= token_atual+12; i++){
            if (syntax[i].token == 'C'){ valores[0]=i; }
            else if (syntax[i].token == 'B'){ valores[1]=i; break; }
        }

        asa[atual].token = '|';
        asa[atual].filhos = 2;

        // C, B
        for (int i = 0; i < asa[atual].filhos; i++){
            asa[tam_asa].token = syntax[valores[i]].token;
            asa[tam_asa].indice = 2*asa[atual].indice+(i+1);
            asa[tam_asa++].hash_original = syntax[valores[i]].indice;
        }

    }
    
    // B
    else if (asa[atual].token == 'B'){
        if (syntax[token_atual].token == '.'){ 
            asa[atual].token = syntax[token_atual].token;
            asa[atual].hash_original = syntax[token_atual].indice;
            asa[atual].visitado = 1;
        }
        else{
            int valores[2];
            for (int i = token_atual; i <= token_atual+12; i++){
                if (syntax[i].token == 'C'){ valores[0]=i; }
                else if (syntax[i].token == 'B'){ valores[1]=i; break; }
            }

            asa[atual].token = '|';
            asa[atual].filhos = 2;

            // C, B
            for (int i = 0; i < asa[atual].filhos; i++){
                asa[tam_asa].token = syntax[valores[i]].token;
                asa[tam_asa].indice = 2*asa[atual].indice+(i+1);
                asa[tam_asa++].hash_original = syntax[valores[i]].indice;
            }
        }
    }

    // E
    else if (asa[atual].token == 'E'){
        // (EXE)
        if (syntax[token_atual].token == '('){ token_atual++;
            // X
            asa[atual].token = syntax[token_atual+1].token;
            asa[atual].hash_original = syntax[token_atual+1].indice;
            asa[atual].filhos = 2;
            // E1
            asa[tam_asa].token = syntax[token_atual].token;
            asa[tam_asa].hash_original = syntax[token_atual].indice;
            asa[tam_asa++].indice = 2*asa[atual].indice+1;
            // E2
            asa[tam_asa].token = syntax[token_atual+2].token;
            asa[tam_asa].hash_original = syntax[token_atual+2].indice;
            asa[tam_asa++].indice = 2*asa[atual].indice+2;
        }
        // 
        else {
            asa[atual].token = syntax[token_atual].token;
            asa[atual].hash_original = syntax[token_atual].indice;
            asa[atual].visitado = 1;
        }
    }

    else if (asa[atual].token == 'X'){
        asa[atual].token = syntax[token_atual].token;
        asa[atual].hash_original = syntax[token_atual].indice;
        place_token(syntax, asa, atual);
    }

    else if (asa[atual].token == 'C'){
        if (syntax[token_atual].token == 'h' || syntax[token_atual].token == 'i'){
            // =
            asa[atual].token = syntax[token_atual+1].token;
            asa[atual].hash_original = syntax[token_atual+1].indice;
            asa[atual].filhos = 2;
            // h|i
            asa[tam_asa].token = syntax[token_atual].token;
            asa[tam_asa].hash_original = syntax[token_atual].indice;
            asa[tam_asa].indice = 2*asa[atual].indice+1;
            asa[tam_asa++].visitado = 1;
            // 
            token_atual = 0;
            if (syntax[token_atual].token == 'h'){ while (syntax[token_atual].token != 'G'){ token_atual++; } }
            else{ while (syntax[token_atual].token != 'G'){ token_atual++; } }
            asa[tam_asa].token = syntax[token_atual+2].token;
            asa[tam_asa].hash_original = syntax[token_atual+2].indice;
            asa[tam_asa++].indice = 2*asa[atual].indice+2;
        }
        else if (syntax[token_atual].token == 'j' || syntax[token_atual].token == 'k' || syntax[token_atual].token == 'z'){
            // =
            asa[atual].token = syntax[token_atual+1].token;
            asa[atual].hash_original = syntax[token_atual+1].indice;
            asa[atual].filhos = 2;
            // j|k|z
            asa[tam_asa].token = syntax[token_atual].token;
            asa[tam_asa].hash_original = syntax[token_atual].indice;
            asa[tam_asa].indice = 2*asa[atual].indice+1;
            asa[tam_asa++].visitado = 1;
            // E
            asa[tam_asa].token = syntax[token_atual+2].token;
            asa[tam_asa].hash_original = syntax[token_atual+2].indice;
            asa[tam_asa++].indice = 2*asa[atual].indice+2;
        }
        else if (syntax[token_atual].token == '('){ token_atual++;
            // X
            asa[atual].token = syntax[token_atual+1].token;
            asa[atual].hash_original = syntax[token_atual+1].indice;
            asa[atual].filhos = 2;
            // E1
            asa[tam_asa].token = syntax[token_atual].token;
            asa[tam_asa].hash_original = syntax[token_atual].indice;
            asa[tam_asa++].indice = 2*asa[atual].indice+1;
            // E2
            asa[tam_asa].token = syntax[token_atual+2].token;
            asa[tam_asa].hash_original = syntax[token_atual+2].indice;
            asa[tam_asa++].indice = 2*asa[atual].indice+2;
        }
        else if (syntax[token_atual].token == 'w' || syntax[token_atual].token == 'f'){
            // w, f
            asa[atual].token = syntax[token_atual].token;
            asa[atual].hash_original = syntax[token_atual].indice;
            // E
            asa[tam_asa].token = syntax[token_atual+2].token;
            asa[tam_asa].hash_original = syntax[token_atual+2].indice;
            asa[tam_asa++].indice = 2*asa[atual].indice+1;
            // F
            asa[tam_asa].token = 'F';
            asa[tam_asa].hash_original = syntax[token_atual+5].indice;
            asa[tam_asa++].indice = 2*asa[atual].indice+2;
        }
        else if (syntax[token_atual].token == 'o'){
            int valores[4], test = 0;
            for (int i = 0; i < tam_syntax; i++){
                if (syntax[i].indice == asa[atual].hash_original*12+1){
                    for (int j = i; j < i+12; j++){
                        if (syntax[j].token == 'E'){ valores[test++] = j; }
                        else if (syntax[j].token == 'C'){ valores[test] = j; break; }
                    }
                    break;
                }
            }
            int indice;
            // o
            asa[atual].token = syntax[token_atual].token;
            asa[atual].hash_original = syntax[token_atual].indice;
            asa[atual].filhos = 2;
            // E2
            asa[tam_asa].token = ';';
            asa[tam_asa].hash_original = syntax[valores[1]].indice;
            asa[tam_asa].indice = 2*asa[atual].indice+1;
            indice = asa[tam_asa++].indice;
            // E
            asa[tam_asa].token = syntax[valores[1]].token;
            asa[tam_asa].hash_original = syntax[valores[1]].indice;
            asa[tam_asa++].indice = 2*indice+1;
            // E1
            asa[tam_asa].token = ';';
            asa[tam_asa].hash_original = syntax[valores[0]].indice;
            asa[tam_asa].indice = 2*asa[atual].indice+2;
            indice = asa[tam_asa++].indice;
            // E
            asa[tam_asa].token = syntax[valores[0]].token;
            asa[tam_asa].hash_original = syntax[valores[0]].indice;
            asa[tam_asa++].indice = 2*indice+1;
            // C pai
            asa[tam_asa].token = ';';
            asa[tam_asa].hash_original = syntax[valores[3]].indice;
            asa[tam_asa].indice = 2*indice+2;
            asa[tam_asa].filhos = 2;
            indice = asa[tam_asa++].indice;
            // C
            asa[tam_asa].token = 'F';
            asa[tam_asa].hash_original = syntax[valores[3]].indice;
            asa[tam_asa++].indice = 2*indice+1;
            // E
            asa[tam_asa].token = syntax[valores[2]].token;
            asa[tam_asa].hash_original = syntax[valores[2]].indice;
            asa[tam_asa++].indice = 2*indice+2;
        }
    }
    else if (asa[atual].token == 'F'){
        int valores[2];
        valores[0]=token_atual;
        valores[1]=token_atual+1;

        asa[atual].token = '|';
        asa[atual].filhos = 2;

        // C, B
        for (int i = 0; i < asa[atual].filhos; i++){
            asa[tam_asa].token = syntax[valores[i]].token;
            asa[tam_asa].indice = 2*asa[atual].indice+(i+1);
            asa[tam_asa++].hash_original = syntax[valores[i]].indice;
        }
    }

    else if (asa[atual].token == 'D'){
        if (syntax[token_atual].token == '.'){ 
            asa[atual].token = syntax[token_atual].token;
            asa[atual].hash_original = syntax[token_atual].indice;
            asa[atual].visitado = 1;
        }
        else{
            int valores[2];
            for (int i = token_atual; i <= token_atual+12; i++){
                if (syntax[i].token == 'C'){ valores[0]=i; }
                else if (syntax[i].token == 'D'){ valores[1]=i; break; }
            }

            asa[atual].token = '|';
            asa[atual].filhos = 2;

            // C, D
            for (int i = 0; i < asa[atual].filhos; i++){
                asa[tam_asa].token = syntax[valores[i]].token;
                asa[tam_asa].indice = 2*asa[atual].indice+(i+1);
                asa[tam_asa++].hash_original = syntax[valores[i]].indice;
            }
        }
    }

}