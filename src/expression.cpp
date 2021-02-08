#include "header.hpp"

namespace statement {
Expression::Expression(string label) : label(move(label)) {}

Assignment::Assignment(string label, string var, string op)
    : Expression(move(label)), var(move(var)), op(move(op)) {}

Conditional::Conditional(string label, string condition,
                         Statement::ptr true_branch,
                         Statement::ptr false_branch)
    : Expression(move(label)),
      condition(move(condition)),
      true_branch(move(true_branch)),
      false_branch(move(false_branch)) {}
Print::Print(string label, string var)
    : Expression(move(label)), var(move(var)) {};
Goto::Goto(string label, string condition, string dest)
    : Expression(move(label)), condition(move(condition)), dest(move(dest)) {};

WhileLoop::WhileLoop(string label, Statement::ptr body, string condition)
    : Expression(move(label)), body(move(body)), condition(move(condition)) {}

/* Expression::operator string() { return "// Label: " + label + "\n"; } */
Expression::operator string() { return label + ": "; }
Assignment::operator string() {
  return Expression::operator string() + var + "=" + op + ";\n";
}
Conditional::operator string() {
  return Expression::operator string() + "if (" + condition + ") {\n" +
         (true_branch ? static_cast<string>(*true_branch) + "\n": "") + "}\nelse {\n" +
         (false_branch ? static_cast<string>(*false_branch) + "\n": "") + "}\n";
}
WhileLoop::operator string() {
  return Expression::operator string() + "while (" + condition + ") {\n" +
         static_cast<string>(*body) + "\n}\n";
}
Print::operator string() {
  return Expression::operator string() + "std::cout << " + var + " << std::endl;\n";
}
Goto::operator string() {
  return Expression::operator string() + "if (" + condition + ") {\ngoto " +
         dest + "\n}\n";
}

/* ostream& operator<<(ostream& os, statement::Expression::ptr a) { */
/*   return os << a.to_st; */
/* } */
}  // namespace program
