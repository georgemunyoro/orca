#include "environment.h"

void Environment::define(std::string name, RuntimeValue *value) {
  final.insert_or_assign(name, value);
}

RuntimeValue *Environment::assign(std::string name, RuntimeValue *value) {
  if (final.count(name) > 0) {
    final.insert_or_assign(name, value);
    return value;
  }

  if (enclosing != nullptr) {
    enclosing->assign(name, value);
    return value;
  }

  throw std::runtime_error("Undefined variable '" + name + "'.");
}

RuntimeValue *Environment::get(Token name) {
  if (final.count(name.lexeme) > 0)
    return final.at(name.lexeme);

  if (enclosing != nullptr)
    return enclosing->get(name);

  throw std::runtime_error("Undefined variable '" + name.lexeme + "'.");
}

RuntimeValue *Environment::assign_at(int distance, std::string name,
                                     RuntimeValue *value) {
  Environment *env = ancestor(distance);
  return env->assign(name, value);
}

RuntimeValue *Environment::get_at(int distance, Token name) {
  return ancestor(distance)->get(name);
}

Environment *Environment::ancestor(int distance) {
  Environment *env = this;
  for (int i = 0; i < distance; ++i) {
    if (env->enclosing != nullptr)
      env = env;
    else
      return env;
  }
  return env;
}
