#include "header.hpp"

using namespace std;

namespace statement {

tuple<Expression::ptr, Expression::ptr, Condition> splitConnection(string s) {
  s.erase(remove(s.begin(), s.end(), ' '), s.end());
  size_t delitimer = s.find("-->");
  if (delitimer == std::string::npos)
    throw runtime_error("Delimiter was not found");
  string first = s.substr(0, delitimer),
         second = s.substr(delitimer + "-->"s.size());

  Condition cond = Condition::None;
  if (!second.rfind("|True|", 0)) {
    cond = Condition::True;
    second = second.substr("|True|"s.size());
  }
  if (!second.rfind("|False|", 0)) {
    cond = Condition::False;
    second = second.substr("|False|"s.size());
  }

  return {expressionFactory(first), expressionFactory(second), cond};
}

void error(const Expression::ptr& e, string message) {
  throw runtime_error(e->label + ": " + message);
}

unordered_map<string, pair<Expression::ptr, string>> gen_parse_tree(
    list<string> s) {
  unordered_map<string, pair<Expression::ptr, string>> t;
  vector<string> keys;

  for (const auto& e : s) {
    auto [left, right, cond] = splitConnection(e);
    if (find(keys.begin(), keys.end(), left->label) == keys.end())
      keys.push_back(left->label);
    if (find(keys.begin(), keys.end(), right->label) == keys.end())
      keys.push_back(right->label);
    if (!t.size()) t[left->label] = {left, ""};
    assert(t.count(left->label));

    // Is right in t?
    if (t.count(right->label)) {
      if (cond == Condition::None) error(left, "tried to redefine connection");
      if (cond == Condition::True) {
        if (dynamic_cast<Goto*>(t[left->label].first.get())->dest.size())
          error(left, "tried to redefine connection");
        dynamic_cast<Goto*>(t[left->label].first.get())->dest = right->label;
      } else if (cond == Condition::False) {
        if (t[left->label].second.size())
          error(left, "tried to redefine connection");
        t[left->label].second = right->label;
      }
    } else {
      t[right->label] = {right, ""};
      if (cond == Condition::None) {
        if (t[left->label].second.size())
          error(left, "tried to redefine connection");
        t[left->label].second = right->label;
      }
      if (cond == Condition::True) {
        if (dynamic_cast<Goto*>(t[left->label].first.get())->dest.size())
          error(left, "tried to redefine connection");
        dynamic_cast<Goto*>(t[left->label].first.get())->dest = right->label;
      } else if (cond == Condition::False) {
        if (t[left->label].second.size())
          error(left, "tried to redefine connection");
        t[left->label].second = right->label;
      }
    }
  }

  vector<string> t_keys(t.size());

  // This is the crucial bit: Transform map to list of keys (or values)
  auto key_selector = [](auto pair){return pair.first;};
  transform(t.begin(), t.end(), t_keys.begin(), key_selector);

  assert(set(keys.begin(), keys.end()) == set(t_keys.begin(), t_keys.end()));

  for (auto i : keys) 
    std::cout << string(*t[i].first) + "goto " + (t[i].second.size() ? t[i].second : "terminate") + "\n";

  return t;
}

Statement::ptr parse_tree_to_statement(
    unordered_map<string, pair<Expression::ptr, string>> t) {
  return make_shared<Statement>(list<Expression::ptr>());
}

Statement::ptr statementFactory(list<string> s) {
  if (!!s.front().rfind("graph ", 0))
    throw runtime_error("The first line should be \'graph\'");
  s.pop_front();

  auto parse_tree = gen_parse_tree(s);
  return parse_tree_to_statement(parse_tree);
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

Statement::operator string() {
  string ret;
  for (const auto& e : s) ret += static_cast<string>(*e);
  return ret;
}

Statement::Statement(list<Expression::ptr> s) : s(move(s)) {}
}  // namespace statement
