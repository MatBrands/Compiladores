# P-Code Machine
## Implementação da p-code machine em C++

O código é uma aplicação em C++ para o Proj1d de um algoritmo que pode ser obtido em:
http://th.cpp.sh/9nsyz 

- Comandos válidos
- Começando
- Referências

## Comandos válidos:

Comandos aceitos pelo p-code-machine.

- **LIT 0, a :** load constant a;
- **OPR 0, a :** execute operation a (13 operations: RETURN, 5 math functions, and 7 comparison functions);
- **LOD l, a :** load variable a from level l;
- **STO l, a :** store variable a at level l;
- **CAL l, a :** call procedure a at level l;
- **INT 0, a :** increment t-register by a;
- **JMP 0, a :** jump to a;
- **JPC 0, a :** jump conditional to a (if '0' jump, else don't jump).

Tabela referente ass operações:

| code  | Symbol | Semantics |
| --- | --- | --- |
| 0  | Return  | Return from a subroutine.  |
| 1  | Negate  | Negate the value on the top of the stack (i.e. multiply by -1).  |
| 2  | Add  | x=pop();y=pop();push(y+x).  |
| 3  | Subtract  | x=pop();y=pop();push(y-x).  |
| 4  | Multiply  | x=pop();y=pop();push(y*x).  |
| 5  | Divide  | x=pop();y=pop();push(y/x).  |
| 6  | Odd?  | Test the value at the top of the stack to see if it's odd or not.  |
| 7  | == | x=pop();y=pop();push(y==x). |
| 8  | <> | x=pop();y=pop();push(y<>x). |
| 9  | < | x=pop();y=pop();push(y<x). |
| 10  | >= | x=pop();y=pop();push(y>=x). |
| 11  | > | x=pop();y=pop();push(y>x). |
| 12  | <= | x=pop();y=pop();push(y<=x). |

## Começando

Para a execução não é necessário o uso de nenhuma dependência, basta compila-lo.

```sh
g++ nome_do_arquivo.cpp -o nome_do_arquivo
```

Então execute-o.

```sh
./nome_do_arquivo
```

## Referências
https://en.wikipedia.org/wiki/P-code_machine   

http://th.cpp.sh/9nsyz 

https://blackmesatech.com/2011/12/pl0/pl0.xhtml

http://th.cpp.sh/9nsyz