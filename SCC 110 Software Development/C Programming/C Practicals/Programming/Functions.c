#include <stdio.h>

int multByTwo(int a){
    a = a * 2;
    return(a);
}
int isFirstBigger(int a,int b){
    int bigger = 0;
    if (a>b){
        bigger = 1;
    }
    return(bigger);
}
float addVat(float a){
    a = a * 1.2;
    return(a);
}