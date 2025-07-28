#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

char* getItem(int a){
    switch(a){
        case 0: return("bell");
        break;
        case 1: return("orange");
        break;
        case 2: return("cherry");
        break;
        case 3: return("horseshoe");
        break;
        default:
        char b[8] ={" Item "};
        b[6] = a +'0';
        char* d = b;
        return(d);
        break;
    }
}
void start(int numItems, int *p){
    int reel[numItems];
    for(int i = 0; i < numItems;i++){
        reel[i]=i;
    }
    for(int i =0; i<3;i++){
        p[i]= reel[rand()%numItems];
    }
}

int main(){
    srand(time(NULL));
    int spin[3];
    int size = 5;
    int *p;
    p = spin;
    bool stop = false;
    char responce;
    while(!stop){
        start(size, p);
        printf("%s - %s - %s\n",getItem(spin[0]),getItem(spin[1]),getItem(spin[2]));
        if(spin[0] == spin[1] && spin[0] == spin[2]){
            printf("Jackpot");
        }
        else if(spin[0] == spin[1] || spin[0] == spin[2] || spin[2] == spin[1]){
            printf("You have won.");
        }
        else{
            printf("You have lost.");
        }
        printf("\nDo you want to spin again? (Y/N)\n");
        scanf("%c",&responce);
        if(responce == 'N' || responce == 'n'){
            stop = true;
        }
        else if(responce == 'Y' || responce == 'y'){
            size++;
        }
    }

}