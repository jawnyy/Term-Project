#include <iostream>
#include <fstream>
#include <cctype>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
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
vector<string> idTable;
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

int symbolTableIndex = 0;
int countLineNumber = 1;