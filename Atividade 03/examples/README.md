# Codigos a serem convertidos

# Fatorial(4)

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

# Fibonacci (5)

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