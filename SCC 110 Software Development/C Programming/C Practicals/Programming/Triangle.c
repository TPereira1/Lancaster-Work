#include <stdio.h>
int main(){
    int height;
    int position = 0;
    printf("Enter a height for a triangle:");
    scanf("%d", &height);
    //2n-1 sequence
    int baseLength = 2*height - 1;
    int centre = baseLength/2;
    for(int Line = 1; Line <= height; Line++){
        while(position < baseLength){
            if(position == centre){
                 int StarsToPrint = Line*2 -1;
                for(int star = 0; star < StarsToPrint; star++){
                    printf("*");
                    position++;
                }
                centre--;
            }
            else{
                printf(" ");
                position++;
            }
        }
        printf("\n");
        position = 0;
    }
}