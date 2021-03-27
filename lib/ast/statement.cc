#include <memory>
#include "ast.h"

using namespace std;

namespace ast {

void fix_parents(Expr& e, Stmt* parent) {
  if (parent) e.par_stmt = parent;
  if (holds_alternative<If>(e.contents))
    get<If>(e.contents).branch->par_expr = &e;
  if (holds_alternative<While>(e.contents))
    get<While>(e.contents).body->par_expr = &e;
}

void Stmt::push_back(Expr&& expr) {
  s.push_back(move(expr));
  fix_parents(s.back(), this);
}

Stmt::Iterator Stmt::insert(const Iterator& parent, Expr&& expr) {
  assert(parent == end() || parent->par_stmt == this);
  
  auto s_i = parent.get_it();
  auto p = this->s.insert(s_i, move(expr));
  fix_parents(*p, this);
  return {p, this};
}

bool Stmt::empty() const { return s.empty(); }

Stmt::Iterator Stmt::erase(const Iterator& it) {
  if (it == end() || it->par_stmt == this)
    return {s.erase(it.get_it()), this};
  return it->par_stmt->erase(it);
}

Stmt::ptr Stmt::extract_from(const Stmt::Iterator& begin) {
  return extract(begin, end());
}

Stmt::ptr Stmt::extract(const Iterator& begin, const Iterator& end) {
  Stmt::ptr extracted_s = make_unique<Stmt>();
  assert(begin->par_stmt == this);
  assert(end == this->end() || end->par_stmt == this);

  auto b = begin.get_it(), e = end.get_it();
  assert(b != s.end() and distance(s.begin(), b) <= distance(s.begin(), e));

  extracted_s->s.splice(extracted_s->s.begin(), s, b, e);
  for (auto& e : extracted_s->s) 
    fix_parents(e, extracted_s.get());

  return extracted_s;
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
  /* Iterator rez; */
  /* for (rez = begin(); rez != end(); rez++) */
  /*   if (rez->label == label) break; */
  /* return rez; */
  return std::find_if(this->begin(), this->end(),
                      [&label](const Expr& e) { return e.label == label; });
}

Stmt::Iterator Stmt::find_direct_child(const Expr& e) {
  return {std::find(this->s.begin(), this->s.end(), e), this};
}

Stmt::Iterator Stmt::begin() { return Iterator(s.begin(), this); }
Stmt::Iterator Stmt::end() { return Iterator(s.end(), this); }

void Stmt::dump(const string& fn) {
  ofstream f;
  f.open(fn);
  f << "#include <iostream>\nusing namespace std;\n\nint main() {\n";

  unordered_set<string> s;
  for (const auto& it : *this)
    if (auto p = get_if<Assign>(&it.contents); p and !s.count(p->var)) {
      s.emplace(p->var);
      f << "int " + p->var + "=0;\n";
    }

  f << *this;
  f << "}\n";
  f.close();
}
}  // namespace ast
