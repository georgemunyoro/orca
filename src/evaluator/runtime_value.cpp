#include "runtime_value.h"

bool RuntimeValue::is_callable() { return false; }
bool RuntimeValue::is_truthy() {
  if (is_nil())
    return false;

  if (is_bool())
    return as_bool();

  return true;
}
bool RuntimeValue::is_string() { return literal_value.type == STR; }
bool RuntimeValue::is_number() { return literal_value.type == NUM; }
bool RuntimeValue::is_bool() { return literal_value.type == BOOL; }
bool RuntimeValue::is_nil() { return literal_value.type == NIL_; }

RuntimeValueType RuntimeValue::get_type() const {
  if (is_literal_value) {
    switch (literal_value.type) {
    case NUM:
      return RT_NUMBER;
    case BOOL:
      return RT_BOOL;
    case NIL_:
      return RT_NIL;
    case STR:
      return RT_STRING;
    }
  }

  throw std::runtime_error("Unable to implicitly discern type of value.");
}

std::string RuntimeValue::as_string() const {
  return literal_value.as_string();
}

bool RuntimeValue::as_bool() const {
  if (get_type() != RT_BOOL)
    throw std::runtime_error("Attempted to convert non-bool value into bool.");
  return std::get<bool>(literal_value.value);
}

float RuntimeValue::as_number() const {
  if (get_type() != RT_NUMBER)
    throw std::runtime_error(
        "Attempted to convert non-numeric value into number.");
  return std::get<float>(literal_value.value);
}
