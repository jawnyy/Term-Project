#include "parser.hpp"

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
    
    symbolTable[symbolTableIndex] =  {lexeme, nextToken, countLineNumber};
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
            cout << "Error, invalid identifier name!" << endl;
            exit(1);
    } else {
        auto dupCheck = find(idTable.begin(), idTable.end(), point->second.name);
        if (dupCheck != idTable.end()) {
            cout << "Duplicate id declaration found!" << endl;
            exit(2);
        } else if (point->second.name != ",") {
            idTable.push_back(point->second.name);
        } 
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
    cout << "DECL_SEC" << endl;
    DECL03();
}

void DECL03() {
    cout << "DECL" << endl;
    bool colonCheck = true;

    while (point->second.name != ":") {
        ID_LIST04();
        NextLex();
    }
    NextLex();
    TYPE18();
    NextLex();
    
    // TODO: Skip semicolon, need check here
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
    // TODO: Some call to ID that checks for correct syntax
    if (point->second.name != ",") {
        cout << "Checking for correct ID: " << point->second.name << endl;
    }
    CheckReservedWord();

}

void ID05() {
    cout << point->second.name << endl;
}

void STMT_SEC06()  {
    cout << "STMT_SEC\n";
    NextLex();
    STMT07();
}

void STMT07() {
    cout << "STMT\n";
    if (point->second.type == ASSIGN) {
        cout << "ASSIGN\n";
        ASSIGN08();
    } else if (point->second.type == IFSTMT) {
        cout << "IF_STMT\n";
        IFSTMT09();
    } else if (point->second.type == WHILESTMT) {
        cout << "WHILE_STMT\n";
        WHILESTMT10();
    } else if (point->second.type == INPUT) {
        cout << "INPUT\n";
        INPUT11();
    } else if (point->second.type == OUTPUT) {
        cout << "OUTPUT\n";
        OUTPUT12();
    } else {
        cout << "Incorrect statement!\n";
        exit(3);
    }
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
    // TODO: Figure out how to handle line 6 in input file
    for (auto xyz : idTable){
        cout << xyz << " ";
    }
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
    if (point->second.name != "int" && point->second.name != "float" && point->second.name != "double") {
        cout << "Error, wrong type declared!\n";
        exit(1);
    }
}

