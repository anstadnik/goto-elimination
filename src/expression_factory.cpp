#include "header.hpp"


namespace statement {

namespace {
struct E {
  string label;
  string statement;
  string to_print;
  string condition;

  friend ostream& operator<<(ostream& os, const E& e);
};

ostream& operator<<(ostream& os, const E& e) {
  return os << termcolor::reset << "Label: " << termcolor::green << e.label
            << termcolor::reset << ", statement: " << termcolor::green
            << e.statement << termcolor::reset
            << ", to_print: " << termcolor::green << e.to_print
            << termcolor::reset << ", condition: " << termcolor::green
            << e.condition << termcolor::reset;
}
}  // namespace

void parse_token(const string& s, size_t& beg, size_t& end, E& e) {
  if (!e.label.size()) {
    e.label = s.substr(0, end);
    beg = end;
    return;
  }

  switch (s[end]) {
    case '[':  // Assignment
      if ((end = s.find(']', beg)) == string::npos)
        throw runtime_error("Unclosed square bracket");
      e.statement = s.substr(beg, s.find("]", beg));
      break;
    case '(':  // Print
      if ((end = s.find(")", beg)) == string::npos)
        throw runtime_error("Unclosed square bracket");
      e.to_print = s.substr(beg, s.find(")]", beg));
      break;
    case '{':  // Condition
      if ((end = s.find("}", beg)) == string::npos)
        throw runtime_error("Unclosed curly bracket");
      e.condition = s.substr(beg, s.find("}", beg));
      break;
    default:
      IC(s[beg], s[end]);
      throw runtime_error("Invalid line:\n" + s);
  }
  beg = end;
}

Expression::ptr expressionFactory(string s) {
  size_t beg = 0, end = s.size();
  string::iterator end_i;

  E e;

  while ((end_i = find_if_not(s.begin() + beg, s.end(),
                              static_cast<int (*)(int)>(isalpha))) != s.end()) {
    end = distance(s.begin(), end_i);
    parse_token(s, beg, end, e);
    /* IC(beg); */
  }

  std::cout << termcolor::reset << "s: " << termcolor::green << s << ". " << e << endl;

  /* TODO: create an Expression <01-02-21, astadnik> */
  return make_shared<Assignment>(s, "", "");
}

tuple<Expression::ptr, Expression::ptr, Condition> splitConnection(string s) {
  s.erase(remove(s.begin(), s.end(), ' '), s.end());
  size_t delitimer = s.find("-->");
  if (delitimer == std::string::npos)
    throw runtime_error("Delimiter was not found");
  string first = s.substr(0, delitimer),
         second = s.substr(delitimer + "-->"s.size());
  /* second[static_cast<size_t>(2e10)] = 'n'; */

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

}  // namespace program
