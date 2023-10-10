#pragma once

#include <string>

class Interpreter {
public:
  Interpreter() {}

  void run_file(std::string filepath);
  void run(std::string source);
};
