#define MYSTRING 0

#if MYSTRING
#include "mystring.h"	//include for your string library
#define STRLEN  mystrlen
#define STRCMP  mystrcmp
#define STRNCMP mystrncmp
#define STRCPY  mystrcpy
#define STRNCPY mystrncpy
#define STRDUP  mystrdup
#else
#include <string.h>	//include for standard string library
#define STRLEN  strlen
#define STRCMP  strcmp
#define STRNCMP strncmp
#define STRCPY  strcpy
#define STRNCPY strncpy
#define STRDUP  strdup
#endif

#include <stdio.h>	//include for printf
#include <stdlib.h>	//include for free


int main(void){
	char test_string_1[] = "hello world!";
	char test_string_2[] = "hello";
	char test_string_3[] = "hallo";
	char test_string_4[] = "XXXXXXXXXXXXXXXX";
	char test_string_5[] = "XXXXXXXXXXXXXXXX";
	char* test_string_6 = NULL;

	printf("strlen(\"%s\") = %d\n", test_string_1, (int) STRLEN(test_string_1));
	printf("strcmp(\"%s\",\"%s\") = %d\n", test_string_1, test_string_2, STRCMP(test_string_1,test_string_2));
	printf("strcmp(\"%s\",\"%s\") = %d\n", test_string_2, test_string_3, STRCMP(test_string_2,test_string_3));
	printf("strncmp(\"%s\",\"%s\",%d) = %d\n", test_string_1, test_string_2, 3, STRNCMP(test_string_1,test_string_2,3));
	printf("strncmp(\"%s\",\"%s\",%d) = %d\n", test_string_1, test_string_2, 5, STRNCMP(test_string_1,test_string_2,5));
	printf("strncmp(\"%s\",\"%s\",%d) = %d\n", test_string_1, test_string_2, 6, STRNCMP(test_string_1,test_string_2,6));

	STRNCPY(test_string_4, test_string_2, 10);
	printf("test_string_4 = \"%s\"\n", test_string_4);

	STRCPY(test_string_5, test_string_2);
	printf("test_string_5 = \"%s\"\n", test_string_5);

	test_string_6 = STRDUP(test_string_1);
	if (test_string_6) {
		printf("test_string_6 = \"%s\"\n", test_string_6);
		free(test_string_6);
	}
}

