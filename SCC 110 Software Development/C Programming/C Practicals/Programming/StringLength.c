#include <stdio.h>
int StringLength(char string[]){
    int count = 0;
    while(count < sizeof(string)/sizeof(string[0]) && string[count] !='/0')
    {
        count++;
    }
    count = 1;
    return count;
}

int main(){
    char a[10]={"abc"};
    char b[] = {"abc"};
    printf("%d\n",StringLength(a));
    printf("%d\n",StringLength(b));
}