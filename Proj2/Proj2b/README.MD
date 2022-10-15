# Implementação do Proj2_b

- Produções
- Exemplos
- Começando
- Referências

## Produções

```sh
P1: S -> M
P2: S -> GM
P3: S -> NGM
P4: N -> n(){ C; r(E); }
P5: G -> g(){ C; r(E); }
P6: M -> m() { C; r(E); }
P7: E -> 0
P8: E -> 1
P9: E -> x
P10: E -> y
P11: E -> (EXE)
P12: X -> +
P13: X -> -
P14: X -> *
P15: X -> /
P16: C -> h=E
P17: C -> i=E
P18: C -> j=E
P19: C -> k=E
P20: C -> z=E
P21: C -> (EXE)
P22: C -> w(E){ C; }
P23: C -> f(E){ C; }
P24: C -> o(E;E;E){ C; }
```

## Exemplos

```sh
m(){ r(1); }
m(){ h=(x+y); r(0); }
m(){ (1-1); r(1); }
m(){ w(1) { (1/x); }; r(1); }
n() { (0/y); r(y); } g() { i=y; r(x); } m() { (1-x); r(0); }
```

## Começando

Para a execução não é necessário o uso de nenhuma dependência, basta compila-lo.

```sh
gcc proj2_b.c -o proj2_b
```

Ao executá-lo redija o nome do arquivo destino contendo as instruções, caso contrario resultará em [erro][PlDb].

```sh
./proj2_b examples/input
```

## Referências