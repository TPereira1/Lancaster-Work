#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

char* getItem(int a){
    switch(a){
        char item[10];
        case 0: return("   Bell   ");
        break;
        case 1: return("  Orange  ");
        break;
        case 2: return("  Cherry  ");
        break;
        case 3: return(" Horseshoe");
        break;
    }
}

void printReel(int spin[]){
    printf("\r%s-%s-%s",getItem(spin[0]),getItem(spin[1]),getItem(spin[2]));
}

int maximum(int a, int b, int c){
    if (a >= b && a >= c){
        return(a);
    }
    if (b >= a && b >= c){
        return(b);
    }
    else{
        return(c);
    }
}
int minimum(int a, int b, int c){
    if (a <= b && a <= c){
        return(a);
    }
    if (b <= a && b <= c){
        return(b);
    }
    else{
        return(c);
    }
}
int medium(int a, int b, int c){
    if (a != maximum(a,b,c) && a != minimum(a,b,c)){
        return(a);
    }
    if (b != maximum(a,b,c) && b != minimum(a,b,c)){
        return(b);
    }
    else{
        return(c);
    }
}

void nudge(int spin[]){
    printf("Which reel do you want to nudge.");
    int num;
    scanf("%d",&num);
    spin[num-1] = spin[num-1] + 1;
    printReel(spin);
    printf("\n");
    checkResults(spin);
}

void checkResults(int spin[]){
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
int main(){
    srand(time(NULL));
    int reel[] = {0,1,2,3};
    int spin[3] ={0,0,0};
    printf("Beginning spin:\n");
    printReel(spin);
    int reel1 = rand()%50+1;
    int reel2 = rand()%50+1;
    int reel3 = rand()%50+1;
    for(int i = 0; i <= maximum(reel1,reel2,reel3);i++){
        if(i <= minimum(reel1,reel2,reel3)){
            spin[0]=spin[0]+1;
            if(spin[0]==4){
                spin[0]=0;
            }
        }
        if(i <= medium(reel1,reel2,reel3)){
            spin[1]=spin[1]+1;
            if(spin[1]==4){
                spin[1]=0;
            }
        }
        spin[2]=spin[2]+1;
        if(spin[2]==4){
            spin[2]=0;
        }
    
        Sleep(50);
        printReel(spin);
    }
    printf("\n");
    checkResults(spin);
    printf("\nDo you want to nudge a reel(Y/N):");
    char res;
    scanf("%c",&res);
    if ('Y' == res ){
        nudge(spin);
    }
}