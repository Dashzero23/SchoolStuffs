/********************************************************************************* 
* Nguyen Vu, npvu
* 2023 Spring CSE101 PA6
* BigInteger.cpp
* Implementation of BigInteger ADT
*********************************************************************************/
// I converted fprintf and return exit failure to throw error like the pdf said compared to how I did things before
// improved to get number of BASE digit instead of 1
// added in helper functions since I couldn't get the arithmetic functions to work
// rewatched lecture many times and implement functions closely based on lectures
#include "BigInteger.h"
#include <stdexcept>
#include <cctype> // used for checking digit
#include <string> // to check length

#define POWER 9
#define BASE 1000000000

BigInteger::BigInteger() { // zero state
    signum = 0;
    digits = List();
}

BigInteger::BigInteger(long x) { 
    if (x == 0) {
        signum = 0;
        //printf("Constructor: Signum = 0\n");
        return; // no need to insert no number
    }

    if (x > 0) {
        signum = 1;
    }

    else {
        signum = -1;
        x = -x; // since sign is defined by signum, making x positive make the loop to insert data a lot easier
    }
    
    //printf("Constructor: Main loop\n");
    while (x > 0) {
        int lastDigit = x % BASE; // get the last digit
        digits.insertBefore(lastDigit);
        x /= BASE; // move to next 10th in the number
    }

    // pad the first element if it doesn't have enough digits
    int frontDigits = digits.front();
    int numZero = POWER - std::to_string(frontDigits).length();
    
    for (int i = 0; i < numZero; ++i) {
        frontDigits *= 10;
    }
    
    digits.moveFront();
    digits.eraseAfter();
    digits.insertBefore(frontDigits);
}

BigInteger::BigInteger(std::string s) { // fixed to get the proper list
    if (s.empty()) {
        throw std::invalid_argument("BigInteger: Constructor: empty string");
    }

    int sign = 0;
    int index = 0;

    if (s[0] == '+' || s[0] == '-') {
        sign = 1;
        index = 1;
    }

    if (sign == 1) {
        if (s[0] == '+') {
            signum = 1;
        }
        
        else {
            signum = -1;
        }
    }
    
    else {
        signum = 1;
    }

    int removedSignLength = s.length() - index;
    std::string digitsString = s.substr(index, removedSignLength);

    if (removedSignLength % POWER != 0) {
        int paddingZeros = POWER - (removedSignLength % POWER);
        digitsString = std::string(paddingZeros, '0') + digitsString;
    }

    for (size_t i = 0; i < digitsString.length(); i += POWER) { // fixed the loop so it works properly
        std::string group = digitsString.substr(i, POWER);
        int element = std::stoi(group);
        digits.insertBefore(element);
    }
}

BigInteger::BigInteger(const BigInteger& N) { // copy all data
    signum = N.signum;
    digits = N.digits;
}

int BigInteger::sign() const {
    return signum;
}

int BigInteger::compare(const BigInteger& N) const { // improved to get proper output
    // compare the signs
    if (signum < N.signum) {
        return -1; // less than N
    } 
    
    else if (signum > N.signum) {
        return 1; // greater than N
    } 
    
    else { // same sign, compare the digits
        if (signum == 1) { // compare positive length
            if (digits.length() < N.digits.length()) {
                return -1;
            }
            
            else if (digits.length() > N.digits.length()) {
                return 1;
            }
        }
        
        else { // compare negative length
            if (digits.length() < N.digits.length()) { 
                return 1;
            }
            
            else if (digits.length() > N.digits.length()) {
                return -1;
            }
        }

        // this part onward is greatly inspired by equals() from pa5
        /*Node* nodeA = frontDummy->next;
        Node* nodeB = R.frontDummy->next;

        while (nodeA != backDummy && nodeB != R.backDummy) { // since we have the backDummy, a while loop is faster to write
            if (nodeA->data != nodeB->data) {
                return false;
            }

            nodeA = nodeA->next;
            nodeB = nodeB->next;
        }*/
        
        // if equal length and sign, do digit check
        List A = digits;
        List B = N.digits;
        A.moveFront();
        B.moveFront();

        while (A.position() < A.length()) {
            if (A.peekNext() < B.peekNext()) {
                return -signum; // less than N
            }
            
            else if (A.peekNext() > B.peekNext()) {
                return signum; // greater than N
            }
            
            A.moveNext();
            B.moveNext();
        }
    }

    return 0; // if hasn't returned, they are equal
}

void BigInteger::makeZero() {
    signum = 0;
    digits.clear();
}

void BigInteger::negate() {
    signum *= -1;
}

void negateList(List& L) { // reverse sign of all element, similar to negate
    L.moveFront();
    
    while (L.position() < L.length()) {
        size_t newNode = L.peekNext() * -1;
        L.setAfter(newNode);
        L.moveNext();
    }
}

void sumList(List& S, List A, List B) { // based on old function add, fixed to get correct output
    /*int carry = 0;
    
    while (A.length() > 0 || B.length() > 0 || carry != 0) {
        int digitSum = carry;
        
        if (A.length() > 0) {
            digitSum += A.back();
            A.eraseBefore();
        }
        
        if (B.length() > 0) {
            digitSum += B.back();
            B.eraseBefore();
        }
        
        carry = digitSum / BASE;
        digitSum %= BASE;
        
        sum.insertBefore(digitSum);
    }*/

    S.clear();
    A.moveBack();
    B.moveBack();
    
    while (A.position() > 0 || B.position() > 0) { // doesn't need to worry about carry, normalizeList will take care of that
        long sum = 0;
    
        if (A.position() > 0) {
            sum += A.peekPrev();
            A.movePrev();
        }
    
        if (B.position() > 0) {
            sum += B.peekPrev();
            B.movePrev();
        }
        
        S.insertAfter(sum);
    }
    
    // remove 0 element at the front if needed in case of subtraction
    S.moveFront();
    
    while (S.front() == 0 && S.length() > 1) {
        S.eraseAfter();
    }
}

int normalizeList(List& L) { // lecture is so helpful for this function
    if (L.front() == 0) {
        return 0;
    }
    
    int sign = 1;
    
    if (L.front() < 0) {
        sign = -1;
    }

    L.moveBack(); // start from the back (least significant digit)
    int carry = 0;
    
    while (L.position() > 0 || carry != 0) {
        if (L.position() == 0) { // if reach position 0 and have carry
            L.insertBefore(0);
        }
        
        else {
            long currElement = L.peekPrev() + carry;
            carry = currElement / BASE;
            
            if (currElement >= BASE) { // element is too big
                L.setBefore(currElement % BASE);
            }
        
            else if (L.peekPrev() < 0 && sign == 1) { // negative element in a positive list
                L.setBefore(currElement + BASE);
                carry = -1; // if doesn't change to -1, it cause some calculation to be off by a single digit
            }
        
            else if (L.peekPrev() > 0 && sign == -1) { // positive element in a negative list
                L.setBefore(BASE - currElement);
            } 
        
            else { // element sign and list sign are the same
                L.setBefore(abs(currElement % BASE)); // use absolute to remove negative sign in the middle of the number
            }
        
            L.movePrev();
        }
    }
    
    return sign;
}

void shiftList(List& L, int num) { // shift list to the left num time
    L.moveBack();
    
    for (int i = 0; i < num; i++) {
        L.insertAfter(0);
    }
}

void scalarMultList(List& L, ListElement num) { // mult all element by num
    L.moveFront();
    
    while (L.position() < L.length()) {
        L.setAfter(L.peekNext() * num);
        L.moveNext();
    }
}

BigInteger BigInteger::add(const BigInteger& N) const {
    BigInteger Sum;
    List A = digits;
    List B = N.digits;
    
    // this was before the helper function, was missing a lot of things
    /*if (signum < 0) {
        A.moveFront();
        A.setBefore(A.front() * -1);
    }
    
    if (N.signum < 0) {
        B.moveFront();
        B.setBefore(B.front() * -1);
    }*/
    
    if (signum < 0) {
        negateList(A);
    }
    
    if (N.signum < 0) {
        negateList(B);
    }
    
    // before helper function, didn't work properly
    /*BigInteger sum;
    int carry = 0;
    
    while (A.length() > 0 || B.length() > 0 || carry != 0) {
        int digitSum = carry;
        
        if (A.length() > 0) {
            digitSum += A.back();
            A.eraseBefore();
        }
        
        if (B.length() > 0) {
            digitSum += B.back();
            B.eraseBefore();
        }
        
        carry = digitSum / BASE;
        digitSum %= BASE;
        
        sum.insertBefore(digitSum);
    }*/
    
    sumList(Sum.digits, A, B);

    Sum.signum = normalizeList(Sum.digits);

    return Sum;
}

BigInteger BigInteger::sub(const BigInteger& N) const { // same concept as Matrix difference from pa4
    BigInteger negatedN = N;
    negatedN.negate();
    BigInteger Difference = add(negatedN);
    
    return Difference;
}

BigInteger BigInteger::mult(const BigInteger& N) const {
    BigInteger Product;
    
    if (signum == 0 || N.signum == 0) {
        Product = BigInteger("0");
        Product.signum = 0;
        return Product;
    }
    
    List product;
    List A = digits;
    List B = N.digits;
    B.moveBack();
    int shift = 0;
    int sign = 0;
    
    while (B.position() > 0) {
        ListElement factor = B.peekPrev();

        if (factor != 0) {
            List tempProduct = A; // product of A with current element in list of B
            scalarMultList(tempProduct, factor);
            sign = normalizeList(tempProduct);
            shiftList(tempProduct, shift);
            sumList(product, product, tempProduct);
            sign = normalizeList(product);
        }

        B.movePrev();
        shift++;
    }
    
    if (signum == 1 && N.signum == 1) {
        Product.signum = sign;
    }
    
    else if (signum == -1 && N.signum == -1) {
        Product.signum = sign;
    }
    
    else {
        Product.signum = -sign;
    }

    Product.digits = product;
    
    return Product;
}

std::string BigInteger::to_string() { // major helped from the TA to remove the leading zeros and get proper output
    std::string out = "";

    if (signum == 0) {
        return out += '0';
    } 
    
    else if (signum == -1) {
        out += '-';
    }

    digits.moveFront();
    
    while (digits.front() == 0 && digits.length() > 1) { // remove leading zeros
        digits.eraseAfter();
    }
    
    while (digits.position() < digits.length()) {
        std::string digitString = std::to_string(digits.peekNext());

        if (digits.position() != 0 && digitString.length() < POWER) { // pad zeros in case the the element doesn't have POWER digits
            std::string paddingZeros(POWER - digitString.length(), '0');
            out += paddingZeros;
        }

        out += digitString;
        digits.moveNext();
    }

    return out;
}

std::ostream& operator<<(std::ostream& stream, BigInteger N) {
    return stream << N.BigInteger::to_string();
}

bool operator==(const BigInteger& A, const BigInteger& B) {
    if (A.compare(B) == 0) {
        return true;
    }
    
    return false;
}

bool operator<(const BigInteger& A, const BigInteger& B) {
    if (A.compare(B) == -1) {
        return true;
    }
    
    return false;
}

bool operator<=(const BigInteger& A, const BigInteger& B) {
    if (A.compare(B) <= 0) {
        return true;
    }
    
    return false;
}

bool operator>(const BigInteger& A, const BigInteger& B) {
    if (A.compare(B) == 1) {
        return true;
    }
    
    return false;
}

bool operator>=(const BigInteger& A, const BigInteger& B) {
    if (A.compare(B) >= 0) {
        return true;
    }
    
    return false;
}

BigInteger operator+(const BigInteger& A, const BigInteger& B) {
    return A.BigInteger::add(B);
}

BigInteger operator+=(BigInteger& A, const BigInteger& B) {
    A = A.BigInteger::add(B);
    return A;
}

BigInteger operator-(const BigInteger& A, const BigInteger& B) {
    return A.BigInteger::sub(B);
}

BigInteger operator-=(BigInteger& A, const BigInteger& B) {
    A = A.BigInteger::sub(B);
    return A;
}

BigInteger operator*(const BigInteger& A, const BigInteger& B) {
    return A.BigInteger::mult(B);
}

BigInteger operator*=(BigInteger& A, const BigInteger& B) {
    A = A.BigInteger::mult(B);
    return A;
}
