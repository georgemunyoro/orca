#include "../variable/environment.h"
#include "./evaluator.h"
#include "./runtime_value.h"

class RuntimeCallable : public RuntimeValue {
public:
  RuntimeCallable() { this->is_literal_value = false; }
  bool is_callable() override;
  std::string as_string() const override;

  virtual int arity() const;
  virtual RuntimeValue call(Evaluator &evaluator,
                            std::vector<RuntimeValue> arguments);
};

class Environment;

class RuntimeFunction : public RuntimeCallable {
public:
  RuntimeFunction(FunctionDeclarationStmt declaration, Environment *closure)
      : declaration(declaration), closure(closure) {}

  RuntimeFunction bind(RuntimeValue instance) {
    Environment *environment = new Environment(closure);
    environment->define("this", instance);
    return RuntimeFunction(declaration, environment);
  }

  RuntimeValue call(Evaluator &evaluator,
                    std::vector<RuntimeValue> arguments) override;
  std::string as_string() const override;
  int arity() const override;

  FunctionDeclarationStmt declaration;
  Environment *closure = nullptr;
};
