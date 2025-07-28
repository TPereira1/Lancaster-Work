#include <stdio.h>
int main(){
    char *filename = "pin.txt";
    FILE *fp = fopen(filename,"w");

    for(int i = 0; i<=9;i++){
        for(int x = 0; x<=9;x++){
            for(int y = 0; y<=9;y++){
                for(int z = 0; z<=9;z++){
                    fprintf(fp,"%d%d%d%d\n",i,x,y,z);
                }
            }
        }
    }
}