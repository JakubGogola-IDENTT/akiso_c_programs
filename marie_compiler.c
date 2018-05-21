#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

void printZero (unsigned long int number) {
    for (int i = 0; i < number; i++) {
        printf ("0");
    }
}

int main (int argc, char** argv) {
    
    int counter;
    int OPCode;
    int number;
    
    char* inputFileName;
    char* line;
    char* temp;
    char** command;
    
    FILE* inputFile;
    FILE* outputFile;
    
    size_t length;
    
    
    if (argc != 2) {
        fprintf (stderr, "Missing path to file!\n");
        exit (1);
    }
    
    inputFileName = argv[1];
    
    //Trying to open file.
    inputFile = fopen (inputFileName, "r");
    
    //Checking if file exists.
    if (inputFile == NULL) {
        fprintf (stderr, "File does not exist!\n");
        exit (1);
    }
    
    command = malloc (2 * sizeof (char*));
    while (getline (&line, &length, inputFile) != -1 ) {
        
        counter = 0;
        temp = strtok (line, " ");
        while (temp != NULL) {
            command [counter] = temp;
            temp = strtok (NULL, " ");
            counter++;
        }
        
        if (strcmp (command[0], "JNS") == 0) {
            OPCode = 0;
        } else if (strcmp (command[0], "LOAD") == 0) {
            OPCode = 1;
        } else if (strcmp (command[0], "STORE") == 0) {
            OPCode = 2;
        } else if (strcmp (command[0], "ADD") == 0) {
            OPCode = 3;
        } else if (strcmp (command[0], "SUBT") == 0) {
            OPCode = 4;
        } else if (strcmp (command[0], "INPUT") == 0) {
            OPCode = 5;
        } else if (strcmp (command[0], "OUTPUT") == 0) {
            OPCode =6 ;
        } else if (strcmp (command[0], "HALT") == 0) {
            OPCode = 7;
        } else if (strcmp (command[0], "SKIPCOND") == 0) {
            OPCode = 8;
        } else if (strcmp (command[0], "JUMP") == 0) {
            OPCode = 9;
        } else if (strcmp (command[0], "CLEAR") == 0) {
            OPCode = 10;
        } else if (strcmp (command[0], "ADDI") == 0) {
            OPCode = 11;
        } else if (strcmp (command[0], "JUMPI") == 0) {
            OPCode = 12;
        } else if (strcmp (command[0], "HEX") == 0) {
            number = myAtoi(command[1], 16);
            temp = malloc (BUFSIZE);
            temp = myItoa (number, temp, 2);
            printZero (4 + 12 - strlen(temp));
            printf ("%s\n", temp);
            free (temp);
            continue;
        } else if (strcmp (command[0], "DEC") == 0) {
            number = myAtoi(command[1], 10);
            temp = malloc (BUFSIZE);
            temp = myItoa (number, temp, 2);
            printZero (4 + 12 - strlen(temp));
            printf ("%s\n", temp);
            free (temp);
            continue;
        } else {
            fprintf (stderr, "Wrong command!\n");
            printf ("%s", command[0]);
            exit (1);
        }
        
        if (command[1] == '\0') {
            command [1] = "0";
        }
        
        if (command[1] != NULL) {
            number = myAtoi(command[1], 16);
        }
        
        temp = NULL;
        temp = malloc (BUFSIZE);
        
        temp = myItoa (OPCode, temp, 2);
        printZero (4 - strlen(temp));
        printf ("%s", temp);
        
        temp = myItoa (number, temp, 2);
        printZero (12 - strlen(temp));
        printf ("%s\n", temp);
        free (temp);
    }
    
    return 0;
}