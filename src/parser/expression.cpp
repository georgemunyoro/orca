#include "expression.h"

template <>
RuntimeValue *Expression::accept(ExpressionVisitor<RuntimeValue *> &visitor) {
  return do_accept(visitor);
}

template <> void Expression::accept(ExpressionVisitor<void> &visitor) {
  return do_accept(visitor);
}
