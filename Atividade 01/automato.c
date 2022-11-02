#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int token;
    char palavra[64] = "";

    INICIO:
        scanf("%d", &token);

        if (token == 0){
            strcat(palavra, "0");
        }
        else if (token == 1){
            strcat(palavra, "1");
        }
        else{
            goto FIM;  
        }

        goto INICIO;

    FIM:
        printf ("Fim de execução com a palavra: '%s'\n", palavra);
        return 0;
}