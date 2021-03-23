#include "goto_elimination.h"

using namespace ast;
using namespace std;

namespace goto_elimination
{
  Stmt::ptr eliminateGoto(Stmt::ptr stmt) {
    /* while not empty(goto _li st) do { */
    for (auto g = stmt->find_type<Goto>(); g != stmt->end();
         g = stmt->find_type<Goto>()) {
      /* select the next goto/label pair */
      /* g := select a goto from goto_list */
      /* l := label matching g */
      auto l = stmt->find(get<Goto>(g->contents).dest);

      // Impossible, because I implement "else" as a separate if
      /* force g and l to be directly related */
      /* if (indirectly_related(g, l)) { */
        /* No check for different_statements since I don't implement it */

        /* move g out using outward-movement transformation until it becomes
         * directly related to an if or switch containing l */
        /* while (!directly_related(g, l)) { */
        /*   g = move_outward(g); */
        /*   cout << *stmt << endl; */
        /*   cin.get(); */
        /* } */

        /* If g and l are in different statements (functions) move g out using
         * outward-movement transf. until it becomes directly related to l */
      /* } */

      std::cout << g->label << " and " << l->label << " are directly related "
                << std::endl;
      std::cout << *stmt << std::endl;
      break;

      /* force g and l to be siblings */
      if (directly_related(g, l)) {
        if (level(*g) > level(*l)) /* then move g out to level(l) */
                                   /* using outward-movement transformations */
          while (level(*g) > level(*l)) {
            g = move_outward(g);
            cout << *stmt << endl;
            cin.get();
          }
        else { /* level(g) < level(l) */
          if (offset(*g) > offset(*l))
            /* then lift g to above stmt containing l */
            /* using goto-lifting transformations */
            lift(g, offset(*l));
          /* move g in to level(l) using inward-movement transf. */
          while (level(*g) < level(*l)) {
            g = move_inward(g);
            cout << *stmt << endl;
            cin.get();
          }
        }

        /* g and l are guaranteed to be siblings, eliminate g */
        /* if offset(g) < offset(l) */
        /* then eliminate g with a conditional */
        /* else */
        /* eliminate g with a do-loop */
        /* } */
        /* eliminate labels */
        /* for each label Li in label_list */
        /* do */
        /* eliminate Li */
      }
    }

    return stmt;
  }
} /* goto_elimination */ 
