#pragma once

#include "../lexer.h"
#include "expression.h"
#include <vector>

class Parser {
public:
  Parser(std::vector<Token> tokens) : tokens(tokens), current(0) {}

  std::vector<Token> tokens;
  int current;

  std::vector<Expression *> parse();

  Expression *expression();
  Expression *equality();
  Expression *comparison();
  Expression *term();
  Expression *factor();
  Expression *unary();
  Expression *primary();
  Expression *declaration();
  Expression *variable_declaration();
  Expression *statement();
  Expression *expression_statement();
  Expression *print_statement();
  Expression *block();
  Expression *assignment();
  Expression *call();
  Expression *finish_call(Expression *callee);
  Expression *if_statement();
  Expression *while_statement();
  Expression *function(std::string type);
  Expression *class_statement();
  Expression *return_statement();
  Expression *for_statement();
  Expression *array();

  void synchronize();
  Token consume(TokenType type, std::string message);

  bool match(std::vector<TokenType> types);
  bool match(TokenType type);
  bool check(TokenType type);
  bool is_at_end();

  Token previous_token();
  Token current_token();
  Token advance();
};
