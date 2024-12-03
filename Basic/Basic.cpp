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
  scanner.addWordCharacters("_");

  std::string token = scanner.nextToken();
  TokenType type = scanner.getTokenType(token);
  if (type == NUMBER) {
    int linenumber = stringToInteger(token);
    if (scanner.hasMoreTokens()) {
      try {
        program.addSourceLine(linenumber, line);
        if (program.getParsedStatement(linenumber) == nullptr) {
          Statement *tmp;
          program.setParsedStatement(linenumber, tmp);
        }
      } catch (ErrorException &ex) {
        program.removeSourceLine(linenumber);
        error("SYNTAX ERROR");
      }
    } else {
      program.removeSourceLine(linenumber);
    }
  } else if (type == WORD) {
    if (token == "LET") {
      if (!scanner.hasMoreTokens()) {
        error("SYNTAX ERROR");
      }
      LET tmp(parseExp(scanner));
      tmp.execute(state, program);
    } else if (token == "INPUT") {
      if (!scanner.hasMoreTokens()) {
        error("SYNTAX ERROR");
      }
      INPUT tmp(parseExp(scanner));
      std::string str = tmp.getExpression()->toString();
      for (int i = 0; i < str.length(); ++i) {
        if (str[i] == '=') {
          error("SYNTAX ERROR");
          return;
        }
      }
      tmp.execute(state, program);
    } else if (token == "PRINT") {
      if (!scanner.hasMoreTokens()) {
        error("SYNTAX ERROR");
      }
      PRINT tmp(parseExp(scanner));
      std::string str = tmp.getExpression()->toString();
      for (int i = 0; i < str.length(); ++i) {
        if (str[i] == '=') {
          error("SYNTAX ERROR");
        }
      }
      tmp.execute(state, program);
    } else if (token == "RUN") {
      if (scanner.hasMoreTokens()) {
        error("SYNTAX ERROR");
      }
      program.setCurrentLine(program.getFirstLineNumber());
      while (program.getCurrentLine() != -1) {
        int lineNumber = program.getCurrentLine();
        if (program.getParsedStatement(lineNumber) == nullptr) {
          Statement *tmp;
          program.setParsedStatement(lineNumber, tmp);
        }
        program.getParsedStatement(lineNumber)->execute(state, program);
        program.setCurrentLine(program.getNextLineNumber(lineNumber));
      }
    } else if (token == "LIST") {
      if (scanner.hasMoreTokens()) {
        error("SYNTAX ERROR");
      }
      program.getListedLine();
    } else if (token == "CLEAR") {
      if (scanner.hasMoreTokens()) {
        error("SYNTAX ERROR");
      }
      program.clear();
      state.Clear();
    } else if (token == "QUIT") {
      if (scanner.hasMoreTokens()) {
        error("SYNTAX ERROR");
      }
      exit(0);
    } else if (token == "HELP") {
      std::cout << "I WANT TO BUY A NEW GUITAR\n";
    } else {
      error("SYNTAX ERROR");
    }
  } else {
    error("SYNTAX ERROR");
  }
}
