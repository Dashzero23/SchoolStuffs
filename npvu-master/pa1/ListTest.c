/********************************************************************************* 
* Nguyen Vu, npvu
* 2023 Winter CSE101 PA1
* ListTest.c 
* Testing List.c
*********************************************************************************/ 
// Test cases for all functions based on ModelListTest.c and guidance from TAs during sections
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "List.h"

int main() {
    // Test empty length
    fprintf(stderr, "Entering Empty Length test.\n");
    
    List A = newList();
    if (length(A) != 0) {
        fprintf(stderr, "Length is not 0.\n");
    }
    
    else {
        fprintf(stderr, "Empty Length passed.\n");
    }
    
    // Test append, moveBack and moveNext
    fprintf(stderr, "Entering Append test.\n");
    
    append(A, 1);
    append(A, 2);
    append(A, 3);
    moveBack(A);
    
    if (get(A) != 3) {
        fprintf(stderr, "Move back did not point to correct node.\n");
    }
    
    else {
        fprintf(stderr, "Move back passed.\n");
    }
    
    append(A, 4);
    moveNext(A);
    
    if (length(A) != 4) {
        fprintf(stderr, "Append Length is incorrect.\n");
    }
    
    else {
        fprintf(stderr, "Append Length passed.\n");
    }
    
    if (index(A) != 3) {
        fprintf(stderr, "Index Move next is incorrect.\n");
    }
    
    else {
        fprintf(stderr, "Move next Index passed.\n");
    }
    
    // Free list A, also test for freeList
    fprintf(stderr, "Free list A.\n");
    freeList(&A);
    
    // Test prepend and moveFront
    fprintf(stderr, "Entering Empty Length test.\n");
    // Make new list
    A = newList();

    // Same concept as Append test    
    prepend(A, 1);
    prepend(A, 2);
    prepend(A, 5);
    moveFront(A);
    
    if (get(A) != 5) {
        fprintf(stderr, "Move front did not point to correct node.\n");
    }
    
    else {
        fprintf(stderr, "Move front passed.\n");
    }
    
    prepend(A, 4);
    movePrev(A);
    
    if (length(A) != 4) {
        fprintf(stderr, "Prepend Length is incorrect.\n");
    }
    
    else {
        fprintf(stderr, "Prepend Length passed.\n");
    }
    
    if (index(A) != 0) {
        fprintf(stderr, "Move Prev is incorrect.\n");
    }
    
    else {
        fprintf(stderr, "Move Prev passed.\n");
    }
    
    // Test insertBefore
    // After the movePrev test, the cursor is at the front node
    moveNext(A);
    insertBefore(A, 10);
    movePrev(A);
    
    if (length(A) != 5)  {
        fprintf(stderr, "Length Insert Before is incorrect.\n");
    }
    
    else {
        fprintf(stderr, "Length Insert Before passed.\n");
    }
    
    if (get(A) != 10) {
        fprintf(stderr, "Insert Before is incorrect.\n");
    }
    
    else {
        fprintf(stderr, "Insert Before passed.\n");
    }
    
    // Test insertAfter
    // Same concept as insertBefore
    moveNext(A);
    insertAfter(A, 20);
    moveNext(A);
    
    if (length(A) != 6)  {
        fprintf(stderr, "Length Insert After is incorrect.\n");
    }
    
    else {
        fprintf(stderr, "Length Insert After passed.\n");
    }
    
    if (get(A) != 20) {
        fprintf(stderr, "Insert After is incorrect.\n");
    }
    
    else {
        fprintf(stderr, "Insert After passed.\n");
    }
    
    // Test deleteFront
    moveFront(A);
    deleteFront(A);
    
    if (index(A) != -1) {
        fprintf(stderr, "Index Delete Front is incorrect for front node.\n");
    }
    
    else {
        fprintf(stderr, "Index Delete Front passed for front node.\n");
    }
    
    moveBack(A);
    deleteFront(A);
    
    if (index(A) != 3) {
        fprintf(stderr, "Index Delete Front is incorrect.\n");
    }
    
    else {
        fprintf(stderr, "Index Delete Front passed.\n");
    }
    
    if (length(A) != 4) {
        fprintf(stderr, "Length Delete Front is incorrect.\n");
    }
    
    else {
        fprintf(stderr, "Length Delete Front passed.\n");
    }
    
    // Test deleteBack
    // Same concept as deleteFront
    moveBack(A);
    deleteBack(A);
    
    if (index(A) != -1) {
        fprintf(stderr, "Index Delete Back is incorrect for front node.\n");
    }
    
    else {
        fprintf(stderr, "Index Delete Back passed for front node.\n");
    }
    
    moveFront(A);
    deleteBack(A);
    
    if (index(A) != 0) {
        fprintf(stderr, "Index Delete Back is incorrect.\n");
    }
    
    else {
        fprintf(stderr, "Index Delete Back passed.\n");
    }
    
    if (length(A) != 2) {
        fprintf(stderr, "Length Delete Back is incorrect.\n");
    }
    
    else {
        fprintf(stderr, "Length Delete Back passed.\n");
    }
    
    // Test delete
    insertAfter(A, 50);
    moveNext(A);
    delete(A);
    
    if (length(A) != 2) {
        fprintf(stderr, "Length Delete is incorrect.\n");
    }
    
    else {
        fprintf(stderr, "Length Delete Back passed.\n");
    }
    
    if (index(A) != -1) {
        fprintf(stderr, "Index Delete is incorrect.\n");
    }
    
    else {
        fprintf(stderr, "Index Delete passed.\n");
    }
    
    freeList(&A);
    return 0;
}
