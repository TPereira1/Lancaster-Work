#include <stdio.h>

int main(){
    int i;
    int sideLength;
    printf("Enter a side length for a square:");
    scanf("%d", &sideLength);
    for( int x = 0; x < sideLength; x++){
        for(i = 0; i < sideLength; i++){ 
            if(i == 0 || x == 0 || x == sideLength-1 || i == sideLength-1){
                  printf("*");
            }
            else{
                printf(" ");
            }
        }
        printf("\n");
    }
}