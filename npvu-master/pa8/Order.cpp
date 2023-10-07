/********************************************************************************* 
* Nguyen Vu, npvu
* 2023 Spring CSE101 PA7
* Order.cpp
* Main program
*********************************************************************************/
// input/output was copied from FileIO, then followed pseudo code from the same TA for Dictionary.cpp

#include "Dictionary.h"
#include <fstream>

int main(int argc, char * argv[]){
    Dictionary D; // make a dictionary "D"
    std::ifstream in;
    std::ofstream out;
    
    // check command line for correct number of arguments
    if( argc != 3 ){
       std::cerr << "Usage: " << argv[0] << " <input file> <output file>" << std::endl;
       return(EXIT_FAILURE);
    }

    // open files for reading and writing 
    in.open(argv[1]);
    if( !in.is_open() ){
       std::cerr << "Unable to open file " << argv[1] << " for reading" << std::endl;
       return(EXIT_FAILURE);
    }

    out.open(argv[2]);
    if( !out.is_open() ){
       std::cerr << "Unable to open file " << argv[2] << " for writing" << std::endl;
       return(EXIT_FAILURE);
    }
    
    std::string line;
    int line_num = 1;

    while(getline(in, line)){ //iterate reading in each "line" from the input file:
        D.setValue(line, line_num); // in dictionary "D", set each "line number" as the value for each "line" as the key
        line_num++; // increment the line number you are on for the next set of keys and values
    }

    out << D.to_string() << std::endl; // print out the dictionary to the outfile
    out << D.pre_string(); // print out the pre-order traversal of the tree to the outfile
    
    // close files 
    in.close();
    out.close();
    D.clear();

    return 1;
}
