#pragma once

#include <optional>
#include <algorithm>
#include <cassert>
#include <list>
#include <memory>
#include <set>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <stack>

#include "algs/algs.h"

using namespace std;

namespace statement {

enum class Cond { None, True, False };

class BaseExpr;
class Stmt;
class Assign;
class If;
class While;
class Print;
class Goto;
class Break;

typedef variant<BaseExpr, Assign, If, While, Print, Goto, Break> Expr;

/**********************************
 *  Base class for an expression  *
 **********************************/

class BaseExpr {
 public:
  Stmt* parent_stmt = nullptr;
  string label;

  BaseExpr(string label) : label(move(label)) {}
  BaseExpr() = default;
  /* BaseExpr(const BaseExpr&) = default; */
  BaseExpr(BaseExpr&&) = default;

  /* BaseExpr& operator=(const BaseExpr&) = default; */
  BaseExpr& operator=(BaseExpr&&) = default;

  bool operator==(const BaseExpr&) const = default;

  virtual ~BaseExpr() = default;

  friend ostream& operator<<(ostream&, const BaseExpr&);

  friend class Stmt;
};

/***************
 *  Statement  *
 ***************/

class Stmt {
 private:
  // Possible UB, Expr is not defined
  list<Expr> s;

 public:
  using ptr = unique_ptr<Stmt>;
  struct Iterator;
  Expr* parent_expr = nullptr;

  Stmt(Expr* = nullptr);
  Stmt(list<Expr>&&, Expr* = nullptr);
  Stmt(Expr&&, Expr* = nullptr);
  Stmt(const Stmt&) = default;
  Stmt(Stmt&&) = default;

  Stmt& operator=(const Stmt&) = default;
  Stmt& operator=(Stmt&&) = default;

  ~Stmt() = default;

  void insert(const string& parent, Expr expr, bool after);
  void push_back(Expr&& expr);
  void remove(const string& label);
  void replace(string pattern, Expr& replacement);
  Iterator find(const string& label);
  template <class T>
  Iterator find_type();

  Iterator begin();
  Iterator end();
  size_t size() const {return s.size();};
  bool empty() const;

  Iterator move_outward(const Iterator& it);

  friend ostream& operator<<(ostream& os, const Stmt& a);
};

/*******************************
 *  Expression specifications  *
 *******************************/

class Assign : public BaseExpr {
 public:
  string var;
  string op;

  Assign(string label, string var, string op)
      : BaseExpr(move(label)), var(move(var)), op(move(op)) {}
  Assign() = default;
  /* Assign(const Assign&) = default; */
  Assign(Assign&&) = default;

  /* Assign& operator=(const Assign&) = default; */
  Assign& operator=(Assign&&) = default;

  friend ostream& operator<<(ostream& os, const Assign& a);

  virtual ~Assign() = default;
};

class If : public BaseExpr {
 public:
  Stmt::ptr true_branch;
  string cond;

  If(string label, string condition, Stmt::ptr true_branch)
      : BaseExpr(move(label)),
        cond(move(condition)),
        true_branch(move(true_branch)) {}
  If() = default;
  /* If(const If&) = default; */
  If(If&&) = default;

  /* If& operator=(const If&) = default; */
  If& operator=(If&&) = default;

  friend ostream& operator<<(ostream& os, const If& a);

  virtual ~If() = default;
};

class While : public BaseExpr {
 public:
  Stmt::ptr body;
  string cond;

  While(string label, Stmt::ptr body, string condition)
      : BaseExpr(move(label)), body(move(body)), cond(move(condition)) {}
  While() = default;
  /* While(const While&) = default; */
  While(While&&) = default;

  /* While& operator=(const While&) = default; */
  While& operator=(While&&) = default;

  friend ostream& operator<<(ostream& os, const While& a);

  virtual ~While() = default;
};

class Print : public BaseExpr {
 public:
  string var;

  Print(string label, string var) : BaseExpr(move(label)), var(move(var)){};
  Print() = default;
  /* Print(const Print&) = default; */
  Print(Print&&) = default;

  /* Print& operator=(const Print&) = default; */
  Print& operator=(Print&&) = default;

  friend ostream& operator<<(ostream& os, const Print& a);

  virtual ~Print() = default;
};

class Goto : public BaseExpr {
 public:
  string dest;
  string cond;

  Goto(string label, string cond, string dest = "")
      : BaseExpr(move(label)), cond(move(cond)), dest(move(dest)){};
  Goto() = default;
  /* Goto(const Goto&) = default; */
  Goto(Goto&&) = default;

  /* Goto& operator=(const Goto&) = default; */
  Goto& operator=(Goto&&) = default;
  /* template <class T> */
  /* bool operator==(const T& other) { */
  /* return BaseExpr::operator==(static_cast<BaseExpr&>(other)); */
  /* } */

  friend ostream& operator<<(ostream& os, const Goto& a);

  virtual ~Goto() = default;
};

class Break : public BaseExpr {
 public:
  Break(string label) : BaseExpr(move(label)){};
  Break() = default;
  /* Break(const Break&) = default; */
  Break(Break&&) = default;

  /* Break& operator=(const Break&) = default; */
  Break& operator=(Break&&) = default;

  friend ostream& operator<<(ostream& os, const Break& a);

  virtual ~Break() = default;
};

/******************
 *  Declarations  *
 ******************/

typedef unordered_map<string, pair<Expr, string>> ParseTree;
ostream& operator<<(ostream& os, const statement::ParseTree& t);

ostream& operator<<(ostream& os, const Expr& v);
string get_label(const Expr& v);
/* Stmt* get_parent_stmt(const Expr& v); */
Stmt* get_parent_stmt(const Expr& v);
Expr* get_parent_expr(const Stmt& v);
Expr* get_parent_expr(const Expr& v);
void set_parent_stmt(Expr& v, Stmt* parent_stmt);
void set_parent_expr(Stmt& v, Expr* parent_expr);

Expr expressionFactory(string s);
Stmt::ptr statementFactory(list<string> s);

template <typename T, enable_if_t<is_base_of<BaseExpr, T>::value, bool> = true>
bool operator==(const T& first, const T& second) {
  return static_cast<const BaseExpr&>(first) ==
         static_cast<const BaseExpr&>(second);
}

/**************************
 *  Statement's iterator  *
 **************************/

class Stmt::Iterator {
  private:
    bool ensure_not_end();
  public:
  using iterator_category = std::forward_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using value_type = Expr;
  using pointer = list<Expr>::iterator;
  using reference = value_type&;

  Iterator(const pointer& it) : it(it){};
  Iterator(pointer&& it) : it(it){};
  Iterator(const Iterator&) = default;
  Iterator(Iterator&&) = default;
  Iterator& operator=(const Iterator&) = default;
  Iterator& operator=(Iterator&&) = default;
  Iterator& operator=(const pointer& it) {
    this->it = it;
    return *this;
  };
  Iterator& operator=(pointer&& it) {
    this->it = it;
    return *this;
  }

  reference operator*() const { return *it; }
  pointer operator->() { return it; }

  // Prefix increment
  Iterator& operator++();

  // Postfix increment
  Iterator operator++(int) {
    Iterator tmp = *this;
    ++(*this);
    return tmp;
  }

  friend bool operator==(const Iterator& a, const Iterator& b) {
    return a.it == b.it;
  };
  friend bool operator!=(const Iterator& a, const Iterator& b) {
    return a.it != b.it;
  };

 private:
  pointer it;
  stack<pointer> history;
};

template <class T>
Stmt::Iterator Stmt::find_type() {
  return std::find_if(this->begin(), this->end(), [](const Expr& expr) {
    return holds_alternative<T>(expr);
  });
}
}  // namespace statement
