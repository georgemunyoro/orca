#include "ast_printer.h"
#include "expression.h"
#include <iostream>

void AstPrinter::print(Expression *expr) {
  expr->accept(*this);
  printf("\n");
}

void AstPrinter::visit(BinaryExpr expr) const {
  printf("( Binary %s ", expr.op.lexeme.c_str());
  expr.left.accept<void>(*this);
  expr.right.accept<void>(*this);
  printf(" )");
};

void AstPrinter::visit(LiteralExpr expr) const {
  printf(" %s ", expr.value.as_string().c_str());
};

void AstPrinter::visit(GroupingExpr expr) const {
  printf("[");
  expr.expr.accept(*this);
  printf("]");
};

void AstPrinter::visit(UnaryExpr expr) const {
  printf("( Unary %s ", expr.op.lexeme.c_str());
  expr.right.accept(*this);
  printf(" )");
};

void AstPrinter::visit(CallExpr expr) const {
  printf("( Call");
  expr.callee->accept(*this);
  for (auto arg : expr.arguments) {
    printf("\n");
    arg->accept(*this);
  }
  printf(")");
}

void AstPrinter::visit(VariableReferenceExpr expr) const {
  printf("( VarRef %s )", expr.op.lexeme.c_str());
};

void AstPrinter::visit(GetExpr expr) const {
  printf("( Get %s", expr.name.lexeme.c_str());
  expr.obj->accept(*this);
  printf(")");
};

void AstPrinter::visit(SetExpr expr) const {
  printf("( Set %s", expr.name.lexeme.c_str());
  expr.obj->accept(*this);
  expr.value->accept(*this);
  printf(")");
};

void AstPrinter::visit(ThisExpr expr) const { printf(" This "); };

void AstPrinter::visit(IndexExpr expr) const {
  printf("( Index ");
  expr.index->accept(*this);
  expr.obj->accept(*this);
  printf(")");
};

void AstPrinter::visit(SetIndexExpr expr) const {
  printf("( SetIndex ");
  expr.index->accept(*this);
  expr.obj->accept(*this);
  expr.value->accept(*this);
  printf(")");
};

void AstPrinter::visit(ArrayExpr expr) const {
  printf("( Array[");
  expr.length->accept(*this);
  printf("]");
  for (Expression *value : expr.values) {
    value->accept(*this);
  }
  printf(")");
};

void AstPrinter::visit(ExpressionStmt stmt) const {
  stmt.expression->accept(*this);
}

void AstPrinter::visit(PrintStmt stmt) const {
  printf("( Print");
  stmt.expression->accept(*this);
  printf(")");
}

void AstPrinter::visit(VariableDeclarationStmt stmt) const {
  printf("( VarDecl %s", stmt.name.lexeme.c_str());
  stmt.initializer->accept(*this);
  printf(")");
}

void AstPrinter::visit(AssignmentStmt stmt) const {
  printf("( Assign %s ", stmt.name.lexeme.c_str());
  stmt.value->accept(*this);
  printf(")");
};

void AstPrinter::visit(BlockStmt stmt) const {
  printf("( BLOCK\n");
  for (Statement *e : stmt.statements) {
    e->accept(*this);
    printf("\n");
  }
  printf(")");
};

void AstPrinter::visit(IfStmt stmt) const {
  printf("( If");
  stmt.condition->accept(*this);
  stmt.then_branch->accept(*this);
  stmt.else_branch->accept(*this);
  printf(")");
};

void AstPrinter::visit(WhileStmt stmt) const {
  printf("( While");
  stmt.condition->accept(*this);
  stmt.body->accept(*this);
  printf(")");
};

void AstPrinter::visit(ReturnStmt stmt) const {
  printf("( Return");
  stmt.value->accept(*this);
  printf(")");
};

void AstPrinter::visit(ClassStmt stmt) const {
  printf("( Class %s", stmt.name.lexeme.c_str());
  for (auto method : stmt.methods) {
    printf("\n");
    method.accept(*this);
  }
  printf(")");
};

void AstPrinter::visit(FunctionDeclarationStmt stmt) const {
  printf("( FunDecl %s: ", stmt.name.lexeme.c_str());
  for (auto param : stmt.params) {
    printf(" %s", param.lexeme.c_str());
  }
  BlockStmt(stmt.body).accept(*this);
  printf(")");
};
