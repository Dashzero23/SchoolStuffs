/********************************************************************************* 
* Nguyen Vu, npvu
* 2023 Spring CSE101 PA8
* Dictionary.cpp
* Implementation of Dictionary ADT
*********************************************************************************/
// This is mostly copied from the TA given pseudo code
// modified the getting input into dictionary

#include<algorithm>
#include<iostream>
#include<fstream>
#include<string>
#include"Dictionary.h"

using namespace std;


int main(int argc, char* argv[]) {
    Dictionary D;
    ifstream in;
    ofstream out;
    string line;
    string token;
    string delim = " \t\\\"\',<.>/?;:[{]}|`~!@#$%^&*()-_=+0123456789";
    size_t begin, end, len;

    // check command line for correct number of arguments
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <input file> <output file>" << endl;
        return(EXIT_FAILURE);
    }

    // open files for reading and writing
    in.open(argv[1]);
    if (!in.is_open()) {
        cerr << "Unable to open file " << argv[1] << " for reading" << endl;
        return(EXIT_FAILURE);
    }

    out.open(argv[2]);
    if (!out.is_open()) {
        cerr << "Unable to open file " << argv[2] << " for writing" << endl;
        return(EXIT_FAILURE);
    }

    // read each line of input file in a while loop
    while (getline(in, line)) {
        len = line.length();
        begin = min(line.find_first_not_of(delim, 0), len);
        end = min(line.find_first_of(delim, begin), len);
        token = line.substr(begin, end - begin);

        while (token != "") {
            // change word to all lowercase
            transform(token.begin(), token.end(), token.begin(), ::tolower);

            if (D.contains(token)) { // increment word count if it's already there
                int count = D.getValue(token) + 1;
                D.setValue(token, count);
            }
        
            else { // else add it to the dictionary
                D.setValue(token, 1);
            }

            // get the next token
            begin = min(line.find_first_not_of(delim, end + 1), len);
            end = min(line.find_first_of(delim, begin), len);
            token = line.substr(begin, end - begin);
        }
    }

    // I thought the outfile in the example is word frequency, not order
    /*int line_num = 1;

    while(getline(in, line)){ //iterate reading in each "line" from the input file:
        D.setValue(line, line_num); // in dictionary "D", set each "line number" as the value for each "line" as the key
        line_num++; // increment the line number you are on for the next set of keys and values
    }*/

    // print Dictionary to output file
    out << D.to_string() << endl;

    // close files
    in.close();
    out.close();
    D.clear();

    return 1;
}


