#include "../evaluator/evaluator.h"
#include "../parser/expression.h"

enum ResolverFunctionType { RES_FN_NONE, RES_FN_FUNCTION, RES_FN_METHOD };
enum ResolverClassType { RES_CL_NONE, RES_CL_CLASS };

class Resolver : ExpressionVisitor<void> {
public:
  Resolver(Evaluator *evaluator)
      : evaluator(evaluator), current_function(RES_FN_NONE),
        current_class(RES_CL_NONE){};

  void resolve_expr(Expression *expression);
  void resolve(std::vector<Expression *> exprs);

  void visit(BinaryExpr expr) override;
  void visit(GroupingExpr expr) override;
  void visit(LiteralExpr expr) override;
  void visit(UnaryExpr expr) override;
  void visit(CallExpr expr) override;
  void visit(VariableReferenceExpr expr) override;
  void visit(GetExpr expr) override;
  void visit(SetExpr expr) override;
  void visit(ThisExpr expr) override;
  void visit(ArrayExpr expr) override;
  void visit(IndexExpr expr) override;
  void visit(SetIndexExpr expr) override;

  void visit(ExpressionStmt stmt) override;
  void visit(PrintStmt stmt) override;
  void visit(VariableDeclarationStmt stmt) override;
  void visit(AssignmentStmt stmt) override;
  void visit(BlockStmt stmt) override;
  void visit(IfStmt stmt) override;
  void visit(WhileStmt stmt) override;
  void visit(ReturnStmt stmt) override;
  void visit(ClassStmt stmt) override;
  void visit(FunctionDeclarationStmt stmt) override;

  void resolve_function(FunctionDeclarationStmt declaration,
                        ResolverFunctionType type);
  void resolve_local(Expression &expression, Token name);
  void declare(Token name);
  void define(Token name);
  void begin_scope();
  void end_scope();

  Evaluator *evaluator;
  std::vector<std::unordered_map<std::string, bool>> scopes;
  ResolverFunctionType current_function;
  ResolverClassType current_class;
};
