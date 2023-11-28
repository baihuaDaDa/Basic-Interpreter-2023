/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include "program.hpp"



Program::Program() = default;

Program::~Program() = default;

void Program::clear() {
    // Replace this stub with your own code
    //todo
    for (auto iter = line_list.begin(); iter != line_list.end(); iter++) {
        delete parsed_line[*iter];
    }
    for (auto iter = temporary_line.begin(); iter != temporary_line.end(); iter++) {
        delete *iter;
    }
    parsed_line.clear();
    temporary_line.clear();
    line_list.clear();
    source_line.clear();
}

void Program::addSourceLine(int lineNumber, const std::string &line) {
    // Replace this stub with your own code
    //todo
    line_list.insert(lineNumber);
    source_line[lineNumber] = line;
}

void Program::removeSourceLine(int lineNumber) {
    // Replace this stub with your own code
    //todo
    auto iter_lineNumber = line_list.find(lineNumber);
    if (iter_lineNumber == line_list.end()) {
        return;
    }
    line_list.erase(iter_lineNumber);
    source_line.erase(lineNumber);
    delete parsed_line[lineNumber];
    parsed_line.erase(lineNumber);
}

std::string Program::getSourceLine(int lineNumber) {
    // Replace this stub with your own code
    //todo
    if (line_list.find(lineNumber) == line_list.end()) {
        return "";
    }
    return source_line[lineNumber];
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
    // Replace this stub with your own code
    //todo
    auto iter_lineNumber = line_list.find(lineNumber);
    if (iter_lineNumber == line_list.end()) {
        error("Compiled Error");
        return;
    }
    delete parsed_line[lineNumber];
    parsed_line[lineNumber] = stmt;
}

//void Program::removeSourceLine(int lineNumber) {

Statement *Program::getParsedStatement(int lineNumber) {
    // Replace this stub with your own code
    //todo
    auto iter_lineNumber = line_list.find(lineNumber);
    if (iter_lineNumber == line_list.end()) {
        return nullptr;
    }
    return parsed_line[lineNumber];
}

int Program::getFirstLineNumber() {
    // Replace this stub with your own code
    //todo
    if (line_list.empty()) {
        return -1;
    }
    return *line_list.begin();
}

int Program::getNextLineNumber() {
    // Replace this stub with your own code
    //todo
    auto iter_lineNumber = line_list.upper_bound(nowLineNumber);
    if (iter_lineNumber == line_list.end() || nowLineNumber == -1) {
        return -1;
    }
    return *iter_lineNumber;
}

//more func to add
//todo

int Program::getNowLineNumber() const {
    return nowLineNumber;
}

void Program::changeNowLineNumber(int lineNumber) {
    nowLineNumber = lineNumber;
}

void Program::addTemporaryLine(Statement *Stmt) {
    temporary_line.push_back(Stmt);
}
