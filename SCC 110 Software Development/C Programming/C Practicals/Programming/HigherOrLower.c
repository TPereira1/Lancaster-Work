#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
    int guess;
    int temp;
    int genNum = rand() % 10;
    srand(time(NULL));
    do{
        printf("Higher or Lower than a %d (Enter 0 for higher and 1 for lower)\n>",genNum);
        scanf("%d",&guess);
        temp = genNum;
        genNum = rand() % 10;
    }
    while((guess == 0 && temp < genNum)||(guess == 1 && temp > genNum));
    printf("Game over the number was %d\n",genNum);
}