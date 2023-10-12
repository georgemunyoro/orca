
#include "parser.h"
#include "ast_printer.h"
#include "expression.h"
#include <stdexcept>

std::vector<Expression *> Parser::parse() {
  std::vector<Expression *> exprs;
  while (!is_at_end()) {
    exprs.push_back(declaration());
  }
  return exprs;
}

Expression *Parser::declaration() {
  return match(VAR) ? variable_declaration() : statement();
}

Expression *Parser::block() {
  std::vector<Statement *> statements;
  while (!check(RIGHT_BRACE) && !is_at_end())
    statements.push_back((Statement *)declaration());
  consume(RIGHT_BRACE, "Expect '}' after block.");
  return new BlockStmt(statements);
}

Expression *Parser::if_statement() {
  consume(LEFT_PAREN, "Expect '(' after 'if'.");
  Expression *condition = expression();
  consume(RIGHT_PAREN, "Expect ')' after if condition.");

  Expression *then_branch = statement();
  Expression *else_branch = new BlockStmt({});

  if (match(ELSE)) {
    delete else_branch;
    else_branch = statement();
  }

  return new IfStmt(condition, (Statement *)then_branch,
                    (Statement *)else_branch);
}

Expression *Parser::while_statement() {
  consume(LEFT_PAREN, "Expect '(' after 'while'.");
  Expression *condition = expression();
  consume(RIGHT_PAREN, "Expect ')' after while condition.");

  Expression *body = statement();

  return new WhileStmt(condition, (Statement *)body);
}

Expression *Parser::function(std::string type) {
  Token name = consume(IDENTIFIER, "Expect function name.");
  consume(LEFT_PAREN, "Expect '(' after function name.");

  std::vector<Token> parameters;
  if (!check(RIGHT_PAREN)) {
    do {
      parameters.push_back(consume(IDENTIFIER, "Expect parameter name."));
    } while (match(COMMA));
  }

  consume(RIGHT_PAREN, "Expect ')' after parameters.");
  consume(LEFT_BRACE, "Expect '{' before function body.");

  BlockStmt *body = (BlockStmt *)block();
  return new FunctionDeclarationStmt(name, parameters, body->statements);
}

Expression *Parser::class_statement() {
  Token name = consume(IDENTIFIER, "Expect class name.");
  consume(LEFT_BRACE, "Expect '{' after class name.");
  std::vector<FunctionDeclarationStmt> methods;
  while (!check(RIGHT_BRACE) && !is_at_end())
    methods.push_back(*(FunctionDeclarationStmt *)function("method"));
  consume(RIGHT_BRACE, "Expect '}' after methods.");
  return new ClassStmt(name, methods);
}

Expression *Parser::return_statement() {
  Token keyword = previous_token();
  Expression *value = new LiteralExpr();
  if (!check(SEMICOLON)) {
    delete value;
    value = expression();
  }
  consume(SEMICOLON, "Expect ';' after return value.");
  return new ReturnStmt(keyword, value);
}

Expression *Parser::statement() {
  if (match(PRINT))
    return print_statement();
  if (match(LEFT_BRACE))
    return block();
  if (match(IF))
    return if_statement();
  if (match(WHILE))
    return while_statement();
  if (match(FUN))
    return function("function");
  if (match(RETURN))
    return return_statement();
  if (match(CLASS))
    return class_statement();
  if (match(FOR))
    return for_statement();

  return expression_statement();
}

Expression *Parser::for_statement() {
  consume(LEFT_PAREN, "Expect '(' after 'for'.");

  bool set_increment, set_initializer, set_condition = false;

  Expression *initializer = new LiteralExpr();
  if (match(VAR)) {
    delete initializer;
    initializer = variable_declaration();
    set_initializer = true;
  } else {
    delete initializer;
    initializer = expression_statement();
    set_initializer = true;
  }

  Expression *condition = new LiteralExpr();
  if (!check(SEMICOLON)) {
    delete condition;
    condition = expression();
    set_condition = true;
  }

  consume(SEMICOLON, "Expect ';' after for loop condition.");

  Expression *increment = new LiteralExpr();
  if (!check(RIGHT_PAREN)) {
    delete increment;
    increment = expression();
    set_increment = true;
  }

  consume(RIGHT_PAREN, "Expect ')' after for clauses.");

  Statement *body = (Statement *)statement();

  if (set_increment)
    body = new BlockStmt({(Statement *)body, new ExpressionStmt(increment)});

  if (!set_condition) {
    delete condition;
    condition = new LiteralExpr();
  }

  body = new WhileStmt(condition, body);

  if (set_initializer)
    body = new BlockStmt({(Statement *)initializer, body});

  return body;
}

Expression *Parser::expression_statement() {
  Expression *expr = expression();
  consume(SEMICOLON, "Expect ';' after expression.");
  return new ExpressionStmt(expr);
}

Expression *Parser::print_statement() {
  Expression *expr = expression();
  consume(SEMICOLON, "Expect ';' after expression.");
  return new PrintStmt(expr);
}

Expression *Parser::variable_declaration() {
  Token name = consume(IDENTIFIER, "Expected variable name.");

  bool initializer_set;
  Expression *initializer = new LiteralExpr();

  if (match(LEFT_BRACK)) {
    ArrayExpr *arrLenSpecifier = (ArrayExpr *)array();

    if (arrLenSpecifier->values.size() == 0)
      throw std::runtime_error("Array length specifier required.");

    auto arrLen = arrLenSpecifier->values[0];
    consume(RIGHT_BRACK, "Expect ']' after array length specifier.");
    initializer = new ArrayExpr(arrLen);

    initializer_set = true;
  }

  if (match(EQUAL)) {
    if (initializer_set) {
      auto expr = expression();
      if (expr->getType() != ARRAY_EXPR)
        throw std::runtime_error("Declared array with length specifier but "
                                 "used non-array initializer.");

      ((ArrayExpr *)initializer)->values = ((ArrayExpr *)expr)->values;
    }
  }

  consume(SEMICOLON, "Expect ';' after variable declaration.");
  return new VariableDeclarationStmt(name, initializer);
}

Expression *Parser::expression() { return assignment(); }

Expression *Parser::assignment() {
  Expression *expr = equality();

  if (match(EQUAL)) {
    Token equals = previous_token();
    Expression *value = assignment();

    if (expr->getType() == VARIABLE_REFERENCE_EXPR) {
      VariableReferenceExpr *v = (VariableReferenceExpr *)expr;
      Token name = v->op;
      return new AssignmentStmt(name, value);
    } else if (expr->getType() == GET_EXPR) {
      GetExpr *v = (GetExpr *)expr;
      return new SetExpr(v->obj, v->name, value);
    } else if (expr->getType() == INDEX_EXPR) {
      IndexExpr *v = (IndexExpr *)expr;
      return new SetIndexExpr(v->obj, v->index, value);
    }

    throw std::runtime_error("Invalid assignment target");
  }

  return expr;
}

Expression *Parser::array() {
  std::vector<Expression *> values;
  if (!check(RIGHT_PAREN)) {
    do {
      values.push_back(expression());
    } while (match(COMMA));
  }
  return new ArrayExpr(values);
}

Expression *Parser::primary() {
  if (match(FALSE))
    return new LiteralExpr(false);

  if (match(TRUE))
    return new LiteralExpr(true);

  if (match(NIL))
    return new LiteralExpr(Literal{});

  if (match({NUMBER, STRING}))
    return new LiteralExpr(previous_token().literal);

  if (match(LEFT_PAREN)) {
    Expression *expr = expression();
    consume(RIGHT_PAREN, "Expect ')' after expression.");
    return new GroupingExpr(*expr);
  }

  if (match(LEFT_BRACK)) {
    Expression *expr = array();
    consume(RIGHT_BRACK, "Expect ']' after array values.");
    return expr;
  }

  if (match(IDENTIFIER))
    return new VariableReferenceExpr(previous_token());

  if (match(THIS))
    return new ThisExpr(previous_token());

  return new LiteralExpr();
}

Expression *Parser::unary() {
  if (match({BANG, MINUS})) {
    Token op = previous_token();
    Expression *right = unary();
    return new UnaryExpr(op, *right);
  }
  return call();
}

Expression *Parser::call() {
  Expression *expr = primary();

  while (true) {
    if (match(LEFT_PAREN)) {
      expr = finish_call(expr);
    } else if (match(DOT)) {
      Token name = consume(IDENTIFIER, "Expect property name after '.'.");
      expr = new GetExpr(expr, name);
    } else if (match(LEFT_BRACK)) {
      Expression *index = expression();
      expr = new IndexExpr(expr, index);
      consume(RIGHT_BRACK, "Expect ']' after index.");
    } else {
      break;
    }
  }

  return expr;
}

Expression *Parser::finish_call(Expression *callee) {
  std::vector<Expression *> arguments;
  if (!check(RIGHT_PAREN)) {
    do {
      arguments.push_back(expression());
    } while (match(COMMA));
  }
  Token paren = consume(RIGHT_PAREN, "Expect ')' after arguments.");
  return new CallExpr(callee, paren, arguments);
}

Expression *Parser::factor() {
  Expression *expr = unary();
  while (match({SLASH, STAR})) {
    Token op = previous_token();
    Expression *right = unary();
    expr = new BinaryExpr(*expr, op, *right);
  }
  return expr;
}

Expression *Parser::comparison() {
  Expression *expr = term();
  while (match({GREATER, GREATER_EQUAL, LESS, LESS_EQUAL})) {
    Token op = previous_token();
    Expression *right = term();
    expr = new BinaryExpr(*expr, op, *right);
  }
  return expr;
}

Expression *Parser::term() {
  Expression *expr = factor();
  while (match({PLUS, MINUS})) {
    Token op = previous_token();
    Expression *right = factor();
    expr = new BinaryExpr(*expr, op, *right);
  }
  return expr;
}

Expression *Parser::equality() {
  Expression *expr = comparison();
  while (match({BANG_EQUAL, EQUAL_EQUAL})) {
    Token op = previous_token();
    Expression *right = comparison();
    expr = new BinaryExpr(*expr, op, *right);
  }
  return expr;
}

void Parser::synchronize() {
  advance();
  while (!is_at_end()) {
    if (previous_token().type == SEMICOLON)
      return;

    switch (current_token().type) {
    case CLASS:
    case FUN:
    case VAR:
    case FOR:
    case IF:
    case WHILE:
    case PRINT:
    case RETURN:
      return;
    }

    advance();
  }
}

Token Parser::consume(TokenType type, std::string message) {
  if (check(type))
    return advance();

  throw std::runtime_error(message);
}

bool Parser::match(std::vector<TokenType> types) {
  for (TokenType type : types) {
    if (check(type)) {
      advance();
      return true;
    }
  }

  return false;
}

bool Parser::match(TokenType type) {
  if (check(type)) {
    advance();
    return true;
  }

  return false;
}

bool Parser::check(TokenType type) {
  if (is_at_end())
    return false;
  return current_token().type == type;
}

bool Parser::is_at_end() { return current_token().type == E_O_F; }

Token Parser::current_token() { return tokens[current]; }
Token Parser::previous_token() { return tokens[current - 1]; }

Token Parser::advance() {
  if (!is_at_end())
    ++current;
  return previous_token();
}
