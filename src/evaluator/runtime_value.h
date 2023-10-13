#pragma once

#include "../lexer.h"

class Environment;
class Evaluator;

enum RuntimeValueType {
  RT_NUMBER,
  RT_BOOL,
  RT_STRING,
  RT_NIL,
  RT_FUNCTION,
  RT_CALLABLE,
  RT_ARRAY
};

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

  virtual RuntimeValueType get_type() const;

  float as_number() const;
  virtual std::string as_string() const;
  bool as_bool() const;

  bool is_truthy();

  Literal literal_value;
  bool is_literal_value;
};

class RuntimeArrayValue : public RuntimeValue {
public:
  RuntimeArrayValue(std::vector<RuntimeValue *> values)
      : array_values(values){};

  std::vector<RuntimeValue *> array_values;

  void set(int index, RuntimeValue *updated_value) {
    if (index < 0 || index >= array_values.size())
      throw std::runtime_error("Index key out of bounds.");

    array_values[index] = updated_value;
  }

  RuntimeValue *get(int index) {
    if (index < 0 || index >= array_values.size())
      throw std::runtime_error("Index key out of bounds.");

    return array_values[index];
  }

  RuntimeValueType get_type() const override { return RT_ARRAY; }
};
