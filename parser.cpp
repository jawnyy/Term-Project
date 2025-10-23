#include <iostream>
#include <fstream>
#include <cctype>
#include <map>
using namespace std;

// Declare functions
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

// Symbol Info Structure
struct SymbolInfo {
    string name;
    int type;
};

// Symbol table
map<int, SymbolInfo> symbolTable;

// Semantic error tracking
//vector<string> semanticErrors;

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define COMMA 2
#define COLON 3
#define SEMI 4
#define EQUAL 5
#define UNKNOWN 99

/* Token codes */
#define PROGRAM 10
#define DECL_SEC 11
#define DECL 12
#define ID_LIST 13
#define ID 14
#define OPERAND 15

#define STMT_SEC 20
#define STMT 21

#define IFSTMT 30
#define WHILESTMT 31
#define INPUT 32
#define OUTPUT 33

#define ASSIGN 40
#define EXPR 41
#define FACTOR 42 
#define COMP 43

#define NUM 50
#define TYPE 51

int j = 0;

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
        cout << "Error opening file!" << endl;
    } else {
        getChar();
        do {
            lex();
        } while (nextToken != EOF);

        inputFile.close();
    }

    for (const auto& pair : symbolTable) {
        cout << pair.first << ": {" << pair.second.name << ", "<< pair.second.type << "}" << endl;
    }

    return 0;
}

int lookup(char ch) {
    switch (ch) {
        case ',':
            addChar();
            nextToken = COMMA;
            break;
        case ':':
            addChar();
            nextToken = COLON;
            break;
        case ';':
            addChar();
            nextToken = SEMI;
            break;
        case '=':
            addChar();
            nextToken = EQUAL;
            break;
        case '(':
        case ')':
            addChar();
            nextToken = OPERAND;
            break;
        case '<':
        case '>':
            addChar();
            nextToken = COMP;
            break;
        case '+':
        case '-':
            addChar();
            nextToken = EXPR;
            break;
        case '*':
        case '/':
            addChar();
            nextToken = FACTOR;
            break;
        default:
            addChar();
            nextToken = EOF;
            break;
    }
    return nextToken;
}
// program, begin, end, if, then, else, input, output, int, while, loop.
int id_lookup(string word) {
    if (word == "program") {
        nextToken = PROGRAM;
    }
    else if (word == "input") {
        nextToken = INPUT;
    } 
    else if (word == "output") {
        nextToken = OUTPUT;
    }
    else if (word == "int" || word == "float" || word == "double"){
        nextToken = TYPE;
    }
    else if (word == "begin" || word == "end" || word == "if" || word == "then"
    || word == "else" || word == "while" || word == "loop") {
        nextToken = ID_LIST;
    } else {
        nextToken = ID;
    }
    return nextToken;
}

/*****************************************************/
/* addChar - a function to add nextChar to lexeme */
void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    }
    else
        cout << "Error - lexeme is too long \n" << endl;
}

/*****************************************************/
/* getChar - a function to get the next character of
   input and determine its character class */
void getChar() {
    nextChar = inputFile.get();
    if (inputFile.good()) {
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else 
            charClass = UNKNOWN;
    }
    else
        charClass = EOF;
}

/*****************************************************/
/* getNonBlank - a function to call getChar until it
   returns a non-whitespace character */
void getNonBlank() {
    while (isspace(nextChar))
        getChar();
}

/*****************************************************/
/* lex - a simple lexical analyzer for arithmetic
   expressions */
int lex() {
    lexLen = 0;
    getNonBlank();
    switch (charClass) {
        /* Parse identifiers */
        case LETTER:
            addChar();
            getChar();
            while (charClass == LETTER || charClass == DIGIT) {
                addChar();
                getChar();
            }
            id_lookup(string(lexeme));
            //nextToken = IDENT;
            break;
        /* Parse integer literals */
        case DIGIT:
            addChar();
            getChar();
            while (charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = NUM;
            break;
        /* Parentheses and operators */
        case UNKNOWN:
            lookup(nextChar);
            getChar();
            break;
        /* EOF */
        case EOF:
            nextToken = EOF;
            lexeme[0] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = 0;
            break;
    } /* End of switch */
    // Book way of showing the lexeme and its value
    // cout << "Next token is: " << nextToken << ", Next lexeme is " << lexeme << endl;
    
    symbolTable[j] =  {lexeme, nextToken};
    j++;
    return nextToken;
} /* End of function lex */

