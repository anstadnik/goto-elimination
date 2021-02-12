#include "3rd_party/termcolor.hpp"
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

/* ostream& operator<<(ostream& os, statement::Expression a) { */
/*   return os << a.to_st; */
/* } */
}  // namespace statement
