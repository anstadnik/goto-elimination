#include "goto_elimination.h"

using namespace statement;

Stmt eliminateGoto(Stmt stmt) {
  for (const auto& expr : stmt) {
    if (get_label(expr).find("_goto_") != string::npos) 
      continue;
    string label = "_goto_" + get_label(expr);
    stmt.insert(get_label(expr), Assign(label, label, "0"));
  }
  std::_List_iterator<class std::variant<
      class statement::BaseExpr, class statement::Assign, class statement::If,
      class statement::While, class statement::Print, class statement::Goto>>
      hm = stmt.find_type<Goto>();
  auto it = stmt.find_type<Goto>();
  /* static_assert(typeid(hm).name() == typeid(it).name()) */
  static_assert(is_same<decltype(hm), decltype(it)>::value);
  std::cout << typeid(hm).name() << endl << typeid(it).name()  << std::endl;

  for (auto it = stmt.find_type<Goto>(); it != stmt.end();
       it = stmt.find_type<Goto>()) {
    std::cout << *it << std::endl;
    /* eliminate gotos */
    /* while not empty(goto _li st) do { */
    /* select the next goto/label pair */
    /* g := select a goto from goto_list */
    /* l := label matching g */
    /* force g and l to be directly related */
    /* if indirectly related(g, l) */
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
