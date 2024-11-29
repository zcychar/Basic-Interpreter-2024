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

#include <complex.h>


Program::Program() = default;

Program::~Program() {
    for(auto it : parsed_storage) {
        delete it.second;
    }
};

void Program::clear() {
    // Replace this stub with your own code
    line_storage.clear();
}

void Program::addSourceLine(int lineNumber, const std::string &line) {
    // Replace this stub with your own code
    line_storage[lineNumber]=line;
    if(parsed_storage.count(lineNumber)) {
        parsed_storage.erase(lineNumber);
    }
}

void Program::removeSourceLine(int lineNumber) {
    // Replace this stub with your own code
    line_storage.erase(lineNumber);
    parsed_storage.erase(lineNumber);
}

std::string Program::getSourceLine(int lineNumber) {
    // Replace this stub with your own code
    auto it=line_storage.find(lineNumber);
    if(it==line_storage.end()) {
        return "";
    }
    return it->second;
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
    // Replace this stub with your own code
    if(!line_storage.count(lineNumber)) {
        error(" LINE NUMBER ERROR");
    }
    if (parsed_storage.count(lineNumber)) {
        delete parsed_storage[lineNumber];
    }
    TokenScanner scanner(line_storage[lineNumber]);
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.nextToken();
    std::string token=scanner.nextToken();
    if(token=="LET") {
        Expression *tmp=parseExp(scanner);
        stmt=new LET(tmp);
        parsed_storage[lineNumber]=stmt;
    }else if(token=="PRINT") {
        Expression *tmp=parseExp(scanner);
        stmt=new PRINT(tmp);
        parsed_storage[lineNumber]=stmt;
    }else if(token=="INPUT") {
        Expression *tmp=parseExp(scanner);
        stmt=new INPUT(tmp);
        parsed_storage[lineNumber]=stmt;
    }else if(token=="END") {
        if(scanner.hasMoreTokens()) {
            error("SYNTAX ERROR");
        }
        stmt=new END;
        parsed_storage[lineNumber]=stmt;
    }else if(token=="GOTO") {
        Expression *tmp=parseExp(scanner);
        stmt=new GOTO(tmp);
        parsed_storage[lineNumber]=stmt;
    }else if(token=="IF") {
        int front_pos=scanner.getPosition();
        while(scanner.hasMoreTokens()) {
            token=scanner.nextToken();
            if(token=="THEN") {
                int back_pos=scanner.getPosition();
                Expression* direction=parseExp(scanner);
                std::string subexp=line_storage[lineNumber].substr(front_pos+1,back_pos-front_pos-6);
                for(int i=0;i<subexp.length();++i) {
                    if(subexp[i]=='='||subexp[i]=='<'||subexp[i]=='>') {
                        TokenScanner exp1(subexp.substr(0,i));
                        TokenScanner exp2(subexp.substr(i+1));
                        stmt=new IFTHEN(subexp[i],parseExp(exp1),parseExp(exp2),direction);
                        parsed_storage[lineNumber]=stmt;
                        return;
                    }
                }
            }
        }
    }else if(token=="REM") {
        stmt=new REM();
        parsed_storage[lineNumber]=stmt;
    }
}

Statement *Program::getParsedStatement(int lineNumber) {
   // Replace this stub with your own code
    if(!line_storage.count(lineNumber)) {
        error(" LINE NUMBER ERROR");
    }
    auto it=parsed_storage.find(lineNumber);
    if(it==parsed_storage.end()) {
        return nullptr;
    }
    return it->second;
}

int Program::getFirstLineNumber() {
    // Replace this stub with your own code
    return line_storage.begin()->first;
}

int Program::getNextLineNumber(int lineNumber) {
    // Replace this stub with your own code
    if(saved_next_line!=-2) {
        int next_line=saved_next_line;
        saved_next_line=-2;
        return next_line;
    }
    auto it=std::next(line_storage.find(lineNumber));
    if(it==line_storage.end())return -1;
    return it->first;
}

//more func to add
void Program::setCurrentLine(int lineNumber) {
    if(lineNumber==-1||line_storage.count(lineNumber)) {
        current_line=lineNumber;
    }else {
        error("LINE NUMBER ERROR");
    }
}

int Program::getCurrentLine() {
    return current_line;
}

void Program::save(int lineNumber) {
    saved_next_line=lineNumber;
}

void Program::getListedLine() {
    for(const auto& it:line_storage) {
        std::cout<<it.second<<"\n";
    }
}





