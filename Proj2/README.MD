# Análise Sintática
## Implementação em C

Os projetos são aplicações em C de algoritmos que buscam simular um autômato pilha que reconhece a linguagem gerada por uma gramática livre de contexto.

- Produções
- Proj2a
- Proj2b

## Produções:

```sh
S -> M | GM | NGM
N -> n(){ C; r(E); }
G -> g(){ C; r(E); }
M -> m() { C; r(E); }
E -> 0 | 1 | x | y | (EXE)
X -> + | - | * | /
C -> h=E | i=E | j=E | k=E | z=E | (EXE) | w(E){ C; } | f(E){ C; } | o(E;E;E){ C; }
```