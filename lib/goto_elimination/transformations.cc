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

    const string prefix =  "_out_" + label + "_" + to_string(level(*it));
    const string bool_name =  prefix + "_cond_";

    // Insert condition
    par_s.insert(it, Expr(bool_name, Assign{bool_name, e.cond}));
    if (level(*next(it)) == level(*it)) {
      // Put everything after the goto in If
      Stmt::ptr branch = par_s.extract_from(next(it));
      par_s.insert(it,
                   Expr(prefix + "_if_", If{move(branch), "!" + bool_name}));
    }
    par_s.remove(label);
    // Insert goto after it's previous parent
    Stmt::Iterator par_e =
        par_s.par_expr->par_stmt->find(par_s.par_expr->label);
    Stmt::Iterator next_e = next(par_e);
    while (next_e != par_e->par_stmt->end() &&
           level(*next_e) != level(*par_e)) 
      ++next_e;

    it = par_e->par_stmt->insert(next_e, Expr(label, Goto{e.dest, bool_name}));

    return it;
  }

  Stmt::Iterator _move_outward_while(Stmt::Iterator it, Stmt& par_s) {
    // Not a reference, cause we will detele it
    const Goto e = get<Goto>(it->contents);
    const string label = it->label;

    const string prefix =  "_out_" + label + "_" + to_string(level(*it));
    const string bool_name =  prefix + "_cond_";

    // Insert condition
    par_s.insert(it, Expr(bool_name, Assign{bool_name, e.cond}));
    par_s.insert(it, Expr(prefix+ "_if_" , Break{e.cond}));

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
      return _move_outward_while(it, par_s);
    else
      /* Here could be logic for switch */
      throw runtime_error("I don't know this type of expression");
    return it;
  }

  Stmt::Iterator move_inward(Stmt::Iterator it, Stmt::Iterator target) {
    assert(holds_alternative<Goto>(it->contents));
    assert(it->par_stmt == target->par_stmt);

    // Not a reference, cause we will detele it
    const Goto e = get<Goto>(it->contents);
    const string label = it->label;
    Stmt& par_s = *it->par_stmt;

    const string prefix = "_in_" + label + "_" + to_string(level(*it));
    const string bool_name = prefix + "_cond_";

    // Insert condition
    par_s.insert(it, Expr(bool_name, Assign{bool_name, e.cond}));

    // Put everything between the goto and target in If
    if (level(*next(it)) == level(*it)) {
      auto&& branch = par_s.extract(next(it), target);
      par_s.insert(
          it, Expr(prefix + "_if_" + label, If{move(branch), "!" + bool_name}));
    }
    par_s.remove(label);

    Stmt* nested_s;
    // Change the condition and get nested body / branch
    if (holds_alternative<If>(target->contents)) {
      get<If>(par_s.find_direct_child(*target)->contents).cond +=
          " || " + bool_name;
      nested_s = get<If>(target->contents).branch.get();
    } else if (holds_alternative<While>(target->contents)) {
      get<While>(par_s.find_direct_child(*target)->contents).cond +=
          " || " + bool_name;
      nested_s = get<While>(target->contents).body.get();
    } else
      /* Here could be logic for switch */
      throw runtime_error("I don't know this type of expression");

    // Cannot be empty, cause there is something onto which the goto points
    assert(nested_s->size());
    // Add the goto
    it = nested_s->insert(nested_s->begin(),
                          Expr(label, Goto{e.dest, bool_name}));

    // Change the condition to false to avoid infinite loop
    nested_s->insert(nested_s->find(e.dest),
                     Expr(bool_name + "f_", Assign{bool_name, "0"}));

    return it;
  }

  Stmt::Iterator lift(Stmt::Iterator it, Stmt::Iterator target) {
    assert(level(*it) == level(*target));
    assert(offset(*it) > offset(*target));

    // Not a reference, cause we will detele it
    const Goto e = get<Goto>(it->contents);
    const string label = it->label;

    const string prefix = "_lift_" + label +"_" +  to_string(level(*it));

    Stmt& par_s = *it->par_stmt;

    // Insert conditionals
    par_s.insert(par_s.size() ? par_s.begin() : par_s.end(),
       Expr(prefix + "_goto_f_", Assign{prefix + "_goto_", "0"}));
    par_s.insert(par_s.size() ? par_s.begin() : par_s.end(),
       Expr(prefix + "_f_t_", Assign{prefix + "_f_", "1"}));

    // Save as a reference to the it position
    const auto& next_e = next(it);

    // Put everything between the target and it in If
    auto&& body = par_s.extract(target, it);

    // Change flags
    body->insert(body->begin(), Expr(prefix + "_f_f_", Assign{prefix + "_f_", "0"}));
    body->insert(body->end(), Expr(prefix + "_goto_", Assign{prefix + "_goto_", e.cond}));

    // Go in loop for the first time or if the goto cond is true
    const string while_cond = prefix + "_goto_" + " || " + prefix + "_f_";

    it = body->insert(body->find(target->label), Expr(label, Goto{e.dest, prefix + "_goto_"}));

    par_s.insert(next_e,
                 Expr(prefix + "_if_", While{move(body), while_cond}));
    par_s.remove(label);

    return it;
  }

  Stmt::Iterator eliminate(Stmt::Iterator it) {
    assert(holds_alternative<Goto>(it->contents));

    const Goto& g = get<Goto>(it->contents);
    Stmt& par_s = *it->par_stmt;
    const Stmt::Iterator& target = par_s.find(g.dest);
    assert(level(*it) == level(*target));
    const string label =  it->label;
    auto prefix = "_elim_" + label;

    if (offset(*it) < offset(*par_s.find(g.dest))) {
      // Remove everything until the target
      Stmt::ptr branch = par_s.extract(next(it), target);
      if (branch->size())
        it = par_s.insert(it, Expr(prefix, If{move(branch), "!" + g.cond}));
    } else {
      // Remove everything between the target and the goto
      Stmt::ptr body = par_s.extract(target, it);
      if (body->size()) {
        par_s.insert(it, Expr(prefix + "_f_t_", Assign{prefix + "_f_", "1"}));
        body->insert(body->begin(),
                     Expr(prefix + "_f_f_", Assign{prefix + "_f_", "0"}));
        it = par_s.insert(
            it, Expr(prefix,
                     While{move(body), g.cond + " || " + prefix + "_f_"}));
      }
    }
    par_s.find(label)->contents = Empty();
    return it;
  }

} /* goto_elimination */ 
