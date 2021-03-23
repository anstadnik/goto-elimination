#include "ast.h"

using namespace std;

namespace ast {

void fix_parents(Expr& e, Stmt* parent) {
  e.par_stmt = parent;
  if (holds_alternative<If>(e.contents)) 
    get<If>(e.contents).true_branch->par_expr = &e;
  if (holds_alternative<While>(e.contents))
    get<While>(e.contents).body->par_expr = &e;
}

void Stmt::push_back(Expr&& expr) {
  s.push_back(move(expr));
  fix_parents(s.back(), this);
}

void Stmt::insert(const string& parent, Expr expr, bool after) {
  Iterator i = find(parent);
  if (i == this->end()) throw runtime_error("Parent statement not found");
  auto s_parent = i->par_stmt;
  auto s_i = ::find(s_parent->s.begin(), s_parent->s.end(), *i);
  assert(s_i != s_parent->s.end());
  auto p = s_parent->s.insert(after ? ++s_i : s_i, move(expr));
  fix_parents(*p, this);
}

bool Stmt::empty() const { return s.empty(); }

void Stmt::remove(const string& label) {
  for (auto& e : *this)
    if (e.label == label) e.par_stmt->s.remove(e);
}

Stmt::ptr Stmt::extract_from(Iterator begin, Iterator end) {
  assert(begin->par_stmt == end->par_stmt);
  list<Expr> extracted_s;
  auto& s = begin->par_stmt->s;
  auto b = ::find(s.begin(), s.end(), *begin),
       e = ::find(s.begin(), s.end(), *end);
  extracted_s.splice(extracted_s.begin(), extracted_s, b, e);
  return make_unique<Stmt>(move(extracted_s));
}

Stmt::Stmt(list<Expr>&& s, Expr* parent_stmt)
    : s(move(s)), par_expr(parent_stmt) {
  for (auto& e : s) e.par_stmt = this;
}

Stmt::Stmt(Expr&& e, Expr* parent_expr) : par_expr(parent_expr) {
  s.push_back(move(e));
  s.back().par_stmt = this;
}

Stmt::Iterator Stmt::find(const string& label) {
  auto rez = std::find_if(this->begin(), this->end(),
                      [&label](const Expr& e) { return e.label == label; });
  assert(rez != this->end());
  return rez;
}

Stmt::Iterator Stmt::begin() { return Iterator(s.begin()); }
Stmt::Iterator Stmt::end() { return Iterator(s.end()); }
}  // namespace ast
