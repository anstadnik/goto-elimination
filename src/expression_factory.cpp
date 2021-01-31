#include <program.hpp>

namespace program {
Expression::ptr expressionFactory(string s) {
  string::iterator b = s.begin(), e = s.end();
  string::iterator end = s.end();
  while ((e = find_if_not(b, end, static_cast<int (*)(int)>(isalpha))) != end) {
    // Is it a conditional?
    if (*e == '[') {
      if (s.find(']', distance(s.begin(), b)) == string::npos)
        throw runtime_error("Unclosed square bracket");

      if (*(e + 1) == '(') {
        // It is a print expression
        if (s.find(")]", distance(s.begin(), b)) == string::npos)
          throw runtime_error("Unclosed square bracket");
      }
    }
    if (*e == '[') {
    }
    b = next(e);
  }
  return make_shared<Assignment>(s, "", "");
}

pair<Expression::ptr, Expression::ptr> splitConnection(const string& s) {
  size_t delitimer = s.find("-->");
  if (delitimer == std::string::npos)
    throw runtime_error("Delimiter was not found");
  string first = s.substr(0, delitimer), second = s.substr(delitimer + 3);
  first.erase(remove(first.begin(), first.end(), ' '), first.end());
  second.erase(remove(second.begin(), second.end(), ' '), second.end());
  return {expressionFactory(first), expressionFactory(second)};
}

}  // namespace program
