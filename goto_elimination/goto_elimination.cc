#include "goto_elimination.h"

#include <array>
#include <iostream>
#include <variant>

using namespace statement;
using namespace std;

bool indirectly_related(const Stmt::Iterator& g, const Stmt::Iterator& l) {
  for (auto parent = g->par_stmt; parent->par_expr;
       parent = parent->par_expr->par_stmt)
    if (l->par_stmt == parent) return false;
  for (auto parent = l->par_stmt; parent->par_expr;
       parent = parent->par_expr->par_stmt)
    if (l->par_stmt == parent) return false;
  return true;
}

Stmt* get_common_parent(const Stmt::Iterator& a, const Stmt::Iterator& b) {
  for (auto parent_a = a->par_stmt; parent_a->par_expr;
       parent_a = parent_a->par_expr->par_stmt)
    for (auto parent_b = b->par_stmt; parent_b->par_expr;
         parent_b = parent_b->par_expr->par_stmt)
      if (parent_a == parent_b) return parent_a;
  return nullptr;
}

Stmt::Iterator move_outward(Stmt::Iterator it) {
  assert(holds_alternative<Goto>(it->contents));

  const Goto& e = get<Goto>(it->contents);
  Stmt& parent = *it->par_stmt;
  if (holds_alternative<If>(parent.par_expr->contents)) {
    string bool_name = "_cond_" + it->label;
    // Insert condition
    parent.insert(it->label, Expr(bool_name, Assign{bool_name, e.cond}));
    // Put everything after the goto in If
    parent.insert(it->label, Expr("_if_" + it->label,
                                  If{parent.extract_from(it, parent.end()),
                                     "!" + bool_name}));
  } else if (holds_alternative<While>(it->contents)) {
    throw runtime_error("Not implemented");
  } else
    throw runtime_error("I don't know this type of expression");
  return it;
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
    auto l = stmt->find(get<Goto>(g->contents).dest);

    /* force g and l to be directly related */

    /* if indirectly related(g, l) */
    if (indirectly_related(g, l)) {
      auto parent = get_common_parent(g, l);
      // Can be if or switch. Since I don't have switch...
      if (holds_alternative<If>(parent->par_expr->contents))
        while (g->par_stmt != parent) {
          g = move_outward(g);
          cout << *stmt << endl;
          cin.get();
        }
      // Here could be logic for the case when g and l are in different
      // statements (functions)
      else
        throw runtime_error(
            "Indirectly related, but not through an if statement");
    }
    std::cout << g->label << " and " << l->label << " are directly related " << std::endl;
    std::cout << *stmt << std::endl;
    break;
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
