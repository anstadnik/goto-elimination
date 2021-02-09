#include "header.hpp"

using namespace std;

namespace statement {

/* auto get_label = [](auto&& expr) -> string { expr.label; }; */
string get_label(const Expr& v) {
  return visit([](auto&& expr) {return expr.label;}, v);
}

tuple<Expr, Expr, Cond> splitConnection(string s) {
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

void error(const Expr& e, string message) {
  throw runtime_error(get_label(e) + ": " + message);
}

pair<ParseTree, string> gen_parse_tree(list<string> s) {
  ParseTree t;
  string first;

  for (const auto& e : s) {
    auto [left, right, cond] = splitConnection(e);
    if (!t.size()) t[get_label(left)] = {left, ""}, first = get_label(left);
    assert(t.count(get_label(left)));

    // Is right in t?
    if (t.count(get_label(right))) {
      if (cond == Cond::None) error(left, "tried to redefine connection");
      if (cond == Cond::True) {
        if (get<Goto>(t[get_label(left)].first).dest.size())
          error(left, "tried to redefine connection");
        get<Goto>(t[get_label(left)].first).dest = get_label(right);
      } else if (cond == Cond::False) {
        if (t[get_label(left)].second.size())
          error(left, "tried to redefine connection");
        t[get_label(left)].second = get_label(right);
      }
    } else {
      t[get_label(right)] = {right, ""};
      if (cond == Cond::None) {
        if (t[get_label(left)].second.size())
          error(left, "tried to redefine connection");
        t[get_label(left)].second = get_label(right);
      }
      if (cond == Cond::True) {
        if (get<Goto>(t[get_label(left)].first).dest.size())
          error(left, "tried to redefine connection");
        get<Goto>(t[get_label(left)].first).dest = get_label(right);
      } else if (cond == Cond::False) {
        if (t[get_label(left)].second.size())
          error(left, "tried to redefine connection");
        t[get_label(left)].second = get_label(right);
      }
    }
  }
  print_parse_tree(t);
  return {t, first};
}

Stmt parse_tree_to_statement(ParseTree t, const string& first) {
  pair<Expr, string> cur = t[first];
  Stmt s;
  unordered_set<string> keys;

  /* while (42) { */

  /* } */
  return s;
}

Stmt statementFactory(list<string> s) {
  if (!!s.front().rfind("graph ", 0))
    throw runtime_error("The first line should be \'graph\'");
  s.pop_front();

  auto [parse_tree, first] = gen_parse_tree(s);
  auto stmt = parse_tree_to_statement(parse_tree, first);
  return stmt;
}

/* void Statement::add(Expr& left, Expr& right, Condition
 * cond) { */
/*   if (!s.size()) */
/*     s[get_label(left)] = left; */
/*   /1* auto l_i = find_if(s.begin(), s.end(), [&left](const Expr&
 * e) { *1/ */
/*   /1*   return e.label == get_label(left);}); *1/ */
/*   /1* auto r_i = find_if(s.begin(), s.end(), [&right](const Expr&
 * e) { *1/ */
/*   /1*   return e.label == get_label(right);}); *1/ */

/*   if (s.count(get_label(right))) { */
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
/*       s[get_label(right)] = right, s[get_label(left)].next = get_label(right); */
/*     if (cond == Condition::None) */
/*       // We can only push_back, or add to conditionals */
/*       error(left, "tried to redefine connection"); */
/*     auto st = make_shared<Statement>(list<Expr>{right}); */

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

Stmt::operator string() const {
  string ret;
  for (const auto& e : s) ret += visit([](auto&& expr) { return string(expr); }, e);
  return ret;
}

Stmt::operator bool() const { return s.empty(); }

Stmt::Stmt(list<Expr> s) : s(move(s)) {}
}  // namespace statement
