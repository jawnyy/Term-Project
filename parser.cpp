#include <iostream>
#include <fstream>
#include <cctype>
using namespace std;

int charClass;
char lexeme[100];
char nextChar;
int lexLen;
int token;
int nextToken;
ifstream inputFile;

/* Function declarations */
void addChar();
void getChar();
void getNonBlank();
int lex();

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/* Token codes */
#define INT_LIT 10
#define IDENT 11

#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26

int main() {
    // Uncomment this later, for easier testing purposes.
    /*
    string fileName;
    cout << "Enter file name: ";
    cin >> fileName;
    ifstream inputFile(fileName);
    */
    inputFile.open("input1.txt");
    int i = 1;

    if (!inputFile) {
        cerr << "Error opening file!" << endl;
    } else {
        char ch;
        while (inputFile.get(ch)) {
            if (ch == '\n') {
                cout << "New line at line " << i << endl;
                i++;
            }
            cout << ch << " at line " << i << endl;
        }

        // getChar();
        // do {
        //     lex();
        // } while (nextToken != EOF);
        inputFile.close();
    }

    // if (inputFile.is_open()) {
    //     cout << "Opened file" << endl;
    //     string line;
    //     int i = 1;
    //     while (getline(inputFile, line)) {
    //         cout << line << "--------line #" << i << endl;
    //         i++;
    //     }
    //     inputFile.close();
    // } else {
    //     cerr << "Error opening file!";
    //     // error
    // }

    return 0;
}