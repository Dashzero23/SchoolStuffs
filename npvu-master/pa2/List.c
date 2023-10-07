/********************************************************************************* 
* Nguyen Vu, npvu
* 2023 Winter CSE101 PA2
* List.c 
* Implementation for List ADT
*********************************************************************************/ 
// Node struct, function newNode, freeNode, newList, freeList, length, index, front, back and get was inspired from given example Queue.c file in the Linked List folder
// Index, cursor and length fixing in all delete(front/back) functions were helped by TAs during sections
// Function prepend and append were done with the help of TA's help
// I realized each node doesn't have index after doing most of the code so there are sections in the function where I update the index for the nodes, which does not work
#include "List.h"

// Set struct data type

typedef struct NodeObj* Node;

typedef struct NodeObj {
    int data;
    Node prev;
    Node next;
} NodeObj;

typedef struct ListObj* List;

typedef struct ListObj {
    Node cursor;
    Node front;
    Node back;
    int index;
    int length;
} ListObj;

Node newNode(int data) { // Creates and returns a new empty Node.
    Node N = malloc(sizeof(NodeObj));
    N->data = data;
    N->prev = NULL;
    N->next = NULL;
    return N;
}

void freeNode(Node *pN) { // Frees all heap memory associated with *pN, and sets *pN to NULL.
    if (pN != NULL && *pN != NULL) { // Check if pL is NULL or not
        // Free memory and set to NULL
        free(*pN);
        *pN = NULL;
    }
}

List newList(void) { // Creates and returns a new empty List.
    List list = malloc(sizeof(ListObj));
    list->cursor = NULL;
    list->front = NULL;
    list->back = NULL;
    list->index = -1;
    list->length = 0;
    return list;
}

void freeList(List* pL) { // Frees all heap memory associated with *pL, and sets *pL to NULL.
    if (pL != NULL && *pL != NULL) { // Check if pL is NULL or not
        // Check if length of pL is 0
        while (length(*pL) != 0) {
            // Delete first element using deleteFront function
            deleteFront(*pL);
        }

        // Free memory and set to NULL
        free(*pL);
        *pL = NULL;
    }
}

int length(List L) { // Returns the number of elements in L.
    if (L == NULL) { // Check for invalid inputs
        fprintf(stderr, "Function length: List is NULL.\n");
        exit(EXIT_FAILURE);
    }

    return L->length;
}

int index(List L) { // Returns index of cursor element if defined, -1 otherwise.
    if (L == NULL) { // Check for invalid inputs
        fprintf(stderr, "Function index: List is NULL.\n");
        exit(EXIT_FAILURE);
    }

    if (L->cursor == NULL) { // Check if cursor is undefined
        return -1;
    }

    return L->index;
}

int front(List L)  { // Returns front element of L. Pre: length()>0
    if (L == NULL || L->length <= 0) { // Check for invalid inputs
        fprintf(stderr, "Function front: List is NULL or length is 0.\n");
        exit(EXIT_FAILURE);
    }

    return L->front->data;
}

int back(List L)  { // Returns back element of L. Pre: length()>0
    if (L == NULL || L->length <= 0) { // Check for invalid inputs
        fprintf(stderr, "Function back: List is NULL or length is 0.\n");
        exit(EXIT_FAILURE);
    }

    return L->back->data;
}

int get(List L) { // Returns cursor element of L. Pre: length()>0, index()>=0
    if (L == NULL || L->length <= 0 || L->index < 0) { // Check for invalid inputs
        fprintf(stderr, "Function get: List is NULL, length is 0, or index is -1.\n");
        exit(EXIT_FAILURE);
    }

    return L->cursor->data;
}

bool equals(List A, List B) { // Returns true iff Lists A and B are in same state, and returns false otherwise.
    if (A == NULL || B == NULL) { // Check for invalid inputs
        fprintf(stderr, "Function equals: Either list is NULL.\n");
        exit(EXIT_FAILURE);
    }

    if (length(A) != length(B)) {
        return false;
    }

    // check and compare each node
    Node nodeA = A->front;
    Node nodeB = B->front;

    for (int i = 0; i < length(A); i++) {
        if (nodeA->data != nodeB->data) {
            return false;
        }

        nodeA = nodeA->next;
        nodeB = nodeB->next;
    }

    return true;
}

void clear(List L) { // Resets L to its original empty state.
    if (L == NULL) { // Check for invalid inputs
        fprintf(stderr, "Function clear: List is NULL.\n");
        exit(EXIT_FAILURE);
    }
  
    while (length(L) > 0) { // Check if length of L is 0
        // Delete first element using deleteFront function
        deleteFront(L);
    }

    // Reset everything
    L->cursor = NULL;
    L->front = NULL;
    L->back = NULL;
    L->index = -1;
    L->length = 0;
}

void set(List L, int x) { // Overwrites the cursor element's data with x. Pre: length()>0, index()>=0
    if (L == NULL || L->length <= 0 || L->index < 0) { // Check for invalid inputs
        fprintf(stderr, "Function set: List is NULL, length is 0, or index is -1.\n");
        exit(EXIT_FAILURE);
    }

    L->cursor->data = x;
}

void moveFront(List L) { // If L is non-empty, sets cursor under the front element, otherwise does nothing.
    if (L == NULL || L->length <= 0) { // Check for invalid inputs
        fprintf(stderr, "Function moveFront: List is NULL or length is 0.\n");
        exit(EXIT_FAILURE);
    }

    L->cursor = L->front;
    L->index = 0;
}

void moveBack(List L) { // If L is non-empty, sets cursor under the back element, otherwise does nothing.
    if (L == NULL || L->length <= 0) { // Check for invalid inputs
        fprintf(stderr, "Function moveFront: List is NULL or length is 0.\n");
        exit(EXIT_FAILURE);
    }

    L->cursor = L->back;
    L->index = length(L) - 1;
}

void movePrev(List L) { // If cursor is defined and not at front, move cursor one step toward the front of L; 
// if cursor is defined and at front, cursor becomes undefined; 
// if cursor is undefined, do nothing
    if (L == NULL || L->length <= 0) { // Check for invalid inputs
        fprintf(stderr, "Function movePrev: List is NULL or length is 0.\n");
        exit(EXIT_FAILURE);
    }
    if (L->cursor == NULL) { //if cursor is undefined, do nothing
        return;
    }

    else if (L->cursor == L->front) { // if cursor is defined and at front, cursor becomes undefined
        L->cursor = NULL;
        L->index = -1;
    }

    else if (L->cursor != NULL) { // if cursor is defined and not at front, move cursor one step toward the front of L
        L->cursor = L->cursor->prev;
        L->index--;
    }
}

void moveNext(List L) { // If cursor is defined and not at back, move cursor one step toward the back of L; 
// if cursor is defined and at back, cursor becomes undefined; 
// if cursor is undefined, do nothing
    if (L == NULL || L->length <= 0) { // Check for invalid inputs
        fprintf(stderr, "Function moveNext: List is NULL or length is 0.\n");
        exit(EXIT_FAILURE);
    }

    if (L->cursor == NULL) { //if cursor is undefined, do nothing
        return;
    }

    else if (L->cursor == L->back) { // if cursor is defined and at back, cursor becomes undefined
        L->cursor = NULL;
        L->index = -1;
    }

    else if (L->cursor != NULL) { // if cursor is defined and not at back, move cursor one step toward the back of L
        L->cursor = L->cursor->next;
        L->index++;
    }
}

void prepend(List L, int x) { // Insert new element into L. If L is non-empty, insertion takes place before front element.
    if (L == NULL) { // Check for invalid inputs
        fprintf(stderr, "Function prepend: List is NULL.\n");
        exit(EXIT_FAILURE);
    }
    
    Node new_node = newNode(x);
    
    if (length(L) <= 0) { // if the list is empty, set the new node as the back node, and set it to the front node after
        L->back = new_node;
    }

    else { // otherwise, add the new node to the front of the list
        new_node->next = L->front;
        L->front->prev = new_node;
        
        if (L->cursor != NULL) { // update index if cursor isn't NULL
            L->index++;
        }
    }
    
    // increment the length of the list, set front node to the new node, and update the cursor position
    L->length++;
    L->front = new_node;

    /* if (L->cursor != NULL) {
        moveNext(L);
    } */
    
    /* L->index++;

    if (L->cursor != NULL) {
        L->cursor->index++;
    } */
}

void append(List L, int x) { // Insert new element into L. If L is non-empty, insertion takes place after back element.
    if (L == NULL) { // Check for invalid inputs
        fprintf(stderr, "Function append: List is NULL.\n");
        exit(EXIT_FAILURE);
    }
    
    Node new_node = newNode(x);
    
    if (length(L) <= 0) { // if the list is empty, set the new node as the front node, and set it to the back node after
        L->front = new_node;
    }

    else { // otherwise, add the new node to the end of the list
        new_node->prev = L->back;
        L->back->next = new_node;

        // no need to update index since it's added at the back, no nodes index will be affected
    }
    
    // increment the length of the list, set back node to the new node, and update the cursor position
    L->length++;
    L->back = new_node;
    
    /* if (L->cursor != NULL) {
        movePrev(L);
    } */
    /* L->index = length(L) - 1;

    if (L->cursor != NULL) {
        L->cursor->index = index(L);
    } */
}

void insertBefore(List L, int x) { // Insert new element before cursor. Pre: length()>0, index()>=0
    if (L == NULL || L->length <= 0 || L->index < 0) { // Check for invalid inputs
        fprintf(stderr, "List is NULL, length is 0, or index is -1.\n");
        exit(EXIT_FAILURE);
    }

    // set variable to the nodes that I want to use
    Node new_node = newNode(x);
    Node cursor = L->cursor;
    Node cursor_prev = cursor->prev;
    Node front = L->front;

    if (cursor_prev == NULL) { // if the cursor is at the front of the list, add the new node to the front
        // set the node after the new_node to front node
        new_node->next = front;
        // set the prev node of the original front node to new_node
        front->prev = new_node;
        // set the front node of the list to new_node
        L->front = new_node;
    }

    else { // normal case where it's at a random point in the list
        // set the next node of new_node to the cursor
        new_node->next = cursor;
        // set the prev node of new_node to the prev node of the cursor
        new_node->prev = cursor_prev;
        // set the next node of the node before the cursor to new_node
        cursor_prev->next = new_node;
        // set the prev node of cursor to new_node
        cursor->prev = new_node;
    }

    // increment the length of the list and update the cursor position
    L->length++;
    L->index++;
    
    // update the index for all nodes after new_node
    // cursor = new_node->next;

    /* while (cursor_prev != NULL) {
        //update index
        cursor->index++;
        // go to next node
        cursor = cursor->next;
    } */

    // set the cursor to new_node (not needed)
    // L->cursor = new_node;
}

void insertAfter(List L, int x) { // Insert new element after cursor. Pre: length()>0, index()>=0
    if (L == NULL || L->length <= 0 || L->index < 0) { // Check for invalid inputs
        fprintf(stderr, "List is NULL, length is 0, or index is -1.\n");
        exit(EXIT_FAILURE);
    }
    
    if (L->cursor != NULL) { // check if cursor is NULL or not
    // set variables to nodes that I wanted to use
    Node new_node = newNode(x);
    Node cursor = L->cursor;
    Node cursor_next = cursor->next;
    Node back = L->back;

    if (cursor == back) { // if the cursor is at the back of the list, add the new node to the back
        // set prev node of new_node to the last node
        new_node->prev = back;
        // set next node of the last node to new_node
        back->next = new_node;
        // set the last node of the list to new_node
        L->back = new_node;
    }
    
    else { // otherwise, add the new node after the cursor
        // set the next node of new_node to the node after the cursor
        new_node->next = cursor_next;
        // set the prev node of new_node to the cursor node
        new_node->prev = cursor;
        // set the prev node of the node after the cursor to new_node (again, sorry for the confusing wording)
        cursor_next->prev = new_node;
        // set the next node of the cursor pointed node to new_node
        cursor->next = new_node;
    }

    // increment the length of the list and update the cursor position, no need to update index since it's added after the cursor
    L->length++;

    // update the index for all nodes after new_node
    /* cursor = new_node->next;
    
    while (cursor_next != NULL) {
        //update index
        cursor->index++;
        // go to next node
        cursor = cursor->next;
    } */

    // set the cursor to new_node (not needed)
    // L->cursor = new_node;
    }
}

void deleteFront(List L) { // Delete the front element. Pre: length()>0
    if (L == NULL || L->length <= 0) { // Check for invalid inputs
        fprintf(stderr, "Function deleteFront: List is NULL or length is 0.\n");
        exit(EXIT_FAILURE);
    }
    
    Node temp = L->front; // store front node to check cursor after front node is deleted

    if (length(L) == 1) { // if the length is 1, delete the only node
        L->front = NULL;
        L->back = NULL;
        L->cursor = NULL;
        L->index = -1;
    }

    else { 
        // set the front node to the next node and delete the previous node (original front node)
        L->front = L->front->next;
        L->front->prev = NULL;

        if (L->cursor == temp) { // if cursor was at the original front node, makes index -1 and cursor NULL
            L->index = -1;
            L->cursor = NULL;
        }
        
        else { // decrease index by 1
            L->index--;
        }
        
        // decrease the index of all nodes by 1 after the removed node, starting from the front node
        /* Node current = L->front;

        while (current != NULL) {
            current->index--;
            current = current->next;
        } */
    }

    // free memory and reduce length by 1
    freeNode(&temp);
    // freeNode(&current);
    L->length--;
}

void deleteBack(List L) { // Delete the back element. Pre: length()>0
    if (L == NULL || L->length <= 0) { // Check for invalid inputs
        fprintf(stderr, "Function deleteBack: List is NULL or length is 0.\n");
        exit(EXIT_FAILURE);
    }
    
    Node temp = L->back; // store back node to check cursor after back node is deleted

    if (length(L) == 1) { // if the length is 1, delete the only node
        L->front = NULL;
        L->back = NULL;
        L->cursor = NULL;
        L->index = -1;
    }

    else {
        // set the back node to the prev node and delete the next node (original back node)
        L->back = L->back->prev;
        L->back->next = NULL;

        if (L->cursor == temp) { // if cursor was at the original back node, makes index -1 and cursor NULL
            L->index = -1;
            L->cursor = NULL;
        }

        // No need to decrease index of other nodes since we are removing the last node, other nodes' index are not affected
    }

    // free memory and reduce length by 1
    freeNode(&temp);
    L->length--;
}

void delete(List L) { // Delete the cursor element and make the cursor undefined
    if (L == NULL || L->length <= 0 || L->cursor == NULL || L->index < 0) { // Check for invalid inputs
        fprintf(stderr, "List is NULL, length is 0, cursor is NULL, or index is -1.\n");
        exit(EXIT_FAILURE);
    }

    Node curr_node = L->cursor;
    // Node next_node = curr_node->next; // save the next node for fixing indices

    if (curr_node == L->front) { // if the cursor is at the front, delete the front
        L->front = L->front->next;
        L->front->prev = NULL;
    }
    
    else if (curr_node == L->back) { // if the cursor is at the back, delete the back
        L->back = L->back->prev;
        L->back->next = NULL;
    }
    
    else { // otherwise, remove the current node and fix indices
        curr_node->prev->next = curr_node->next;
        curr_node->next->prev = curr_node->prev;

        /* while (next_node != NULL) { // Decrement the index of nodes after the deleted node
            next_node->index--;
            next_node = next_node->next;
        } */
    }
    
    // free memory, set cursor to NULL and reduce length by 1
    freeNode(&curr_node);
    // freeNode(&next_node);
    L->length--;
    L->cursor = NULL;
    L->index = -1;
}

void printList(FILE* out, List L) { // Prints to the file pointed to by out, 
// a string representation of L consisting of a space separated sequence of integers, with front on left.
    if (out == NULL || L == NULL) { // Check for invalid inputs
        fprintf(stderr, "List is NULL or file is NULL.\n");
        exit(EXIT_FAILURE);
    }

    Node curr_node = L->front; // set the first node to print to the front node

    while (curr_node != NULL) { // loop through all nodes and print them out to the out file with a space sepatated them
        fprintf(out, "%d ", curr_node->data);
        curr_node = curr_node->next;
    }
}

List copyList(List L) { // Returns a new List representing the same integer sequence as L. 
// The cursor in the new list is undefined, regardless of the state of the cursor in L. 
// The state of L is unchanged.
    if (L == NULL) { // Check for invalid inputs
        fprintf(stderr, "List is NULL.\n");
        exit(EXIT_FAILURE);
    }

    List new_list = newList(); // make new list

    if (length(L) == 0) { // check if the length is 0, if yes, no nodes need to be added
        return new_list;
    }

    Node curr_node = L->front;

    while (curr_node != NULL) { // copy nodes from the old List to the new one
        append(new_list, curr_node->data);
        curr_node = curr_node->next;
    }

    return new_list;
}
