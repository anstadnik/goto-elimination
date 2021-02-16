#include "statement.h"

using namespace std;
using namespace termcolor;

namespace statement {

namespace {
struct E {
  string label;
  pair<string, string> assignment;
  string to_print;
  string condition;

  friend ostream& operator<<(ostream& os, const E& e);
};

ostream& operator<<(ostream& os, const E& e) {
  return os << reset << "Label: " << green << e.label << reset
            << ", statement: " << green << e.assignment.first << "="
            << e.assignment.second << reset << ", to_print: " << green
            << e.to_print << reset << ", condition: " << green << e.condition
            << reset;
}
}  // namespace

void parseToken(const string& s, size_t& beg, size_t& end, E& e) {
  if (e.label.empty()) {
    e.label = s.substr(0, end);
    beg = end;
    return;
  }

  switch (s[end]) {
    case '[':  // Assignment
      if ((end = s.find(']', beg)) == string::npos)
        throw runtime_error("Unclosed square bracket");
      size_t eq;
      if ((eq = s.find('=', beg)) == string::npos)
        throw runtime_error("No assignment in assignment expression");
      e.assignment = {
          s.substr(beg + 1, eq - 2),
          s.substr(eq + 1, s.find("]", eq + 1) - (eq + 1)),
      };
      break;
    case '(':  // Print
      if ((end = s.find(")", beg)) == string::npos)
        throw runtime_error("Unclosed square bracket");
      e.to_print = s.substr(beg + 1, s.find(")", beg) - 2);
      break;
    case '{':  // Condition
      if ((end = s.find("}", beg)) == string::npos)
        throw runtime_error("Unclosed curly bracket");
      e.condition = s.substr(beg + 1, s.find("}", beg) - 2);
      break;
    default:
      throw runtime_error("Invalid line:\n" + s);
  }
  beg = end + 1;
}

Expr expressionFactory(string s) {
  size_t beg = 0, end;
  string::iterator end_i;

  E e;

  while ((end_i = find_if_not(s.begin() + beg, s.end(),
                              static_cast<int (*)(int)>(isalpha))) != s.end()) {
    end = distance(s.begin(), end_i);
    parseToken(s, beg, end, e);
  }

  if (!e.condition.empty()) return Goto(e.label, e.condition);
  if (!e.to_print.empty()) return Print(e.label, e.to_print);
  if (!e.assignment.first.empty())
    return Assign(e.label, e.assignment.first, e.assignment.second);
  if (!e.label.empty()) throw runtime_error("Wrong input");
  return BaseExpr(s);
}
}  // namespace statement
