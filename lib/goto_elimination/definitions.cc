#include "goto_elimination.h"

using namespace ast;

namespace goto_elimination {

size_t offset(const Expr& e) {
  Stmt* parent = e.par_stmt;
  while (parent->par_expr) 
    parent = parent->par_expr->par_stmt;
  return distance(parent->begin(), parent->find(e.label));
}

size_t level(const Expr& e) {
  if (!e.par_stmt)
    return 0;
  const Expr* p = e.par_stmt->par_expr;
  if (!p)
    return 0;
  size_t level = 1;
  while ((p = p->par_stmt->par_expr))
    level++;
  return level;
}

bool siblings(const Stmt::Iterator& g, const Stmt::Iterator& l) {
  return g->par_stmt == l->par_stmt;
}

bool directly_related(const Stmt::Iterator& g, const Stmt::Iterator& l) {
  for (auto parent = g->par_stmt->par_expr; parent;
       parent = parent->par_stmt->par_expr)
    if (l->par_stmt == parent->par_stmt) return true;
  for (auto parent = l->par_stmt->par_expr; parent;
       parent = parent->par_stmt->par_expr)
    if (g->par_stmt == parent->par_stmt) return true;
  return false;
}

bool indirectly_related(const Stmt::Iterator& g, const Stmt::Iterator& l) {
  return !siblings(g, l) && !directly_related(g, l);
}

}  // namespace goto_elimination
