/*
 * File: Basic.cpp
 * ---------------
 * This file is the starter project for the BASIC interpreter.
 */

#include <cctype>
#include <iostream>
#include <list>
#include <string>
#include <threads.h>

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

  std::string token = scanner.nextToken();
  TokenType type = scanner.getTokenType(token);
  if (type == NUMBER) {
    int linenumber = stringToInteger(token);
    if(scanner.hasMoreTokens()) {
      program.addSourceLine(linenumber,line);
    }else {
      program.removeSourceLine(linenumber);
    }
  }else if(type == WORD) {
    if(token=="LET") {
      Statement* tmp=new LET(parseExp(scanner));
      tmp->execute(state,program);
    }else if(token=="INPUT") {
      Statement* tmp=new INPUT(parseExp(scanner));
      tmp->execute(state,program);
    }else if(token=="PRINT") {
      Statement* tmp=new PRINT(parseExp(scanner));
      tmp->execute(state,program);
    }else if(token=="RUN") {
      program.setCurrentLine(program.getFirstLineNumber());
      while(program.getCurrentLine()!=-1) {
        int lineNumber=program.getCurrentLine();
        Statement* tmp=program.getParsedStatement(lineNumber);
        if(tmp==nullptr) {
          program.setParsedStatement(lineNumber,tmp);
        }
        tmp->execute(state,program);
        program.setCurrentLine(program.getNextLineNumber(lineNumber));
      }
    }else if(token=="LIST"){
      program.getListedLine();
    }else if(token=="CLEAR") {
      program.clear();
      state.Clear();
    }else if(token=="QUIT") {
      std::string input;
      getline(std::cin, input);
      if(!input.empty())error("SYNTAX ERROR");
      exit(0);
    }else if(token=="HELP") {
      std::cout<<"THIS IS BASIC-INTERPRETED COMPLETED BY GuitarHero\n";
    }
  }
}
