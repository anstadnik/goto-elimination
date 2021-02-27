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

Stmt * get_nested_stmt(const Expr& p) {
  if (auto i = get_if<If>(&p))
    return i->true_branch->size() ? i->true_branch.get() : nullptr;
  else if (auto i = get_if<While>(&p))
    return i->body->size() ? i->body.get() : nullptr;
  return nullptr;
}

bool Stmt::Iterator::ensure_not_end() {
  // If reached the end of the current statement, try to go to upper level
  if (!history.empty() && ++pointer(it) == get_parent_stmt(*it)->end()) {
    it = history.top();
    history.pop();
    return ensure_not_end();
  }

  return true;
}

Stmt::Iterator& Stmt::Iterator::operator++() {
  if (auto i = get_nested_stmt(*it)) {
    history.push(it);
    it = i->begin().it;
    return *this;
  }
  ensure_not_end();
  ++it;
  return *this;
}

}  // namespace statement
