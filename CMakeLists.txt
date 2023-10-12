cmake_minimum_required(VERSION 3.20.0)
project(orclang)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

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

# Find the libraries that correspond to the LLVM components
llvm_map_components_to_libnames(llvm_libs support core irreader)

# Link against LLVM libraries
target_link_libraries(orc ${llvm_libs})
