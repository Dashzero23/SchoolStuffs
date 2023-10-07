/********************************************************************************* 
* Nguyen Vu, npvu
* 2023 Spring CSE101 PA5
* List.cpp
* Implementation of List ADT
*********************************************************************************/
// I'm actually very clueless on C++ syntax so I will write in how I would in C in this first push
// I learned that I need (Type):: whenever dealing with that type functions instead of just (Type), like List:: vs List
// fix precondition to match List.h instead of using from pa1
// it says "‘friend’ used outside of class" so I removed them
#include "List.h"

List::Node::Node(ListElement x) { // helped from TA's pseudo code
    data = x; // ListElement data
    next = nullptr; // set next and prev to NULL, this is from description for an empty List
    prev = nullptr;
}

List::List() { // apparently I don't need to initialize variable with data type because it's already initialized in List.h
    frontDummy = new Node(-99999999); // with 2 dummy nodes at the front and back
    backDummy = new Node(99999999); // changed to big numbers so they are different from the other and doesn't match other element
    // for some reason other number gave me seg fault so I'm using the TA's number from his pseudo code
    
    frontDummy->next = backDummy; // pointing to each other as next and prev, respectively
    backDummy->prev = frontDummy;

    beforeCursor = frontDummy; // use two Node pointers, which are called beforeCursor and afterCusor
    afterCursor = backDummy; // If the cursor is at the front of the List (position 0), then beforeCursor will point to frontDummy. 
    // Likewise if the cursor is at the back of the List (position n), then afterCursor will point to backDummy.
    pos_cursor = 0; // set position to 0
    num_elements = 0; // set total element to 0 (dummy doesn't count)
}

List::List(const List& L){
    // same thing as making a list
    frontDummy = new Node(-99999999);
    backDummy = new Node(99999999);
    frontDummy->next = backDummy;
    backDummy->prev = frontDummy;
    beforeCursor = frontDummy;
    afterCursor = backDummy;
    pos_cursor = 0;
    num_elements = 0;


    // greatly inspired by copyList from pa1
    /*Node curr_node = L->front;

    while (curr_node != NULL) { // copy nodes from the old List to the new one
        append(new_list, curr_node->data);
        curr_node = curr_node->next;
    }*/

    Node* curr_node = L.frontDummy->next; // use frontDummy->next because dummy node doesn't count, so get to first node

    while (curr_node != L.backDummy) {
        insertBefore(curr_node->data);
        curr_node = curr_node->next;
    }
}

List::~List() {
    clear(); // Deletes all elements in this List, setting it to the empty state. Got this from the List.h

    delete frontDummy; // need to delete dummy since they are not elements in the list
    delete backDummy;
}

int List::length() const {
    return num_elements;
}

ListElement List::front() const {
    if (num_elements <= 0) { // precondition check
        fprintf(stderr, "Function front: num_element is or less than 0.\n");
        exit(EXIT_FAILURE);
    }

    return frontDummy->next->data; // since dummy node doesn't count, the next node is the real node
}

ListElement List::back() const {
    if (num_elements <= 0) { // precondition check
        fprintf(stderr, "Function back: num_element is or less than 0.\n");
        exit(EXIT_FAILURE);
    }

    return backDummy->prev->data; // same concept as function front
}

int List::position() const {
    if (pos_cursor < 0 || pos_cursor > num_elements) { // precondition check
        fprintf(stderr, "Function position: pos_cursor is out of range.\n");
        exit(EXIT_FAILURE);
    }

    return pos_cursor;
}

ListElement List::peekNext() const {
    if (position() >= length()) { // precondition check
        fprintf(stderr, "Function peekNext: cursor is at the back or out of range.\n");
        exit(EXIT_FAILURE);
    }

    return afterCursor->data;
}

ListElement List::peekPrev() const {
    if (position() <= 0) { // precondition check
        fprintf(stderr, "Function peekPrev: cursor is at the front or out of range.\n");
        exit(EXIT_FAILURE);
    }

    return beforeCursor->data;
}

void List::clear() { // greatly inspired by clear() in List.c from pa1
    /*while (length(L) > 0) { // Check if length of L is 0
        // Delete first element using deleteFront function
        deleteFront(L);
    }*/

    
    while (length() > 0) { // check if length of L is 0
        moveFront(); // we don't have deleteFront so I will moveFront and delete the element after the cursor (first element)
        eraseAfter(); // delete the element after the cursor (which is at the front)
    }

    // reset everything
    // this gave me seg fault
    /*frontDummy->next = backDummy;
    backDummy->prev = frontDummy;
    beforeCursor = frontDummy;
    afterCursor = backDummy;
    pos_cursor = 0;
    num_elements = 0;*/
}

void List::moveFront() { // greatly inspired by moveFront from pa1
    // L->cursor = L->front;
    beforeCursor = frontDummy;
    afterCursor = frontDummy->next;

    // L->index = 0;
    pos_cursor = 0;
}

void List::moveBack() { // greatly inspired by moveBack from pa1
    // L->cursor = L->back;
    afterCursor = backDummy;
    beforeCursor = backDummy->prev;

    // L->index = length(L) - 1; instead of index, it's pos_cursor
    pos_cursor = num_elements;
}
 
ListElement List::moveNext() { // move forward and return the previous (passed over)
    if (position() >= length()) { // precondition check
        fprintf(stderr, "Function moveNext: cursor is at the back or out of range.\n");
        exit(EXIT_FAILURE);
    }

    beforeCursor = beforeCursor->next; // move forward
    afterCursor = afterCursor->next;
    pos_cursor++; // add index by 1

    return beforeCursor->data; // return the previous
}

ListElement List::movePrev() { // similar concept to moveNext
    if (position() <= 0) { // precondition check
        fprintf(stderr, "Function movePrev: cursor is at the front or out of range.\n");
        exit(EXIT_FAILURE);
    }

    beforeCursor = beforeCursor->prev;
    afterCursor = afterCursor->prev;
    pos_cursor--;

    return afterCursor->data; // this time it's the after (since we moved backward)
}

void List::insertAfter(ListElement x) { // with help of TA from converting pa1 function to work here
// fix some running issue
    Node* new_node = new Node(x); // make new node with data x

    // set the details of the new node to proper pointer
    new_node->next = afterCursor;
    new_node->prev = beforeCursor;

    // change the details of nodes around the new node
    // added in check for NULL nodes based on TA suggestion
    if (beforeCursor) {
        beforeCursor->next = new_node;
    }
    
    if (afterCursor) {
        afterCursor->prev = new_node; // set the prev of the node originally after the cursor (before insert newNode) to newNode
        // change to afterCursor before changing to new_node for easier reading and understanding
    }
    
    afterCursor = new_node;

    // increase total element
    num_elements++;
    // decrease pos_cursor, added in after ListTest, not needed since cursor never move
    //pos_cursor--;
}

void List::insertBefore(ListElement x) { // similar concept to insertAfter
    Node* new_node = new Node(x); // make new node with data x

    // set the details of the new node to proper pointer
    new_node->next = afterCursor;
    new_node->prev = beforeCursor;

    // change the details of nodes around the new node
    // added in check for NULL nodes based on TA suggestion
    if (beforeCursor) {
        beforeCursor->next = new_node;
    }
    
    if (afterCursor){
        afterCursor->prev = new_node;
    }
    
    beforeCursor = new_node;

    // increases total element
    num_elements++;
    // increase pos_cursor, added in after ListTest
    pos_cursor++;
}

void List::setAfter(ListElement x) { // simply changes the data
    if (position() >= length()) { // precondition check
        fprintf(stderr, "Function setAfter: cursor is at the back or out of range.\n");
        exit(EXIT_FAILURE);
    }

    afterCursor->data = x;
}

void List::setBefore(ListElement x) {
    if (position() <= 0) { // precondition check
        fprintf(stderr, "Function setBefore: cursor is at the front or out of range.\n");
        exit(EXIT_FAILURE);
    }

    beforeCursor->data = x;
}

void List::eraseAfter() { // inspired by List.c in pa1 in setting the right pointer for nodes after deleting
    if (position() >= length()) { // precondition check
        fprintf(stderr, "Function eraseAfter: cursor is at the back or out of range.\n");
        exit(EXIT_FAILURE);
    }
    
    Node* Delete = afterCursor; // get the node

    // change afterCursor
    afterCursor = Delete->next; 
    afterCursor->prev = Delete->prev;

    // change beforeCursor
    beforeCursor->next = afterCursor;

    // delete and change num_element
    delete Delete;
    num_elements--;
}

void List::eraseBefore() { // similar concept to eraseAfter
    if (position() <= 0) { // precondition check
        fprintf(stderr, "Function eraseBefore: cursor is at the front or out of range.\n");
        exit(EXIT_FAILURE);
    }
    
    Node* Delete = beforeCursor; // get the node

    // change beforeCursor
    beforeCursor = Delete->prev;
    beforeCursor->next = Delete->next;

    // change afterCursor
    afterCursor->prev = beforeCursor;

    // this time also uopdate the cursor
    delete Delete;
    pos_cursor--;
    num_elements--;
}

int List::findNext(ListElement x) { // helped with TA's pseudo code
// further modification to get the right value
// debugged by TA to pass ListTest
    //bool found = false; // check if found or not
    Node* tempNode = afterCursor; // get the starting node

    while (tempNode != backDummy) {
        //found = (tempNode->data == x); // TA's idea to update found for later use instead of implementing straight into the if statement
        // modified my way so I don't need the bool anymore
        if (tempNode->data == x) { // if found, set the right pointers and return
            beforeCursor = tempNode;
            afterCursor = tempNode->next;
            pos_cursor++;
            return pos_cursor;
        }
        
        // if not found, move forward
        tempNode = tempNode->next;
        pos_cursor++;
    }
    
    // if hasn't returned, move cursor to the end and return -1
    beforeCursor = tempNode;
    afterCursor = backDummy;
    return -1;
}

int List::findPrev(ListElement x) { // similar concept to findNext, but backward
    //bool found = false; // check if found or not
    Node* tempNode = beforeCursor; // get the starting node
    
    while (tempNode != frontDummy) { // added in more condition check to prevent seg fault
        //found = (tempNode->data == x);

        if (tempNode->data == x) { // if found, set the right pointers and return
            beforeCursor = tempNode->prev;
            afterCursor = tempNode;
            pos_cursor--;
            return pos_cursor;
        }
        
        tempNode = tempNode->prev;        
        pos_cursor--;
    }

    // if hasn't returned, return -1
    beforeCursor = frontDummy;
    afterCursor = tempNode;
    return -1;
}

void List::cleanup() { // similar ideas to findNext, but start at the front and delete nodes instead of returning it
// fixed with TA pseudo code
    Node* current = frontDummy->next; // start with the first node
    
    while (current != backDummy) {
        Node* tempNode = frontDummy->next; // node to compare, changed from current->next to frontDummy to check from the beginning and prevent seg fault
        Node* next = current->next; // save the next node
        //int new_pos = 1; // cursor position after deleting (if there's a dup), no need since set to back at the end
        bool found = false; // has this to prevent infinite loop
        
        while (!found && tempNode != current) {
            found = (tempNode->data == current->data);
            
            if (found) { // found a dup
                // update the surrounding nodes
                // TA suggested this method to check for special case of current to update cursor if needed
                if (current == beforeCursor){
                    beforeCursor->prev->next = afterCursor;
                    afterCursor->prev = beforeCursor->prev;
                    beforeCursor = beforeCursor->prev;
                } 
                
                else if (current == afterCursor) {
                    beforeCursor->next = afterCursor->next;
                    afterCursor->next->prev = beforeCursor;
                    afterCursor = afterCursor->next;
                }
                
                else {
                    current->prev->next = current->next;
                    current->next->prev = current->prev;
                }
                
                /*if(pos_cursor >= new_pos) { // fix cursor position
                    pos_cursor--;
                }*/
                
                num_elements--;
                delete current;
            } 
            
            else { // if no dup found
                tempNode = tempNode->next; // go to next node
            }
        }
        
        current = next; // go to the next node
        //new_pos++; // increase actual position by 1
    }
    
    pos_cursor = length();
}
 
List List::concat(const List& L) const { // followed the given pseudo code for join() in Queue ADT
    List Concat; // Queue J
    Node* N = frontDummy->next; // Node* N = this->front; frontDummy doesn't count so frontDummy->next is the front node
    Node* M = L.frontDummy->next; // Node* M = Q.front; in this case Q is L
    
    /*while( N!=nullptr ){
        J.Enqueue(N->data);
        N = N->next;
    }*/

    while (N != backDummy) { // I use NULL because I initialize things as NULL instead of nullptr
    // switched to nullptr since TA recommended
    // switched to backDummy to prevent it inserting backDummy into Concat
        Concat.insertBefore(N->data);
        N = N->next;
    }

    /*while( M!=nullptr ){
        J.Enqueue(M->data);
        M = M->next;
    }*/

    while (M != L.backDummy) { // same thingas the first half of Concat
        Concat.insertBefore(M->data);
        M = M->next;
    }

    Concat.List::moveFront(); // put cursor at the front
    return Concat; // return J;
}

bool List::equals(const List& R) const { // greatly inspired by equals() in List.c from pa1
    /*if (length(A) != length(B)) {
        return false;
    }*/

    if (num_elements != R.num_elements) {
        return false;
    }

    // from pa1
    /*Node nodeA = A->front;
    Node nodeB = B->front;

    for (int i = 0; i < length(A); i++) {
        if (nodeA->data != nodeB->data) {
            return false;
        }

        nodeA = nodeA->next;
        nodeB = nodeB->next;
    }*/

    // check and compare each node
    Node* nodeA = frontDummy->next;
    Node* nodeB = R.frontDummy->next;

    while (nodeA != backDummy && nodeB != R.backDummy) { // since we have the backDummy, a while loop is faster to write
        if (nodeA->data != nodeB->data) {
            return false;
        }

        nodeA = nodeA->next;
        nodeB = nodeB->next;
    }
    
    // check if both lists reached their backDummy at the same time, added in after debugging ListClient
    if (nodeA == backDummy && nodeB == R.backDummy) {
        return true;
    } 
    
    else {
        return false;
    }
}

std::string List::to_string() const { // greatly inspired by to_string function in Queue.cpp
   std::string s = "";

   for(Node* N = frontDummy->next; N != backDummy; N = N->next){
      s += std::to_string(N->data)+ " ";
   }
   
   return s;
}

std::ostream& operator<<(std::ostream& stream, const List& L) { // this is print to file if I understand correctly
// done with help from TA
// fixed by changing to pseudo code from Queue
// fixed some syntax
   return stream << L.List::to_string();
}

bool operator==(const List& A, const List& B) { // the same as equals() with modification to match list names
// fixed by changing to pseudo code from Queue
// fixed syntax
    return A.List::equals(B);
}

List& List::operator=(const List& L) { // so I understand this as "replace the current list with given list"
// very similar concept to concat, just without inserting element from current list
// helped by TA so I know how to return "this"
// fix to match the pseudo code
    if (this != &L) { // not self assignment
        // make a copy of L
        List temp = L; // Queue temp = Q

        // then swap the copy's fields with fields of this, TA helped with getting all the pointers correct
        std::swap(frontDummy, temp.frontDummy);
        std::swap(backDummy, temp.backDummy);
        std::swap(beforeCursor, temp.beforeCursor);
        std::swap(afterCursor, temp.afterCursor);
        std::swap(pos_cursor, temp.pos_cursor);
        std::swap(num_elements, temp.num_elements);
    }

    // return this with the new data installed
    return *this;

    // the copy, if there is one, is deleted upon return
}
