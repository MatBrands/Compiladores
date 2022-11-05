#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int stack_pointer, h, ok;
char input[100], stack[100], action[30];
FILE *file;
FILE *readFile(char *);
void readInput(FILE *, char *);
int action_table();
int production_5(int);
int production_1_3(int, int);
void operation();

int main(int argc, char **argv) {
    if(argc == 2){ file = readFile(argv[1]); }
    else{ printf("Carregue o arquivo contendo os inputs.\n"); exit(1); }

    while (!feof(file)){
        readInput(file, input);
        strcpy(stack, "0");
    
        stack_pointer = 1; ok = 1;
        printf("Stack\t\t|Input\t\t|Action\n");
        while(1){
            // Erro
            if(!ok){
                printf("%s\t\t|%s\t\t|\n", stack, input);
                printf("Erro");
                break;
            }
            // Accept
            if(input[0]=='$' && !strcmp(action, "accept")){
                printf("%s\t\t|%s\t\t|Accept", stack, input);
                break;
            }
            // Shift
            if(action_table()){
                if(input[0]=='i' && input[1]=='d'){
                    printf("%s\t\t|%s\t\t|%s\n", stack, input, action);
                    for (int i = 0; i < 2; i++) { stack[stack_pointer+i] = input[i]; }
                    for (int i = 0; input[i+2] != '\0'; i++){ input[i] = input[i+2]; }
                    input[strlen(input)-2] = '\0';
                    stack[stack_pointer+2]='\0';
                    stack_pointer++;
                }
                else{
                    printf("%s\t\t|%s\t\t|%s\n", stack, input, action);
                    stack[stack_pointer] = input[0];
                    for (int i = 0; input[i+1] != '\0'; i++){ input[i] = input[i+1]; }
                    input[strlen(input)-1] = '\0';
                    stack[stack_pointer+1] = '\0';
                }
                operation();
            }
            // Reduce
            else{
                operation();
                stack_pointer = strlen(stack)-1;
            }
            stack_pointer++;
        }
        printf("\n**************************************************************************\n");
    }

    fclose(file); file = NULL;
	return(0);
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

void readInput(FILE *file, char *inputs){
    strcpy(inputs, "");
    int count = 0;
    char token;
    do{
        token = fgetc(file);
        inputs[count++] = token;
    }while(token != '\n' && !feof(file));
    inputs[count-1] = '$';
    inputs[count] = '\0';
}

int action_table(){
    int ultimo = strlen(stack)-1, penultimo = strlen(stack)-2;
    if(input[0] == 'i' && input[1]=='d'){
        if(stack[ultimo]=='0' || stack[ultimo]=='4' || stack[ultimo]=='6' || stack[ultimo]=='7'){
            strcpy(action,"S5");
            return 1;
        }
        else{ ok = 0; return -1; }
    }
    else if(input[0] == '+'){
        if(stack[ultimo]=='1'){
            if (stack[penultimo]=='1') {
                strcpy(action,"R5");
                operation();
                return 0;    
            }
            else{
                strcpy(action,"S6");
                return 1;
            }
        }
        else if(stack[ultimo]=='2'){
            strcpy(action,"R2");
            operation();
            return 0;
        }
        else if(stack[ultimo]=='3'){
            strcpy(action,"R4");
            operation();
            return 0;
        }
        else if(stack[ultimo]=='5'){
            strcpy(action,"R6");
            operation();
            return 0;
        }
        else if(stack[ultimo]=='8'){
            strcpy(action,"S6");
            return 1;
        }
        else if(stack[ultimo]=='9'){
            strcpy(action,"R1");
            operation();
            return 0;
        }
        if(stack[ultimo]=='0'){
            strcpy(action,"R3");
            operation();
            return 0;
        }
        else{ ok = 0; return -1;}
    }
    else if(input[0]=='*'){
        if(stack[ultimo]=='0'){
            strcpy(action,"R3");
            operation();
            return 0;
        }
        else if(stack[ultimo]=='1'){
            strcpy(action,"R5");
            operation();
            return 0;
        }
        else if(stack[ultimo]=='2' || stack[ultimo]=='9'){
            strcpy(action,"S7");
            return 1;
        }
        else if(stack[ultimo]=='3'){
            strcpy(action,"R4");
            operation();
            return 0;
        }
        else if(stack[ultimo]=='5'){
            strcpy(action,"R6");
            operation();
            return 0;
        }
        else{ ok = 0; return -1;}
    }
    else if(input[0]=='('){
        if(stack[ultimo]=='0' || stack[ultimo]=='4' || stack[ultimo]=='6' || stack[ultimo]=='7'){
            strcpy(action,"S4");
            return 1;
        }
        else{ ok = 0; return -1;}
    }
    else if(input[0]==')'){
        if(stack[ultimo]=='0'){
            strcpy(action,"R3");
            operation();
            return 0;
        }
        else if(stack[ultimo]=='1'){
            strcpy(action,"R5");
            operation();
            return 0;
        }
        else if(stack[ultimo]=='2'){
            strcpy(action,"R2");
            operation();
            return 0;
        }
        else if(stack[ultimo]=='3'){
            strcpy(action,"R4");
            operation();
            return 0;
        }
        else if(stack[ultimo]=='5'){
            strcpy(action,"R6");
            operation();
            return 0;
        }
        else if(stack[ultimo]=='8'){
            strcpy(action,"S11");
            return 1;
        }
        else if(stack[ultimo]=='9'){
            strcpy(action,"R1");
            operation();
            return 0;
        }
        else{ ok = 0; return -1;}
    }
    else if(input[0]=='$'){
        if(stack[ultimo]=='0'){
            strcpy(action,"R3");
            operation();
            return 0;
        }
        else if(stack[ultimo]=='1'){
            if(stack[penultimo]=='1'){ 
                strcpy(action,"R5");
                operation();
                return 0;
            }
            else {
                strcpy(action,"accept");
                return 0;
            }
        }
        else if(stack[ultimo]=='2'){
            strcpy(action,"R2");
            operation();
            return 0;
        }
        else if(stack[ultimo]=='3'){
            strcpy(action,"R4");
            operation();
            return 0;
        }
        else if(stack[ultimo]=='5'){
            strcpy(action,"R6");
            operation();
            return 0;
        }
        else if(stack[ultimo]=='9'){
            strcpy(action,"R1");
            operation();
            return 0;
        }
        else{ ok = 0; return -1;}
    }
}

int production_5(int posicao_atual){
    int marcador_1 = -1, marcador_2 = -1;
    for(int i = posicao_atual-1; i >= 0; i--){
        if(stack[i] == 'E' && marcador_1 == -1){ marcador_1 = i; }
        else if (stack[i] == '(' && marcador_2 == -1){ marcador_2 = i; }
    }
    h = marcador_2-1;
    if(marcador_1 && marcador_2 && marcador_1 > marcador_2) { return 1; }
    else { return 0; }
}

int production_1_3(int posicao_atual, int num){
    int marcador_1 = -1, marcador_2 = -1;
    char token_1, token_2;
    if (num){ token_1 = '*'; token_2 = 'F'; }
    else { token_1 = '+'; token_2 = 'T'; }

    for (int i = posicao_atual+1; i < strlen(stack); i++){
        if(stack[i] == token_1 && marcador_1 == -1){ marcador_1 = i; }
        else if(stack[i] == token_2 && marcador_2 == -1){ marcador_2 = i; }
    }
    if(marcador_1 && marcador_2 && marcador_2 > marcador_1) { return 1; } else { return 0; }
}

void operation(){
    if(action[0]=='S'){
        strcat(stack, strtok(action,"S"));
        stack_pointer += strlen(strtok(action,"S"));
    }
    else if(action[0]=='R'){
        if(action[1]=='6'){
            for(int i = strlen(stack)-1; i >= 0; i--){
                if(stack[i]=='i' && stack[i+1]=='d') {
                    if(stack[i-1]=='0'){
                        printf("%s\t\t|%s\t\t|%s\n", stack, input, action);
                        stack[i+1]='3';
                        stack[i]='F';
                        stack[i+2]='\0';
                    }
                    else if(stack[i-1]=='4'){
                        printf("%s\t\t|%s\t\t|%s\n", stack, input, action);
                        stack[i+1]='3';
                        stack[i]='F';
                        stack[i+2]='\0';
                    }
                    else if(stack[i-1]=='6'){
                        printf("%s\t\t|%s\t\t|%s\n", stack, input, action);
                        stack[i+1]='3';
                        stack[i]='F';
                        stack[i+2]='\0';
                    }
                    else if(stack[i-1]=='7'){
                        printf("%s\t\t|%s\t\t|%s\n", stack, input, action);
                        stack[i+1]='1';
                        stack[i+2]='0';
                        stack[i]='F';
                        stack[i+3]='\0';
                    }
                    return;
                }
            }
        }
        else if(action[1]=='5'){
            for(int i = strlen(stack)-1; i >= 0; i--){
                if(stack[i]==')') {
                    if(production_5(i)){
                        if(stack[h]=='0'){
                            printf("%s\t\t|%s\t\t|%s\n", stack, input, action);
                            stack[h+2]='3';
                            stack[h+1]='F';
                            stack[h+3]='\0';
                        }
                        else if(stack[h]=='4'){
                            printf("%s\t\t|%s\t\t|%s\n", stack, input, action);
                            stack[h+2]='3';
                            stack[h+1]='F';
                            stack[h+3]='\0';
                        }
                        else if(stack[h]=='6'){
                            printf("%s\t\t|%s\t\t|%s\n", stack, input, action);
                            stack[h+2]='3';
                            stack[h+1]='F';
                            stack[h+3]='\0';
                        }
                        else if(stack[h]=='7'){
                            printf("%s\t\t|%s\t\t|%s\n", stack, input, action);
                            stack[h+2]='1';
                            stack[h+3]='0';
                            stack[h+1]='F';
                            stack[h+4]='\0';
                        }
                    }
                    return;
                }
            }
        }
        else if(action[1]=='4'){
            for(int i = strlen(stack)-1; i >= 0; i--){
                if(stack[i]=='F') {
                    if(stack[i-1]=='0'){
                        printf("%s\t\t|%s\t\t|%s\n", stack, input, action);
                        stack[i]='T';
                        stack[i+1]='2';
                        stack[i+2]='\0';
                    }
                    else if(stack[i-1]=='4'){
                        printf("%s\t\t|%s\t\t|%s\n", stack, input, action);
                        stack[i+1]='2';
                        stack[i]='T';
                        stack[i+2]='\0';
                    }
                    else if(stack[i-1]=='6'){
                        printf("%s\t\t|%s\t\t|%s\n", stack, input, action);
                        stack[i+1]='9';
                        stack[i]='T';
                        stack[i+2]='\0';
                    }
                    return;
                }
            }
        }
        else if(action[1]=='3'){
            for(int i = strlen(stack)-1; i >= 0; i--){
                if(stack[i]=='T') {
                    if(production_1_3(i, 1)){
                        if(stack[i-1]=='0'){
                            printf("%s\t\t|%s\t\t|%s\n", stack, input, action);
                            stack[i+1]='2';
                            stack[i]='T';
                            stack[i+2]='\0';
                        }
                        else if(stack[i-1]=='4'){
                            printf("%s\t\t|%s\t\t|%s\n", stack, input, action);
                            stack[i+1]='2';
                            stack[i]='T';
                            stack[i+2]='\0';
                        }
                        else if(stack[i-1]=='6'){
                            printf("%s\t\t|%s\t\t|%s\n", stack, input, action);
                            stack[i+1]='9';
                            stack[i]='T';
                            stack[i+2]='\0';
                        }
                    }
                    return;
                }
            }
        }
        else if(action[1]=='2'){
            for(int i = strlen(stack)-1; i >= strlen(stack)-2; i--){
                if(stack[i]=='T') {
                    if(stack[i-1]=='0'){
                        printf("%s\t\t|%s\t\t|%s\n", stack, input, action);
                        stack[i+1]='1';
                        stack[i]='E';
                        stack[i+2]='\0';
                    }
                    else if(stack[i-1]=='4'){
                        printf("%s\t\t|%s\t\t|%s\n", stack, input, action);
                        stack[i+1]='8';
                        stack[i]='E';
                        stack[i+2]='\0';
                    }
                    else if(stack[i-1]=='1'){
                        printf("%s\t\t|%s\t\t|%s\n", stack, input, action);
                        stack[i+1]='1';
                        stack[i]='E';
                        stack[i+2]='\0';
                    }
                    return;
                }
            }
        }
        else if(action[1]=='1'){
            for(int i = strlen(stack)-1; i >= 0; i--){
                if(stack[i]=='E') {
                    if(production_1_3(i, 0)){
                        if(stack[i-1]=='0'){
                            printf("%s\t\t|%s\t\t|%s\n", stack, input, action);
                            stack[i+1]='1';
                            stack[i]='E';
                            stack[i+2]='\0';
                        }
                        else if(stack[i-1]=='4'){
                            printf("%s\t\t|%s\t\t|%s\n", stack, input, action);
                            stack[i+1]='8';
                            stack[i]='E';
                            stack[i+2]='\0';
                        }
                        else if(stack[i-1]=='1'){
                            printf("%s\t\t|%s\t\t|%s\n", stack, input, action);
                            stack[i+1]='1';
                            stack[i]='E';
                            stack[i+2]='\0';
                        }
                    }
                    return;
                }
            }
        }
    }
}