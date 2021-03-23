#include "ast.h"

using namespace std;

namespace statement {

Stmt::ptr StatementFactory::parse_tree_to_statement(
    ParseTree& t, const string& first, unordered_set<string>& keys) {
  Stmt::ptr stmt = make_unique<Stmt>();
  string cur_label = first;

  while (42) {
    if (cur_label == "") break;
    auto& [expr, next] = t.at(cur_label);
    if (keys.count(cur_label)) break;
    if (auto p = get_if<Goto>(&expr.contents)) {
      string dest = p->dest, cond = p->cond;
      if (keys.count(dest) && keys.count(next)) {
        stmt->push_back(Expr(cur_label, Goto{cond, dest}));
        stmt->push_back(Expr("_" + cur_label, Goto{"true", next}));
        break;
      }
      if (keys.count(dest))
        stmt->push_back(Expr(cur_label, Goto{cond, dest}));
      else if (keys.count(next)) {
        stmt->push_back(Expr(cur_label, Goto{"!" + cond, next}));
        next = dest;
      } else {
        auto nested_stmt = parse_tree_to_statement(t, dest, keys);
        stmt->push_back(Expr(cur_label, If{move(nested_stmt), cond}));
        // We've added next to the nested_stmt
        if (keys.count(next)) {
          stmt->push_back(Expr("_" + cur_label, Goto{next, "true"}));
          break;
        }
      }
    } else
      stmt->push_back(move(expr));

    keys.insert(cur_label);
    cur_label = next;
  }
  return stmt;
}

Stmt::ptr StatementFactory::parse_tree_to_statement(ParseTree& t,
                                                    const string& first) {
  unordered_set<string> keys;
  return parse_tree_to_statement(t, first, keys);
}

Stmt::ptr StatementFactory::gen_statement(list<string> s) {
  if (!!s.front().rfind("graph ", 0))
    throw runtime_error("The first line should be \'graph\'");
  s.pop_front();

  auto [parse_tree, first] = gen_parse_tree(s);
  /* std::cout << parse_tree << std::endl; */
  auto stmt = parse_tree_to_statement(parse_tree, first);
  /* std::cout << *stmt << std::endl; */
  return stmt;
}

}  // namespace statement
