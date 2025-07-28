#include <stdio.h>
int main(){
    int numbers[10];
    int total = 0;
    printf("Enter ten numbers:\n");
    for(int i =0;i<10;i++){
        scanf("%d\n",&numbers[i]);
    }
    printf("Results are:\n");
    for(int x = 0; x < 10; x++){
        printf("numbers[%d]=%d\n",x,numbers[x]);
        if(numbers[x]==5){
            numbers[x]=4;
        }
    }
    printf("Results after substitution are:\n");
    for(int y = 0; y < 10; y++){
        printf("numbers[%d]=%d\n",y,numbers[y]);
        total = total + numbers[y];
    }
    printf("\nThe sum is: %d\n",total);
}