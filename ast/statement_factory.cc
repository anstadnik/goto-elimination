#include "statement.h"

using namespace std;

namespace statement {

void error(const Expr& e, string message) {
  error(get_label(e), message);
}

void error(const string& expr_label, string message) {
  throw runtime_error(expr_label + ": " + message);
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

pair<ParseTree, string> gen_parse_tree(list<string> s) {
  ParseTree t;
  string first;

  for (const auto& e : s) {
    auto [left, right, cond] = splitConnection(e);
    string left_label = get_label(left), right_label = get_label(right);
    if (!t.size()) {
      t.emplace(piecewise_construct, forward_as_tuple(left_label), forward_as_tuple(move(left), ""));
      first = left_label;
    }
    assert(t.count(left_label));
    if (left_label[0] == '_' || right_label[0] == '_')
      throw runtime_error("Label cannot start with an underscore");

    // Is right in t?
    if (!t.count(right_label))
      t.emplace(piecewise_construct, forward_as_tuple(right_label),
                forward_as_tuple(move(right), ""));

    if (cond == Cond::True) {
      if (get<Goto>(t[left_label].first).dest.size())
        error(left_label, "Connection already exists");
      get<Goto>(t[left_label].first).dest = right_label;
    } else {
      if (t[left_label].second.size())
        error(left_label, "Connection already exists");
      t[left_label].second = right_label;
    }
  }
  return {move(t), first};
}

Stmt::ptr parse_tree_to_statement(
    ParseTree& t, const string& first, unordered_set<string>& keys) {
  pair<Expr, string>& cur = t.at(first);
  Stmt::ptr s = make_unique<Stmt>();
  /* unordered_set<string> keys; */

  while (42) {
    string next = cur.second;
    if (get_label(cur.first) == "Q") next = cur.second;
    if (keys.count(get_label(cur.first))) break;
    if (holds_alternative<Goto>(cur.first)) {
      string dest = get<Goto>(cur.first).dest,
             cond = get<Goto>(cur.first).condition;
      if (keys.count(dest) && keys.count(next)) {
        s->push_back(Goto(get_label(cur.first), cond, dest));
        s->push_back(Goto("_" + get_label(cur.first), "true", next));
        break;
      }
      if (keys.count(dest))
        s->push_back(Goto(get_label(cur.first), cond, dest));
      else if (keys.count(next)) {
        s->push_back(Goto(get_label(cur.first), "!" + cond, next));
        next = dest;
      } else {
        auto lab = get_label(cur.first);
        auto&& stmt = parse_tree_to_statement(t, dest, keys);
        s->push_back(If(lab, cond, move(stmt)));
        /* keys.insert(keys_.begin(), keys_.end()); */
      }
    } else
      s->push_back(move(cur.first));

    keys.insert(get_label(cur.first));
    if (next == "") break;
    cur = move(t.at(next));
  }
  return move(s);
}

Stmt::ptr parse_tree_to_statement(ParseTree& t, const string& first) {
  unordered_set<string> keys;
  return parse_tree_to_statement(t, first, keys);
}

Stmt::ptr statementFactory(list<string> s) {
  if (!!s.front().rfind("graph ", 0))
    throw runtime_error("The first line should be \'graph\'");
  s.pop_front();

  auto [parse_tree, first] = gen_parse_tree(s);
  auto stmt = parse_tree_to_statement(parse_tree, first);
  return stmt;
}

}  // namespace statement
