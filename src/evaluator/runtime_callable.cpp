#include "runtime_callable.h"

bool RuntimeCallable::is_callable() { return true; }

std::string RuntimeCallable::as_string() const { return "<func>"; }

int RuntimeCallable::arity() const { return 0; };

RuntimeValue RuntimeCallable::call(Evaluator &evaluator,
                                   std::vector<RuntimeValue> arguments) {
  return RuntimeValue();
}

// =======================
// === RuntimeFunction ===
// =======================

RuntimeValue RuntimeFunction::call(Evaluator &evaluator,
                                   std::vector<RuntimeValue> arguments) {
  Environment *env =
      new Environment(closure == nullptr ? evaluator.environment : closure);

  for (int i = 0; i < arguments.size(); ++i) {
    env->define(declaration.params[i].lexeme, arguments[i]);
  }

  evaluator.execute_block(BlockStmt(declaration.body), env);

  return RuntimeValue();
}

std::string RuntimeFunction::as_string() const {
  return "<func : " + declaration.name.lexeme + ">";
}

int RuntimeFunction::arity() const { return declaration.params.size(); }
