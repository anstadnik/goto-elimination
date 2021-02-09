#include "header.hpp"

using namespace termcolor;

namespace statement {
Expr::Expr(string label) : label(move(label)) {}

Assign::Assign(string label, string var, string op)
    : Expr(move(label)), var(move(var)), op(move(op)) {}

If::If(string label, string condition,
                         Stmt::ptr true_branch,
                         Stmt::ptr false_branch)
    : Expr(move(label)),
      condition(move(condition)),
      true_branch(move(true_branch)),
      false_branch(move(false_branch)) {}
Print::Print(string label, string var)
    : Expr(move(label)), var(move(var)) {};
Goto::Goto(string label, string condition, string dest)
    : Expr(move(label)), condition(move(condition)), dest(move(dest)) {};

While::While(string label, Stmt::ptr body, string condition)
    : Expr(move(label)), body(move(body)), condition(move(condition)) {}

/* Expression::operator string() { return "// Label: " + label + "\n"; } */
Expr::operator string() { return label + ": "; }
Assign::operator string() {
  return Expr::operator string() + var + "=" + op + ";\n";
}
If::operator string() {
  return Expr::operator string() + "if (" + condition + ") {\n" +
         (true_branch ? static_cast<string>(*true_branch) + "\n": "") + "}\nelse {\n" +
         (false_branch ? static_cast<string>(*false_branch) + "\n": "") + "}\n";
}
While::operator string() {
  return Expr::operator string() + "while (" + condition + ") {\n" +
         static_cast<string>(*body) + "\n}\n";
}
Print::operator string() {
  return Expr::operator string() + "std::cout << " + var + " << std::endl;\n";
}
Goto::operator string() {
  return Expr::operator string() + "if (" + condition + ") {\ngoto " +
         dest + "\n}\n";
}

/* ostream& operator<<(ostream& os, statement::Expression::ptr a) { */
/*   return os << a.to_st; */
/* } */
}  // namespace program
