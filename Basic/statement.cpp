/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include "statement.hpp"

#include <utility>


/* Implementation of the Statement class */

int stringToInt(std::string str);

Statement::Statement() = default;

Statement::~Statement() = default;

//todo

LetStmt::LetStmt(Expression *exp) : exp(exp) {}

void LetStmt::execute(EvalState &state, Program &program) {
    exp->eval(state);
}

LetStmt::~LetStmt() {
    delete exp;
}

PrintStmt::PrintStmt(Expression *exp) : exp(exp) {}

void PrintStmt::execute(EvalState &state, Program &program) {
    std::cout << exp->eval(state) << '\n';
}

PrintStmt::~PrintStmt() {
    delete exp;
}

InputStmt::InputStmt(IdentifierExp *valName) : valName(valName) {}

void InputStmt::execute(EvalState &state, Program &program) {
    std::cout << " ? ";
    int value;
    while (!(std::cin >> value)) {
        std::cout << "SYNTAX ERROR\n";
    }
    getchar();
    state.setValue(valName->getName(), value);
}

InputStmt::~InputStmt() {
    delete valName;
}

void EndStmt::execute(EvalState &state, Program &program) {
    program.changeNowLineNumber(-1);
}

void QuitStmt::execute(EvalState &state, Program &program) {
    exit(0);
}

void HelpStmt::execute(EvalState &state, Program &program) {
    std::cout << "What you have said is right, "
              << "but Basic-Interpreter-2023 is a new open world adventure game developed in-house by ACM-Class-2023."
              << '\n';
}

void ListStmt::execute(EvalState &state, Program &program) {
    program.changeNowLineNumber(program.getFirstLineNumber());
    while (program.getNowLineNumber() != -1) {
        std::cout << program.getSourceLine(program.getNowLineNumber());
        program.changeNowLineNumber(program.getNextLineNumber());
    }
}

void ClearStmt::execute(EvalState &state, Program &program) {
    program.clear();
    state.Clear();
}

IfStmt::IfStmt(Expression *lhs, std::string cmp, Expression *rhs, int toLineNumber) : lhs(lhs), cmp(std::move(cmp)), rhs(rhs), toLineNumber(toLineNumber) {}

void IfStmt::execute(EvalState &state, Program &program) {
    int lhsValue = lhs->eval(state);
    int rhsValue = rhs->eval(state);
    bool flag;
    if (cmp == "=") {
        flag = (lhsValue == rhsValue);
    } else if (cmp == ">") {
        flag = (lhsValue > rhsValue);
    } else {
        flag = (lhsValue < rhsValue);
    }
    if (flag) {
        if (program.getParsedStatement(toLineNumber) == nullptr) {
            std::cout << "LINE NUMBER ERROR\n";
            return;
        }
        program.changeNowLineNumber(toLineNumber);
    }
}

IfStmt::~IfStmt() {
    delete lhs;
    delete rhs;
}

GoToStmt::GoToStmt(int toLineNumber) : toLineNumber(toLineNumber) {}

void GoToStmt::execute(EvalState &state, Program &program) {
    if (program.getParsedStatement(toLineNumber) == nullptr) {
        std::cout << "LINE NUMBER ERROR\n";
        return;
    }
    program.changeNowLineNumber(toLineNumber);
}

void RunStmt::execute(EvalState &state, Program &program) {
    program.changeNowLineNumber(program.getFirstLineNumber());
    while (program.getNowLineNumber() != -1) {
        program.getParsedStatement(program.getNowLineNumber())->execute(state, program);
        program.changeNowLineNumber(program.getNextLineNumber());
    }
}
