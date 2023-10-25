#include "interpreter.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
    return 1;
  }

  std::string filePath = argv[1];
  Interpreter terp = Interpreter();
  terp.run_file(filePath);

  return 0;
}