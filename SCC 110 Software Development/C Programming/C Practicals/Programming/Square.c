#include <stdio.h>

int main(){
    int i;
    int sideLength;
    printf("Enter a side length for a square:");
    scanf("%d", &sideLength);
    for( int x = 0; x < sideLength; x++){
        for(i = 0; i < sideLength; i++){ 
              printf("*");
        }
        printf("\n");
    }
}