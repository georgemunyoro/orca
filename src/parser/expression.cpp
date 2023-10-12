#include "expression.h"

template <>
RuntimeValue
Expression::accept(const ExpressionVisitor<RuntimeValue> &visitor) {
  return do_accept(visitor);
}

template <> void Expression::accept(const ExpressionVisitor<void> &visitor) {
  return do_accept(visitor);
}
