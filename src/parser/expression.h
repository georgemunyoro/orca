#pragma once

#include "../evaluator/runtime_value.h"
#include "../lexer.h"

template <typename T> class ExpressionVisitor;

enum ExpressionType {
  BINARY_EXPR,
  GROUPING_EXPR,
  LITERAL_EXPR,
  UNARY_EXPR,
  CALL_EXPR,
  VARIABLE_REFERENCE_EXPR,
  GET_EXPR,
  SET_EXPR,
  THIS_EXPR,
  ARRAY_EXPR,
  INDEX_EXPR,

  EXPRESSION_STMT,
  PRINT_STMT,
  VARIABLE_DECLARATION_STMT,
  ASSIGNMENT_STMT,
  BLOCK_STMT,
  IF_STMT,
  WHILE_STMT,
  RETURN_STMT,
  CLASS_STMT,
  FUNCTION_DECLARATION_STMT,
};

class Expression {
public:
  template <typename T> T accept(const ExpressionVisitor<T> &visitor);

  virtual ~Expression() = default;
  virtual ExpressionType getType() const = 0;

protected:
  virtual RuntimeValue
  do_accept(const ExpressionVisitor<RuntimeValue> &visitor) {
    return RuntimeValue();
  };
  virtual void do_accept(const ExpressionVisitor<void> &visitor){};
};

// Expressions
class BinaryExpr;
class GroupingExpr;
class LiteralExpr;
class UnaryExpr;
class CallExpr;
class VariableReferenceExpr;
class GetExpr;
class SetExpr;
class ThisExpr;
class ArrayExpr;
class IndexExpr;
class SetIndexExpr;

// Statements
class ExpressionStmt;
class PrintStmt;
class VariableDeclarationStmt;
class AssignmentStmt;
class BlockStmt;
class IfStmt;
class WhileStmt;
class ReturnStmt;
class ClassStmt;
class FunctionDeclarationStmt;

template <typename T> class ExpressionVisitor {
public:
  virtual T visit(BinaryExpr expr) const = 0;
  virtual T visit(GroupingExpr expr) const = 0;
  virtual T visit(LiteralExpr expr) const = 0;
  virtual T visit(UnaryExpr expr) const = 0;
  virtual T visit(CallExpr expr) const = 0;
  virtual T visit(VariableReferenceExpr expr) const = 0;
  virtual T visit(GetExpr expr) const = 0;
  virtual T visit(SetExpr expr) const = 0;
  virtual T visit(ThisExpr expr) const = 0;
  virtual T visit(ArrayExpr expr) const = 0;
  virtual T visit(IndexExpr expr) const = 0;
  virtual T visit(SetIndexExpr expr) const = 0;

  virtual T visit(ExpressionStmt stmt) const = 0;
  virtual T visit(PrintStmt stmt) const = 0;
  virtual T visit(VariableDeclarationStmt stmt) const = 0;
  virtual T visit(AssignmentStmt stmt) const = 0;
  virtual T visit(BlockStmt stmt) const = 0;
  virtual T visit(IfStmt stmt) const = 0;
  virtual T visit(WhileStmt stmt) const = 0;
  virtual T visit(ReturnStmt stmt) const = 0;
  virtual T visit(ClassStmt stmt) const = 0;
  virtual T visit(FunctionDeclarationStmt stmt) const = 0;

  virtual ~ExpressionVisitor() = default;
};

template <> class ExpressionVisitor<RuntimeValue> {
public:
  virtual RuntimeValue visit(BinaryExpr expr) const = 0;
  virtual RuntimeValue visit(GroupingExpr expr) const = 0;
  virtual RuntimeValue visit(LiteralExpr expr) const = 0;
  virtual RuntimeValue visit(UnaryExpr expr) const = 0;
  virtual RuntimeValue visit(CallExpr expr) const = 0;
  virtual RuntimeValue visit(VariableReferenceExpr expr) const = 0;
  virtual RuntimeValue visit(GetExpr expr) const = 0;
  virtual RuntimeValue visit(SetExpr expr) const = 0;
  virtual RuntimeValue visit(ThisExpr expr) const = 0;
  virtual RuntimeValue visit(ArrayExpr expr) const = 0;
  virtual RuntimeValue visit(IndexExpr expr) const = 0;
  virtual RuntimeValue visit(SetIndexExpr expr) const = 0;

  virtual RuntimeValue visit(ExpressionStmt stmt) const = 0;
  virtual RuntimeValue visit(PrintStmt stmt) const = 0;
  virtual RuntimeValue visit(VariableDeclarationStmt stmt) const = 0;
  virtual RuntimeValue visit(AssignmentStmt stmt) const = 0;
  virtual RuntimeValue visit(BlockStmt stmt) const = 0;
  virtual RuntimeValue visit(IfStmt stmt) const = 0;
  virtual RuntimeValue visit(WhileStmt stmt) const = 0;
  virtual RuntimeValue visit(ReturnStmt stmt) const = 0;
  virtual RuntimeValue visit(ClassStmt stmt) const = 0;
  virtual RuntimeValue visit(FunctionDeclarationStmt stmt) const = 0;
};

template <> class ExpressionVisitor<void> {
public:
  virtual void visit(BinaryExpr expr) const = 0;
  virtual void visit(GroupingExpr expr) const = 0;
  virtual void visit(LiteralExpr expr) const = 0;
  virtual void visit(UnaryExpr expr) const = 0;
  virtual void visit(CallExpr expr) const = 0;
  virtual void visit(VariableReferenceExpr expr) const = 0;
  virtual void visit(GetExpr expr) const = 0;
  virtual void visit(SetExpr expr) const = 0;
  virtual void visit(ThisExpr expr) const = 0;
  virtual void visit(ArrayExpr expr) const = 0;
  virtual void visit(IndexExpr expr) const = 0;
  virtual void visit(SetIndexExpr expr) const = 0;

  virtual void visit(ExpressionStmt stmt) const = 0;
  virtual void visit(PrintStmt stmt) const = 0;
  virtual void visit(VariableDeclarationStmt stmt) const = 0;
  virtual void visit(AssignmentStmt stmt) const = 0;
  virtual void visit(BlockStmt stmt) const = 0;
  virtual void visit(IfStmt stmt) const = 0;
  virtual void visit(WhileStmt stmt) const = 0;
  virtual void visit(ReturnStmt stmt) const = 0;
  virtual void visit(ClassStmt stmt) const = 0;
  virtual void visit(FunctionDeclarationStmt stmt) const = 0;
};

class BinaryExpr : public Expression {
public:
  BinaryExpr(Expression &left, Token op, Expression &right)
      : left(left), op(op), right(right) {}
  ExpressionType getType() const override { return BINARY_EXPR; };

  Token op;
  Expression &left;
  Expression &right;

protected:
  RuntimeValue
  do_accept(const ExpressionVisitor<RuntimeValue> &visitor) override {
    return visitor.visit(*this);
  };

  void do_accept(const ExpressionVisitor<void> &visitor) override {
    return visitor.visit(*this);
  };
};

class GroupingExpr : public Expression {
public:
  GroupingExpr(Expression &expr) : expr(expr) {}
  ExpressionType getType() const override { return GROUPING_EXPR; };

  Expression &expr;

protected:
  RuntimeValue
  do_accept(const ExpressionVisitor<RuntimeValue> &visitor) override {
    return visitor.visit(*this);
  };

  void do_accept(const ExpressionVisitor<void> &visitor) override {
    return visitor.visit(*this);
  };
};

class LiteralExpr : public Expression {
public:
  LiteralExpr(Literal value) : value(value) {}
  LiteralExpr() : value(Literal{}) {}
  ExpressionType getType() const override { return LITERAL_EXPR; };

  Literal value;

protected:
  RuntimeValue
  do_accept(const ExpressionVisitor<RuntimeValue> &visitor) override {
    return visitor.visit(*this);
  };

  void do_accept(const ExpressionVisitor<void> &visitor) override {
    return visitor.visit(*this);
  };
};

class UnaryExpr : public Expression {
public:
  UnaryExpr(Token op, Expression &right) : op(op), right(right){};
  ExpressionType getType() const override { return UNARY_EXPR; };

  Token op;
  Expression &right;

protected:
  RuntimeValue
  do_accept(const ExpressionVisitor<RuntimeValue> &visitor) override {
    return visitor.visit(*this);
  };

  void do_accept(const ExpressionVisitor<void> &visitor) override {
    return visitor.visit(*this);
  };
};

class CallExpr : public Expression {
public:
  CallExpr(Expression *callee, Token paren, std::vector<Expression *> arguments)
      : callee(callee), paren(paren), arguments(arguments){};
  ExpressionType getType() const override { return CALL_EXPR; };

  Expression *callee;
  Token paren;
  std::vector<Expression *> arguments;

protected:
  RuntimeValue
  do_accept(const ExpressionVisitor<RuntimeValue> &visitor) override {
    return visitor.visit(*this);
  };

  void do_accept(const ExpressionVisitor<void> &visitor) override {
    return visitor.visit(*this);
  };
};

class VariableReferenceExpr : public Expression {
public:
  VariableReferenceExpr(Token op) : op(op){};
  ExpressionType getType() const override { return VARIABLE_REFERENCE_EXPR; };

  Token op;

protected:
  RuntimeValue
  do_accept(const ExpressionVisitor<RuntimeValue> &visitor) override {
    return visitor.visit(*this);
  };

  void do_accept(const ExpressionVisitor<void> &visitor) override {
    return visitor.visit(*this);
  };
};

class IndexExpr : public Expression {
public:
  IndexExpr(Expression *obj, Expression *index) : obj(obj), index(index){};
  ExpressionType getType() const override { return INDEX_EXPR; };

  Expression *obj;
  Expression *index;

protected:
  RuntimeValue
  do_accept(const ExpressionVisitor<RuntimeValue> &visitor) override {
    return visitor.visit(*this);
  };

  void do_accept(const ExpressionVisitor<void> &visitor) override {
    return visitor.visit(*this);
  };
};

class SetIndexExpr : public Expression {
public:
  SetIndexExpr(Expression *obj, Expression *index, Expression *value)
      : obj(obj), index(index), value(value){};
  ExpressionType getType() const override { return INDEX_EXPR; };

  Expression *obj;
  Expression *index;
  Expression *value;

protected:
  RuntimeValue
  do_accept(const ExpressionVisitor<RuntimeValue> &visitor) override {
    return visitor.visit(*this);
  };

  void do_accept(const ExpressionVisitor<void> &visitor) override {
    return visitor.visit(*this);
  };
};

class GetExpr : public Expression {
public:
  GetExpr(Expression *obj, Token name) : obj(obj), name(name){};
  ExpressionType getType() const override { return GET_EXPR; };

  Expression *obj;
  Token name;

protected:
  RuntimeValue
  do_accept(const ExpressionVisitor<RuntimeValue> &visitor) override {
    return visitor.visit(*this);
  };

  void do_accept(const ExpressionVisitor<void> &visitor) override {
    return visitor.visit(*this);
  };
};

class SetExpr : public Expression {
public:
  SetExpr(Expression *obj, Token name, Expression *value)
      : obj(obj), name(name), value(value){};
  ExpressionType getType() const override { return SET_EXPR; };

  Expression *obj;
  Expression *value;
  Token name;

protected:
  RuntimeValue
  do_accept(const ExpressionVisitor<RuntimeValue> &visitor) override {
    return visitor.visit(*this);
  };

  void do_accept(const ExpressionVisitor<void> &visitor) override {
    return visitor.visit(*this);
  };
};

class ThisExpr : public Expression {
public:
  ThisExpr(Token keyword) : keyword(keyword){};
  ExpressionType getType() const override { return THIS_EXPR; };

  Token keyword;

protected:
  RuntimeValue
  do_accept(const ExpressionVisitor<RuntimeValue> &visitor) override {
    return visitor.visit(*this);
  };

  void do_accept(const ExpressionVisitor<void> &visitor) override {
    return visitor.visit(*this);
  };
};

class ArrayExpr : public Expression {
public:
  ArrayExpr(std::vector<Expression *> values)
      : values(values), length(new LiteralExpr((float)(values.size()))) {}
  ArrayExpr(Expression *length) : length(length) {}
  ExpressionType getType() const override { return ARRAY_EXPR; }

  std::vector<Expression *> values;
  Expression *length;

protected:
  RuntimeValue
  do_accept(const ExpressionVisitor<RuntimeValue> &visitor) override {
    return visitor.visit(*this);
  };

  void do_accept(const ExpressionVisitor<void> &visitor) override {
    return visitor.visit(*this);
  };
};

// ====================
// ==== STATEMENTS ====
// ====================

class Statement : public Expression {};

class ExpressionStmt : public Statement {
public:
  ExpressionStmt(Expression *expression) : expression(expression) {}
  ExpressionType getType() const override { return EXPRESSION_STMT; };

  Expression *expression;

protected:
  RuntimeValue
  do_accept(const ExpressionVisitor<RuntimeValue> &visitor) override {
    return visitor.visit(*this);
  };

  void do_accept(const ExpressionVisitor<void> &visitor) override {
    return visitor.visit(*this);
  };
};

class PrintStmt : public Statement {
public:
  PrintStmt(Expression *expression) : expression(expression) {}
  ExpressionType getType() const override { return PRINT_STMT; };

  Expression *expression;

protected:
  RuntimeValue
  do_accept(const ExpressionVisitor<RuntimeValue> &visitor) override {
    return visitor.visit(*this);
  };

  void do_accept(const ExpressionVisitor<void> &visitor) override {
    return visitor.visit(*this);
  };
};

class VariableDeclarationStmt : public Statement {
public:
  VariableDeclarationStmt(Token name, Expression *initializer)
      : name(name), initializer(initializer) {}
  ExpressionType getType() const override { return VARIABLE_DECLARATION_STMT; };

  Token name;
  Expression *initializer;

protected:
  RuntimeValue
  do_accept(const ExpressionVisitor<RuntimeValue> &visitor) override {
    return visitor.visit(*this);
  };

  void do_accept(const ExpressionVisitor<void> &visitor) override {
    return visitor.visit(*this);
  };
};

class AssignmentStmt : public Statement {
public:
  AssignmentStmt(Token name, Expression *value) : name(name), value(value) {}
  ExpressionType getType() const override { return ASSIGNMENT_STMT; };

  Token name;
  Expression *value;

protected:
  RuntimeValue
  do_accept(const ExpressionVisitor<RuntimeValue> &visitor) override {
    return visitor.visit(*this);
  };

  void do_accept(const ExpressionVisitor<void> &visitor) override {
    return visitor.visit(*this);
  };
};

class BlockStmt : public Statement {
public:
  BlockStmt(std::vector<Statement *> statements) : statements(statements){};
  ExpressionType getType() const override { return BLOCK_STMT; };

  std::vector<Statement *> statements;

protected:
  RuntimeValue
  do_accept(const ExpressionVisitor<RuntimeValue> &visitor) override {
    return visitor.visit(*this);
  };

  void do_accept(const ExpressionVisitor<void> &visitor) override {
    return visitor.visit(*this);
  };
};

class IfStmt : public Statement {
public:
  IfStmt(Expression *condition, Statement *then_branch, Statement *else_branch)
      : condition(condition), then_branch(then_branch),
        else_branch(else_branch){};
  ExpressionType getType() const override { return IF_STMT; };

  Expression *condition;
  Statement *then_branch;
  Statement *else_branch;

protected:
  RuntimeValue
  do_accept(const ExpressionVisitor<RuntimeValue> &visitor) override {
    return visitor.visit(*this);
  };

  void do_accept(const ExpressionVisitor<void> &visitor) override {
    return visitor.visit(*this);
  };
};

class WhileStmt : public Statement {
public:
  WhileStmt(Expression *condition, Statement *body)
      : condition(condition), body(body){};
  ExpressionType getType() const override { return WHILE_STMT; };

  Expression *condition;
  Statement *body;

protected:
  RuntimeValue
  do_accept(const ExpressionVisitor<RuntimeValue> &visitor) override {
    return visitor.visit(*this);
  };

  void do_accept(const ExpressionVisitor<void> &visitor) override {
    return visitor.visit(*this);
  };
};

class FunctionDeclarationStmt : public Statement {
public:
  FunctionDeclarationStmt(Token name, std::vector<Token> params,
                          std::vector<Statement *> body)
      : name(name), params(params), body(body){};
  ExpressionType getType() const override { return FUNCTION_DECLARATION_STMT; };

  Token name;
  std::vector<Token> params;
  std::vector<Statement *> body;

protected:
  RuntimeValue
  do_accept(const ExpressionVisitor<RuntimeValue> &visitor) override {
    return visitor.visit(*this);
  };

  void do_accept(const ExpressionVisitor<void> &visitor) override {
    return visitor.visit(*this);
  };
};

class ReturnStmt : public Statement {
public:
  ReturnStmt(Token keyword, Expression *value)
      : keyword(keyword), value(value){};
  ExpressionType getType() const override { return RETURN_STMT; };

  Token keyword;
  Expression *value;

protected:
  RuntimeValue
  do_accept(const ExpressionVisitor<RuntimeValue> &visitor) override {
    return visitor.visit(*this);
  };

  void do_accept(const ExpressionVisitor<void> &visitor) override {
    return visitor.visit(*this);
  };
};

class ClassStmt : public Statement {
public:
  ClassStmt(Token name, std::vector<FunctionDeclarationStmt> methods)
      : name(name), methods(methods){};
  ExpressionType getType() const override { return CLASS_STMT; };

  Token name;
  std::vector<FunctionDeclarationStmt> methods;

protected:
  RuntimeValue
  do_accept(const ExpressionVisitor<RuntimeValue> &visitor) override {
    return visitor.visit(*this);
  };

  void do_accept(const ExpressionVisitor<void> &visitor) override {
    return visitor.visit(*this);
  };
};
