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

REM::REM(){};

void REM::execute(EvalState &state, Program &program) {}


LET::LET(Expression *expression):expression_(expression){}

void LET::execute(EvalState &state, Program &program) {
 expression_->eval(state);
}

LET::~LET() {
 delete expression_;
}

PRINT::PRINT(Expression *expression):expression_(expression) {}

void PRINT::execute(EvalState &state, Program &program) {
 std::cout<<expression_->toString()<<"\n";
}

PRINT::~PRINT() {
 delete expression_;
}

void END::execute(EvalState &state, Program &program) {
 program.setCurrentLine(-1);
}

INPUT::INPUT(Expression* expression):expression_(expression){}

void INPUT::execute(EvalState &state, Program &program) {
 std::cout<<"?";
 std::string token;
 std::cin>>token;
 state.setValue(expression_->toString(),stringToInteger(token));
}

INPUT::~INPUT() {
 delete expression_;
}

GOTO::GOTO(Expression *expression):expression_(expression){}

void GOTO::execute(EvalState &state, Program &program) {
 program.save(expression_->eval(state));
}

GOTO::~GOTO() {
 delete expression_;
}

IFTHEN::IFTHEN(CompoundExp *expression, Expression *direction):expression_(expression),direction_(direction) {}

void IFTHEN::execute(EvalState &state, Program &program) {
 if(expression_->getOp()=="=") {
  if(expression_->getLHS()->eval(state)==expression_->getRHS()->eval(state)) {
   GOTO direction(direction_);
   direction.execute(state,program);
  }
 }else if(expression_->getOp()=="<") {
  if(expression_->getLHS()->eval(state)<expression_->getRHS()->eval(state)) {
   GOTO direction(direction_);
   direction.execute(state,program);
  }
 }else if(expression_->getOp()==">") {
  if(expression_->getLHS()->eval(state)>expression_->getRHS()->eval(state)) {
   GOTO direction(direction_);
   direction.execute(state,program);
  }
 }else {
  error("SYNTAX ERROR");
 }
}

IFTHEN::~IFTHEN() {
 delete expression_;
 delete direction_;
}
