
#include "expression.h"

class AstPrinter : public ExpressionVisitor<void> {
public:
  AstPrinter(){};

  void print(Expression *expr);

  void visit(BinaryExpr *expr) override;
  void visit(GroupingExpr *expr) override;
  void visit(LiteralExpr *expr) override;
  void visit(UnaryExpr *expr) override;
  void visit(CallExpr *expr) override;
  void visit(VariableReferenceExpr *expr) override;
  void visit(GetExpr *expr) override;
  void visit(SetExpr *expr) override;
  void visit(ThisExpr *expr) override;
  void visit(ArrayExpr *expr) override;
  void visit(IndexExpr *expr) override;
  void visit(SetIndexExpr *expr) override;

  void visit(ExpressionStmt *stmt) override;
  void visit(PrintStmt *stmt) override;
  void visit(VariableDeclarationStmt *stmt) override;
  void visit(AssignmentStmt *stmt) override;
  void visit(BlockStmt *stmt) override;
  void visit(IfStmt *stmt) override;
  void visit(WhileStmt *stmt) override;
  void visit(ReturnStmt *stmt) override;
  void visit(ClassStmt *stmt) override;
  void visit(FunctionDeclarationStmt *stmt) override;

private:
  int indent = 0;
  void print_indent();
};
