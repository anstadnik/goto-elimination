#include "ast.h"

namespace ast {

bool Stmt::Iterator::goDeeper() {
  if (auto i = get_if<If>(&it->contents); i && i->branch->size() )
    it = i->branch.get()->begin().it;
  else if (auto i = get_if<While>(&it->contents); i && i->body->size())
    it = i->body.get()->begin().it;
  else
    return false;
  return true;
}

bool Stmt::Iterator::ensureNotEnd() {
  // If reached the end of the current statement, try to go to upper level
  if (Expr* par_expr = it->par_stmt->par_expr;
      it->par_stmt != top_stmt && ++pointer(it) == it->par_stmt->end().it &&
      par_expr) {
    it = par_expr->par_stmt->find(par_expr->label).it;
    return ensureNotEnd();
  }
  return true;
}

Stmt::Iterator& Stmt::Iterator::operator++() {
  /* If current iterator points at if or while, which has body, go deeper. */
  if (goDeeper()) return *this;
  /* If we reached the end of current statement, try to go upper (to the parent
   * if or while statement */
  ensureNotEnd();
  ++it;
  return *this;
}
}  // namespace ast
