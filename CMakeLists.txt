cmake_minimum_required(VERSION 3.20.0)
project(orclang)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build source
add_executable(
    orc
    src/main.cpp
    src/lexer.cpp
    src/utils.cpp
    src/interpreter.cpp
    src/parser/expression.cpp
    src/parser/parser.cpp
    src/parser/statement.cpp
    src/parser/ast_printer.cpp
    src/evaluator/evaluator.cpp
    src/evaluator/runtime_value.cpp
    src/evaluator/runtime_callable.cpp
    src/variable/environment.cpp
    src/variable/resolver.cpp
)
