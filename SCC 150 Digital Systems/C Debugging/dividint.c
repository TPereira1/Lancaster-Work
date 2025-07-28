#include <stdio.h>
int DivideInt(int a, int b);
int main()
{
	int x = 5, y= 2;
    printf(" x/y= %d \n", DivideInt(x,y));
	x = 3;
	y = 0;
	printf(" x/y= %d \n", DivideInt(x,y));
	return 0;
}
int DivideInt(int a, int b)
{
	return a/b;
}
