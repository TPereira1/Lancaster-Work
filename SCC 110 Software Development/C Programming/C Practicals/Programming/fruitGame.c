#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char* getItem(int a){
    switch(a){
        char item[10];
        case 0: return("bell");
        break;
        case 1: return("orange");
        break;
        case 2: return("cherry");
        break;
        case 3: return("horseshoe");
        break;
    }
}

int main(){
    srand(time(NULL));
    int reel[] = {0,1,2,3};
    int spin[3];
    for(int i =0; i<3;i++){
        spin[i]= reel[rand()%4];
    }
    printf("Reel is spinning\n");
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

}