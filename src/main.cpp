#include "interpreter.h"
#include <iostream>

int main() {
  Interpreter terp = Interpreter();
  terp.run_file("/Users/georgeguvamatanga/workspace/georgemunyoro/orc-v2/"
                "examples/test.orc");
  return 0;
}
