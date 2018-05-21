#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFSIZE 1024


char* reverse (char* buffer, int firstPos, int lastPos) {

  char* temp = malloc(BUFSIZE);
  int pos = lastPos;

  for (int i = firstPos; i <= lastPos; i++) {
    temp[i] = buffer[pos];
    pos--;
  }
  return temp;
}

// Function to convert integer value to string.
char* myItoa (int value, char* buffer, int base) {

  int n;
  int rest;
  int counter;

  //Checking if n is negative;
  if (value < 0) {
    n = -value;
  } else {
    n = value;
  }

  counter = 0;
  while(n > 0) {
    rest = n % base;

    if (rest >= 10) {
      buffer[counter] = 'a' + (rest - 10);
      counter++;
    } else {
      buffer[counter] = '0' + rest;
      counter++;
    }

    n = n / base;
  }

  if (counter == 0) {
    buffer[0] = '0';
  }

  if (value < 0 && base == 10) {
    buffer[counter] = '-';
  }
  return reverse(buffer, 0, counter - 1);
}

//Function to convert string to integer.
int myAtoi(char* buffer, int base) {
	int n = 0;

	while (*buffer != '\0' && (*buffer >= '0' && *buffer <= '9') || (*buffer >= 'a' && *buffer <= 'f')) {
    if(*buffer >= 'a' && *buffer <= 'f') {
      n = n * base + (*buffer - 'a' + 10);
    } else {
      n = n * base + (*buffer - '0');
    }
    buffer++;
	}

	return n;
}



void myPrintf (char* text, ...) {

  char* string = malloc(BUFSIZE);
  //Pointer to arguments of myPrintf.
  char* ptr;

  //Variables to put on them values of arguments.
  int intArg;
  char* strArg = malloc(BUFSIZE);

  //Setting pointer to first argument - text.
  //It works like va_start macro.
  ptr = (char*) &text + sizeof(text);

  while (*text != '\0') {
    if (*text == '%') {
      text++;
      switch (*text) {
        case 'd': {
          intArg = (*(int *) ptr);
          ptr += sizeof(int);

          string = myItoa(intArg, string, 10);
          while(*string != '\0') {
            write(1, string, sizeof(char));
            string++;
          }
          text++;
          break;
        }
        case 'b': {
          intArg = (*(int *) ptr);
          ptr += sizeof(int);

          string = myItoa(intArg, string, 2);

          while(*string != '\0') {
            write(1, string, sizeof(char));
            string++;
          }
          text++;
          break;
        }
        case 'x': {
          intArg = (*(int *) ptr);
          ptr += sizeof(int);

          string = myItoa(intArg, string, 16);
          while(*string != '\0') {
            write(1, string, sizeof(char));
            string++;
          }
          text++;
          break;
        }
        case 's': {
          strArg = (*(char**) ptr);
          ptr += sizeof(char*);

          while(*strArg != '\0') {
            write(1, strArg, sizeof(*strArg));
            strArg++;
          }
          text++;
          break;
        }
      }
    } else {
      write(1, text, sizeof(char));
      text++;
    }
  }
}

void myScanf(char* text, ...) {
  char* string = malloc(BUFSIZE);

  //Pointer to arguments of myPrintf.
  char* ptr;

  //Variables to put on them values of arguments.
  int* intArg;
  char** strArg = malloc(BUFSIZE);

  //Setting pointer to first argument - text.
  //It works like va_start macro.
  ptr = (char*) &text + sizeof(text);

  while (*text != '\0') {
    if (*text == '%') {
      text++;
      switch (*text) {
        case 'd': {
          intArg = (int*) (*(int*) ptr);
          ptr += sizeof(int*);
          read(0, string, BUFSIZE);
          *intArg = myAtoi(string, 10);
          break;
        }
        case 'b': {
          intArg = (int*) (*(int*) ptr);
          ptr += sizeof(int*);
          read(0, string, BUFSIZE);
          *intArg = myAtoi(string, 2);
          break;
        }
        case 'x': {
          intArg = (int*) (*(int*) ptr);
          ptr += sizeof(int*);
          read(0, string, BUFSIZE);
          *intArg = myAtoi(string, 16);
          break;
        }
        case 's': {
          strArg = (char**) (*(char**) ptr);
          ptr += sizeof(*strArg);
          read(0, string, BUFSIZE);
          *strArg = string;
          break;
        }
      }
    }
    text++;
  }
}

int main(){

  int integer, binary, hexadecimal;
  char* str;

  myPrintf("Ala %d ma %b kota %x %s\n", 256, 0b111011, 0xa6, "i psa");

  myScanf("%d", &integer);
  myPrintf("Integer: %d\n", integer);

  myScanf("%b", &binary);
  myPrintf("Binary: %b\n", binary);

  myScanf("%x", &hexadecimal);
  myPrintf("Hexadecimal: %x\n", hexadecimal);

  myScanf("%s", &str);
  myPrintf("String: %s\n", str);

  myScanf("%d\n%b\n%x\n%s\n", &integer, &binary, &hexadecimal, &str);
  myPrintf("%d, %b, %x, %s\n", integer, binary, hexadecimal, str);
  return 0;
}
