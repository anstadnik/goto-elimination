#include "statement.h"

using namespace std;

namespace statement {

void Stmt::push_back(Expr&& expr) {
  set_parent_stmt(expr, this);
  s.push_back(move(expr));
}

void Stmt::insert(const string& parent, Expr expr, bool after=false) {
  Iterator i = find(parent);
  if (i == this->end()) throw runtime_error("Parent statement not found");
  auto s_parent = get_parent_stmt(*i);
  set_parent_stmt(expr, s_parent);
  auto s_i = ::find(s_parent->s.begin(), s_parent->s.end(), *i);
  assert(s_i != s_parent->s.end());
  s_parent->s.insert(after ? ++s_i : s_i, move(expr));
}

bool Stmt::empty() const { return s.empty(); }

void Stmt::remove(const string& label) {
  for (auto& e : *this) {
    if (get_label(e) == label) get_parent_stmt(e)->s.remove(e);
  }
}

Stmt::Stmt(Expr* parent_stmt) : parent_expr(parent_stmt) {}

Stmt::Stmt(list<Expr>&& s, Expr* parent_stmt)
    : s(move(s)), parent_expr(parent_stmt) {
  for (auto& e : s) set_parent_stmt(e, this);
}

Stmt::Stmt(Expr&& e, Expr* parent_stmt) {
  set_parent_stmt(e, this);
  s.push_back(move(e));
}

Stmt::Iterator Stmt::find(const string& label) {
  IC();
  for (auto& i : *this) {
    std::cout << i << std::endl;
  }
  IC();
  for (Stmt::Iterator i = begin(); i != end(); i++) {
    std::cout << *i << " | " << get_label(*i) << std::endl;
    if (get_label(*i) == label)
      return i;
  }
  IC();
  return end();
  /* return std::find_if(this->begin(), this->end(), */
  /*               [&label](const Expr& e) { return get_label(e) == label; }); */
}

Stmt::Iterator Stmt::begin() { return Iterator(move(s.begin())); }
Stmt::Iterator Stmt::end() { return Iterator(move(s.end())); }

/**********************
*  Goto elimination  *
**********************/

Stmt::Iterator Stmt::move_outward(const Stmt::Iterator& it) {
  assert(holds_alternative<Goto>(*it));

  const Goto& e = get<Goto>(*it);
  Stmt& parent = *get_parent_stmt(*it);
  if (holds_alternative<If>(*get_parent_expr(*it))){
    string bool_name = "_cond_" + e.label;
    parent.insert(get_label(*it), Assign(bool_name, bool_name, e.cond));

    Expr cond = If(e.label, bool_name, make_unique<Stmt>(Break("_break_" + e.label)));

  }
  else if (holds_alternative<If>(*get_parent_expr(*it))){

  }
  else throw runtime_error("I don't know this type of expression");
  return it;
}

}  // namespace statement
