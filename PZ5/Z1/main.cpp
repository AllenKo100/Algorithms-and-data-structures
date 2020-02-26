#include <iostream>

//koristit cemo repnu rekurziju
int fib2_0(int n, int a=0, int b=1){
    if(n==0) return a;
    if(n==1) return b;
    return fib2_0(n-1, b, a+b);
}

//koristimo euklidov algoritam za ovu rekurziju
int nzd(int x, int y){
    if(y==0) return x;
    return nzd(y, x%y);
}

bool TestFibonacci1(){
    //Testiramo da li pravilno ispisuje 10. Fibonaccijev broj
    std::cout << "Deseti Fibonnacijev broj je 55, nas ispis je: " << fib2_0(10) << std::endl;
    if(fib2_0(10)!=55) return false;
    
    return true;
}

bool TestFibonacci2(){
    std::cout << "Nulti Fibonaccijev broj je 0, a nas ispis je: " << fib2_0(0) << std::endl;
    if(fib2_0(0)!=0) return false;
    
    return true;
}

bool TestFibonacci3(){
    std::cout << "Prvi Fibonaccijev broj je 1, a nas ispis je: " << fib2_0(1) << std::endl;
    if(fib2_0(1)!=1) return false;
    
    return true;
}

bool TestNZD(){
    //Testiramo da li pravilno racuna NZD(20,5)
    int x=20;
    int y=5;
    std::cout << "NZD 20 i 5 je 5, nas ispis je: " << nzd(x,y) << std::endl;
    if(nzd(x,y)!=5) return false;
    return true;
}


int main() {
    
    if(TestFibonacci1() && TestFibonacci2() && TestFibonacci3() && TestNZD()){
        std::cout << "Prosli svi testovi!" << std::endl;
    }
    else{
        std::cout << "Neki testovi su pali!" << std::endl;
    }
    return 0;
}