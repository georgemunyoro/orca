#pragma once

#include "../lexer.h"

class Environment;
class Evaluator;

enum RuntimeValueType { RT_NUMBER, RT_BOOL, RT_STRING, RT_NIL };

class RuntimeValue {
public:
  RuntimeValue() : literal_value(), is_literal_value(true){};
  RuntimeValue(Literal value) : literal_value(value), is_literal_value(true) {}
  RuntimeValue(std::string value) {
    literal_value = Literal();
    literal_value.value = value;
    literal_value.type = STR;
  }

  virtual bool is_callable();
  bool is_string();
  bool is_number();
  bool is_bool();
  bool is_nil();

  RuntimeValueType get_type() const;

  float as_number() const;
  virtual std::string as_string() const;
  bool as_bool() const;

  bool is_truthy();

  Literal literal_value;
  bool is_literal_value;
};
