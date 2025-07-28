#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
    int num;
    //int PlayerGuesses[10];
    int choice;
    int turn;
    int guessHorL = -1;
    printf("Choose between heads(1) or tails(0) to decide who will go first\n");
    scanf("%d",&choice);
    srand(time(NULL));
    int coinToss = rand() % 2;
    if(coinToss == choice){
        printf("You won the coin toss and can go first.\n");
        turn = 1;
    }
    else{
        printf("You lost the coin toss, so the computer goes first.\n");
        turn = 0;
    }
    int genNum = rand() % 10;
    printf("I've thought of a number.\nEnter your guess\n");
    do{
        if (turn == 0){
            int temp = num;
            num = rand()%10;
            while (guessHorL == 0 && num <= temp){
                num = rand()%10;
            }
            while (guessHorL == 1 && num >= temp){
                num = rand()%10;
            }
            /*int count = 0;
            while(PlayerGuesses > 0 && count < sizeof(PlayerGuesses)){
                if (PlayerGuesses[count] == num){
                    count = 0;
                    num = rand()%10;
                }
                else{
                    count++;
                }
            }*/
            turn = 1;
                printf("Computer guessed %d", num);
        }
        else{
            printf(">");
            scanf("%d", &num);
            //PlayerGuesses[sizeof(PlayerGuesses)] = num;
            turn = 0;
        }

        if(num == genNum){
            printf("\nWell done, the number I thought of was %d\n", genNum);
        }
        if(num < genNum){
            printf("\nhigher\n");
            guessHorL = 0;
        }
        if(num > genNum){
            printf("\nlower\n");
            guessHorL = -1;
        }
    }while(num != genNum);
}