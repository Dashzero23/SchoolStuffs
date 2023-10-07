/********************************************************************************* 
* Nguyen Vu, npvu
* 2023 Spring CSE101 PA6
* Arithmetic.cpp
* Main program
*********************************************************************************/
//input/output part was copied/pasted from the given example FileIO
#include "BigInteger.h"
#include <fstream>

int main(int argc, char *argv[]) {
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

    // read in strings
    std::string StringA, StringB, blank;
    getline(in, StringA);
    getline(in, blank);
    getline(in, StringB);

    BigInteger A = BigInteger(StringA);
    BigInteger B = BigInteger(StringB);
    
    // A
    out << A << std::endl << std::endl;
    
    // B
    out << B << std::endl << std::endl;
    
    // A + B
    out << A + B << std::endl << std::endl;
    
    // A - B
    out << A - B << std::endl << std::endl;
    
    // A - A
    out << A - A << std::endl << std::endl;
    
    // 3A - 2B
    BigInteger C("3");
    BigInteger D("2");
    
    out << (C * A) - (D * B) << std::endl << std::endl;
    
    // AB
    out << A * B << std::endl << std::endl;    
    
    // A^2
    out << A * A << std::endl << std::endl;
    
    // B^2
    out << B * B << std::endl << std::endl;
    
    // 9A^4 + 16B^5
    BigInteger E("9");
    BigInteger F("16");
    
    out << (E * A * A * A * A) + (F * B * B * B * B * B) << std::endl << std::endl;

    // close files
    in.close();
    out.close();
    
    return 1;
}
