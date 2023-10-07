/********************************************************************************* 
* Nguyen Vu, npvu
* 2023 Spring CSE101 PA7
* Dictionary.cpp
* Implementation of Dictionary ADT
*********************************************************************************/
// a lot of inspiration from previous PA
// used a lot of pseudo code from TA Michael Mcallister with his given pseudo code file for pretty much all functions for this file and the main program
// added in checking for node val to make sure it's not nil

#include "Dictionary.h"

Dictionary::Node::Node(keyType k, valType v) { // set key to k, val to v, left & right & parent nodes to null pointers
    key = k;
    val = v;
    parent = nullptr;
    left = nullptr;
    right = nullptr;
}

Dictionary::Dictionary() {
    /* set nil to a new Node("someLongAssStringHere", 0) // key shouldn't be something in the dictionary
    set the root to nil
    set current nil
    set num_pairs to 0
    */
    nil = new Node("TA said use a very long string that will never appear in the dictionary", 0);
    root = nil; // 
    current = nil;
    num_pairs = 0;
}

Dictionary::Dictionary(const Dictionary& D) {
    /*set nil to a new Node("someLongAssStringHere", 0) // key shouldn't be something in the dictionary
    set the root to nil
    set current nil
    set num_pairs to 0
    pre order copy the pairs of D (root of D, nil of D) into this Dictionary 
    */
    nil = new Node ("TA said use a very long string that will never appear in the dictionary", 0);
    root = nil;
    current = nil;
    num_pairs = 0;

    preOrderCopy(D.root, D.nil); // from TA pseudo code: pre order copy the pairs of D (root of D, nil of D) into this Dictionary 
}

Dictionary::~Dictionary() {
    postOrderDelete(root); // post order delete on the root node
    delete nil;
}

void Dictionary::inOrderString(std::string& s, Node* R) const {
    if (R != nil && R->key != "TA said use a very long string that will never appear in the dictionary") { // if R is not nil:
        inOrderString(s, R->left); // recursively call inOrderString on the left node of R
        s += R->key + " : " + std::to_string(R->val) + "\n"; // append the node's key and value (using to string function) to "s"
        inOrderString(s, R->right); // recursively call inOrderString on the right node of R
    }
}

void Dictionary::preOrderString(std::string& s, Node* R) const {
    if (R != nil) { // if R is not nil
        s += R->key + "\n"; // append the node's key to "s"
        preOrderString(s, R->left); // recursively call preOrderString on the left node of R
        preOrderString(s, R->right); // recursively call preOrderString on the right node of R
    }
}

void Dictionary::preOrderCopy(Node* R, Node* N) {
    if (R != N) { // if R and N are different:
        setValue(R->key, R->val); // set the value of R to its key
        preOrderCopy(R->left, N); // recursively call preOrderCopy on R's left subtree and N
        preOrderCopy(R->right, N); // recursively call preOrderCopy on R's right subtree and N
    }
}

void Dictionary::postOrderDelete(Node* R) {
    if (R != nil) {
        postOrderDelete(R->left); // recursively call postOrderDelete on R->left
        postOrderDelete(R->right); // recursively call postOrderDelete on R->right
        delete R;
    }
}

Dictionary::Node* Dictionary::search(Node* R, keyType k) const {
    while (R != nil || R->key != "TA said use a very long string that will never appear in the dictionary") { // iterate as long as R isn't nil:
        if (R == nil || k == R->key) { // if root is nil or k is the same as Node R's key:
            return R;
        }
        
        else if (k < R->key) { // else if k is smaller than Node R's key:
            R = R->left;
        }
        
        else { // else: // k greater than R->key
            R = R->right;
        }
    }

    return nil;
}

Dictionary::Node* Dictionary::findMin(Node* R) {
    Node* min = R; // set a temp node*min to R;

    if (min != nil && R->key != "TA said use a very long string that will never appear in the dictionary") { // if min isn't nil:
        while (min->left != nil) { // loop min to hug (set to) the nodes on the left side of the tree as long as it's not nil
            min = min->left;
        }
    }

    return min;
}

Dictionary::Node* Dictionary::findMax(Node* R) { // same as above but hug the nodes on the right side
    Node* max = R;

    if (max != nil && R->key != "TA said use a very long string that will never appear in the dictionary") {
        while (max->right != nil) {
            max = max->right;
        }
    }

    return max;
}

Dictionary::Node* Dictionary::findNext(Node* N) {
    if (N == nil) { // if N is nil return nil
        return nil;
    }

    else if (N->right != nil && N->right->key != "TA said use a very long string that will never appear in the dictionary") { // else if N->right isn't nil return the minimum of N->right
        return findMin(N->right);
    }

    else {
        Node* y = N->parent; // set a temp Node* y to the parent node of N

        while (y != nil && N == y->right && y->key != "TA said use a very long string that will never appear in the dictionary") { // iterate as long as y isn't nil and N is y->right:
            N = y; // set N to y and then set y to it's parent node
            y = y->parent;
        }

        return y;
    }
}

Dictionary::Node* Dictionary::findPrev(Node* N) { // same as above but with N->left, also instead of finding the minimum find the maximum of N->left
    if (N == nil) {
        return nil;
    }
    
    else if (N->left != nil && N->left->key != "TA said use a very long string that will never appear in the dictionary") {
        return findMax(N->left);
    }
    
    else {
        Node* y = N->parent;

        while (y != nil && N == y->left && y->key != "TA said use a very long string that will never appear in the dictionary") {
            N = y;
            y = y->parent;
        }

        return y;
    }
}

void Dictionary::transplant(Node* u, Node* v) {
    if (u->parent == nil || u->parent->key == "TA said use a very long string that will never appear in the dictionary") { // if u.parent == NIL
        root = v;
    }
    
    else if (u == u->parent->left) { // else if u == u.parent.left
        u->parent->left = v;
    }
    
    else {
        u->parent->right = v;
    }

    if (v != nil && v->key != "TA said use a very long string that will never appear in the dictionary") { // if v != NIL
        v->parent = u->parent;
    }
}

int Dictionary::size() const{
    return num_pairs;
}

bool Dictionary::contains(keyType k) const {
    Node* N = search(root, k); // set a temp Node* N to search from the root if k is in the dectionary

    if (N == nil) {
       return false;
    }

    else {
        return true;
    }
}

valType& Dictionary::getValue(keyType k) const{
    Node* N = search(root, k); // set a temp Node* N to search from the root if k is in the dectionary

    if (N == nil) { // if N is nil:
        throw std::logic_error("Dictionary: getValue(): key \""+k+"\" does not exist");
    } 
    
    return N->val;
}

bool Dictionary::hasCurrent() const{ // return current is not nil
    if (current == nil) {
        return false;
    }

    else {
        return true;
    }
}

keyType Dictionary::currentKey() const{
    if (hasCurrent() == false){ // if hasCurrent returns false:
        throw std::logic_error("Dictionary: currentKey(): current undefined");
    }
    
    return current->key;
}

valType& Dictionary::currentVal() const{
    if (hasCurrent() == false) { // !hasCurrent()
        throw std::logic_error("Dictionary: currentVal(): current undefined");
    }

    return current->val;
}

void Dictionary::clear() {
    postOrderDelete(root); // post order delete on the root node
    root = nil;
    current = nil;
    num_pairs = 0;
}

void Dictionary::setValue(keyType k, valType v) {
    Node* P = nil; // set a temp node* P to nil
    Node* R = root; // set a temo node* R to root

    while (R != nil) { // iterate as long as R is not nil:
        P = R; // set P to R

        if (k < R->key) { // check to see if k is smaller than the key of R:
            R = R->left; // if so, shift R to it's left node
        }
        
        else if (k > R->key) { // else if k is greater than the key of R:
            R = R->right; // if so, shift R to it's right node
        }
        
        else { // k is equal to R->key
            R->val = v; // set the value of R to v
            return;
        }
    }

    Node* N = new Node(k, v); // set a temp node* N to a new Node of key=k and val=v
    N->parent = P; // set the parent of N to P
    N->left = nil; // set every member node of N to nil
    N->right = nil;
    
    if (P == nil) { // if P equalls nil:
        root = N; // set the root to N;
    }
    
    else if (k < P->key) { // else if k is smaller than the key of node P:
        P->left = N; // set the left node of P to N
    }
    
    else {
        P->right = N; // set the right node of P to N
    }

    num_pairs++; // increment num_pairs
}

void Dictionary::remove(keyType k) {
    Node* N = search(root, k); // set a temp Node* N to search for k from the root
    
    if (N == nil || N->key == "TA said use a very long string that will never appear in the dictionary") { // if N is nil:
        throw std::logic_error("Dictionary: remove(): key \"" + k + "\" does not exist");
    }

    if (N->left == nil || N->left->key == "TA said use a very long string that will never appear in the dictionary") { // if the left node of N is nil:
        transplant(N, N->right); // call transplant on N and it's right node, decrement num_pairs
        num_pairs--;
    }
    
    else if (N->right == nil || N->right->key == "TA said use a very long string that will never appear in the dictionary") { // else if the right node of N is nil:
        transplant(N, N->left); // call transplant on N and it's left node, derement num_pairs
        num_pairs--;
    }
    
    else {
        Node* y = findMin(N->right); // make a temp node* y to find the minimum leaf node on the right node of N:
        
        if (y->parent != N) { // if the parent node of y is not the same as N:
            transplant(y, y->right); // call transplant on y and it's right node
            y->right = N->right; // set the right node of y to the right node of N
            y->right->parent = y; // set the parent node of the right node of y to y, from TA pseudo code, he said this part is confusing and he helped me with explaining it further
        }
        
        else {
            transplant(N, y); // call transplant on N and y
            y->left = N->left; // set the left node of y to the left node of N
            y->left->parent = y; // set the parent node of the left node of y to y
        }

        num_pairs--; // decrement num_pairs
    }
}

void Dictionary::begin() {
    current = findMin(root); // set the current node to the minimum leaf node from the root
}

void Dictionary::end() {
    current = findMax(root); // set the current node to the maximum leaf node from the root
}

void Dictionary::next() {
    if (hasCurrent() == false) { // if hasCurrent is false:
        throw std::logic_error("Dictionary: next(): current not defined");
    }

    current = findNext(current); // set the current node to the next paired leaf node from the current
}

void Dictionary::prev() {
    if (hasCurrent() == false) { // if hasCurrent is false:
        throw std::logic_error("Dictionary: next(): current not defined");
    }

    current = findPrev(current); // set the current node to the previous paired leaf node from the current
}

std::string Dictionary::to_string() const {
    std::string s;
    inOrderString(s, root); // perform an in order operation from the root and append to "s"
    return s;
}

std::string Dictionary::pre_string() const {
    std::string s;
    preOrderString(s, root); // perform an pre order operation from the root and append to "s"
    return s;
}

bool Dictionary::equals(const Dictionary& D) const { // return num_pairs is the same as num_pairs of D and the strings of both Dictionaries
    if (num_pairs != D.num_pairs) { // check num parts
        return false;
    }

    // check strings
    std::string currentDict = to_string();
    std::string DictD = D.to_string();

    if (currentDict == DictD) {
        return true;
    }

    else {
        return false;
    }
}
    
std::ostream& operator<<(std::ostream& stream, Dictionary& D) {
    stream << D.to_string();
    return stream;
}

bool operator==(const Dictionary& A, const Dictionary& B) {
    return A.equals(B);
}

Dictionary& Dictionary::operator=(const Dictionary& D) {
    if (this == &D) { // if they are the same, return instantly
        return *this;
    }

    clear();  // clear the current dictionary
    preOrderCopy(D.root, D.nil);  // copy the pairs of D into this Dictionary

    return *this;
}
