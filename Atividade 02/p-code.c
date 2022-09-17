#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Const (1-4)
#define levmax 3 // maximum depth of block nesting
#define cxmax 200 // size of code arra

// (6-12)
typedef enum fct{LIT, OPR, LOD, STO, CAL, INT, JMP, JPC} fct;
char * instructions[] = { "LIT", "OPR", "LOD", "STO", "CAL", "INT", "JMP", "JPC"};
typedef struct instruction{
	fct f;
	int l;
	int a;
} instruction;

// Functions
void readFile(char *, instruction *);
int idInstruction(char *);
int base(int, int, int *);
void interpret(instruction *);

instruction code[cxmax]; // 14-15
int countForInstructions = 0;

int main (int argc, char **argv){
    // Load a file
    if(argc == 2){
		readFile(argv[1], code);
	}
    else{
        printf ("Erro, carregue o arquivo.\n");
		exit(1);
	}

    interpret(code);

    return 0;
}

void readFile(char *nome, instruction *code){
    char instruct[4];
    FILE *file;

    file = fopen(nome, "r");
    if(file == NULL){
        printf("Erro ao abrir arquivo.\n");
        exit(-1);
    }

    while(!feof(file)){
        fscanf (file, "%s %d %d", &instruct, &code[countForInstructions].l, &code[countForInstructions].a);
        if(strcmp(instruct, "OPR") != 0 || code[countForInstructions].l != 0 || code[countForInstructions].a != 0){
            code[countForInstructions].f = idInstruction(instruct);
            countForInstructions++;
        }
    }

    fclose(file);
    file = NULL;
}

int idInstruction(char *inst){
	for (int i = 0; i < 8;i++){
		if(strcmp(inst, instructions[i]) == 0){
			return i;
		}
	}
	return -1;
}

char * getInstructionByID(int id){
	if(id < 8){
		return instructions[id];
	}
	return "";
}

int base(int l, int b, int *s){
	int b1 = b;
	while(l > 0){
		b1 = s[b1];
		l -= 1;
	}
	return b1;
}

void interpret(instruction *code){	
    // 21-24
    int p = 0, b = 1, t = 0; // program-, base-, topstack-registers
    instruction i; // instruction register
    int s[cxmax] = {-1}; // datastore
	s[1] = s[2] = s[3] = 0;

	printf("\nInstruction\tLevel\tArgument\tStackPtr\tProgCounter\tStack\n");

	do{
		i = code[p];
		p++;

		switch(i.f){
			case LIT:{
				t++;
				s[t] = i.a;
				break;
			}
			case OPR:{
				switch(i.a){
					case 0:{
						t = b - 1;
						p = s[t + 3];
						b = s[t + 2];
						break;
					} 
					case 1:{
						s[t] = ~s[t];
						break;
					}
					case 2:{
						t--;
						s[t] = s[t] + s[t + 1];
						break;
					}
					case 3:{
						t--;
						s[t] = s[t] - s[t + 1];
						break;
					}
					case 4:{
						t--;
						s[t] = s[t] * s[t + 1];
						break;
					}
					case 5:{
						t--;
						s[t] = s[t] / s[t + 1];
						break;
					}
					case 6:{
						s[t] = (s[t] % 2 == 1);
						break;
					}
					case 8:{
						t--;
						s[t] = (s[t] == s[t + 1]);
						break;
					}
					case 9:{
						t--;
						s[t] = (s[t] != s[t + 1]);
						break;
					}
					case 10:{
						t--;
						s[t] = (s[t] < s[t + 1]);
						break;
					}
					case 11:{
						t--;
						s[t] = (s[t] >= s[t + 1]);
						break;
					}
					case 12:{
						t--;
						s[t] = (s[t] > s[t + 1]);
						break;
					}
					case 13:{
						t--;
						s[t] = (s[t] <= s[t + 1]);
						break;
					}
					default:
						break;
				}
				break;
			}
			case LOD:{
				t++;
				s[t] = s[base(i.l, b, s) + i.a];
				break;
			}
			case STO:{
				s[base(i.l, b, s) + i.a] = s[t];
				t--;
				break;
			}
			case CAL:{
				s[t + 1] = base(i.l, b, s);
				s[t + 2] = b;
				s[t + 3] = p;
				b = t + 1;
				p = i.a;
				break;
			}
			case INT:{
				t += i.a;
				break;
			}
			case JMP:{
				p = i.a;
				break;
			}
			case JPC:{
				if(s[t] == 0){
					p = i.a;
				}
				t--;
				break;
			}
			default:{
				printf("\nErro ao identificar instrucao.\n");
				exit(-1);
				break;
			}
		}

		printf("%s\t\t%d\t%d\t\t%d\t\t%d\t\t", getInstructionByID(i.f),i.l, i.a, s[t], p);

		for (int i = b; i<t+1;i++){
			printf("%d ", s[i]);
		}
		printf("\n");
	}while(p < countForInstructions);

	printf("Output: %d\n", s[t]);
}