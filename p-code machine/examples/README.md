# Soma de dois números inteiros.
```
int soma(int x, int y){
    return x + y;
}

void main (){
    int valor;
    valor = soma(1, 2);
}
```

# Soma dos números naturais de 1 até 10. 
```
int soma(int x, int y){
    int resultado = 0;
    for (int i = x; i <= y; i ++){
        resultado += i;
    }
    return resultado;
}

void main (){
    int valor;
    valor = soma(1, 10);
}
```

# Soma dos números naturais de 1 até 100 (iterativamente). 
```
int soma(int x, int y){
    int resultado = 0;
    for (int i = x; i <= y; i ++){
        resultado += i;
    }
    return resultado;
}

void main (){
    int valor;
    valor = soma(1, 100);
}
```

# Soma dos quadrados dos números naturais de 1 até 100 (iterativamente). 
```
int soma_pow(int x, int y){
    int resultado = 0;
    for (int i = x; i <= y; i ++){
        i = i * i;
        resultado += i;
    }
    return resultado;
}

void main (){
    int valor;
    valor = soma_pow(1, 10);
}
```

# Soma  dos  cubos  dos  números  naturais  de  1 até 100 (iterativamente). 
```
int soma_pow_pow(int x, int y){
    int resultado = 0;
    for (int i = x; i <= y; i ++){
        i = i * i * i;
        resultado += i;
    }
    return resultado;
}

void main (){
    int valor;
    valor = soma_pow_pow(1, 10);
}
```

# Fatorial(4) (iterativo)
```
int fat(int n){
    int result = 1;
    for (int i = 1; i <= n; i++){
        result *= i;
    }
    return result;
}

void main(){
    int valor;
    valor = fat(4);
}
```
# Fibonacci(5) (iterativo)
```
int fibonacci(int n){
    int x = 0, y = 1, temp;
    for(int i = 1; i < n; i++){
        temp = x + y;
        x = y;
        y = temp;
    }
    return y;
}
void main(){
    int value;
    value = fibonacci(5)
}
```

# Fatorial(4) (recursivo)
```
int factorial (int n){
    if (n <= 1){
        return 1;
    }
    else{
        return (n*factorial(n-1));
    }
}

void main(){
    int value;
    value = factorial(4);
    
    return;
}
```

# Fibonacci(5) (recursivo)
```
int fibonacci (int n){
    if (n <= 1){
        return n;
    }
    else{
        return (fibonacci(n-1) + fibonacci(n-2));
    }
}

void main(){
    int value;
    value = fibonacci(5);
    
    return;
}
```