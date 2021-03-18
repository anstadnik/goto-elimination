#include "statement.h"

using namespace termcolor;

namespace statement {

string getLabel(const Expr& v) {
  return visit([](auto&& expr) { return expr.label; }, v);
}

Stmt* BaseExpr::getParentStmt() const { return parent_stmt; }

Stmt* getParentStmt(const Stmt& v) {
  if (auto p = getParentExpr(v)) return getParentStmt(*p);
  return nullptr;
}
Stmt* getParentStmt(const Expr& v) {
  return visit([](auto&& expr) { return expr.getParentStmt(); }, v);
}
void setParentStmt(Expr& v, Stmt* parent_stmt) {
  if (auto p = get_if<If>(&v)) setParentExpr(*p->true_branch, &v);
  if (auto p = get_if<While>(&v)) setParentExpr(*p->body, &v);
  visit([parent_stmt](auto&& expr) { return expr.parent_stmt = parent_stmt; },
        v);
}
Expr* getParentExpr(const Expr& v) { return getParentExpr(*getParentStmt(v)); }
Expr* getParentExpr(const Stmt& v) { return v.parent_expr; }

void setParentExpr(Stmt& v, Expr* parent_expr) {
  assert(!v.parent_expr);
  v.parent_expr = parent_expr;
}

Stmt* getNestedStmt(const Expr& p) {
  if (auto i = get_if<If>(&p))
    return i->true_branch->size() ? i->true_branch.get() : nullptr;
  else if (auto i = get_if<While>(&p))
    return i->body->size() ? i->body.get() : nullptr;
  return nullptr;
}

bool Stmt::Iterator::ensure_not_end() {
  // If reached the end of the current statement, try to go to upper level
  if (!history.empty() && ++pointer(it) == getParentStmt(*it)->end()) {
    it = history.top();
    history.pop();
    return ensure_not_end();
  }

  return true;
}

Stmt::Iterator& Stmt::Iterator::operator++() {
  if (auto i = getNestedStmt(*it)) {
    history.push(it);
    it = i->begin().it;
    return *this;
  }
  ensure_not_end();
  ++it;
  return *this;
}

}  // namespace statement
