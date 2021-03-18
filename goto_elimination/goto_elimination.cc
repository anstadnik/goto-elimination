#include "goto_elimination.h"
#include <array>
#include <iostream>
#include <variant>
#include "statement.h"

using namespace statement;
using namespace std;

bool indirectly_related(const Stmt::Iterator& g, const Stmt::Iterator& l) {
  /* if (!get_parent_stmt(*g) || !get_parent_stmt(*l)) */
  /*   return false; */
  for (auto parent = getParentStmt(*g); parent;
       parent = getParentStmt(*parent))
    if (getParentStmt(*l) == parent) return false;
  for (auto parent = getParentStmt(*l); parent;
       parent = getParentStmt(*parent))
    if (getParentStmt(*l) == parent) return false;
  return true;
}

Stmt* get_common_parent(const Stmt::Iterator& a, const Stmt::Iterator& b) {
  for (auto parent_a = getParentStmt(*a); parent_a;
       parent_a = getParentStmt(*parent_a))
    for (auto parent_b = getParentStmt(*b); parent_b;
         parent_b = getParentStmt(*parent_b))
      if (parent_a == parent_b) return parent_a;
  return nullptr;
}

Stmt::ptr eliminateGoto(Stmt::ptr stmt) {
  /* TODO: Fix for nested <23-02-21, astadnik> */
  /* for (const auto& expr : *stmt) { */
  /*   if (get_label(expr).find("_goto_") != string::npos) */
  /*     continue; */
  /*   string label = "_goto_" + get_label(expr); */
  /*   stmt->insert(get_label(expr), Assign(label, label, "0"), true); */
  /* } */
  /* IC(get<Goto>(*g).dest); */

  /* eliminate gotos */
  /* while not empty(goto _li st) do { */
  /* std::cout << *stmt << std::endl; */
  /* exit(0); */
  for (auto g = stmt->find_type<Goto>(); g != stmt->end();
       g = stmt->find_type<Goto>()) {
    /* select the next goto/label pair */
    /* g := select a goto from goto_list */
    /* l := label matching g */
    auto l = stmt->find(get<Goto>(*g).dest);

    /* force g and l to be directly related */

    /* if indirectly related(g, l) */
    std::cout << *g << std::endl;
    std::cout << *l << std::endl;
    if (indirectly_related(g, l)) {
      auto parent = get_common_parent(g, l);
      // Can be if or switch. Since I don't have switch...
      if (holds_alternative<If>(*getParentExpr(*parent)))
        while (getParentStmt(*g) != parent) {
          g = stmt->move_outward(g);
          cout << stmt << endl;
          cin.get();
        }
      // Here could be logic for the case when g and l are in different
      // statements (functions)
      else
        throw runtime_error(
            "Indirectly related, but not through an if statement");
    }
    /* then if different_statements(g ,l) then */
    /* move g out using outward-movement transf. */
    /* until it becomes directly related to l */
    /* else */
    /* diff.  branches of same if or switch */
    /* move g out using outward-movement transf. */
    /* until it becomes directly related */
    /* to an if or switch containing l */

    /* force g and l to be siblings */
    /* if directly_related(g ,l) */
    /* then if level(g) > level(l) */
    /* then move g out to level(l) */
    /* using outward-movement transformations */
    /* else */
    /* level(g) < level(l) */
    /* { if offset(g) > offset(l) */
    /* then lift g to above stmt containing l */
    /* using goto-lifting transformations */
    /* move g in to level(l) using inward-movement transf. */
    /* } */
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

  return stmt;
}
