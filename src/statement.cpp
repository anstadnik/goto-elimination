#include "header.hpp"

using namespace std;

namespace statement {

tuple<Expr::ptr, Expr::ptr, Cond> splitConnection(string s) {
  s.erase(remove(s.begin(), s.end(), ' '), s.end());
  size_t delitimer = s.find("-->");
  if (delitimer == std::string::npos)
    throw runtime_error("Delimiter was not found");
  string first = s.substr(0, delitimer),
         second = s.substr(delitimer + "-->"s.size());

  Cond cond = Cond::None;
  if (!second.rfind("|True|", 0)) {
    cond = Cond::True;
    second = second.substr("|True|"s.size());
  }
  if (!second.rfind("|False|", 0)) {
    cond = Cond::False;
    second = second.substr("|False|"s.size());
  }

  return {expressionFactory(first), expressionFactory(second), cond};
}

void error(const Expr::ptr& e, string message) {
  throw runtime_error(e->label + ": " + message);
}

pair<ParseTree, string> gen_parse_tree(list<string> s) {
  ParseTree t;
  string first;

  for (const auto& e : s) {
    auto [left, right, cond] = splitConnection(e);
    if (!t.size()) t[left->label] = {left, ""}, first = left->label;
    assert(t.count(left->label));

    // Is right in t?
    if (t.count(right->label)) {
      if (cond == Cond::None) error(left, "tried to redefine connection");
      if (cond == Cond::True) {
        if (dynamic_cast<Goto*>(t[left->label].first.get())->dest.size())
          error(left, "tried to redefine connection");
        dynamic_cast<Goto*>(t[left->label].first.get())->dest = right->label;
      } else if (cond == Cond::False) {
        if (t[left->label].second.size())
          error(left, "tried to redefine connection");
        t[left->label].second = right->label;
      }
    } else {
      t[right->label] = {right, ""};
      if (cond == Cond::None) {
        if (t[left->label].second.size())
          error(left, "tried to redefine connection");
        t[left->label].second = right->label;
      }
      if (cond == Cond::True) {
        if (dynamic_cast<Goto*>(t[left->label].first.get())->dest.size())
          error(left, "tried to redefine connection");
        dynamic_cast<Goto*>(t[left->label].first.get())->dest = right->label;
      } else if (cond == Cond::False) {
        if (t[left->label].second.size())
          error(left, "tried to redefine connection");
        t[left->label].second = right->label;
      }
    }
  }
  print_parse_tree(t);
  return {t, first};
}

Stmt::ptr parse_tree_to_statement(ParseTree t, const string& first) {
  pair<Expr::ptr, string> cur = t[first];
  Stmt::ptr s = make_shared<Stmt>();
  unordered_set<string> keys;

  while (42) {
  }
  return s;
}

Stmt::ptr statementFactory(list<string> s) {
  if (!!s.front().rfind("graph ", 0))
    throw runtime_error("The first line should be \'graph\'");
  s.pop_front();

  auto [parse_tree, first] = gen_parse_tree(s);
  auto stmt = parse_tree_to_statement(parse_tree, first);
  return stmt;
}

/* void Statement::add(Expression::ptr& left, Expression::ptr& right, Condition
 * cond) { */
/*   if (!s.size()) */
/*     s[left->label] = left; */
/*   /1* auto l_i = find_if(s.begin(), s.end(), [&left](const Expression::ptr&
 * e) { *1/ */
/*   /1*   return e->label == left->label;}); *1/ */
/*   /1* auto r_i = find_if(s.begin(), s.end(), [&right](const Expression::ptr&
 * e) { *1/ */
/*   /1*   return e->label == right->label;}); *1/ */

/*   if (s.count(right->label)) { */
/*     // Right was found */
/*     if (cond == Condition::None) */
/*       // We can only push_back, or add to conditionals */
/*       throw runtime_error("You can only push_back, or add to conditionals");
 */
/*     if (cond == Condition::True) */
/*       dynamic_cast<Conditional&>(*(*l_i)).true_branch = move(st); */
/*     else if (cond == Condition::False) */
/*       dynamic_cast<Conditional&>(*(*l_i)).false_branch = move(st); */
/*   } else { */
/*     // Right was not found */
/*     if (l_i != s.end() && next(l_i) == s.end()) */
/*       // Left is the last element, so push a new one to the end */
/*       s[right->label] = right, s[left->label]->next = right->label; */
/*     if (cond == Condition::None) */
/*       // We can only push_back, or add to conditionals */
/*       error(left, "tried to redefine connection"); */
/*     auto st = make_shared<Statement>(list<Expression::ptr>{right}); */

/*     // Make sure left is Conditional */
/*     if (dynamic_cast<Conditional*>(l_i) == nullptr) */
/*       error(left, "trying to use goto without condition"); */

/*     Goto& c = dynamic_cast<Goto&>(**l_i); */
/*     if (c.dest.size()) */
/*       throw runtime_error("Trying to define goto destination twice"); */
/*     c.dest = right; */
/*     /1* if (cond == Condition::True) *1/ */
/*     /1*   dynamic_cast<Conditional&>(**l_i).true_branch = move(st); *1/ */
/*     /1* else if (cond == Condition::False) *1/ */
/*     /1*   dynamic_cast<Conditional&>(*(*l_i)).false_branch = move(st); *1/ */
/*   } */
/* } */

Stmt::operator string() {
  string ret;
  for (const auto& e : s) ret += static_cast<string>(*e);
  return ret;
}

Stmt::Stmt(list<Expr::ptr> s) : s(move(s)) {}
}  // namespace statement
