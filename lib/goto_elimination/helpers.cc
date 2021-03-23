#include "goto_elimination.h"

using namespace ast;
using namespace std;

namespace goto_elimination
{
  Stmt* get_common_parent(const Stmt::Iterator& a, const Stmt::Iterator& b) {
    for (auto parent_a = a->par_stmt; parent_a->par_expr;
         parent_a = parent_a->par_expr->par_stmt)
      for (auto parent_b = b->par_stmt; parent_b->par_expr;
           parent_b = parent_b->par_expr->par_stmt)
        if (parent_a == parent_b) return parent_a;
    return nullptr;
  }
  
} /* goto_elimination */ 
