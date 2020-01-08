//
//  main.cpp
//  Word Search Project
//
//  Created by Logan Trevorrow on 1/7/20.
//  Copyright © 2020 Logan Trevorrow. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

// this will be used in the output of our search for our word
// organizatino of int, int, int, int, bool minimizes space required for struct
struct Output {
    int rowS;
    int columnS;
    int rowE;
    int columnE;
    bool isThere;
};

struct Pointer {
    int row;
    int col;
}; // Pointer

Output evaluateNeighbors(const vector< vector<char> > &grid, const string &word, Pointer &pointer, const int &width, const int &height);

Output conductSearch(const vector< vector<char> > &grid, const int &width, const int &height, const string &word);

int main(int argc, const char * argv[]) {
    
    
    /* - The command line argument will be in the format of:
            command to run program | name of input file | width | height
     
       - The goal is to have an input file that is a .txt file that contains a grid of
         letters
       - The user then can input a word to the computer in the terminal and the program will output
         if the word exists in the grid of letters and the start index and end index of the word
       - To end the program, the user will have to type "!!!"
    */
    
    // read in the information from the command line, initialize variables, and open filestream
    int width = atoi(argv[2]);
    int height = atoi(argv[3]);
    string filename = argv[1];
    
    ifstream inFile;
    inFile.open(filename);
    if (!inFile.is_open()) {
        cout << "File was unable to be opened\n";
        exit(1);
    } // if
    
    vector< vector<char> > grid(height, vector<char> (width, 'a'));
    
    
    // read in the lines of the file
    string line;
    int row = 0;
    int col = 0;
    while (getline(inFile, line)) {
        
        for (size_t i = 0; i < line.size(); ++i) {
            
            grid[row][col] = tolower(line[i]);
            col++;
        } // for
        
        row++;
        col = 0;
    } // while
    
    
    // create loop of input
    cout << "Welcome to Logan's Word Searcher!\n";
    cout << "I will soon prompt you for a word to search the input grid for.\n";
    cout << "In order to end the program, please input '!!!' for the word.\n";
    cout << "Happy Searching!\n";
    
    while (true) {
        
        cout << "What word would you like to search for?\n";
        
        string input;
        cin >> input;
        transform(input.begin(), input.end(), input.begin(), ::tolower);
        
        if (input == "!!!") {
            cout << "Ending Program\n";
            exit(0);
        } // if
        
        else if (input.size() == 0 || input.size() == 1) {
            cout << "Word is too small, words to search for must be greater than 1 character\n";
        } // else if
        
        else {
            Output search;
            search = conductSearch(grid, width, height, input);
            
            if (search.isThere) {
                cout << input << " starts at [" << search.rowS << ", " << search.columnS << "] and ends at [" << search.rowE << ", " << search.columnE << "]\n";
            } // if
            else {
                cout << "Word was not found in the grid\n";
            } // else
        } // else
    } // while
    
    
    return 0;
} // main

Output conductSearch(const vector< vector<char> > &grid, const int &width, const int &height, const string &word) {
    
    Pointer pointer;
    
    // iterate through every row
    for (int row = 0; row < height; ++row) {
        
        // iterate through every column
        for (int col = 0; col < width; ++col) {
            
            // if the beginning letter of the word is equal to the current location in the grid
            if (word[0] == grid[row][col]) {
                
                pointer.row = row;
                pointer.col = col;
                
                // evaluate neighbors and if the correct neighbor is there, then keep going
                Output temp = evaluateNeighbors(grid, word, pointer, width, height);
                
                // if the word is there then return it, else continue to the next location
                if (temp.isThere) {
                    return temp;
                } // if
            } // if
            
        } // for
    } // for
    
    Output dummy;
    dummy.isThere = false;
    return dummy;
} // conductSearch

Output evaluateNeighbors(const vector< vector<char> > &grid, const string &word, Pointer &pointer, const int &width, const int &height) {
    
    int size = 1;
    Output found;
    found.rowS = pointer.row;
    found.columnS = pointer.col;
    
    while (true && size < word.size()) {
        
        if (((pointer.col + 1) < width) &&
            grid.at(pointer.row).at(pointer.col + 1) == word[size]) {
            size += 1;
            pointer.col++;
        } // if
        else if (((pointer.col - 1) > 0) &&
                 grid.at(pointer.row).at(pointer.col - 1) == word[size]) {
            size += 1;
            pointer.col--;
        } // if
        else if (((pointer.row + 1) < height) &&
                 grid.at(pointer.row + 1).at(pointer.col) == word[size]) {
            size += 1;
            pointer.row++;
        } // if
        else if ((pointer.row - 1) > 0 &&
                 ((pointer.col + 1) < width) &&
                 grid.at(pointer.row - 1).at(pointer.col + 1) == word[size]) {
            size += 1;
            pointer.row--;
        } // if
        else if (((pointer.col + 1) < width) &&
                 ((pointer.row + 1) < height) &&
                 grid.at(pointer.row + 1).at(pointer.col + 1) == word[size]) {
            size += 1;
            pointer.row++;
            pointer.col++;
        } // if
        else if ((pointer.col - 1 > 0) &&
                 ((pointer.row + 1) < height) &&
                 grid.at(pointer.row + 1).at(pointer.col - 1) == word[size]) {
            size += 1;
            pointer.col--;
            pointer.row++;
        } // if
        else if ((pointer.col - 1) > 0 &&
                 ((pointer.row - 1) > 0) &&
                 grid.at(pointer.row - 1).at(pointer.col - 1) == word[size]) {
            size += 1;
            pointer.col--;
            pointer.row--;
        } // if
        else if (((pointer.col + 1) < width) &&
                 ((pointer.row - 1) > 0) &&
                 grid.at(pointer.row - 1).at(pointer.col + 1) == word[size]) {
            size += 1;
            pointer.col++;
            pointer.row--;
        } // if
        else {
            found.isThere = false;
            return found;
        } // else
    } // while
    
    found.isThere = true;
    found.columnE = pointer.col;
    found.rowE = pointer.row;
    return found;
} // conductSearch
