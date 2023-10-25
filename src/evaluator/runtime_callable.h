#include "../variable/environment.h"
#include "./evaluator.h"
#include "./runtime_value.h"

class RuntimeCallable : public RuntimeValue {
public:
  RuntimeCallable() { this->is_literal_value = false; }
  bool is_callable() override;
  std::string as_string() const override;

  RuntimeValueType get_type() const override { return RT_CALLABLE; };

  virtual int arity() const;
  virtual RuntimeValue *call(Evaluator *evaluator,
                             std::vector<RuntimeValue *> arguments);
};

class Environment;

class RuntimeFunction : public RuntimeCallable {
public:
  RuntimeFunction(FunctionDeclarationStmt declaration, Environment *closure)
      : declaration(declaration), closure(closure) {}

  RuntimeFunction *bind(RuntimeValue *instance) {
    Environment *environment = new Environment(closure);
    environment->define("this", instance);
    return new RuntimeFunction(declaration, environment);
  }

  RuntimeValue *call(Evaluator *evaluator,
                     std::vector<RuntimeValue *> arguments) override;
  std::string as_string() const override;
  int arity() const override;

  bool is_callable() override { return true; };

  RuntimeValueType get_type() const override { return RT_FUNCTION; };

  FunctionDeclarationStmt declaration;
  Environment *closure = nullptr;
};

class RuntimeClass;

RuntimeFunction *find_class_method(RuntimeClass *class_, std::string name);
std::string get_class_name(RuntimeClass *class_);

class RuntimeClassInstance : public RuntimeValue {
public:
  RuntimeClassInstance(RuntimeClass *class_) : class_(class_) {
    is_literal_value = false;
  }

  RuntimeValueType get_type() const override { return RT_INSTANCE; }

  std::string as_string() const override {
    return "<instance : " + get_class_name(class_) + ">";
  }

  void set(Token name, RuntimeValue *obj) {
    fields.insert_or_assign(name.lexeme, obj);
  }

  RuntimeValue *get(Token name) {

    if (fields.count(name.lexeme) > 0)
      return fields.at(name.lexeme);

    RuntimeFunction *method = find_class_method(class_, name.lexeme);
    if (method != nullptr) {
      return method->bind(this);
    }

    throw "Undefined property " + name.lexeme;
  }

  RuntimeClass *class_;
  std::unordered_map<std::string, RuntimeValue *> fields;
};

class RuntimeClass : public RuntimeCallable {
public:
  RuntimeClass(std::string name,
               std::unordered_map<std::string, RuntimeFunction *> methods)
      : name(name), methods(methods) {}

  std::string as_string() const override { return "<class: " + name + ">"; }

  RuntimeValue *call(Evaluator *evaluator,
                     std::vector<RuntimeValue *> arguments) override {
    RuntimeClassInstance *instance = new RuntimeClassInstance(this);

    RuntimeFunction *initializer = find_method("init");
    if (initializer != nullptr) {
      initializer->bind((RuntimeValue *)instance)->call(evaluator, arguments);
    }

    return instance;
  }

  RuntimeFunction *find_method(std::string name) const {
    if (methods.count(name) > 0)
      return methods.at(name);
    return nullptr;
  }

  int arity() const override {
    RuntimeFunction *init_method = find_method("init");
    if (init_method != nullptr)
      return init_method->arity();
    return 0;
  }

  std::string name;
  std::unordered_map<std::string, RuntimeFunction *> methods;
};
