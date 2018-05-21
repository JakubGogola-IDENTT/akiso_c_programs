#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

#define blockSize (64 / sizeof (double))

int min (int a, int b) {
    if (a < b) {
        return a;
    } else {
        return b;
    }
}

unsigned long long int** createMatrix (unsigned long long int** matrix, int size) {
    matrix = malloc (size * sizeof (*matrix));
    for (int i = 0; i < size; i++) {
        matrix [i] = malloc (size * sizeof (*matrix[i]));
    }
    return matrix;
}

unsigned long long int** fillMatrixWithRandomValues (unsigned long long int** matrix, int size) {
    unsigned long long int random;
    
    srand(time(0));
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            random = rand() % 10;
            matrix[i][j] = random;
        }
    }
    
    return matrix;
}

void printMatrix (unsigned long long int** matrix, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%llu ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void freeMatrix (unsigned long long int** matrix,int size) {
    for (int i = 0; i < size; i++) {
        free (matrix[i]);
    }
    free (matrix);
}

unsigned long long int** multiplyMatrixes (unsigned long long int** matrixA, unsigned long long int** matrixB, unsigned long long int** matrixC, int size) {

    // i - row, j - column
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            for (int k = 0; k < size; k++) {
                matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
            }
        }
    }
    return matrixC;
}

unsigned long long int** multiplyMatrixesWithTransposition (unsigned long long int** matrixA, unsigned long long int** matrixB, unsigned long long int** matrixC, int size) {
    
    // i - row, j - column
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            for (int k = 0; k < size; k++) {
                matrixC[i][j] += matrixA[i][k] * matrixB[j][k];
            }
        }
    }
    return matrixC;
}

unsigned long long int** transposeMatrix (unsigned long long int** matrix, int size) {
    unsigned long long int** tempMatrix = createMatrix (tempMatrix, size);
    for (int i = 0; i < size; i++)  {
        for (int j = 0; j < size; j++) {
            tempMatrix[j][i] = matrix[i][j];
        }
    }
    return tempMatrix;
}

unsigned long long int** multiplyMatrixesWithOptimalization_OLD (unsigned long long int** matrixA, unsigned long long int** matrixB, unsigned long long int** matrixC, int size) {
    //int blockSize = (int) sqrt (size);
    
    
    for (int i = 0; i < size; i += blockSize) {
        for (int j = 0; j < size; j += blockSize) {
            for (int k = 0; k < size; k += blockSize) {
                for (int l = i; l < min (i + blockSize, size); l++) {
                    for (int m = j; m < min (j + blockSize, size); m++) {
                        for (int n = k; n < min (k + blockSize, size); n++) {
                            matrixC[l][m] += matrixA[l][n] * matrixB[n][m];
                        }
                    }
                }
            }
        }
    }
    return matrixC;
}

unsigned long long int** multiplyMatrixesWithOptimalization (unsigned long long int** matrixA, unsigned long long int** matrixB, unsigned long long int** matrixC, int size) {
    
    unsigned long long int* tempA;
    unsigned long long int* tempB;
    unsigned long long int* tempC;
    
    int l;
    int m;
    int n;
    
    for (int i = 0; i < size; i += blockSize) {
        for (int j = 0; j < size; j += blockSize) {
            for (int k = 0; k < size; k += blockSize) {
                for (l = 0, tempC = &matrixC[i][j], tempA = &matrixA[i][k]; l < blockSize; l++, tempC += size, tempA += size) {
                    for (m = 0, tempB = &matrixB[k][j]; m < blockSize; m++, tempB += size) {
                        for (n = 0; n < blockSize; n++) {
                            tempC[n] += tempA[m] * tempB[n];
                        }
                    }
                }
            }
        }
    }
    return matrixC;
}


int main (int argc, char** argv) {
    int size;
    unsigned long long int** matrixA;
    unsigned long long int** matrixB;
    unsigned long long int** matrixC;
    
    struct timeval startTime;
    struct timeval endTime;
    double executionTime;
    
    if (argc != 2) {
        fprintf (stderr, "Size of matrix expected in arguments!\n");
        exit (1);
    } else {
        size = atoi (argv[1]);
    }
    
    //Creating matrixes to multiply.
    matrixA = createMatrix (matrixA, size);
    matrixB = createMatrix (matrixB, size);
    
    matrixA = fillMatrixWithRandomValues (matrixA, size);
    sleep (1);
    matrixB = fillMatrixWithRandomValues (matrixB, size);
    
    //Multiplying matrixes.
    printf ("Multiplying matrixA * matrixB.\n");
    
    matrixC = createMatrix (matrixC, size);

    gettimeofday (&startTime, NULL);
    matrixC = multiplyMatrixes (matrixA, matrixB, matrixC, size);
    gettimeofday (&endTime, NULL);
    //printMatrix (matrixC, size);
    freeMatrix (matrixC, size);
    
    executionTime = (endTime.tv_sec - startTime.tv_sec) + ((endTime.tv_usec - startTime.tv_usec) / 1000000.0);
    printf ("Time of multiplying: %lf", executionTime);
    
    
    //Transposition of matrix
    printf("\n\nTransposition of matrixB.\n");
    printf ("Multiplying matrixA * matrixB with transposition.\n");
    
    matrixC = createMatrix (matrixC, size);
    gettimeofday (&startTime, NULL);
    matrixB = transposeMatrix (matrixB, size);
    matrixC = multiplyMatrixesWithTransposition (matrixA, matrixB, matrixC, size);
    gettimeofday (&endTime, NULL);
    
    executionTime = (endTime.tv_sec - startTime.tv_sec) + ((endTime.tv_usec - startTime.tv_usec) / 1000000.0);
    printf ("Time of multiplying after transposition: %lf\n\n", executionTime);
    //printMatrix (matrixC, size);
    
    freeMatrix (matrixC, size);
   
    //Multiplying matrix with cache optimalization.
    matrixB = transposeMatrix (matrixB, size);

    printf ("Multiplying matrixA * matrixB with cache optimalization.\n");
    
    matrixC = createMatrix (matrixC, size);
    gettimeofday (&startTime, NULL);
    matrixC = multiplyMatrixesWithOptimalization (matrixA, matrixB, matrixC, size);
    //multiplyMatrixesWithOptimalization (matrixA, matrixB, matrixC, size);
    gettimeofday (&endTime, NULL);
    
    executionTime = (endTime.tv_sec - startTime.tv_sec) + ((endTime.tv_usec - startTime.tv_usec) / 1000000.0);
    printf ("Time of multiplying after optimalization: %lf\n\n", executionTime);
    //printMatrix (matrixC, size);
    
    //freeMatrix (matrixA, size);
    //freeMatrix (matrixB, size);
    //freeMatrix (matrixC, size);
    
    return 0;
}