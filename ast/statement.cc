#include "statement.h"

using namespace std;

namespace statement {

void Stmt::push_back(Expr expr) {
  set_parent_stmt(expr, this);
  s.push_back(move(expr));
}

void Stmt::insert(const string& parent, Expr expr) {
  list<Expr>::iterator i = std::find_if(
      s.begin(), s.end(),
      [&parent](const Expr& e) -> bool { return get_label(e) == parent; });
  if (i == s.end()) throw runtime_error("Parent statement not found");
  set_parent_stmt(expr, this);
  s.insert(++i, move(expr));
}

bool Stmt::empty() const { return s.empty(); }

void Stmt::remove(const string& label) {
  for (auto it = s.begin(); it != s.end(); it++) {
    if (get_label(*it) == label) {
      s.erase(it);
      return;
    }
    if (auto body = get_if<If>(&*it)) body->true_branch.remove(label);
    if (auto body = get_if<While>(&*it)) body->body.remove(label);
  }
}

Stmt::Stmt(Stmt* parent_stmt) : parent_stmt(parent_stmt) {}

Stmt::Stmt(list<Expr> s, Stmt* parent_stmt)
    : s(move(s)), parent_stmt(parent_stmt) {
  for (auto& e : s) set_parent_stmt(e, this);
}

Stmt::Iterator Stmt::begin() { return Iterator(move(s.begin())); }
Stmt::Iterator Stmt::end() { return Iterator(move(s.end())); }
}  // namespace statement
