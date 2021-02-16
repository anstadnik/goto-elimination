#include "statement.h"

using namespace termcolor;

namespace statement {

string get_label(const Expr& v) {
  return visit([](auto&& expr) { return expr.label; }, v);
}
Stmt* get_parent_stmt(Expr& v) {
  return visit([](auto&& expr) { return expr.parent_stmt; }, v);
}
Stmt* get_parent_stmt(Stmt& v) {
  return v.parent_stmt;
}
void set_parent_stmt(Expr& v, Stmt* parent_stmt) {
  visit([](auto&& expr) -> Stmt*& { return expr.parent_stmt; }, v) =
      parent_stmt;
}

Stmt::Iterator& Stmt::Iterator::operator++() {
  // If reached the end of the current statement, try to go to upper level
  if (it == get_parent_stmt(*it)->end()) {
    if (auto i = get_parent_stmt(*get_parent_stmt(*it)))
      it = i->begin().it;
    else if (auto i = get_if<If>(&*it))
      it = i->true_branch.begin().it;
    else if (auto i = get_if<While>(&*it))
      it = i->body.begin().it;
  } else
    ++it;

  return *this;
}

}  // namespace statement
