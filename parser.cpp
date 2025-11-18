#include "parser.hpp"

int main() {
    // Change the name of the file on the line below.
    inputFile.open("input6.txt");
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
            nextToken = UNKNOWN;
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
        if (isalpha(nextChar)) {
            charClass = LETTER;
        }
        else if (isdigit(nextChar)) {
            charClass = DIGIT;
        }
        else {
            charClass = UNKNOWN;
        }

        if (nextChar == '\n') {
            countLineNumber++;
        }

    }
    else {
        charClass = EOF;
    }
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
    int lexLineNumber = countLineNumber;
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

    // Enter the values into the symbol table.
    symbolTable[symbolTableIndex] =  {lexeme, nextToken, lexLineNumber};
    symbolTableIndex++;
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
            cout << "Error, cannot use reserved word as identifier name - line " << point->second.lineNumber <<"\n\n";
            exit(99);
    } else {
        auto dupCheck = find(idTable.begin(), idTable.end(), point->second.name);
        if (dupCheck != idTable.end()) {
            cout << "Duplicate identifier declaration - found on line " << point->second.lineNumber << "\n\n";
            exit(99);
        } else if (point->second.name != ",") {
            idTable.push_back(point->second.name);
        } 
    }
}

void PROGRAM01() {
    // program DECL_SEC begin STMT_SEC end; | program begin STMT_SEC end;
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

            int keyVal = point->first;
            auto pointAt = symbolTable.find(keyVal);
            auto pointNext = std::next(pointAt);
            auto pointNextNext = std::next(pointNext);
            if (point->second.name == "end") {
                NextLex();
                if (point->second.name == ";") {
                    // Program ended successfully!
                } else {
                    cout << "Expected a ';' before line " << point->second.lineNumber << "\n\n";
                    exit(1);
                }
            } else if (pointNextNext->second.name == "end") {
                NextLex();
                NextLex();
                NextLex();
                if (point->second.name == ";") {
                    // Program ended successfully!
                } else {
                    cout << "Expected a ';' before line " << point->second.lineNumber << "\n\n";
                    exit(1);
                }
            } else {
                cout << "Expected 'end' after line " << point->second.lineNumber << "\n\n";
                exit(1);
            }

        } else {
            cout << "Incorrect start of program!" << endl;
        }

        cout << "\n";
    }
}

void DECL_SEC02() {
    cout << "DECL_SEC" << endl;
    DECL03();
}

void DECL03() {
    cout << "DECL" << endl;
    bool colonCheck = false;

    while (point->second.name != ":") {
        ID_LIST04();
        NextLex();
    }
    NextLex();
    TYPE18();
    NextLex();
    
    // Semicolon ends the line, else syntax error.
    if (point->second.name != ";") {
        cout << "Expected semicolon before line " << point->second.lineNumber << "\n\n";
        exit(3);
    }

    NextLex();
    // Checks for more declaration sections.
    if (point->second.name == "begin") {
        STMT_SEC06();
    } else {
        DECL_SEC02();
    }

    // can also skip "begin" here
}

void ID_LIST04() {
    
    // If type is in identifier list, then error (no colon)
    if (point->second.name == "int" || point->second.name == "float" || point->second.name == "double") {
        cout << "Expected colon in declaration section before line " << point->second.lineNumber << "\n\n";
        exit(4);
    }
    
    
    // Skips the commas, not identifiers
    if (point->second.name != ",") {

        // Call to ID that checks for correct syntax.
        ID05();

        // Check if the id name is valid.
        CheckReservedWord();
        cout << "ID_LIST\n";
    }

}

void ID05() {
    int idLength = point->second.name.length() - 1;

    if (isalpha(point->second.name[0]) || point->second.name[0] == '_') {
        for (int i = 1; i <= idLength; i++) {
            if (isalnum(point->second.name[i]) || point->second.name[i] == '_') {
                continue;
            }else{
                cout << point->second.name << " : Invalid character in identifier at line " << point->second.lineNumber << "\n\n";
                exit(5);
            }
        }
    }else{
        cout << point->second.name << " : Invalid character in identifier at line " << point->second.lineNumber << "\n\n";
        exit(5);
    }  
}

void STMT_SEC06()  {
    // Function that just calls STMT07.
    cout << "STMT_SEC\n";
    NextLex();
    STMT07();
}

void STMT07() {
    // Checks for what type of statement was called.
    cout << "STMT\n";
    if (point->second.name == "if") {
        cout << "IF_STMT\n";
        NextLex();
        IFSTMT09();

        if (point->second.type == OUTPUT) {
            cout << "OUTPUT\n";
            NextLex();
            while (point->second.name != ";") {
                if (point->second.name != ",") {
                    OUTPUT12();
                    NextLex();
                } else {
                    NextLex();
                }
            }
            NextLex();
        } else {
            // Assuming that output follows right after the end of if loop.
            cout << "Error, expected output on line " << point->second.lineNumber << "\n\n";
            exit(7);
        }
    } else if (point->second.name == "while") {
        cout << "WHILE_STMT\n";
        NextLex();
        WHILESTMT10();

        
    } else if (point->second.type == INPUT) {
        cout << "INPUT\n";
        NextLex();
        INPUT11();
    } else if (point->second.type == OUTPUT) {
        cout << "OUTPUT\n";
        NextLex();
        OUTPUT12();
    } else if (point->second.name == "end") {
        // Do nothing, just return to if or while loop.
    } else {
        // Making strong assumption that if nothing else, it has to be ASSIGN statement.
        ASSIGN08();
    }
}

void ASSIGN08() {
    // Checks if valid identifier.
    ID05(); 

    auto idTableCheck = find(idTable.begin(), idTable.end(), point->second.name);
    if (idTableCheck == idTable.end()) {
        cout << "Variable not declared on line " << point->second.lineNumber << "\n\n";
        exit(8);
    }

    NextLex();

    int keyVal = point->first;
    auto pointAt = symbolTable.find(keyVal); // points at ":"
    auto pointNext = std::next(pointAt); // points at "="
    if (pointAt->second.name == ":" && pointNext->second.name == "="){
        // Correct assigment operator.
        cout << "ASSIGN\n";
        NextLex();
        NextLex();

        EXPR13();
    } else {
        cout << "Not a valid assignment operator at line " << point->second.lineNumber << "\n\n";
        exit(8);
    }
}

void IFSTMT09() {
    // if COMP then STMT_SEC end if ; | if COMP then STMT_SEC else STMT_SEC end if ;
    COMP17();

    if (point->second.name == "then") {
        STMT_SEC06();

        if (point->second.name == "else") {
            STMT_SEC06();
        }
        
        if (point->second.name == "end") {
            NextLex();
            if (point->second.name == "if") {
                NextLex();
                if (point->second.name != ";") {
                    cout << "Expected ';' on line before " << point->second.lineNumber << "\n\n";
                    exit(9);
                } else {
                    NextLex();
                }
            } else {
                cout << "Expected 'if' on line " << point->second.lineNumber << "\n\n";
                exit(9);
            }
        } else {
            cout << point->second.name << " at " << point->first << endl;
            cout << "Expected 'end' on line " << point->second.lineNumber << "\n\n";
            exit(9);
        }

    }
}

void WHILESTMT10() {
    // while COMP loop STMT_SEC end loop ;
    COMP17();

    if (point->second.name == "loop") {
        STMT_SEC06();
        if (point->second.name == "end") {
            NextLex();
            if (point->second.name == "loop") {
                NextLex();
                if (point->second.name != ";") {
                    cout << "Expected ';' on line before " << point->second.lineNumber << "\n\n";
                    exit(10);
                } else {
                    int keyVal = point->first; // currently ';'
                    auto pointAt = symbolTable.find(keyVal);
                    auto pointNext = std::next(pointAt);
                    if (pointNext->second.name == "else"){
                        NextLex();
                    } else if (pointNext->second.name == "end") {
                        NextLex();
                    } else {
                        STMT_SEC06();
                    }
                }
            } else {
                cout << "Expected 'loop' at line " << point->second.lineNumber << "\n\n";
                exit(10);
            }
        } else {
            cout << "Expected 'end' at line " << point->second.lineNumber << "\n\n";
            exit(10);
        }
    }
}

void INPUT11() {
    // Whenever there is an INPUT, the variables have to be in idList.
    if (point->second.name != ";") {
        if (point->second.name != ",") {
            auto idTableCheck = find(idTable.begin(), idTable.end(), point->second.name);
            if (idTableCheck != idTable.end()) {
                cout << "ID_LIST\n";
                NextLex();
                INPUT11();
            } else {
                cout << "Undeclared variable found on line " << point->second.lineNumber << "\n\n";
                exit(11);
            }
        } else {
            NextLex();
            INPUT11();
        }
    } else {
        STMT_SEC06();
    }
}

void OUTPUT12() {
    // It either goes to ID_LIST or NUM, this assumes no other possibility.
    auto idTableCheck = find(idTable.begin(), idTable.end(), point->second.name);
    if (idTableCheck != idTable.end()) {
        cout << "ID_LIST\n";
    } else if (isdigit(point->second.name[0])) {
        NUM16();
        NextLex();

        int keyVal = point->first;
        auto pointAt = symbolTable.find(keyVal);
        auto pointNext = std::next(pointAt);
        if (pointNext->second.name == "end") {
            NextLex();
        } else {
            STMT_SEC06();
        }
    }
}

void EXPR13() {
    // Calls FACTOR first and then if the next lexeme is a plus then we call EXPR again.
    // EXPR -> FACTOR | FACTOR + EXPR | FACTOR - EXPR
    cout << "EXPR\n";
    auto idTableCheck = find(idTable.begin(), idTable.end(), point->second.name);
    if (idTableCheck == idTable.end()) {
        if (isdigit(point->second.name[0])) {
            // skip
        } else {
            cout << "Variable not declared on line " << point->second.lineNumber << "\n\n";
            exit(13);
        }
    }
    FACTOR14();
    if (point->second.name != ";") {
        if (point->second.name == "+" || point->second.name == "-") {
            NextLex();
            EXPR13();
        }
    } else {
        int keyVal = point->first;
        auto pointAt = symbolTable.find(keyVal);
        auto pointNext = std::next(pointAt);
        auto pointNextNext = std::next(pointNext);

        if (pointNextNext->second.name == "loop") {
            NextLex();
        } else if (pointNext->second.name == "else") {
            NextLex();
        } else {
            STMT_SEC06();
        }
    }
}

void FACTOR14() {
    // Calls OPERAND first then if * or / it call itself again.
    cout << "FACTOR\n";
    auto idTableCheck = find(idTable.begin(), idTable.end(), point->second.name);
    if (idTableCheck == idTable.end()) {
        if (isdigit(point->second.name[0])) {
            // skip
        } else {
            cout << "Variable not declared on line " << point->second.lineNumber << "\n\n";
            exit(14);
        }
    }
    OPERAND15();
    if (point->second.name == "*" || point->second.name == "/") {
        NextLex();
        FACTOR14();
    }
}

void OPERAND15() {
    // OPERAND -> NUM | ID | ( EXPR )
    cout << "OPERAND\n";
    NextLex();

}

void NUM16() {
    // (0 | 1 | ... | 9)+ [.(0 | 1 | ... | 9)+]
    int idLength = point->second.name.length() - 1;

    if (idLength > 9) {
        cout << "Number too long on line " << point->second.lineNumber << "\n\n";
        exit(16);
    } else {
        if (isdigit(point->second.name[0])) {
            for (int i = 1; i <= idLength; i++) {
                if (isdigit(point->second.name[i]) || point->second.name == ".") {
                    continue;
                }else{
                    cout << "Invalid character in number on line " << point->second.lineNumber << "\n\n";
                    exit(16);
                }
            }
        }else{
            cout << "Number must start with valid integer on line " << point->second.lineNumber << "\n\n";
            exit(16);
        }
    }
}

void COMP17() {
    // Takes the comparison section of loops and verifies the correct syntax.
    if (point->second.name == "(") {
        cout << "COMP\n";
        NextLex();
        while (point->second.name != ")") {
            if (point->second.type == NUM || point->second.type == ID) {
                OPERAND15();
            } else {
                // <>, <, >, =
                NextLex();
            }
        }
        NextLex();
    } else {
        cout << "Missing opening parenthesis at line " << point->second.lineNumber << endl;
        exit(17);
    }
}

void TYPE18() {
    if (point->second.name != "int" && point->second.name != "float" && point->second.name != "double") {
        cout << "Error, wrong type declared on line " << point->second.lineNumber << "\n\n";
        exit(18);
    }
}

