#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
    int num;
    srand(time(NULL));
    int genNum = rand() % 10;
    printf("I've thought of a number.\nEnter your guess\n");
    do{
        printf(">");
        scanf("%d", &num);
        if(num == genNum){
            printf("\nWell done, the number I thought of was %d\n", genNum);
        }
        if(num < genNum){
            printf("\nhigher\n");
        }
        if(num > genNum){
            printf("\nlower\n");
        }
    }while(num != genNum);
}