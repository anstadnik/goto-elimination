#include <variant>
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

  Stmt::Iterator _move_outward_if(Stmt::Iterator it, Stmt& par_s) {
    // Not a reference, cause we will detele it
    const Goto e = get<Goto>(it->contents);
    const string label = it->label;

    const string suffix = to_string(level(*it)) + "_out_" + label;
    const string bool_name = "_cond_" + suffix;

    // Insert condition
    par_s.insert(it, Expr(bool_name, Assign{bool_name, e.cond}));
    // Put everything after the goto in If
    Stmt::ptr extracted = par_s.extract_from(next(it)->label);
    par_s.insert(it, Expr("_if_" + suffix, If{move(extracted), "!" + bool_name}));
    par_s.remove(label);
    // Insert goto after it's previous parent
    Stmt::Iterator par_e =
        par_s.par_expr->par_stmt->find(par_s.par_expr->label);
    Stmt::Iterator next_e = next(par_e);
    while (level(*next_e) != level(*par_e)) ++next_e;

    it = par_e->par_stmt->insert(next_e, Expr(label, Goto{e.dest, bool_name}));

    return it;
  }

  Stmt::Iterator move_outward(Stmt::Iterator it) {
    assert(holds_alternative<Goto>(it->contents));
    Stmt& par_s = *it->par_stmt;
    assert(par_s.par_expr);

    if (holds_alternative<If>(par_s.par_expr->contents))
      return _move_outward_if(it, par_s);
    else if (holds_alternative<While>(it->contents))
      throw runtime_error("Not implemented");
    else
      /* Here could be logic for switch */
      throw runtime_error("I don't know this type of expression");
    return it;
  }

  Stmt::Iterator move_inward(Stmt::Iterator it, Stmt::Iterator target) {
    assert(holds_alternative<Goto>(it->contents));
    assert(it->par_stmt == target->par_stmt->par_expr->par_stmt);

    const Goto& e = get<Goto>(it->contents);
    Stmt& par_s = *it->par_stmt;
    const string& label = it->label;

    if (holds_alternative<If>(target->contents)) {
      const string suffix = to_string(level(*it)) + "_in_";
      string bool_name = "_cond_" + suffix + label;
      // Insert condition
      par_s.insert(it, Expr(bool_name, Assign{bool_name, e.cond}));
      // Put everything between the goto and target in If
      auto&& extracted = par_s.extract_from(next(it)->label, target->label);
      par_s.insert(it, Expr("_if_" + suffix + label,
                            If{move(extracted), "!" + bool_name}));
      par_s.remove(label);
      // Change the condition
      get<If>(par_s.find_direct_child(*target)->contents).cond += " || " + bool_name;
      // Insert goto after it's previous parent
      auto& target_s = get<If>(target->contents);
      if (target_s.true_branch->size())
        it = target_s.true_branch->insert(target_s.true_branch->begin(),
            Expr(label, Goto{e.dest, bool_name}));
      else
        it = target_s.true_branch->insert(target_s.true_branch->end(),
            Expr(label, Goto{e.dest, bool_name}));

    } else if (holds_alternative<While>(it->contents)) {
      throw runtime_error("Not implemented");

    } else
        /* Here could be logic for switch */
      throw runtime_error("I don't know this type of expression");

    return it;
  }

  Stmt::Iterator lift(Stmt::Iterator it, size_t offset) {
    (void)it;
    (void)offset;
    return it;
  }
  
} /* goto_elimination */ 
