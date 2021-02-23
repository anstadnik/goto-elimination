#include "statement.h"

using namespace std;

namespace statement {

void Stmt::push_back(Expr&& expr) {
  set_parent_stmt(expr, this);
  s.push_back(move(expr));
}

void Stmt::insert(const string& parent, Expr expr) {
  Iterator i = find(parent);
  if (i == this->end()) throw runtime_error("Parent statement not found");
  auto s_parent = get_parent_stmt(*i);
  set_parent_stmt(expr, s_parent);
  auto s_i = ::find(s_parent->s.begin(), s_parent->s.end(), *i);
  assert(s_i != s_parent->s.end());
  s_parent->s.insert(++s_i, move(expr));
}

bool Stmt::empty() const { return s.empty(); }

void Stmt::remove(const string& label) {
  for (auto& e : *this) {
    if (get_label(e) == label) get_parent_stmt(e)->s.remove(e);
  }
}

Stmt::Stmt(Expr* parent_stmt) : parent_stmt(parent_stmt) {}

Stmt::Stmt(list<Expr> s, Expr* parent_stmt)
    : s(move(s)), parent_stmt(parent_stmt) {
  for (auto& e : s) set_parent_stmt(e, this);
}

Stmt::Iterator Stmt::find(const string& label) {
  return std::find_if(this->begin(), this->end(),
                [&label](const Expr& e) { return get_label(e) == label; });
}

Stmt::Iterator Stmt::begin() { return Iterator(move(s.begin())); }
Stmt::Iterator Stmt::end() { return Iterator(move(s.end())); }

/**********************
*  Goto elimination  *
**********************/

Stmt::Iterator Stmt::move_outward(const Stmt::Iterator& it) {
  return it;
}

}  // namespace statement
