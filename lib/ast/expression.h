#pragma once

#include "statement.h"

namespace ast {

struct Empty {
  bool operator==(const Empty&) const = default;
};

struct Assign {
  string var;
  string op;
  bool operator==(const Assign&) const = default;
};

struct If {
  Stmt::ptr true_branch;
  string cond;
  bool operator==(const If&) const = default;
};

struct While {
  Stmt::ptr body;
  string cond;
  bool operator==(const While&) const = default;
};

struct Print {
  string var;
  bool operator==(const Print&) const = default;
};

struct Goto {
  string dest;
  string cond;
  bool operator==(const Goto&) const = default;
};

struct Break {
  bool operator==(const Break&) const = default;
};

void fix_parents(Expr& e, Stmt* parent);

class Expr {
 public:
  using content = variant<Assign, If, While, Print, Goto, Break, Empty>;
  content contents;
  Stmt* par_stmt = nullptr;
  string label;

  template <typename T>
  Expr(string label, T&& conts) : contents(move(conts)), label(move(label)) {
    fix_parents(*this, nullptr);
  }
  Expr() = default;
  /* Expr(const Expr&) = default; */
  Expr(Expr&&) = default;

  /* Expr& operator=(const Expr&) = default; */
  /* Expr& operator=(Expr&&) = default; */

  bool operator==(const Expr&) const = default;

  virtual ~Expr() = default;

  friend ostream& operator<<(ostream&, const Expr&);

  friend class Stmt;
};
ostream& operator<<(ostream& os, const Expr& v);
Expr expressionFactory(string s);
}  // namespace ast
