#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int printArray(int Array[]){
    for(int x = 0; x < 10;x++){
        printf("%d ",Array[x]);
    }
}

int main(){
    int Array[10];
    int genNum;
    int temp;
    int count  = 1;
    srand(time(NULL));
    clock_t t =  clock();
    for (int i = 0; i < 10; i++){
        genNum = rand()%100;
        Array[i] = genNum;
    }
    printf("The unsorted array is: ");
    printArray(Array);

    int length = sizeof(Array)/sizeof(Array[0]);
    for(int y = 0; y < length; y++){
        for(int z = 1; z < length; z++){
            if(Array[z-1] > Array[z]){
                temp = Array[z-1];
                Array[z-1] = Array[z];
                Array[z] = temp;
                count++;
            }
        }

    }
    printf("\nThe sorted array is: ");
    printArray(Array);
    printf("\nIt took %d swap to sort this list after passing through the loop %d times\n", count, length);
    
    t = clock() - t;
    double time = ((double)t)/CLOCKS_PER_SEC;
    printf("it took %f seconds to execute\n", time);

}