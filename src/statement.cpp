#include "header.hpp"

using namespace std;

namespace statement {

void Stmt::push_back(Expr expr) { s.push_back(move(expr)); }

void Stmt::insert(string parent, Expr& expr) {
  list<Expr>::iterator i = find_if(s.begin(), s.end(), [&parent](const Expr& e) -> bool {
    return get_label(e) == parent;
  });
  if (i == s.end())
    throw runtime_error("Parent statement not found");
  s.insert(i, expr);
}

bool Stmt::empty() const { return s.empty(); }

Stmt::Stmt(list<Expr> s) : s(move(s)) {}
}  // namespace statement
