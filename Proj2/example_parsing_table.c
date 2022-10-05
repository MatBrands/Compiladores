#include <stdio.h>
#include <stdlib.h>

FILE *readFile(char *);
void parsing_table(FILE *);
char test(FILE *);

int main (int argc, char **argv){
    FILE *file;
    int i = 0;
    if(argc == 2){
		file = readFile(argv[1]);
	}
    else{
        printf ("Carregue o arquivo contendo os inputs.\n");
		exit(1);
	}

    while(!feof(file)){
        printf("Tabela referente a palavra %d:\n", ++i);
        parsing_table(file);
        if (!feof(file)) printf("\n");
    }

    fclose(file);
    file = NULL;
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

void parsing_table(FILE *file){
    // i | Qi | .W | Stack | Ti | Pi
    char token, stack[512];
    int i = 0, t, top = -1;
    q0:
    printf("i \t Qi \t .W \t Stack \t Ti \t Pi\n");
    printf("%d \t Q0 \t .W \t %c \t T0 \t -\n", i, stack[top]);
    top++;
    stack[top] = 'S';
    token = fgetc(file);
    goto q1;

    q1:
    // T1
    if (token == 'a' && stack[top] == 'S') { printf("%d \t Q1 \t .W \t %c \t T1 \t P1\n", ++i, stack[top]); stack[top] = 'b'; stack[++top] = 'S'; stack[++top] = 'a'; }
    // T2
    else if (token == 'c' && stack[top] == 'S') { printf("%d \t Q1 \t .W \t %c \t T2 \t P2\n", ++i, stack[top]); stack[top] = 'c'; }
    // T3
    else if (token == 'a' && stack[top] == 'a') { printf("%d \t Q1 \t .W \t %c \t T3 \t -\n", ++i, stack[top]); top--; token = fgetc(file); }
    // T4
    else if (token == 'b' && stack[top] == 'b') { printf("%d \t Q1 \t .W \t %c \t T4 \t -\n", ++i, stack[top]); top--; token = fgetc(file); }
    // T5
    else if (token == 'c' && stack[top] == 'c') { printf("%d \t Q1 \t .W \t %c \t T5 \t -\n", ++i, stack[top]); top--; token = fgetc(file); }
    // End of read
    else if ((feof(file) || token == '\n') && top == -1 ) { goto fim; }
    // Error
    else { printf("Erro.\n"); goto fim; }
    // Loop
    goto q1;
    // End
    fim:
}