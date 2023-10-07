/********************************************************************************* 
* Nguyen Vu, npvu
* 2023 Spring CSE101 PA5
* Shuffle.cpp
* Main program
*********************************************************************************/
// done with lots of help from TA
#include "List.h"

void shuffle(List& D) {
    //int loop = 0;
    //std::cout << "Make list" << std::endl;
    List leftHalf, rightHalf;
    //std::cout << "Make list" << std::endl;
    //List oldList = List(D);
    int n = D.length();
    bool odd = false;
    
    if ((n % 2) != 0) { // in case length is odd, reduce it by 1 so the shuffle doesn't add the middle element twice
        odd = true;
        n--;
    }
    
    // shuffle element from first half and second half
    // I tried to do the greedy way but it keeps breaking
    /*for (int i = 0; i < (n / 2); i++) {
        loop++;
        std::cout << "Loop number: " << i << std::endl;
        shuffled.insertBefore((n / 2) + i); // append elements from second half (n/2, n/2 +1, ...)
        shuffled.insertBefore(i); // append elements from first half (0, 1, 2, ...)
        
    } 
    
    D = shuffled;*/
    
    // this is TA's idea to make 2 list and insert accordingly, then add them together in correct position
    D.moveFront();
    int halfLength = n / 2;
    
    while (D.position() < D.length()) {
        if (D.position() < halfLength) {
            leftHalf.insertBefore(D.moveNext());
        }
        
        else {
            rightHalf.insertBefore(D.moveNext());
        }
    }
    
    D.clear(); // clear the list to re-add the shuffled version
    leftHalf.moveFront();
    rightHalf.moveFront();
    
    for (int i = 0; i < halfLength; i++) { // shuffle element from first half and second half
        D.insertBefore(rightHalf.moveNext()); // append elements from second half (n/2, n/2 +1, ...)
        D.insertBefore(leftHalf.moveNext()); // append elements from first half (0, 1, 2, ...)
    }
    
    // this part is giving me infinite loop
    /*if (odd) { // if n is odd, add the last element
        D.insertBefore(oldList.back());
    }*/
    
    if (odd) { // TA's idea to use peekNext
        D.insertBefore(rightHalf.peekNext());
    }
}

int main(int argc, char * argv[]){
    // in/out file was copied/pasted from the given pseudo code for FileIO
    // I realized we are not using input/output file, just take data and print to the terminal but I will use similar concept for the input
    /*ifstream in;
    ofstream out;
    
    // check command line for correct number of arguments
    if( argc != 3 ){
       cerr << "Usage: " << argv[0] << " <input file> <output file>" << endl;
       return(EXIT_FAILURE);
    }

    // open files for reading and writing 
    in.open(argv[1]);
    if( !in.is_open() ){
       cerr << "Unable to open file " << argv[1] << " for reading" << endl;
       return(EXIT_FAILURE);
    }

    out.open(argv[2]);
    if( !out.is_open() ){
       cerr << "Unable to open file " << argv[2] << " for writing" << endl;
       return(EXIT_FAILURE);
    }*/
    
    // check command line for correct number of arguments
    if( argc != 2 ){
       std::cerr << "Usage: " << argv[0] << " <number of cards>" << std::endl;
       return(EXIT_FAILURE);
    }
    
    int maxCards = std::stoi(argv[1]); // this was from TA
    
    // print the header
    std::cout << "deck size\tshuffle count\n"; // \t was from TA
    std::cout << "------------------------------\n";
    std::cout << "1\t\t 1" << std::endl;
    List unShuffled; // make new list for current deck
    List curDeck;

    
    for (int deckSize = 2; deckSize <= maxCards; deckSize++) { // go through all number of deck size
        // add elements until it reaches deckSize-1
        for (int i = 0; i < deckSize; i++) {
            curDeck.insertBefore(i);
        }

        // shuffle and count
        unShuffled = List(curDeck);
        int count = 1;
        shuffle(curDeck);
        
	while (!(curDeck == unShuffled)) {
            shuffle(curDeck);
            count++;
        }
        
        std::cout << deckSize << " \t\t " << count << "\n";
        curDeck.clear();
        unShuffled.clear();
    }

    return 1;
}
    
