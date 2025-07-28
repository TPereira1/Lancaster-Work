#include <stdio.h>

unsigned long factorial(int n) {
   unsigned long f;
   int i;
   for (i = 1; i < n; i++) {
       f = f*i;
   }
   return f;
}

int main() {

    int k;
    printf("Enter a positive integer: ");
    scanf("%u", &k);
    unsigned long fk = factorial(k);
    printf("The value of %u factorial is %lu\n\n", k, fk);

}
