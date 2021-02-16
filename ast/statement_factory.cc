#include "statement.h"

using namespace std;

namespace statement {

void error(const Expr& e, string message) {
  throw runtime_error(get_label(e) + ": " + message);
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
    if (!t.size()) t[get_label(left)] = {left, ""}, first = get_label(left);
    assert(t.count(get_label(left)));
    if (get_label(left)[0] == '_' || get_label(right)[0] == '_')
      throw runtime_error("Label cannot start with an underscore");

    // Is right in t?
    if (!t.count(get_label(right))) t[get_label(right)] = {right, ""};

    if (cond == Cond::True) {
      if (get<Goto>(t[get_label(left)].first).dest.size())
        error(left, "Connection already exists");
      get<Goto>(t[get_label(left)].first).dest = get_label(right);
    } else {
      if (t[get_label(left)].second.size())
        error(left, "Connection already exists");
      t[get_label(left)].second = get_label(right);
    }
  }
  /* std::cout << t << std::endl; */
  return {t, first};
}

pair<Stmt, unordered_set<string>> parse_tree_to_statement(const ParseTree& t,
                                                          const string& first) {
  pair<Expr, string> cur = t.at(first);
  Stmt s;
  unordered_set<string> keys;

  while (42) {
    string next = cur.second;
    if (get_label(cur.first) == "Q") next = cur.second;
    if (keys.count(get_label(cur.first))) break;
    if (holds_alternative<Goto>(cur.first)) {
      string dest = get<Goto>(cur.first).dest,
             cond = get<Goto>(cur.first).condition;
      if (keys.count(dest) && keys.count(next)) {
        s.push_back(Goto(get_label(cur.first), cond, dest));
        s.push_back(Goto("_" + get_label(cur.first), "true", next));
        break;
      }
      if (keys.count(dest))
        s.push_back(Goto(get_label(cur.first), cond, dest));
      else if (keys.count(next)) {
        s.push_back(Goto(get_label(cur.first), "!" + cond, next));
        next = dest;
      } else {
        auto&& [stmt, keys_] = parse_tree_to_statement(t, dest);
        s.push_back(If(get_label(cur.first), cond, stmt));
        keys.insert(keys_.begin(), keys_.end());
      }
    } else
      s.push_back(cur.first);

    keys.insert(get_label(cur.first));
    if (next == "") break;
    cur = t.at(next);
  }
  return {s, keys};
}

Stmt statementFactory(list<string> s) {
  if (!!s.front().rfind("graph ", 0))
    throw runtime_error("The first line should be \'graph\'");
  s.pop_front();

  auto [parse_tree, first] = gen_parse_tree(s);
  auto [stmt, keys] = parse_tree_to_statement(parse_tree, first);
  return stmt;
}

}  // namespace statement
