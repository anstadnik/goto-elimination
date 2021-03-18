#include "statement.h"

using namespace termcolor;

namespace statement {
ostream& operator<<(ostream& os, const statement::ParseTree& t) {
  vector<string> keys(t.size());

  auto key_selector = [](const auto& pair) -> string { return pair.first; };
  transform(t.begin(), t.end(), keys.begin(), key_selector);
  sort(keys.begin(), keys.end());

  string head =
      ""
      "/****************\n"
      "*  PARSE TREE  * \n"
      "****************/\n";

  std::cout << head << std::endl;

  for (auto i : keys)
    visit([&os](auto&& expr) -> ostream& { return cout << expr; },
          t.at(i).first)
        << yellow << "goto "
        << (t.at(i).second.size() ? t.at(i).second : "terminate") << reset
        << "\n-------\n";
  return os;
}

string getIndent(const BaseExpr* e) {
  const Expr* p = getParentExpr(*e->getParentStmt());
  if (!p)
    return "";
  size_t indent = 2;
  while ((p = getParentExpr(*p)))
    indent += 2;
  return string(indent, ' ');
}

ostream& operator<<(ostream& os, const BaseExpr& expr) {
  return os << green << setw(4) << left << expr.label + ":" << reset;
}
ostream& operator<<(ostream& os, const Expr& v) {
  return visit([&os](auto&& expr) -> ostream& { return os << expr; }, v);
}
ostream& operator<<(ostream& os, const Assign& expr) {
  const string i = getIndent(&expr);
  return os << static_cast<const BaseExpr&>(expr) << i << expr.var << "=" << expr.op
            << ";\n";
}
ostream& operator<<(ostream& os, const If& expr) {
  const string i = getIndent(&expr);
  return os << static_cast<const BaseExpr&>(expr) << i << termcolor::dark << "if ("
            << expr.cond << ")" << reset << " {\n"
            << *expr.true_branch << i << "    " << "}\n";
}
ostream& operator<<(ostream& os, const While& expr) {
  const string i = getIndent(&expr);
  return os << static_cast<const BaseExpr&>(expr) << i << "while (" << expr.cond
            << ") {\n"
            << *expr.body << i << "    " << "\n}\n";
}

ostream& operator<<(ostream& os, const Print& expr) {
  const string i = getIndent(&expr);
  return os << static_cast<const BaseExpr&>(expr) << i << "cout << " << expr.var
            << " << endl;\n";
}
ostream& operator<<(ostream& os, const Goto& expr) {
  const string i = getIndent(&expr);
  return os << static_cast<const BaseExpr&>(expr) << i << "if (" << expr.cond
            << ") {" << " " << cyan << "goto " << reset << expr.dest << "; }\n";
}
ostream& operator<<(ostream& os, const Break& expr) {
  const string i = getIndent(&expr);
  return os << static_cast<const BaseExpr&>(expr) << i << "break;"
            << " << endl;\n";
}

ostream& operator<<(ostream& os, const Stmt& stmt) {
  string head = 
    "/***************\n"
    "*  Statement   *\n"
    "***************/\n";

  if (!stmt.parent_expr)
    std::cout << head << std::endl;
  for (const auto& e : stmt.s)
    visit([&os](auto&& expr) -> ostream& { return os << expr; }, e);
  return os;
}

}  // namespace statement
