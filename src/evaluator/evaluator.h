#pragma once

#include "../parser/expression.h"
#include "../variable/environment.h"

class Evaluator {
public:
  Environment *environment;
  RuntimeValue evaluate(Expression *expression);
  void execute_block(BlockStmt block, Environment *env);
  void resolve(Expression *expression, int depth){};
};
