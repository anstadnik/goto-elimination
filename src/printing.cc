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

ostream& operator<<(ostream& os, const BaseExpr& expr) {
  return os << green << expr.label << ": " << reset;
}
ostream& operator<<(ostream& os, const Expr& v) {
  return visit([&os](auto&& expr) -> ostream& { return os << expr; }, v);
}
ostream& operator<<(ostream& os, const Assign& expr) {
  return os << static_cast<const BaseExpr&>(expr) << expr.var << "=" << expr.op
            << ";\n";
}
ostream& operator<<(ostream& os, const If& expr) {
  return os << static_cast<const BaseExpr&>(expr) << termcolor::dark << "if ("
            << expr.cond << ")" << reset << " {\n"
            << *expr.true_branch << "}\n";
}
ostream& operator<<(ostream& os, const While& expr) {
  return os << static_cast<const BaseExpr&>(expr) << "while (" << expr.cond
            << ") {\n"
            << *expr.body << "\n}\n";
}

ostream& operator<<(ostream& os, const Print& expr) {
  return os << static_cast<const BaseExpr&>(expr) << "cout << " << expr.var
            << " << endl;\n";
}
ostream& operator<<(ostream& os, const Goto& expr) {
  return os << static_cast<const BaseExpr&>(expr) << "if (" << expr.cond
            << ") {\n" << cyan << "goto " << reset << expr.dest << ";\n}\n";
}
ostream& operator<<(ostream& os, const Break& expr) {
  return os << static_cast<const BaseExpr&>(expr) << "break;"
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
