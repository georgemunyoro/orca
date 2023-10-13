
#include "interpreter.h"
#include "evaluator/evaluator.h"
#include "lexer.h"
#include "parser/ast_printer.h"
#include "parser/parser.h"
#include "variable/resolver.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

void Interpreter::run_file(std::string filepath) {
  std::ifstream file(filepath);
  std::stringstream buffer;
  buffer << file.rdbuf();
  std::string source = buffer.str();

  run(source);
}

void Interpreter::run(std::string source) {
  Lexer lexer = Lexer(source);
  std::vector<Token> tokens = lexer.scan_tokens();

  // for (Token t : tokens) {
  //   printf("%14s | %s\n", token_type_as_str(t.type).c_str(),
  //   t.lexeme.c_str());
  // }

  Parser parser = Parser(tokens);
  std::vector<Expression *> exprs = parser.parse();

  // AstPrinter printer = AstPrinter();
  Evaluator *evaluator = new Evaluator();
  Resolver *resolver = new Resolver(evaluator);

  for (Expression *expr : exprs) {
    // printer.print(expr);
    resolver->resolve_expr(expr);
  }

  // printf("----------------------------------\n");

  for (Expression *expr : exprs) {
    evaluator->evaluate(expr);
  }
}
