#include "resolver.h"
#include "../parser/ast_printer.h"

void Resolver::resolve_function(FunctionDeclarationStmt declaration,
                                ResolverFunctionType type) {
  ResolverFunctionType enclosing_type = current_function;
  current_function = type;
  begin_scope();

  for (Token param : declaration.params) {
    declare(param);
    define(param);
  }

  for (Expression *stmt : declaration.body)
    resolve_expr(stmt);

  end_scope();
  current_function = enclosing_type;
}

void Resolver::resolve_local(Expression *expression, Token name) {
  int i = scopes.size();

  while (i--) {
    if (scopes[i].count(name.lexeme) > 0) {
      evaluator->resolve(expression, scopes.size() - 1 - i);
      return;
    }
  }
}

void Resolver::declare(Token name) {
  if (scopes.empty())
    return;

  if (scopes.back().count(name.lexeme)) {
    throw std::runtime_error(
        "Attempted to declare variable '" + name.lexeme +
        "Attempted to declare variable '{name.lexeme}' when another variable "
        "was already declared with the same name within the scope.");
  }

  scopes.back().insert_or_assign(name.lexeme, false);
}

void Resolver::define(Token name) {
  if (scopes.empty())
    return;

  scopes.back().insert_or_assign(name.lexeme, true);
}

void Resolver::begin_scope() {
  scopes.push_back(std::unordered_map<std::string, bool>({}));
}

void Resolver::end_scope() { scopes.pop_back(); }

void Resolver::resolve_expr(Expression *expression) {
  expression->accept(*this);
}

void Resolver::resolve(std::vector<Expression *> exprs) {
  for (auto expr : exprs)
    resolve_expr(expr);
}

void Resolver::visit(BinaryExpr *expr) {
  resolve_expr(&expr->left);
  resolve_expr(&expr->right);
};

void Resolver::visit(GroupingExpr *expr) { resolve_expr(&expr->expr); };

void Resolver::visit(LiteralExpr *expr){};

void Resolver::visit(UnaryExpr *expr) { resolve_expr(&expr->right); };

void Resolver::visit(CallExpr *expr) {
  resolve_expr(expr->callee);
  for (auto arg : expr->arguments)
    resolve_expr(arg);
};

void Resolver::visit(VariableReferenceExpr *expr) {
  if (!scopes.empty() && (scopes.back().count(expr->op.lexeme) > 0
                              ? scopes.back().at(expr->op.lexeme)
                              : true) == false)
    throw std::runtime_error(
        "Cannot read local variable in it's own initializer");

  resolve_local(expr, expr->op);
};

void Resolver::visit(GetExpr *expr) { resolve_expr(expr->obj); };

void Resolver::visit(SetExpr *expr) {
  resolve_expr(expr->value);
  resolve_expr(expr->obj);
};

void Resolver::visit(ThisExpr *expr) {
  if (current_class == RES_CL_NONE)
    throw std::runtime_error("Invalid use of 'this' outside of a class.");
  resolve_local(expr, expr->keyword);
};

void Resolver::visit(ArrayExpr *expr) {
  for (auto val : expr->values)
    resolve_expr(val);
};

void Resolver::visit(IndexExpr *expr) { resolve_expr(expr->obj); };

void Resolver::visit(SetIndexExpr *expr) {
  resolve_expr(expr->value);

  resolve_expr(expr->index);

  resolve_expr(expr->obj);
};

void Resolver::visit(ExpressionStmt *stmt) { resolve_expr(stmt->expression); };

void Resolver::visit(PrintStmt *stmt) { resolve_expr(stmt->expression); };

void Resolver::visit(VariableDeclarationStmt *stmt) {
  declare(stmt->name);
  if (stmt->initializer != nullptr)
    resolve_expr(stmt->initializer);
  define(stmt->name);
};

void Resolver::visit(AssignmentStmt *stmt) {
  resolve_expr(stmt->value);
  resolve_local(stmt, stmt->name);
};

void Resolver::visit(BlockStmt *stmt) {
  begin_scope();
  for (Statement *stmt : stmt->statements)
    stmt->accept(*this);
  end_scope();
};

void Resolver::visit(IfStmt *stmt) {
  resolve_expr(stmt->condition);
  resolve_expr(stmt->then_branch);
  resolve_expr(stmt->else_branch);
};

void Resolver::visit(WhileStmt *stmt) {
  resolve_expr(stmt->condition);
  resolve_expr(stmt->body);
};

void Resolver::visit(ReturnStmt *stmt) {
  if (current_function == RES_FN_NONE)
    throw std::runtime_error("Invalid use of return outside of a function");
  resolve_expr(stmt->value);
};

void Resolver::visit(ClassStmt *stmt) {
  ResolverClassType enclosing_class = current_class;
  current_class = RES_CL_CLASS;

  declare(stmt->name);
  define(stmt->name);

  begin_scope();
  scopes.back().insert_or_assign("this", true);
  for (auto method : stmt->methods)
    resolve_function(method, RES_FN_METHOD);
  end_scope();
  current_class = enclosing_class;
};

void Resolver::visit(FunctionDeclarationStmt *stmt) {
  declare(stmt->name);
  define(stmt->name);
  resolve_function(*stmt, RES_FN_FUNCTION);
};
