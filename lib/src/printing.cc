#include "ast.h"
#include "goto_elimination.h"



using namespace termcolor;

namespace ast {
ostream& operator<<(ostream& os, const ast::ParseTree& t) {
  /* vector<string> keys(t.size()); */

  /* auto key_selector = [](const auto& pair) -> string { return pair.first; }; */
  /* transform(t.begin(), t.end(), keys.begin(), key_selector); */
  /* sort(keys.begin(), keys.end()); */

  string head =
      ""
      "/****************\n"
      "*  PARSE TREE  * \n"
      "****************/\n";

  std::cout << head << std::endl;

  /* for (auto i : keys) */
  for (const auto& i : t)
    os << t.at(i.first).first << yellow << "goto "
        << (t.at(i.first).second.size() ? t.at(i.first).second : "terminate")
        << reset << "\n-------\n";
  return os;
}

string getIndent(const Expr& e) {
  return string(2 * goto_elimination::level(e), ' ');
}

ostream& operator<<(ostream& os, const Expr& expr) {
  os << green << setw(25) << left << expr.label + ":" << reset;

  const string i = getIndent(expr);
  /* const string i = ""; */
  if (auto p = get_if<Assign>(&expr.contents))
    return os << i << p->var << "=" << p->op << ";\n";

  if (auto p = get_if<If>(&expr.contents))
    return os << i << magenta << "if (" << p->cond << ")" << reset
              << " {\n" << *p->branch << i << string(25, ' ') << "}\n";
  if (auto p = get_if<While>(&expr.contents))
    return os << i << "while (" << p->cond << ") {\n"
              << *p->body << i << "    " << "\n}\n";
  if (auto p = get_if<Print>(&expr.contents))
    return os << i << "cout << " << p->var << " << endl;\n";
  if (auto p = get_if<Goto>(&expr.contents))
    return os << i << cyan << "if (" << p->cond << ") {"
              << " " << "goto " << p->dest << "; }" << reset << "\n";
  if (get_if<Break>(&expr.contents))
    return os << i << "break;" << " << endl;\n";
  return os;
}

ostream& operator<<(ostream& os, const Stmt& stmt) {
  string head = 
    "/***************\n"
    "*  Statement   *\n"
    "***************/\n";

  if (!stmt.par_expr)
    std::cout << head << std::endl;
  for (const auto& e : stmt.s)
    os << e;
  return os;
}

}  // namespace ast
