#include <stdio.h>

int main(){
    int store[10];
    int num;
    int size =10;
    int largest;
    int smallest;
    int positionOfL;
    int positionOfS;
    int mean=0;
    for(int i = 0; i < size; i++){
        printf("Enter a number to be stored:");
        scanf("%d",&num);
        store[i] = num;
    }
    
    for(int x = 0; x < size;x++){
        printf("%d ",store[x]);
        mean+=store[x];
        
        if(x != 0 && largest < store[x]){
            largest = store[x];
            positionOfL = x;
        }
        if(x != 0 && smallest > store[x]){
            smallest = store[x];
            positionOfS = x;
        }
        if(x==0){
            largest = store[x];
            smallest = store[x];
            positionOfL = x;
            positionOfS = x;
        }
    }
    mean = mean/10;
    printf("\nThe largest number entered is %d which is in position %d of the array", largest, positionOfL+1);
    printf("\nThe smallest number entered is %d which is in position %d of the array", smallest, positionOfS+1);
    printf("\nThe mean of the array is %d\n", mean);
}