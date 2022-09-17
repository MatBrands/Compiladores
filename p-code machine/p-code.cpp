#include <stdio.h>
#define levmax 3
#define amax 2047
#define stacksize 5012

enum fct {LIT, OPR, LOD, STO, CAL, INT, JMP, JPC};
typedef struct tinstruction {
    fct f;
    int l;
    int a;
}instruction;
instruction code[2048];

int p, b, t;

instruction i;
int s[stacksize];

int base(int l){
    int b1;
    b1 = b;
    while (l > 0) {
        b1 = s[b1];
        l  = l - 1;
    }
    return b1;
}

int odd(int x){ return (  ((x%2)==1) ); }

void pcodevhw(){
    printf("\n start pl/0");
    printf("\n t   b   p     f   l   a                 0   1   2   3   4   5   6   7   8   9  10  11  12  13  14");
    printf("\n === === ===   === === ===             === === === === === === === === === === === === === === ===");
    t = -1;
    b = 0;
    p = 0;
    
    s[1] = 0;
    s[2] = 0;
    s[3] = 0;
    
    do {
        i = code[p];
        printf("\n %3d %3d %3d   ", t, b, p);
        p = p + 1;
        switch  (i.f) {
            case LIT : { t = t + 1; s[t] = i.a; printf("LIT %3d %3d", i.l, i.a); } break;
            case OPR :
            printf("OPR %3d %3d", i.l, i.a);
            switch (i.a) {
                case 0: {
                    t = b - 1;
                    p = s[t + 3];
                    b = s[t + 2];
                }
                break;
                case 1: { // *= -1
                    s[t] = -s[t];
                }
                break;
                case 2: { // ADD: PUSH( POP()+POP() )
                    t = t - 1;
                    s[t] = s[t] + s[t + 1];
                }
                break;
                case 3: { // SUBSTRACT: PUSH( POP()-POP() )
                    t = t - 1;
                    s[t] = s[t] - s[t + 1];
                }
                break;
                case 4: { // MULTIPLY: PUSH( POP()*POP() )
                    t = t - 1;
                    s[t] = s[t] * s[t + 1];
                }
                break;
                case 5: { // DIVIDE  : PUSH( POP()/POP() )
                    t = t - 1;
                    s[t] = s[t] / s[t + 1];
                }
                break;
                case 6: { // odd or not 
                    s[t] = odd(s[t]);
                }
                break;
                case 8: { // EQUAL
                    t = t - 1; 
                    s[t] = (s[t] == s[t + 1]);
                }
                break;
                case 9: { // NOT.EQUAL
                    t = t - 1; 
                    s[t] = (s[t] != s[t + 1]);
                }
                break;
                case 10: { // LESS THAN
                    t = t - 1; 
                    s[t] = (s[t] < s[t + 1]);
                }
                break;
                case 11: { // GREATER EQUAL THAN
                    t = t - 1; 
                    s[t] = (s[t] >= s[t + 1]);
                }
                break;
                case 12: { // GREATER THAN
                    t = t - 1; 
                    s[t] = (s[t] > s[t + 1]);
                }
                break;
                case 13: { // LESS EQUAL THAN
                    t = t - 1; 
                    s[t] = (s[t] <= s[t + 1]);
                }
                break;
            }
        break;
            case LOD : {
                printf("LOD %3d %3d", i.l, i.a);
                t = t + 1; 
                s[t] = s[base(i.l) + i.a];
            }
            break;
            case STO : {
                printf("STO %3d %3d", i.l, i.a);
                s[base(i.l)+i.a] = s[t]; 
                t = t - 1;
              }
            break;
            case CAL : {
                printf("CAL %3d %3d", i.l, i.a);
                s[t + 1] = base(i.l); 
                s[t + 2] = b; 
                s[t + 3] = p;
                b = t + 1; 
                p = i.a;
            }
            break;
            case INT : t = t + i.a;printf("INT %3d %3d", i.l, i.a); break;
            case JMP : p = i.a; printf("JMP %3d %3d", i.l, i.a); break;
            case JPC : if (s[t] == 1) { p = i.a; }  t = t - 1; printf("JPC %3d %3d", i.l, i.a); break;
        }
        
        printf("      s[] : ");
        for (int h=0; h<=t; h++) { printf(" %3d", s[h]); }
    } while ( p != 0 );
    printf("\n === === ===   === === ===             === === === === === === === === === === === === === === ===");
    printf("\n t   b   p     f   l   a                 0   1   2   3   4   5   6   7   8   9  10  11  12  13  14");
    printf("\n end pl/0");
}

int main(){

    // Insira seu código aki

	pcodevhw();
	return 0;
}