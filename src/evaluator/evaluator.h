#pragma once

#include "../parser/expression.h"
#include "../variable/environment.h"

class Evaluator : public ExpressionVisitor<RuntimeValue *> {
public:
  Evaluator() : line(1) {
    globals = new Environment();
    environment = globals;
  }

  RuntimeValue *evaluate(Expression *expression);

  RuntimeValue *visit(BinaryExpr *expr) override;
  RuntimeValue *visit(GroupingExpr *expr) override;
  RuntimeValue *visit(LiteralExpr *expr) override;
  RuntimeValue *visit(UnaryExpr *expr) override;
  RuntimeValue *visit(CallExpr *expr) override;
  RuntimeValue *visit(VariableReferenceExpr *expr) override;
  RuntimeValue *visit(GetExpr *expr) override;
  RuntimeValue *visit(SetExpr *expr) override;
  RuntimeValue *visit(ThisExpr *expr) override;
  RuntimeValue *visit(ArrayExpr *expr) override;
  RuntimeValue *visit(IndexExpr *expr) override;
  RuntimeValue *visit(SetIndexExpr *expr) override;

  RuntimeValue *visit(ExpressionStmt *stmt) override;
  RuntimeValue *visit(PrintStmt *stmt) override;
  RuntimeValue *visit(VariableDeclarationStmt *stmt) override;
  RuntimeValue *visit(AssignmentStmt *stmt) override;
  RuntimeValue *visit(BlockStmt *stmt) override;
  RuntimeValue *visit(IfStmt *stmt) override;
  RuntimeValue *visit(WhileStmt *stmt) override;
  RuntimeValue *visit(ReturnStmt *stmt) override;
  RuntimeValue *visit(ClassStmt *stmt) override;
  RuntimeValue *visit(FunctionDeclarationStmt *stmt) override;

  RuntimeValue *lookup_variable(Token name, Expression *expr);

  void execute_block(BlockStmt block, Environment *env);
  void resolve(Expression *expression, int depth);

  Environment *environment;
  Environment *globals;
  std::unordered_map<Expression *, int> locals;
  int line;
};
