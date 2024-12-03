/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include "statement.hpp"


/* Implementation of the Statement class */

int stringToInt(std::string str);

Statement::Statement() = default;

Statement::~Statement() = default;

REM::REM() {
};

void REM::execute(EvalState &state, Program &program) {
}


LET::LET(Expression *expression): expression_(expression) {
}

void LET::execute(EvalState &state, Program &program) {
  expression_->eval(state);
}

LET::~LET() {
  delete expression_;
}

PRINT::PRINT(Expression *expression): expression_(expression) {
}

void PRINT::execute(EvalState &state, Program &program) {
  std::cout << expression_->eval(state) << "\n";
}

Expression *PRINT::getExpression() {
  return expression_;
};

PRINT::~PRINT() {
  delete expression_;
}

void END::execute(EvalState &state, Program &program) {
  program.save(-1);
}

INPUT::INPUT(Expression *expression): expression_(expression) {
}

void INPUT::execute(EvalState &state, Program &program) {
  std::string token;

  while (1) {
    try {
      getline(std::cin, token);
      int value = stringToInteger(token);
      std::cout << " ? ";
      state.setValue(expression_->toString(), stringToInteger(token));
      break;
    } catch (ErrorException &ex) {
      std::cout << " ? INVALID NUMBER\n";
    }
  }
}

Expression *INPUT::getExpression() {
  return expression_;
};

INPUT::~INPUT() {
  delete expression_;
}

GOTO::GOTO(Expression *expression): expression_(expression) {
}

void GOTO::execute(EvalState &state, Program &program) {
  program.save(expression_->eval(state));
}

GOTO::~GOTO() {
  delete expression_;
}

IFTHEN::IFTHEN(char op, Expression *lhs, Expression *rhs, Expression *direction): op(op), lhs(lhs), rhs(rhs),
  direction_(direction) {
}

void IFTHEN::execute(EvalState &state, Program &program) {
  if (op == '=') {
    if (lhs->eval(state) == rhs->eval(state)) {
      program.save(direction_->eval(state));
    }
  } else if (op == '<') {
    if (lhs->eval(state) < rhs->eval(state)) {
      program.save(direction_->eval(state));
    }
  } else if (op == '>') {
    if (lhs->eval(state) > rhs->eval(state)) {
      program.save(direction_->eval(state));
    }
  } else {
    error("SYNTAX ERROR");
  }
}

IFTHEN::~IFTHEN() {
  delete lhs;
  delete rhs;
  delete direction_;
}
