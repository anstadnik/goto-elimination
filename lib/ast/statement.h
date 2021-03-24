#pragma once

#include <algorithm>
#include <cassert>
#include <list>
#include <memory>
#include <stack>
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

  Iterator insert(const string& parent, Expr&& expr, bool after = false);
  void push_back(Expr&& expr);
  void remove(const string& label);
  void replace(string pattern, Expr& replacement);
  Iterator find(const string& label);
  template <class T>
  Iterator find_type();

  Iterator begin();
  Iterator end();
  size_t size() const { return s.size(); };
  bool empty() const;

  /* Iterator move_outward(const Iterator& it); */
  Stmt::ptr extract_from(const string& begin, const string& end = "");

  friend ostream& operator<<(ostream& os, const Stmt& a);
  friend class tests::Test;
  friend Stmt* getNestedStmt(const Expr& p);
};
}  // namespace ast
