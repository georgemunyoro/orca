#pragma once

#include "../evaluator/runtime_value.h"
#include <string>
#include <unordered_map>

class Environment {
public:
  Environment() : enclosing(nullptr){};
  Environment(Environment *enclosing) : enclosing(enclosing){};

  void define(std::string name, RuntimeValue *value);

  RuntimeValue *assign(std::string name, RuntimeValue *value);
  RuntimeValue *get(Token name);
  RuntimeValue *assign_at(int distance, std::string name, RuntimeValue *value);
  RuntimeValue *get_at(int distance, Token name);

  Environment *ancestor(int distance);

  std::unordered_map<std::string, RuntimeValue *> final;
  Environment *enclosing;
};
