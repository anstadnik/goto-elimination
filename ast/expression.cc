#include "statement.h"

using namespace termcolor;

namespace statement {

string get_label(const Expr& v) {
  return visit([](auto&& expr) { return expr.label; }, v);
}
Stmt* get_parent_stmt(const Expr& v) {
  return visit([](auto&& expr) { return expr.parent_stmt; }, v);
}
Expr* get_parent_expr(const Stmt& v) { return v.parent_expr; }
Expr* get_parent_expr(const Expr& v) { return get_parent_expr(*get_parent_stmt(v)); }
void set_parent_stmt(Expr& v, Stmt* parent_stmt) {
  if (auto p = get_if<If>(&v))
    set_parent_expr(*p->true_branch, &v);
  if (auto p = get_if<While>(&v))
    set_parent_expr(*p->body, &v);
  visit([](auto&& expr) -> Stmt*& { return expr.parent_stmt; }, v) =
      parent_stmt;
}

void set_parent_expr(Stmt& v, Expr* parent_expr) {
  v.parent_expr = parent_expr;
}

Stmt::Iterator& Stmt::Iterator::operator++() {
  // If reached the end of the current statement, try to go to upper level
  if (it == get_parent_stmt(*it)->end()) {
    if (auto i = get_parent_stmt(*get_parent_expr(*it)))
      it = i->begin().it;
    else if (auto i = get_if<If>(&*it))
      it = i->true_branch->begin().it;
    else if (auto i = get_if<While>(&*it))
      it = i->body->begin().it;
  } else
    ++it;

  return *this;
}

}  // namespace statement
