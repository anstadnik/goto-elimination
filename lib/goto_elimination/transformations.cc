#include "goto_elimination.h"

using namespace ast;
using namespace std;

namespace goto_elimination
{
  Stmt& get_top_parent(const Expr& e) {
    if (e.par_stmt->par_expr)
      return get_top_parent(*e.par_stmt->par_expr);
    return *e.par_stmt;
  }

  Stmt::Iterator move_outward(Stmt::Iterator it) {
    assert(holds_alternative<Goto>(it->contents));
  
    const Goto& e = get<Goto>(it->contents);
    Stmt& parent = *it->par_stmt;
    const string& label = it->label;
    assert(parent.par_expr);

    if (holds_alternative<If>(parent.par_expr->contents)) {
      string bool_name = "_cond_" + label;
      // Insert condition
      parent.insert(label, Expr(bool_name, Assign{bool_name, e.cond}));
      // Put everything after the goto in If
      auto&& dummy = parent.extract_from(next(it)->label);
      parent.insert(label,
                    Expr("_if_" + label, If{move(dummy), "!" + bool_name}));
      parent.remove(label);
      // Insert goto after it's previous parent
      it = parent.par_expr->par_stmt->insert(parent.par_expr->label,
          Expr(label, Goto{e.dest, bool_name}), true);

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
