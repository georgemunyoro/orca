
#include "expression.h"

class AstPrinter : public ExpressionVisitor<void> {
public:
  AstPrinter(){};

  void print(Expression *expr);

  void visit(BinaryExpr expr) const override;
  void visit(GroupingExpr expr) const override;
  void visit(LiteralExpr expr) const override;
  void visit(UnaryExpr expr) const override;
  void visit(CallExpr expr) const override;
  void visit(VariableReferenceExpr expr) const override;
  void visit(GetExpr expr) const override;
  void visit(SetExpr expr) const override;
  void visit(ThisExpr expr) const override;
  void visit(ArrayExpr expr) const override;
  void visit(IndexExpr expr) const override;
  void visit(SetIndexExpr expr) const override;

  void visit(ExpressionStmt stmt) const override;
  void visit(PrintStmt stmt) const override;
  void visit(VariableDeclarationStmt stmt) const override;
  void visit(AssignmentStmt stmt) const override;
  void visit(BlockStmt stmt) const override;
  void visit(IfStmt stmt) const override;
  void visit(WhileStmt stmt) const override;
  void visit(ReturnStmt stmt) const override;
  void visit(ClassStmt stmt) const override;
  void visit(FunctionDeclarationStmt stmt) const override;

private:
  int indent = 0;
};
