#include <stdio.h>

void printHelloWorld(){
	for(int i = 0; i <= 255; i++)
		printf("\33[38;5;%dm" "Hello, world!\n", i);
	printf("\33[0m");
}

int main(){
	printHelloWorld();
	return 0;
}
