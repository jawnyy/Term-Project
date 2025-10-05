#include <iostream>;

#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

#define IDENT;

#define ADD_OP 40
#define SUB_OP 41
#define MUL_OP 42
#define DIV_OP 43
#define LT_OP 44 // '<'
#define GT_OP 45 // '>'
#define EQ_OP 46 // '='
#define NEQ_OP 47 // '<>'
#define ASS_OP 48 // ':='

#define OP_PAREN 50
#define CL_PAREN 51

/*
Global declarations
Variables 
int charClass;
char lexeme [100];
char nextChar;
int lexLen;
int token;
int nextToken;
FILE *in_fp, *fopen();

Function declarations
void addChar();
void getChar();
void getNonBlank();
int lex();

Character classes
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

Token codes
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
*/