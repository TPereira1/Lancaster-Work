#include <stdio.h>
#include <string.h>

int main(){
    FILE *file;
    file = fopen("access.log", "r");
    if(file != NULL){
        char lineOfText[80];
        int hit = 0;
        int mozzilaHit = 0;
        while(fgets(lineOfText, 80, file) != NULL){
            hit ++;
            for(int i = 0; lineOfText[i] != '\0'; i++){
                if(strncmp(lineOfText,"Mozilla",7)==0){
                    mozzilaHit++;
                }
                if(strncmp(lineOfText,"66.249.66",9)==0){
                   printf("66.249.66 visited: %s\n",lineOfText);
                }
            }
        }
        printf("The total number of hits are %d\n", hit);
        printf("The total number of attacks on Mozzila are %d\n ", mozzilaHit);
        fclose(file);
    }
}