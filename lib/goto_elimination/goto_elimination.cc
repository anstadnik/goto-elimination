#include "goto_elimination.h"

using namespace ast;
using namespace std;

namespace goto_elimination {
Stmt::ptr eliminateGoto(Stmt::ptr stmt) {
  std::cout << "Starting eliminating gotos:" << std::endl << *stmt << std::endl;

  /* while not empty(goto _li st) do { */
  for (auto g = stmt->find_type<Goto>(); g != stmt->end();
       g = stmt->find_type<Goto>()) {
    std::cout << "Working on " << g->label << std::endl;
    /* select the next goto/label pair */
    /* g := select a goto from goto_list */
    /* l := label matching g */
    auto l = stmt->find(get<Goto>(g->contents).dest);
    assert(l != stmt->end());
    assert(!holds_alternative<Goto>(l->contents));

    // Impossible, because I implement "else" as a separate if
    /* force g and l to be directly related */
    if (indirectly_related(g, l)) {
    /* No check for different_statements since I don't implement it */

    /* move g out using outward-movement transformation until it becomes
     * directly related to an if or switch containing l */
    while (!directly_related(g, l))
    {
      std::cout << "Moving " << g->label
                << " outward to make it directly_related with " << l->label
                << std::endl;
      g = move_outward(g);
      cout << *stmt << endl;
    }

    /* If g and l are in different statements (functions) move g out using
     * outward-movement transf. until it becomes directly related to l */
    }

    /* force g and l to be siblings */
    if (directly_related(g, l)) {
      if (level(*g) > level(*l)) /* then move g out to level(l) */
                                 /* using outward-movement transformations */
        while (level(*g) > level(*l)) {
          std::cout << "Moving " << g->label << " outward" << std::endl;
          g = move_outward(g);
          cout << *stmt << endl;
        }
      else { /* level(g) < level(l) */
        if (offset(*g) > offset(*l)) {
          /* then lift g to above stmt containing l */
          /* using goto-lifting transformations */
          Expr* par = l->par_stmt->par_expr;
          while (level(*g) != level(*par)) par = par->par_stmt->par_expr;
          std::cout << "Lifting " << g->label << " , before " << par->label
                    << std::endl;
          g = lift(g, par->par_stmt->find_direct_child(*par));
          std::cout << *stmt << std::endl;
        }
        /* move g in to level(l) using inward-movement transf. */
        while (level(*g) < level(*l)) {
          // Where we want to move the goto
          auto target = l;
          while (level(*g) != level(*target))
            target = target->par_stmt->par_expr->par_stmt->find_direct_child(
                *target->par_stmt->par_expr);
          std::cout << "Moving " << g->label << " inward, into "
                    << target->label << std::endl;
          g = move_inward(g, target);
          cout << *stmt << endl;
        }
      }
    }

    /* g and l are guaranteed to be siblings, eliminate g */
    /* if (offset(g) < offset(l)) { */
    /* then eliminate g with a conditional */
    /* else */
    /* eliminate g with a do-loop */
    std::cout << "Eliminating " << g->label << std::endl;
    eliminate(g);
    cout << *stmt << endl;
    /* } */

    /* eliminate labels */
    /* for each label Li in label_list */
    /* do */
    /* eliminate Li */
  }

  return stmt;
}
}  // namespace goto_elimination
