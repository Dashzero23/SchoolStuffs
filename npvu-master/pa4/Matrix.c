/********************************************************************************* 
* Nguyen Vu, npvu
* 2023 Spring CSE101 PA4
* Matrix.c
* Implementation of Matrix ADT
*********************************************************************************/

#include "List.h"
#include "Matrix.h"

typedef struct EntryObj *Entry;

typedef struct EntryObj {
    int col;
    double val;
} EntryObj;

typedef struct MatrixObj* Matrix;

typedef struct MatrixObj {
    List* entry;
    int size;
    int NNZ;
} MatrixObj;

Entry newEntry(int column, double value) { // greatly inspired by newNode from List.c from pa1
    if (column < 0) { // check for invalid input
        fprintf(stderr, "Function newEntry: column is less than 0.\n");
        exit(EXIT_FAILURE);
    }

    Entry newEn = malloc(sizeof(EntryObj)); // fixed by changing to the object instead of the pointer
    newEn->col = column;
    newEn->val = value;

    return newEn;
}

void freeEntry(Entry *pE) { // greatly inspired by freeNode from List.c from pa1
    if (pE != NULL && *pE != NULL) { // check if pE is NULL or not
        // free memory and set to NULL
        free(*pE);
        *pE = NULL;
    }
}

Matrix newMatrix(int n) { // greatly inspired by newGraph from Graph.c from pa2 and pa3
    if (n <= 0) { // check for invalid input
        fprintf(stderr, "Function newMatrix: n is less than or equal to 0.\n");
        exit(EXIT_FAILURE);
    }

    Matrix newMa = malloc(sizeof(MatrixObj));

    newMa->size = n;
    newMa->NNZ = 0;
    newMa->entry = calloc(n + 1, sizeof(List));

    for (int i = 1; i <= n; i++) {
        newMa->entry[i] = newList();
    }

    return newMa;
}

void freeMatrix(Matrix *pM) { // greatly inspired by freeGraph from Graph.c from pa2 and pa3
    if (*pM != NULL && pM != NULL) { // check if pM is NULL or not
        makeZero(*pM);

        for (int i = 1; i <= size(*pM); i++) {
            freeList(&(*pM)->entry[i]);
            (*pM)->entry[i] = NULL;
        }
        
        free((*pM)->entry);
        free(*pM);
        *pM = NULL;
    }
}

int size(Matrix M) {
    if (M == NULL) { // check for invalid input
        fprintf(stderr, "Function size: M is NULL.\n");
        exit(EXIT_FAILURE);
    }

    return M->size;
}

int NNZ(Matrix M) {
    if (M == NULL) { // check for invalid input
        fprintf(stderr, "Function NNZ: M is NULL.\n");
        exit(EXIT_FAILURE);
    }

    return M->NNZ;
}

int equals(Matrix A, Matrix B) {
    if (A == NULL || B == NULL) { // check for invalid input
        fprintf(stderr, "Function equals: either matrices are NULL.\n");
        exit(EXIT_FAILURE);
    }

    if (size(A) != size(B)) { // precondition check
        return 0;
    }

    if (A == B) {
        //printf("start: A = B");
        return 1;
    }
    
    for (int i = 1; i <= size(A); i++) { // since entry is a list, I will compare each list element to each other as well as their lengthes
        List EntryA = A->entry[i];
        List EntryB = B->entry[i];

        if (length(EntryA) != length(EntryB)) {
            //printf("length: A != B\n");
            return 0;
        }

        // this part onward is greatly inspired by equals() from List.c from pa1 with some modification
        moveFront(EntryA);
        moveFront(EntryB);

        for (int i = 0; i < length(EntryA); i++) { // helped by TA to get the right format to get col and val
            // from pa1
            /*if (nodeA->data != nodeB->data) {
                return false;
            }*/
            
            if (((Entry)get(EntryA))->col != ((Entry)get(EntryB))->col) {
                //printf("col: A != B\n");
                return 0;
            }

            if (((Entry)get(EntryA))->val != ((Entry)get(EntryB))->val) {
                //printf("val: A != B\n");
                return 0;
            }
            
            moveNext(EntryA);
            moveNext(EntryB);
        }

        /*if (length(EntryA) != length(EntryB)) {
            return 0;
        }

        // check and compare each node
        Node nodeA = EntryA->front;
        Node nodeB = EntryB->front;

        for (int i = 0; i < length(EntryA); i++) {
            if (nodeA->data != nodeB->data) {
                return 0;
            }

            nodeA = nodeA->next;
            nodeB = nodeB->next;
        } */
    }
    
    //printf("End: A = B\n");
    return 1;
}

void makeZero(Matrix M) { // helped by TA to debug
    if (M == NULL) { // check for invalid input
        fprintf(stderr, "Function makeZero: M is NULL.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i <= size(M); i++) {
        List cur_entry = M->entry[i];
        //moveFront(cur_entry);
        
        while (length(cur_entry) != 0) {
            Entry e = front(cur_entry); // this is TA's idea since freeEntry takes pointer, which led to seg fault before
            //cur_entry = newList();
            freeEntry(&e);
            deleteFront(cur_entry);
        }
        
        clear(cur_entry);
    }

    // size unchanged since we only making things zero, not removing them from the matrix
    M->NNZ = 0;
}

void changeEntry(Matrix M, int i, int j, double x) { // TA helped with case where there is no entry value and need to insert new entry
    if (M == NULL) { // check for invalid input
        fprintf(stderr, "Function changeEntry: M is NULL.\n");
        exit(EXIT_FAILURE);
    }

    if (i < 1 || i > size(M)) { // precondition check
        fprintf(stderr, "Function changeEntry: i is out of range.\n");
        exit(EXIT_FAILURE);
    }

    if (j < 1 || j > size(M)) { // precondition check
        fprintf(stderr, "Function changeEntry: j is out of range.\n");
        exit(EXIT_FAILURE);
    }

    List i_row = M->entry[i]; // get the ith row

    for (moveFront(i_row); index(i_row) >= 0; moveNext(i_row)) {
        Entry entry = (Entry)get(i_row);

        if (entry->col == j) { // found jth column
            if (x == 0.0) { // add from looking at ModelMatrixTest to get the right NNZ
                //printf("x = 0\n");
                M->NNZ--;
                //Entry e = get(i_row);
                /*printf("delete row\n"); // added in after reading ModelMatrixTest.c
                delete(i_row);
                printf("after delete\n");
                printf("free entry\n");*/
                //freeEntry(&entry);
                delete(i_row);
            }
            
            else {
                entry->val = x; // change the value
            }
            
            //printf("return\n");
            return;
        }
    }

    // if hasn't return yet, it means that the (i, j) entry was not found in the row
    // insert a new entry instead
    if (x != 0.0) { // insert only if x is non-zero
        Entry new_entry = newEntry(j, x);
        
        if (length(i_row) == 0) { // if the row is currently empty
            append(i_row, new_entry); // append the new entry
        } 
        
        else {
            for (moveFront(i_row); index(i_row) >= 0; moveNext(i_row)) {
                Entry cur_entry = (Entry)get(i_row);
                
                if (cur_entry->col > j) { // need to be bigger than j to insertBefore instead of change
                    insertBefore(i_row, new_entry); // insert the new entry before the current entry
                    break;
                } 
                
                else if (index(i_row) == length(i_row) - 1) { // if reach end of row
                    append(i_row, new_entry); // append at the end
                    break;
                }
            }
        }
        
        M->NNZ++;
    }
}

Matrix copy(Matrix A) { // go through all rows and columns and copy entries
    if (A == NULL) { // check for invalid input
        fprintf(stderr, "Function copy: A is NULL.");
        exit(EXIT_FAILURE);
    }
    
    Matrix Copy = newMatrix(A->size);
    
    for (int i = 1; i <= size(A); i++) { // go through all rows
        List row = A->entry[i];
        
        for (moveFront(row); index(row) >= 0; moveNext(row)) { // go through all columns
            Entry entry = (Entry)get(row);
            append(Copy->entry[i], newEntry(entry->col, entry->val));
            Copy->NNZ++;
        }
    }
    
    return Copy;
}

Matrix transpose(Matrix A) { // very similar concept to copy, just swapping row and column
    if (A == NULL) { // check for invalid input
        fprintf(stderr, "Function transpose: A is NULL.");
        exit(EXIT_FAILURE);
    }

    Matrix Transpose = newMatrix(A->size);

    for (int i = 1; i <= size(A); i++) { // go through all rows
        List rowA = A->entry[i];
        
        for (moveFront(rowA); index(rowA) >= 0; moveNext(rowA)) { // go through all columns
            Entry EntryA = (Entry)get(rowA);
            int row = EntryA->col; // since transpose makes row into columns and vice versa
            append(Transpose->entry[row], newEntry(i, EntryA->val));
            Transpose->NNZ++;
        }
    }

    return Transpose;
}

Matrix scalarMult(double x, Matrix A) {
    if (A == NULL) { // check for invalid input
        fprintf(stderr, "Function scalarMult: A is NULL.\n");
        exit(EXIT_FAILURE);
    }
    
    Matrix scalarMatrix = newMatrix(A->size);
    
    for (int i = 1; i <= A->size; i++) {
        List rowA = A->entry[i];
        List rowScalar = scalarMatrix->entry[i];
        moveFront(rowA);
        
        // go through all EntryA and multiply by x and add to rowScalar
        for (moveFront(rowA); index(rowA) >= 0; moveNext(rowA)) {
            Entry EntryA = (Entry)get(rowA);
            append(rowScalar, newEntry(EntryA->col, x * EntryA->val));
        }
        
        scalarMatrix->NNZ += length(rowScalar); // add NNZ equals to total new column in rowScalar
    }
    
    return scalarMatrix;
}

Matrix sum(Matrix A, Matrix B) {
    if (A == NULL || B == NULL) { // check for invalid input
        fprintf(stderr, "Function sum: A or B is NULL.\n");
        exit(EXIT_FAILURE);
    }

    if (A->size != B->size) { // precondition check
        fprintf(stderr, "Function sum: A and B have different size.\n");
        exit(EXIT_FAILURE);
    }
    
    Matrix SumMatrix = newMatrix(A->size);

    if(A == B) { // added after looking at ModelMatrixTest.c
        SumMatrix = scalarMult(2, A);
        
        return SumMatrix;
    }
    
    for (int i = 1; i <= SumMatrix->size; i++) {
        List rowA = A->entry[i];
        List rowB = B->entry[i];
        List Sum = newList();

        // go through each column of A and B in the same row
        moveFront(rowA);
        moveFront(rowB);
        
        /*printf("Get EntryA\n");
        Entry EntryA = get(rowA);
        printf("Get EntryB\n");
        Entry EntryB = get(rowB);*/
        // apparently if rowA or rowB length is 0, it will break here, so I will get the data after checking the index
        Entry EntryA, EntryB;
        
        while (index(rowA) >= 0 && index(rowB) >= 0) {
            //printf("Get A\n");
            EntryA = get(rowA);
            //printf("Get B\n");
            EntryB = get(rowB);
            //printf("Enter program\n");
            
            // if A and B both have entries, add their value
            if (EntryA->col == EntryB->col) {
                double SumValue = EntryA->val + EntryB->val;
                
                if (SumValue != 0.0) { // no need to add if it's 0
                    append(Sum, newEntry(EntryA->col, SumValue));
                }
                
                moveNext(rowA);
                moveNext(rowB);
            }
            
            // if A has an entry before B, add EntryA
            else if (EntryA->col < EntryB->col) {
                append(Sum, newEntry(EntryA->col, EntryA->val));
                moveNext(rowA);
            }
            
            // if B has an entrybefore A, add the EntryB
            else { // EntryB->col < EntryA->col
                append(Sum, newEntry(EntryB->col, EntryB->val));
                moveNext(rowB);
            }
        }
        
        // add remaining entries in A
        //printf("Remainder A\n");
        while (index(rowA) >= 0) {
            EntryA = get(rowA);
            append(Sum, newEntry(EntryA->col, EntryA->val));
            moveNext(rowA);
        }
        
        // add remaining entries in B
        //printf("Remainder B\n");
        while (index(rowB) >= 0) {
            EntryB = get(rowB);
            append(Sum, newEntry(EntryB->col, EntryB->val));
            moveNext(rowB);
        }
        
        // update the row of SumMatrix
        SumMatrix->entry[i] = Sum;
        //printf("Add NNZ\n");
        SumMatrix->NNZ += length(Sum);
    }

    return SumMatrix;
}

Matrix diff(Matrix A, Matrix B) { // subtracting is just adding the negative version of the other number - my high school math teacher, probably
    if (A == NULL || B == NULL) { // check for invalid input
        fprintf(stderr, "Function diff: A or B is NULL.");
        exit(EXIT_FAILURE);
    }
    if (size(A) != size(B)) { // precondition check
        fprintf(stderr, "Function diff: A and B have different sizes.");
    }
    
    Matrix difference = sum(A, scalarMult(-1.0, B));
    return difference;
}

Matrix product(Matrix A, Matrix B) { // I'm doing the way I did it in Linear Algebra
    if (A == NULL || B == NULL) { // check for invalid input
        fprintf(stderr, "Function product: A or B is NULL.");
        exit(EXIT_FAILURE);
    }
    
    if (A->size != B->size) { // precondition check
        fprintf(stderr, "Function product: A and B have different sizes.");
    }
    
    //printf("Make matrix P\n");
    Matrix ProductMatrix = newMatrix(A->size);
    double dotProduct;
    
    List rowA;
    List rowB;
    Entry EntryA;
    Entry EntryB;

    //printf("Make matrix Bt\n");
    Matrix Bt = transpose(B); // since we don't have a column list, transpose matrix has rows as columns, which give us a column list with ->col equal to ->row in the transpose

    for (int i = 1; i <= ProductMatrix->size; i++) {
        //printf("Get rowA\n");
        rowA = A->entry[i];
        
        if (length(rowA) != 0) {
            //printf("Pass length rowA\n");
        }
        
        else {
            //printf("Fail length rowA\n");
            continue; // it has been skipping lines so changed to continue
        }
        
        for (int j = 1; j <= ProductMatrix->size; j++) {
            //printf("Get rowB\n");
            rowB = Bt->entry[j]; // get row Bt, or column B
            
            if (length(rowB) != 0) {
                //printf("Pass length rowB\n");
            }
            
            else {
                //printf("Fail length rowB\n");
                continue; // same case with matrix A
            }
            
            dotProduct = 0;
            moveFront(rowA);
            moveFront(rowB);
            
            while (index(rowA) >= 0 && index(rowB) >= 0) {
                //printf("Get EntryA\n");
                EntryA = get(rowA);
                //printf("Get EntryB\n");
                EntryB = get(rowB);
                //printf("Multiplying A[%d,%d] = %lf and B[%d,%d] = %lf\n", i, EntryA->col, EntryA->val, j, EntryB->col, EntryB->val);
                
                if (EntryA->col == EntryB->col) { // 
                    dotProduct += EntryA->val * EntryB->val;
                    moveNext(rowA);
                    moveNext(rowB);
                }
                
                else if (EntryA->col < EntryB->col) { // if A has a column before B, then skip, since we don't need to multiply by 0
                    moveNext(rowA);
                }
                
                else { // EntryA->col > EntryB->col // and vice versa, skip B if needed
                    moveNext(rowB);
                }
            }
            
            if (dotProduct != 0) {
                append(ProductMatrix->entry[i], newEntry(j, dotProduct));
                ProductMatrix->NNZ++;
            }
        }
    }

    //freeList(rowA);
    //freeList(rowB);
    //freeEntry(&EntryA);
    //freeEntry(&EntryB);
    freeMatrix(&Bt);
    return ProductMatrix;
}

void printMatrix(FILE* out, Matrix M) { // greatly inspired by printGraph
    if (M == NULL) { // check for invalid input
        fprintf(stderr, "Function printMatrix: M is NULL.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i <= size(M); i++) {
        List row = M->entry[i];

        if (length(row) != 0) {
            fprintf(out, "%d: ", i);

            for (moveFront(row); index(row) >= 0; moveNext(row)) {
                Entry entry = (Entry)get(row);
                fprintf(out, "(%d, %.1f) ", entry->col, entry->val);
            }

            fprintf(out, "\n");
        }
    }
}

