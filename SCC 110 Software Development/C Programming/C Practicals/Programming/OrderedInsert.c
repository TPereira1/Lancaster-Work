#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int printArray(int Array[]){
    for(int x = 0; x < 10;x++){
        printf("%d ",Array[x]);
    }
    printf("\n");
}

int main(){
    clock_t t =  clock();
    int Array[10];
    int genNum;
    int position;
    srand(time(NULL));
    for (int i = 0; i < 10; i++){
        genNum = rand()%100;
        if(i == 0){
            Array[i] = genNum;
        }
        if(i > 0 && genNum > Array[i-1]){
            Array[i] = genNum;
        }
        position = i;
        while(position > 0 && genNum < Array[position-1]){
            Array[position] = Array[position-1];
            Array[position-1] = genNum;
            position--;
        }
    }
    printArray(Array);
    t = clock() - t;
    double time = ((double)t)/CLOCKS_PER_SEC;
    printf("it took %f seconds to execute\n", time);
}