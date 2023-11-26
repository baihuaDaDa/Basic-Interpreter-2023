/*
 * File: Basic.cpp
 * ---------------
 * This file is the starter project for the BASIC interpreter.
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp.hpp"
#include "parser.hpp"
#include "program.hpp"
#include "Utils/error.hpp"
#include "Utils/tokenScanner.hpp"
#include "Utils/strlib.hpp"


/* Function prototypes */

void processLine(std::string line, Program &program, EvalState &state);

/* Main program */

int main() {
    EvalState state;
    Program program;
    //cout << "Stub implementation of BASIC" << endl;
    while (true) {
        try {
            std::string input;
            getline(std::cin, input);
            if (input.empty())
                return 0;
            processLine(input, program, state);
        } catch (ErrorException &ex) {
            std::cout << ex.getMessage() << std::endl;
        }
    }
    return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version of
 * implementation, the program reads a line, parses it as an expression,
 * and then prints the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(std::string line, Program &program, EvalState &state) {
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(line);
    std::string token;
    int lineNumber;
    if (scanner.hasMoreTokens()) {
        token = scanner.nextToken();
        if (scanner.getTokenType(token) == NUMBER) {
            lineNumber = stringToInteger(token);
            program.addSourceLine(lineNumber, line);
            token = scanner.nextToken();
            if (token == "REM") {
                return;
            }
            if (token == "LET") {
                Expression *tmpExp = readE(scanner);
                if (scanner.hasMoreTokens()) {
                    std::cout << "SYNTAX ERROR\n";
                    return;
                }
                Statement *letStmt;
                letStmt = new LetStmt(tmpExp);
                program.setParsedStatement(lineNumber, letStmt);
                return;
            }
            if (token == "INPUT") {
                Expression *val = readT(scanner);
                if (scanner.hasMoreTokens()) {
                    std::cout << "SYNTAX ERROR\n";
                    return;
                }
                Statement *inputStmt;
                inputStmt = new InputStmt((IdentifierExp *)val);
                program.setParsedStatement(lineNumber, inputStmt);
                return;
            }
            if (token == "PRINT") {
                Expression *tmpExp = readE(scanner, 1);
                if (scanner.hasMoreTokens()) {
                    std::cout << "SYNTAX ERROR\n";
                    return;
                }
                Statement *printStmt;
                printStmt = new PrintStmt(tmpExp);
                program.setParsedStatement(lineNumber, printStmt);
                return;
            }
            if (token == "END") {
                if (scanner.hasMoreTokens()) {
                    std::cout << "SYNTAX ERROR\n";
                    return;
                }
                Statement *endStmt;
                endStmt = new EndStmt;
                program.setParsedStatement(lineNumber, endStmt);
                return;
            }
            if (token == "IF") {
                Expression *lhs = readE(scanner, 1);
                std::string cmp = scanner.nextToken();
                if (cmp != "<" || cmp != ">" || cmp != "=") {
                    std::cout << "SYNTAX ERROR\n";
                    return;
                }
                Expression *rhs = readE(scanner, 1);
                if (scanner.nextToken() != "THEN") {
                    std::cout << "SYNTAX ERROR\n";
                    return;
                }
                token = scanner.nextToken();
                if (scanner.getTokenType(token) != NUMBER) {
                    std::cout << "SYNTAX ERROR\n";
                    return;
                }
                if (scanner.hasMoreTokens()) {
                    std::cout << "SYNTAX ERROR\n";
                    return;
                }
                Statement *ifStmt;
                ifStmt = new IfStmt(lhs, cmp, rhs, stringToInteger(token));
                program.setParsedStatement(lineNumber, ifStmt);
                return;
            }
            if (token == "GOTO") {
                token = scanner.nextToken();
                if (scanner.getTokenType(token) != NUMBER) {
                    std::cout << "SYNTAX ERROR\n";
                    return;
                }
                if (scanner.hasMoreTokens()) {
                    std::cout << "SYNTAX ERROR\n";
                    return;
                }
                Statement *goToStmt;
                goToStmt = new GoToStmt(stringToInteger(token));
                program.setParsedStatement(lineNumber, goToStmt);
                return;
            }
            std::cout << "SYNTAX ERROR\n";
        } else if (scanner.getTokenType(token) == WORD) {
            if (token == "LET") {
                Expression *tmpExp = readE(scanner);
                if (scanner.hasMoreTokens()) {
                    std::cout << "SYNTAX ERROR\n";
                    return;
                }
                Statement *letStmt;
                letStmt = new LetStmt(tmpExp);
                letStmt->execute(state, program);
                return;
            }
            if (token == "INPUT") {
                Expression *val = readT(scanner);
                if (scanner.hasMoreTokens()) {
                    std::cout << "SYNTAX ERROR\n";
                    return;
                }
                Statement *inputStmt;
                inputStmt = new InputStmt((IdentifierExp *)val);
                inputStmt->execute(state, program);
                return;
            }
            if (token == "PRINT") {
                Expression *tmpExp = readE(scanner, 1);
                if (scanner.hasMoreTokens()) {
                    std::cout << "SYNTAX ERROR\n";
                    return;
                }
                Statement *printStmt;
                printStmt = new PrintStmt(tmpExp);
                printStmt->execute(state, program);
                return;
            }
            if (token == "RUN") {
                if (scanner.hasMoreTokens()) {
                    std::cout << "SYNTAX ERROR\n";
                    return;
                }
                Statement *runStmt;
                runStmt = new RunStmt;
                runStmt->execute(state, program);
                return;
            }
            if (token == "LIST") {
                if (scanner.hasMoreTokens()) {
                    std::cout << "SYNTAX ERROR\n";
                    return;
                }
                Statement *listStmt;
                listStmt = new ListStmt;
                listStmt->execute(state, program);
                return;
            }
            if (token == "HELP") {
                if (scanner.hasMoreTokens()) {
                    std::cout << "SYNTAX ERROR\n";
                    return;
                }
                Statement *helpStmt;
                helpStmt = new HelpStmt;
                helpStmt->execute(state, program);
                return;
            }
            if (token == "CLEAR") {
                if (scanner.hasMoreTokens()) {
                    std::cout << "SYNTAX ERROR\n";
                    return;
                }
                Statement *clearStmt;
                clearStmt = new ClearStmt;
                clearStmt->execute(state, program);
                return;
            }
            if (token == "QUIT") {
                if (scanner.hasMoreTokens()) {
                    std::cout << "SYNTAX ERROR\n";
                    return;
                }
                Statement *quitStmt;
                quitStmt = new QuitStmt;
                quitStmt->execute(state, program);
                return;
            }
            std::cout << "SYNTAX ERROR\n";
        } else {
            std::cout << "SYNTAX ERROR\n";
        }
    } else {
        return;
    }

}

