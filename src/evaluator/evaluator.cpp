#include "evaluator.h"
#include "../parser/ast_printer.h"
#include "runtime_callable.h"
#include "runtime_value.h"

RuntimeValue operator-(RuntimeValue lhs, RuntimeValue const &rhs) {
  if (lhs.get_type() == RT_NUMBER && rhs.get_type() == RT_NUMBER) {
    return RuntimeValue(lhs.as_number() - rhs.as_number());
  }

  throw "Cannot add different types.";
}

RuntimeValue operator+(RuntimeValue lhs, RuntimeValue const &rhs) {
  if (lhs.get_type() == RT_STRING && rhs.get_type() == RT_STRING) {
    return RuntimeValue(lhs.as_string() + rhs.as_string());
  }

  if (lhs.get_type() == RT_NUMBER && rhs.get_type() == RT_NUMBER) {
    return RuntimeValue(lhs.as_number() + rhs.as_number());
  }

  throw "Cannot add different types.";
}

RuntimeValue operator*(RuntimeValue lhs, RuntimeValue const &rhs) {
  if (lhs.get_type() == RT_NUMBER && rhs.get_type() == RT_NUMBER) {
    return RuntimeValue(lhs.as_number() * rhs.as_number());
  }

  throw "Cannot perform substraction on different types.";
}

RuntimeValue operator/(RuntimeValue lhs, RuntimeValue const &rhs) {
  if (lhs.get_type() == RT_NUMBER && rhs.get_type() == RT_NUMBER) {
    return RuntimeValue(lhs.as_number() / rhs.as_number());
  }

  throw "Cannot perform division on different types.";
}

bool operator==(RuntimeValue lhs, RuntimeValue const &rhs) {
  if (lhs.get_type() == RT_STRING && rhs.get_type() == RT_STRING)
    return lhs.as_string() == rhs.as_string();

  if (lhs.get_type() == RT_NUMBER && rhs.get_type() == RT_NUMBER)
    return lhs.as_number() == rhs.as_number();

  if (lhs.get_type() == RT_BOOL && rhs.get_type() == RT_BOOL)
    return lhs.as_bool() == rhs.as_bool();

  if (lhs.get_type() == RT_NIL && rhs.get_type() == RT_NIL)
    return true;

  throw "Cannot perform division on different types.";
}

bool operator!=(RuntimeValue lhs, RuntimeValue const &rhs) {
  return !(lhs == rhs);
}

bool operator<(RuntimeValue lhs, RuntimeValue const &rhs) {
  if (lhs.get_type() == RT_NUMBER && rhs.get_type() == RT_NUMBER)
    return lhs.as_number() < rhs.as_number();

  throw "Cannot perform division on different types.";
}

bool operator>(RuntimeValue lhs, RuntimeValue const &rhs) {
  if (lhs.get_type() == RT_NUMBER && rhs.get_type() == RT_NUMBER)
    return lhs.as_number() > rhs.as_number();

  throw "Cannot perform division on different types.";
}

bool operator<=(RuntimeValue lhs, RuntimeValue const &rhs) {
  return lhs < rhs || lhs == rhs;
}

bool operator>=(RuntimeValue lhs, RuntimeValue const &rhs) {
  return lhs > rhs || lhs == rhs;
}

void Evaluator::execute_block(BlockStmt block, Environment *env) {
  Environment *previous = environment;
  environment = env;

  try {
    for (Statement *statement : block.statements) {
      evaluate(statement);
    }
  } catch (RuntimeValue *return_value) {
    environment = previous;
    throw return_value;
  }

  environment = previous;
}

void Evaluator::resolve(Expression *expression, int depth) {
  locals.insert_or_assign(expression, depth);
}

RuntimeValue *Evaluator::evaluate(Expression *expression) {
  return expression->accept(*this);
}

RuntimeValue *Evaluator::visit(BinaryExpr *expr) {
  RuntimeValue *left = expr->left.accept(*this);
  RuntimeValue *right = expr->right.accept(*this);

  line = expr->op.line;

  switch (expr->op.type) {
  case MINUS:
    return new RuntimeValue(*left - *right);
  case SLASH:
    return new RuntimeValue(*left / *right);
  case STAR:
    return new RuntimeValue(*left * *right);
  case PLUS:
    return new RuntimeValue(*left + *right);

  case GREATER:
    return new RuntimeValue(*left > *right);
  case GREATER_EQUAL:
    return new RuntimeValue(*left >= *right);
  case LESS:
    return new RuntimeValue(*left < *right);
  case LESS_EQUAL:
    return new RuntimeValue(*left <= *right);
  case EQUAL_EQUAL:
    return new RuntimeValue(*left == *right);
  case BANG_EQUAL:
    return new RuntimeValue(*left != *right);
  }

  return new RuntimeValue();
};

RuntimeValue *Evaluator::visit(GroupingExpr *expr) {
  return expr->accept(*this);
};

RuntimeValue *Evaluator::visit(LiteralExpr *expr) {
  return new RuntimeValue(expr->value);
};

RuntimeValue *Evaluator::visit(UnaryExpr *expr) {
  RuntimeValue *right = expr->right.accept(*this);

  line = expr->op.line;

  switch (expr->op.type) {
  case MINUS: {
    if (right->is_number())
      return new RuntimeValue(RuntimeValue(float(0)) - *right);
    break;
  }
  case BANG:
    return new RuntimeValue(!right->is_truthy());

  default:
    throw "Expected unary operation, found " +
                             expr->op.lexeme;
  }

  return new RuntimeValue();
};

RuntimeValue *Evaluator::visit(CallExpr *expr) {
  RuntimeValue *callee = evaluate(expr->callee);

  std::vector<RuntimeValue *> arguments;
  for (auto arg : expr->arguments)
    arguments.push_back(evaluate(arg));

  if (callee->is_callable()) {
    RuntimeFunction *callee_callable = (RuntimeFunction *)callee;
    if (arguments.size() != callee_callable->arity()) {
      throw "Received incorrect number of args.";
    }
    return callee_callable->call(this, arguments);
  }

  throw "Attempted to call non-callable object.";
};

RuntimeValue *Evaluator::visit(VariableReferenceExpr *expr) {
  return lookup_variable(expr->op, expr);
};

RuntimeValue *Evaluator::lookup_variable(Token name, Expression *expr) {
  if (locals.count(expr) > 0) {
    int distance = locals.at(expr);
    return environment->get_at(distance, name);
  }

  return globals->get(name);
}

RuntimeValue *Evaluator::visit(GetExpr *expr) {
  RuntimeValue *obj = evaluate(expr->obj);

  if (obj->get_type() == RT_INSTANCE)
    return ((RuntimeClassInstance *)obj)->get(expr->name);

  throw "Only object instances have properties.";
};

RuntimeValue *Evaluator::visit(SetExpr *expr) {
  RuntimeValue *obj = evaluate(expr->obj);

  if (obj->get_type() != RT_INSTANCE)
    throw "Only instances have fields.";

  RuntimeValue *value = evaluate(expr->value);
  ((RuntimeClassInstance *)obj)->set(expr->name, value);

  return value;
};

RuntimeValue *Evaluator::visit(ThisExpr *expr) {
  return lookup_variable(expr->keyword, expr);
};

RuntimeValue *Evaluator::visit(ArrayExpr *expr) {
  std::vector<RuntimeValue *> values;
  int array_length = evaluate(expr->length)->as_number();

  for (int i = 0; i < array_length; ++i) {
    if (i >= expr->values.size())
      values.push_back(new RuntimeValue());
    else
      values.push_back(evaluate(expr->values[i]));
  }

  return new RuntimeArrayValue(values);
};

RuntimeValue *Evaluator::visit(IndexExpr *expr) {
  RuntimeValue *index = evaluate(expr->index);
  RuntimeValue *obj = evaluate(expr->obj);

  if (obj->get_type() != RT_ARRAY && obj->get_type() != RT_STRING)
    throw "Index should be into an array or string.";

  if (!index->is_number())
    throw "Index key should be a number.";

  if (obj->get_type() == RT_STRING) {
    if (index->as_number() < 0 || index->as_number() >= obj->as_string().size())
      throw "Index key not in range.";

    return new RuntimeValue(
        std::string(1, obj->as_string()[index->as_number()]));
  }

  return ((RuntimeArrayValue *)obj)->get(index->as_number());
};

RuntimeValue *Evaluator::visit(SetIndexExpr *expr) {
  RuntimeValue *value = evaluate(expr->value);
  RuntimeValue *index = evaluate(expr->index);
  RuntimeValue *obj = evaluate(expr->obj);

  if (obj->get_type() != RT_ARRAY)
    throw "Index should be into an array.";

  if (!index->is_number())
    throw "Index key should be a number.";

  if (index->as_number() < 0 ||
      index->as_number() >= ((RuntimeArrayValue *)obj)->array_values.size())
    throw "Index key not in range.";

  ((RuntimeArrayValue *)obj)->set(index->as_number(), value);

  return new RuntimeValue();
};

RuntimeValue *Evaluator::visit(ExpressionStmt *stmt) {
  return evaluate(stmt->expression);
};

RuntimeValue *Evaluator::visit(PrintStmt *stmt) {
  RuntimeValue *value = stmt->expression->accept(*this);
  printf("%s\n", value->as_string().c_str());
  return new RuntimeValue();
};

RuntimeValue *Evaluator::visit(VariableDeclarationStmt *stmt) {
  environment->define(stmt->name.lexeme, evaluate(stmt->initializer));
  return new RuntimeValue();
};

RuntimeValue *Evaluator::visit(AssignmentStmt *stmt) {
  RuntimeValue *value = evaluate(stmt->value);

  if (locals.count(stmt)) {
    int distance = locals.at(stmt);
    environment->assign_at(distance, stmt->name.lexeme, value);
  } else {
    environment->assign(stmt->name.lexeme, value);
  }

  return value;
};

RuntimeValue *Evaluator::visit(BlockStmt *stmt) {
  execute_block(*stmt, new Environment(environment));
  return new RuntimeValue();
};

RuntimeValue *Evaluator::visit(IfStmt *stmt) {
  if (evaluate(stmt->condition)->is_truthy()) {
    evaluate(stmt->then_branch);
  } else {
    evaluate(stmt->else_branch);
  }

  return new RuntimeValue();
};

RuntimeValue *Evaluator::visit(WhileStmt *stmt) {
  while (evaluate(stmt->condition)->is_truthy()) {
    evaluate(stmt->body);
  }

  return new RuntimeValue();
};

RuntimeValue *Evaluator::visit(ReturnStmt *stmt) {
  throw evaluate(stmt->value);
};

RuntimeValue *Evaluator::visit(ClassStmt *stmt) {
  environment->define(stmt->name.lexeme, new RuntimeValue());

  std::unordered_map<std::string, RuntimeFunction *> methods;
  for (FunctionDeclarationStmt method : stmt->methods) {
    RuntimeFunction *function = new RuntimeFunction(method, environment);
    methods.insert_or_assign(method.name.lexeme, function);
  }

  RuntimeClass *class_ = new RuntimeClass(stmt->name.lexeme, methods);
  environment->assign(stmt->name.lexeme, class_);

  return new RuntimeValue();
};

RuntimeValue *Evaluator::visit(FunctionDeclarationStmt *stmt) {
  environment->define(stmt->name.lexeme,
                      new RuntimeFunction(*stmt, environment));
  return new RuntimeValue();
};
