#include "ast.h"

namespace statement {
Stmt* getNestedStmt(const Expr& p) {
  if (auto i = get_if<If>(&p.contents))
    return i->true_branch->size() ? i->true_branch.get() : nullptr;
  else if (auto i = get_if<While>(&p.contents))
    return i->body->size() ? i->body.get() : nullptr;
  return nullptr;
}

bool Stmt::Iterator::ensure_not_end() {
  // If reached the end of the current statement, try to go to upper level
  if (!history.empty() && ++pointer(it) == it->par_stmt->end()) {
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
