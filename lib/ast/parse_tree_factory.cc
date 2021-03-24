#include "ast.h"

using namespace std;

enum class Cond { None, True, False };

namespace ast {

void error(const string &expr_label, string message) {
  throw runtime_error(expr_label + ": " + message);
}

void error(const Expr &e, string message) { error(e.label, message); }

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

void add_connection(ParseTree &t,
                    std::tuple_element<0, tuple<Cond>>::type &cond,
                    std::string &left_label, std::string &right_label) {
  if (cond == Cond::True) {
    assert(holds_alternative<Goto>(t[left_label].first.contents));
    if (get<Goto>(t[left_label].first.contents).dest.size())
      error(left_label, "Connection already exists");
    get<Goto>(t[left_label].first.contents).dest = right_label;
  } else {
    if (t[left_label].second.size())
      error(left_label, "Connection already exists");
    t[left_label].second = right_label;
  }
}

pair<ParseTree, string> gen_parse_tree(list<string> s) {
  ParseTree t;
  string first;

  for (const auto &e : s) {
    if (e.empty() || e.starts_with("%%")) continue;

    auto [left, right, cond] = splitConnection(e);
    string left_label = left.label, right_label = right.label;
    if (!t.size()) {
      t.emplace(piecewise_construct, forward_as_tuple(left_label),
                forward_as_tuple(move(left), ""));
      first = left_label;
    }
    assert(t.count(left_label));
    if (left_label[0] == '_' || right_label[0] == '_')
      throw runtime_error("Label cannot start with an underscore");

    // Is right in t?
    if (!t.count(right_label))
      t.emplace(piecewise_construct, forward_as_tuple(right_label),
                forward_as_tuple(move(right), ""));

    add_connection(t, cond, left_label, right_label);
  }
  return {move(t), first};
}

}  // namespace ast
