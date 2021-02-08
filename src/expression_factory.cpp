#include "header.hpp"

using namespace std;

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
  return os << termcolor::reset << "Label: " << termcolor::green << e.label
            << termcolor::reset << ", statement: " << termcolor::green
            << e.assignment.first << "=" << e.assignment.second
            << termcolor::reset << ", to_print: " << termcolor::green
            << e.to_print << termcolor::reset
            << ", condition: " << termcolor::green << e.condition
            << termcolor::reset;
}
}  // namespace

void parseToken(const string& s, size_t& beg, size_t& end, E& e) {
  if (!e.label.size()) {
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
          s.substr(eq+1, s.find("]", eq+1) - (eq+1)),
      };
      break;
    case '(':  // Print
      if ((end = s.find(")", beg)) == string::npos)
        throw runtime_error("Unclosed square bracket");
      e.to_print = s.substr(beg+1, s.find(")", beg)-2);
      break;
    case '{':  // Condition
      if ((end = s.find("}", beg)) == string::npos)
        throw runtime_error("Unclosed curly bracket");
      e.condition = s.substr(beg+1, s.find("}", beg)-2);
      break;
    default:
      throw runtime_error("Invalid line:\n" + s);
  }
  beg = end + 1;
}

Expression::ptr expressionFactory(string s) {
  size_t beg = 0, end = s.size();
  string::iterator end_i;

  E e;

  while ((end_i = find_if_not(s.begin() + beg, s.end(),
                              static_cast<int (*)(int)>(isalpha))) != s.end()) {
    end = distance(s.begin(), end_i);
    parseToken(s, beg, end, e);
  }

  /* std::cout << termcolor::reset << "s: " << termcolor::green << s << ". " << e << endl; */

  if (e.condition.size())
    return make_shared<Goto>(e.label, e.condition);
  if (e.to_print.size())
    return make_shared<Print>(e.label, e.to_print);
  if (e.assignment.first.size())
    return make_shared<Assignment>(e.label, e.assignment.first, e.assignment.second);
  if (e.label.size())
    throw runtime_error("Wrong input");
  return make_shared<Expression>(s);
}
}  // namespace program
