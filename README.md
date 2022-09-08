# P-Code Machine
## Implementação da p-code machine em C

O código é uma tradução em C de um algoritmo base feito em Pascal code. 

- Comandos válidos
- Começando
- Exemplo

## Comandos válidos:


## Começando

Para a execução não é necessário o uso de nenhuma dependência, basta compila-lo.

```sh
gcc p-code.c -o p-code
```

Ao executá-lo redija o nome do arquivo destino contendo as instruções, caso contrario resultará em [erro][PlDb].

```sh
./p-code arquivo.txt
```

## Exemplo

```sh
./p-code examples/sum_1_and_2
```
Entrada:
```
./examples/sum_1_and_2
```
Saida:
```
Instruction     Level   Argument        StackPtr        ProgCounter     Stack
INT             0       2               0               1               0 0 
LIT             0       1               1               2               0 0 1 
STO             0       0               0               3               1 0 
LIT             0       2               2               4               1 0 2 
STO             0       1               2               5               1 2 
OPR             0       2               3               6               3 
Output: 3
```
## License
