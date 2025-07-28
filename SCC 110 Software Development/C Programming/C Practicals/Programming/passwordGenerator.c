#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

bool checkPassword(char* newPass){
    bool numCheck = false;
    bool symbolCheck = false;
    int upperCheck = 0;
    int lowerCheck = 0;
    for(int i = 0; i < strlen(newPass);i++){
        if(isdigit(newPass[i])){
            numCheck = true;
        }
        else if(!isalpha(newPass[i])){
            symbolCheck = true;
        }
        else if(isupper(newPass[i])){
            upperCheck++;
        }
        else if(islower(newPass[i])){
            lowerCheck ++;
        }
    }
    return numCheck && symbolCheck && upperCheck >= 2 && lowerCheck >= 2;
}

int main(){
    srand(time(NULL));
    char* password;
    bool acceptablePassword = false;
    while(!acceptablePassword){
        for(int i = 0; i < 16; i++){
            int randChar = (rand() % 93) + 34;
            password[i] = randChar + "0";
        }
        password[16] ='\0';
        acceptablePassword = checkPassword(password);
    }
    printf("Generated password is: %s\n",password);
}