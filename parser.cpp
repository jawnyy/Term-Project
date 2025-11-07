#include <iostream>
#include <fstream>
#include <cctype>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
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
void PROGRAM01();
void DECL_SEC02();
void DECL03();
void ID_LIST04();
void ID05();
void STMT_SEC06();
void STMT07();
void ASSIGN08();
void IFSTMT09();
void WHILESTMT10();
void INPUT11();
void OUTPUT12();
void EXPR13();
void FACTOR14();
void OPERAND15();
void NUM16();
void COMP17();
void TYPE18();
void check_semantics(int x);

// Symbol Info Structure
struct SymbolInfo {
    string name;
    int type;
    int lineNumber;
};

// Symbol table
map<int, SymbolInfo> symbolTable;
std::map<int, SymbolInfo>::iterator point;

// Semantic error tracking
vector<string> semanticErrors;

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
int countLineNumber = 0;

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
        cout << pair.first << ": {" << pair.second.name << ", "<< pair.second.type << ", " << pair.second.lineNumber << "}" << endl;
        //check_semantics(pair.second.type);
    }

    cout << "\nStart of the output we should see \n\n";

    PROGRAM01();

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
    
    symbolTable[j] =  {lexeme, nextToken, countLineNumber};
    j++;
    return nextToken;
} /* End of function lex */

/* Start of semantics portion */

void NextLex() {
    symbolTable.erase(point);
    point = symbolTable.begin();
}

void CheckReservedWord() {
    if (point->second.name == " program" || point->second.name == "begin" || point->second.name == "end" || 
        point->second.name == "if" || point->second.name == "then" || point->second.name == "else" || 
        point->second.name == "input" || point->second.name == "output" || point->second.name == "int" || 
        point->second.name == "while" || point->second.name == "loop") {
            cout << "Error, invalid identifier name!" << endl;
            exit(1);
    }
}

void CheckType() {

}

void PROGRAM01() {
    if (!symbolTable.empty()) {
        // Get the first element
        point = symbolTable.begin();
        // If the program starts with "program"
        if (point->second.type == PROGRAM) {
            cout << "PROGRAM" << endl;
            // Delete the first element, get next
            NextLex(); 
            if (point->second.name == "begin") {
                // Go to STMT_SEC
                STMT_SEC06();
            } else {
                // DECL_SEC follows
                DECL_SEC02();
            }
        } else {
            cout << "Incorrect start of program!" << endl;
        }
    }
}

void DECL_SEC02() {
    vector<string> idList;
    bool colonCheck = true;

    cout << "DECL_SEC" << endl;
    while (point->second.name != ";") {
        while (point->second.name != ":" && colonCheck) {
            CheckReservedWord();
            idList.push_back(point->second.name);
            NextLex();
        }
        colonCheck = false;
        idList.push_back(point->second.name);
        NextLex();
    }
    cout << "DECL" << endl;
    if (idList.back() != "int" && idList.back() != "double" && idList.back() != "float") {
        cout << "Error, wrong type declared!" << endl;
        exit(1);
    }
    
    NextLex();
    
    // count the number of commas+1 to display ID_LIST amount of times
    int countCommas = count(idList.begin(), idList.end(), ",") + 1;
    while (countCommas > 0) {
        cout << "ID_LIST" << endl;
        countCommas--;
    }

    // Checks for more declaration sections.
    if (point->second.name == "begin") {
        STMT_SEC06();
    } else {
        DECL_SEC02();
    }

    // can also skip "begin" here
}

void DECL03() {
    cout << point->second.name << endl;
}

void ID_LIST04() {
    cout << point->second.name << endl;
}

void ID05() {
    cout << point->second.name << endl;
}

void STMT_SEC06()  {
    NextLex();
    cout << point->second.name << endl;
    // TODO: Goes to input here, going to have to separate ID_LIST.
}

void STMT07() {
    cout << point->second.name << endl;
}

void ASSIGN08() {
    cout << point->second.name << endl;
}

void IFSTMT09() {
    cout << point->second.name << endl;
}

void WHILESTMT10() {
    cout << point->second.name << endl;
}

void INPUT11() {
    cout << point->second.name << endl;
}

void OUTPUT12() {
    cout << point->second.name << endl;
}

void EXPR13() {
    cout << point->second.name << endl;
}

void FACTOR14() {
    cout << point->second.name << endl;
}

void OPERAND15() {
    cout << point->second.name << endl;
}

void NUM16() {
    cout << point->second.name << endl;
}

void COMP17() {
    cout << point->second.name << endl;
}

void TYPE18() {
    cout << point->second.name << endl;
}

