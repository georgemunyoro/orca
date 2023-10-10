#include "interpreter.h"
#include <iostream>

int main() {
  Interpreter terp = Interpreter();
  terp.run_file("./examples/test.orc");
  return 0;
}
