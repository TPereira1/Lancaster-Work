#include <stdio.h>
#include <string.h>
#include <ctype.h>

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

int main(){
    printf("Enter Password to be check: ");
    char newPass[30];
    scanf("%[^\n]%*c",newPass);

    float consecutive = ConsecutiveCheck(newPass);
    int length = strlen(newPass);
    int numDigits = numOfDigits(newPass);
    int numSymbols = numOfSymbol(newPass);
    int UpperCase = numOfUpper(newPass);
    int LowerCase = length - numDigits - numSymbols - UpperCase;

    if(length > 15 && numDigits > 1 && UpperCase > 2 && LowerCase > 2 && numSymbols > 1 && consecutive < 0.3){
        printf("Password Strength: Strong");
    }
    else if(length > 10 && numDigits >= 1 && UpperCase >= 2 && LowerCase >= 2 && numSymbols >= 1 && consecutive < 0.5){
        printf("Password Strength: Average");
    }
    else if(numDigits >= 1 && UpperCase >= 2 && LowerCase >= 2 && numSymbols >= 1){
        printf("Password Strength: Weak");
    }
    else{
        printf("Error");
    }
}