/********************************************************************************* 
* Nguyen Vu, npvu
* 2023 Spring CSE101 PA4
* Sparse.c
* Main program for the assignment
*********************************************************************************/
// I mean this file is just pretty much given if read the example output
// read input/output were from previous PA, scan 2 matrices, and just compute equations that are just from the Matrix.c
#include <stdio.h>
#include <stdlib.h>
#include "Matrix.h"

int main(int argc, char *argv[]) {
    FILE *in, *out;

    // check command line for correct number of arguments
    if (argc != 3) {
        printf("Usage: %s <input file> <output file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // open files for reading and writing
    in = fopen(argv[1], "r");
    out = fopen(argv[2], "w");

    if (in == NULL) {
        printf("Unable to open file %s for reading\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    if (out == NULL) {
        printf("Unable to open file %s for writing\n", argv[2]);
        exit(EXIT_FAILURE);
    }
    
    // scan matrix sizes
    int size = 0;
    fscanf(in, "%d", &size);
    Matrix A = newMatrix(size);
    Matrix B = newMatrix(size);

    // scan NNZ for both matrices
    int aNNZ = 0;
    int bNNZ = 0;
    fscanf(in, " %d", &aNNZ);
    fscanf(in, " %d", &bNNZ);

    // make both matrices using input file
    int row = 0;
    int col = 0;
    double val = 0.0;
    
    for (int i = 1; i <= aNNZ; i++) {
        fscanf(in, "%d %d %lf", &row, &col, &val); 
        changeEntry(A, row, col, val);
    }
    
    for (int i = 1; i <= bNNZ; i++) {
        fscanf(in, "%d %d %lf", &row, &col, &val); 
        changeEntry(B, row, col, val);
    }
   
    // print matrices after required calculations
    fprintf(out, "A has %d non-zero entries: \n", NNZ(A));
    printMatrix(out, A);
    fprintf(out, "\n");

    fprintf(out, "B has %d non-zero entries: \n", NNZ(B));
    printMatrix(out, B);
    fprintf(out, "\n");

    // (1.5)*A =
    Matrix C = scalarMult(1.5, A);
    fprintf(out, "(1.5)*A = \n");
    printMatrix(out, C);
    fprintf(out, "\n");

    // A+B =
    Matrix D = sum(A, B);
    fprintf(out, "A+B = \n");
    printMatrix(out, D);
    fprintf(out, "\n");

    // A+A =
    Matrix E = sum(A, A);
    fprintf(out, "A+A = \n");
    printMatrix(out, E);
    fprintf(out, "\n");

    // B-A =
    Matrix F = diff(B, A);
    fprintf(out, "B-A = \n");
    printMatrix(out, F);
    fprintf(out, "\n");

    // A-A =
    Matrix G = diff(A, A);
    fprintf(out, "A-A = \n");
    printMatrix(out, G);
    fprintf(out, "\n");

    // Transpose(A) =
    Matrix H = transpose(A);
    fprintf(out, "Transpose(A) = \n");
    printMatrix(out, H);
    fprintf(out, "\n");

    // A*B =
    Matrix I = product(A, B);
    fprintf(out, "A*B = \n");
    printMatrix(out, I);
    fprintf(out, "\n");

    // B*B =
    Matrix J = product(B, B);
    fprintf(out, "B*B = \n");
    printMatrix(out, J);
    fprintf(out, "\n");
    
    // free memory and close files
    freeMatrix(&A);
    freeMatrix(&B);
    freeMatrix(&C);
    freeMatrix(&D);
    freeMatrix(&E);
    freeMatrix(&F);
    freeMatrix(&G);
    freeMatrix(&H);
    freeMatrix(&I);
    freeMatrix(&J);
    fclose(in);
    fclose(out);

    return 1;
}
