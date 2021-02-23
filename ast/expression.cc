#include "statement.h"

using namespace termcolor;

namespace statement {

While::While(string label, Stmt::ptr body, string condition)
    : BaseExpr(move(label)), body(move(body)), condition(move(condition)) {
  this->body->parent_stmt = parent_stmt ? &*parent_stmt->find(label) : nullptr;
}

If::If(string label, string condition, Stmt::ptr true_branch)
    : BaseExpr(move(label)),
      condition(move(condition)),
      true_branch(move(true_branch)) {
  this->true_branch->parent_stmt = parent_stmt ? &*parent_stmt->find(label) : nullptr;
}

string get_label(const Expr& v) {
  return visit([](auto&& expr) { return expr.label; }, v);
}
Stmt* get_parent_stmt(const Expr& v) {
  return visit([](auto&& expr) { return expr.parent_stmt; }, v);
}
Expr* get_parent_stmt(const Stmt& v) { return v.parent_stmt; }
void set_parent_stmt(Expr& v, Stmt* parent_stmt) {
  visit([](auto&& expr) -> Stmt*& { return expr.parent_stmt; }, v) =
      parent_stmt;
}

Stmt::Iterator& Stmt::Iterator::operator++() {
  // If reached the end of the current statement, try to go to upper level
  if (it == get_parent_stmt(*it)->end()) {
    if (auto i = get_parent_stmt(*get_parent_stmt(*get_parent_stmt(*it))))
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
