#include "header.hpp"

using namespace termcolor;

namespace statement {
BaseExpr::BaseExpr(string label) : label(move(label)) {}

Assign::Assign(string label, string var, string op)
    : BaseExpr(move(label)), var(move(var)), op(move(op)) {}

If::If(string label, string condition, Stmt true_branch)
    : BaseExpr(move(label)),
      condition(move(condition)),
      true_branch(move(true_branch)) {}
Print::Print(string label, string var)
    : BaseExpr(move(label)), var(move(var)){};
Goto::Goto(string label, string condition, string dest)
    : BaseExpr(move(label)), condition(move(condition)), dest(move(dest)){};

While::While(string label, Stmt body, string condition)
    : BaseExpr(move(label)), body(move(body)), condition(move(condition)) {}

/* Expression::operator string() const { return "// Label: " + label + "\n"; }
 */
BaseExpr::operator string() const { return label + ": "; }
Assign::operator string() const {
  return BaseExpr::operator string() + var + "=" + op + ";\n";
}
If::operator string() const {
  return BaseExpr::operator string() + "if (" + condition + ") {\n" +
         static_cast<string>(true_branch) + "\n" + "}\n";
}
While::operator string() const {
  return BaseExpr::operator string() + "while (" + condition + ") {\n" +
         static_cast<string>(body) + "\n}\n";
}
Print::operator string() const {
  return BaseExpr::operator string() + "std::cout << " + var +
         " << std::endl;\n";
}
Goto::operator string() const {
  return BaseExpr::operator string() + "if (" + condition + ") {\ngoto " +
         dest + "\n}\n";
}

string get_string(const Expr& v) {
  return visit([](auto&& expr) { return string(expr); }, v);
}
string get_label(const Expr& v) {
  return visit([](auto&& expr) { return expr.label; }, v);
}

/* ostream& operator<<(ostream& os, statement::Expression a) { */
/*   return os << a.to_st; */
/* } */
}  // namespace statement
