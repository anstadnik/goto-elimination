#include "header.hpp"

using namespace std;

namespace statement {

namespace {
enum class Condition { None, True, False };
}

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

Statement::ptr statementFactory(list<string> s) {
  if (!!s.front().rfind("graph ", 0))
    throw runtime_error("The first line should be \'graph\'");
  s.pop_front();

  for (const auto& e : s) {
    auto [left, right, cond] = splitConnection(e);
    /* IC(static_cast<string>(*left), static_cast<string>(*right)); */
    /* std::cout << left << " " << right << std::endl; */
    std::cout << static_cast<string>(*left) << static_cast<string>(*right);
    /* std::cout << s; */
  }

  return make_shared<Statement>(Statement(list<Expression::ptr>()));
}

Statement::operator string() {
  string ret;
  for (const auto& e : s) 
    ret += static_cast<string>(*e);
  return ret;
}

Statement::Statement(list<Expression::ptr> s) : s(move(s)) {}
}  // namespace statement
