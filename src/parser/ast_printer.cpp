#include "ast_printer.h"
#include "expression.h"
#include <iostream>

void AstPrinter::print(Expression *expr) {
  expr->accept(*this);
  printf("\n");
}

void AstPrinter::print_indent() {
  int i = indent;
  while (i--) {
    printf("    ");
  }
}

void AstPrinter::visit(BinaryExpr *expr) {
  printf("\n");
  print_indent();
  printf("Binary %s", expr->op.lexeme.c_str());
  ++indent;
  expr->left.accept<void>(*this);
  expr->right.accept<void>(*this);
  --indent;
};

void AstPrinter::visit(LiteralExpr *expr) {
  printf("\n");
  print_indent();
  printf("%s", expr->value.as_string().c_str());
};

void AstPrinter::visit(GroupingExpr *expr) {
  printf("\n");
  print_indent();
  printf("Grouping");
  ++indent;
  expr->expr.accept(*this);
  --indent;
};

void AstPrinter::visit(UnaryExpr *expr) {
  printf("\n");
  print_indent();
  printf("Unary %s ", expr->op.lexeme.c_str());
  ++indent;
  expr->right.accept(*this);
  --indent;
};

void AstPrinter::visit(CallExpr *expr) {
  printf("\n");
  print_indent();
  printf("Call");
  ++indent;
  expr->callee->accept(*this);
  printf(" <-- CALLEE");
  for (auto arg : expr->arguments) {
    arg->accept(*this);
  }
  --indent;
}

void AstPrinter::visit(VariableReferenceExpr *expr) {
  printf("\n");
  print_indent();
  printf("VarRef %s", expr->op.lexeme.c_str());
};

void AstPrinter::visit(GetExpr *expr) {
  printf("\n");
  print_indent();
  printf("Get %s", expr->name.lexeme.c_str());
  ++indent;
  expr->obj->accept(*this);
  --indent;
};

void AstPrinter::visit(SetExpr *expr) {
  printf("\n");
  print_indent();
  printf("Set %s", expr->name.lexeme.c_str());
  ++indent;
  expr->obj->accept(*this);
  expr->value->accept(*this);
  --indent;
};

void AstPrinter::visit(ThisExpr *expr) {
  printf("\n");
  print_indent();
  printf("This");
};

void AstPrinter::visit(IndexExpr *expr) {
  printf("\n");
  print_indent();
  printf("Index ");
  ++indent;
  expr->index->accept(*this);
  expr->obj->accept(*this);
  --indent;
};

void AstPrinter::visit(SetIndexExpr *expr) {
  printf("\n");
  print_indent();
  printf("SetIndex");
  ++indent;
  expr->index->accept(*this);
  expr->obj->accept(*this);
  expr->value->accept(*this);
  --indent;
};

void AstPrinter::visit(ArrayExpr *expr) {
  printf("\n");
  print_indent();
  printf("Array");
  ++indent;
  expr->length->accept(*this);

  printf("\n");
  print_indent();
  printf("--------");

  for (Expression *value : expr->values) {
    value->accept(*this);
  }
  --indent;
};

void AstPrinter::visit(ExpressionStmt *stmt) {
  printf("\n");
  print_indent();
  printf("ExpressionStmt");
  ++indent;
  stmt->expression->accept(*this);
  --indent;
}

void AstPrinter::visit(PrintStmt *stmt) {
  printf("\n");
  print_indent();
  printf("Print");
  ++indent;
  stmt->expression->accept(*this);
  --indent;
}

void AstPrinter::visit(VariableDeclarationStmt *stmt) {
  printf("\n");
  print_indent();
  printf("VarDecl %s", stmt->name.lexeme.c_str());
  ++indent;
  stmt->initializer->accept(*this);
  --indent;
}

void AstPrinter::visit(AssignmentStmt *stmt) {
  printf("\n");
  print_indent();
  printf("Assign %s ", stmt->name.lexeme.c_str());
  ++indent;
  stmt->value->accept(*this);
  --indent;
};

void AstPrinter::visit(BlockStmt *stmt) {
  printf("\n");
  print_indent();
  printf("BLOCK");
  ++indent;
  for (Statement *e : stmt->statements) {
    e->accept(*this);
  }
  --indent;
};

void AstPrinter::visit(IfStmt *stmt) {
  printf("\n");
  print_indent();
  printf("If");
  ++indent;
  stmt->condition->accept(*this);
  stmt->then_branch->accept(*this);
  stmt->else_branch->accept(*this);
  --indent;
};

void AstPrinter::visit(WhileStmt *stmt) {
  printf("\n");
  print_indent();
  printf("While");
  ++indent;
  stmt->condition->accept(*this);
  stmt->body->accept(*this);
  --indent;
};

void AstPrinter::visit(ReturnStmt *stmt) {
  printf("\n");
  print_indent();
  printf("Return");
  ++indent;
  stmt->value->accept(*this);
  --indent;
};

void AstPrinter::visit(ClassStmt *stmt) {
  printf("\n");
  print_indent();
  printf("Class %s", stmt->name.lexeme.c_str());
  ++indent;
  for (auto method : stmt->methods) {
    method.accept(*this);
    printf("\n");
  }
  --indent;
};

void AstPrinter::visit(FunctionDeclarationStmt *stmt) {
  printf("\n");
  print_indent();
  printf("FunDecl %s: ", stmt->name.lexeme.c_str());
  for (auto param : stmt->params) {
    printf("%s ", param.lexeme.c_str());
  }
  ++indent;
  BlockStmt(stmt->body).accept(*this);
  --indent;
};
