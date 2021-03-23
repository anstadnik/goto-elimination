#include "goto_elimination.h"

using namespace ast;
using namespace std;

namespace goto_elimination
{
  Stmt::Iterator move_outward(Stmt::Iterator it) {
    assert(holds_alternative<Goto>(it->contents));
  
    const Goto& e = get<Goto>(it->contents);
    Stmt& parent = *it->par_stmt;
    assert(parent.par_expr);
    if (holds_alternative<If>(parent.par_expr->contents)) {
      string bool_name = "_cond_" + it->label;
      // Insert condition
      parent.insert(it->label, Expr(bool_name, Assign{bool_name, e.cond}));
      // Put everything after the goto in If
      parent.insert(it->label, Expr("_if_" + it->label,
                                    If{parent.extract_from(it, parent.end()),
                                       "!" + bool_name}));
      parent.remove(it->label);
      // Insert goto outside it's previous parent
      parent.par_expr->par_stmt->insert(parent.par_expr->label,
          Expr(it->label, Goto{e.dest, bool_name}));
    } else if (holds_alternative<While>(it->contents)) {
      throw runtime_error("Not implemented");
    } else
        /* Here could be logic for switch */
      throw runtime_error("I don't know this type of expression");
    return it;
  }

  Stmt::Iterator move_inward(Stmt::Iterator it) {
    (void)it;
    return it;
  }
  
  Stmt::Iterator lift(Stmt::Iterator it, size_t offset) {
    (void)it;
    (void)offset;
    return it;
  }
  
} /* goto_elimination */ 
