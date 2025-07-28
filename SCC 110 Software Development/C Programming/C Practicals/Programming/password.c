#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

bool NumSymbolcheck(char* newPass){
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

float ConsecutiveCheck(char *password){
    int consecutive;
    int size = strlen(password);
    for(int i = 0; i < size-1; i++){
        if(password[i]==password[i+1]||password[i]+1==password[i+1]||password[i]-1==password[i+1]){
            consecutive+=1;
        }
    }
    float percentage = (float)consecutive/size;
    return (percentage);
}

int numOfSymbol(char *password){
    int numSymbol = 0;
    for(int i = 0; i < strlen(password); i++){
        if(!isalpha(password[i]) && !isdigit(password[i])){
            numSymbol = numSymbol + 1;
        }
    }
    return(numSymbol);
}

int numOfDigits(char *password){
    int numOfDigits = 0;
    for(int i = 0; i < strlen(password); i++){
        if(isdigit(password[i])){
            numOfDigits++;
        }
    }
    return(numOfDigits);
}

int numOfUpper(char *password){
    int numberCharacters = 0;
    for(int i = 0; i < strlen(password); i++){
        if(isupper(password[i])){
            numberCharacters++;
        }
    }
    return (numberCharacters);
}

char* passStrength(char* newPass){
    float consecutive = ConsecutiveCheck(newPass);
    int length = strlen(newPass);
    int numDigits = numOfDigits(newPass);
    int numSymbols = numOfSymbol(newPass);
    int UpperCase = numOfUpper(newPass);
    int LowerCase = length - numDigits - numSymbols - UpperCase;

    if(length > 15 && numDigits > 1 && UpperCase > 2 && LowerCase > 2 && numSymbols > 1 && consecutive < 0.3){
        return("Password Strength: Strong");
    }
    else if(length > 10 && numDigits >= 1 && UpperCase >= 2 && LowerCase >= 2 && numSymbols >= 1 && consecutive < 0.5){
        return("Password Strength: Average");
    }
    else if(numDigits >= 1 && UpperCase >= 2 && LowerCase >= 2 && numSymbols >= 1){
        return("Password Strength: Weak");
    }
    else{
        return("Error");
    }
}

int main(){
    char storedPassword[20] = {"hardCod3dPassWord1"};
    char Password[20];
    printf("Please enter your password:\n");
    scanf("%[^\n]%*c",Password);
    if (strcmp(storedPassword,Password)==0){
        printf("Congradulations password matches\n");
        char newPass[20];
        char repeatPass[20];
        int stop = 0;
        while(stop == 0){
            printf("Enter a new password:\n");
            scanf("%[^\n]%*c",newPass);
            printf("%s\n",passStrength(newPass));
            printf("Re-enter the password:\n");
            scanf("%[^\n]%*c",repeatPass);
            if(strlen(newPass) < 8){
                printf("Password is two short, try again.\n");
            }
            else if(strcmp(newPass,repeatPass) != 0){
                printf("Passwords do not match, try again\n");
            }
            else if(!NumSymbolcheck(newPass)){
                printf("Passwords requires atleast two lowercase and uppercase characters; one number and one symbol, try again\n");
            }
            else{
                stop = 1;
            }
        }
        strcpy(storedPassword,newPass);
        printf("Password has now been changed to %s\n",storedPassword);
    }
    else{
        printf("Invalid password\n");
    }
}