#pragma once

#include <algorithm>
#include <cassert>
#include <list>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <variant>

#include "algs/algs.h"

using namespace std;

namespace tests {
class Test;
}

namespace ast {

class Expr;

class Stmt {
 private:
  // Possible UB, Expr is not defined
  list<Expr> s;

 public:
  Expr* par_expr = nullptr;

  using ptr = unique_ptr<Stmt>;
  class Iterator;

  Stmt() = default;
  Stmt(list<Expr>&&, Expr* = nullptr);
  Stmt(Expr&&, Expr* = nullptr);
  Stmt(const Stmt&) = default;
  Stmt(Stmt&&) = default;

  ~Stmt() = default;

  Iterator insert(const Iterator& parent, Expr&& expr);
  void push_back(Expr&& expr);
  Iterator erase(const Iterator& it);
  void replace(string pattern, Expr& replacement);
  Iterator find(const string& label);
  Iterator find_direct_child(const Expr& e);
  template <class T>
  Iterator find_type();

  Iterator begin();
  Iterator end();
  size_t size() const { return s.size(); };
  bool empty() const;

  /* Iterator move_outward(const Iterator& it); */
  Stmt::ptr extract_from(const Iterator& begin);
  Stmt::ptr extract(const Iterator& begin, const Iterator& end);

  void dump(const string& fn);

  friend ostream& operator<<(ostream& os, const Stmt& a);
  friend class tests::Test;
  friend Stmt* getNestedStmt(const Expr& p);
};
}  // namespace ast
