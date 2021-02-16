#pragma once

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

typedef variant<BaseExpr, Assign, If, While, Print, Goto> Expr;

/**********************************
 *  Base class for an expression  *
 **********************************/

class BaseExpr {
 public:
  Stmt* parent_stmt = nullptr;
  string label;

  BaseExpr() = default;
  BaseExpr(const BaseExpr&) = default;
  BaseExpr(BaseExpr&&) = default;
  BaseExpr& operator=(const BaseExpr&) = default;
  BaseExpr& operator=(BaseExpr&&) = default;
  BaseExpr(string label) : label(move(label)) {}
  friend ostream& operator<<(ostream& os, const BaseExpr& a);
  virtual ~BaseExpr() = default;
  friend class Stmt;
};

/***************
 *  Statement  *
 ***************/

class Stmt {
 private:
  struct Iterator;
  // Possible UB, Expr is not defined
  list<Expr> s;

 public:
  Stmt* parent_stmt = nullptr;

  Stmt(Stmt* parent_stmt = nullptr);
  Stmt(list<Expr>, Stmt* parent_stmt = nullptr);
  Stmt(const Stmt&) = default;
  Stmt(Stmt&&) = default;
  Stmt& operator=(const Stmt&) = default;
  Stmt& operator=(Stmt&&) = default;

  void insert(const string& parent, Expr expr);
  void push_back(Expr expr);
  template <class T>
  list<Expr>::iterator find_type() {
    return std::find_if(s.begin(), s.end(), [](const Expr& expr) {
      return holds_alternative<T>(expr);
    });
  }
  void remove(const string& label);
  Iterator begin();
  Iterator end();

  /* void move_to_cond(string cond_label, list<Expr>::iterator begin,
   * list<Expr>::iterator end); */
  void replace(string pattern, Expr& replacement);
  friend ostream& operator<<(ostream& os, const Stmt& a);
  bool empty() const;
  virtual ~Stmt() = default;
};

/*******************************
 *  Expression specifications  *
 *******************************/

class Assign : public BaseExpr {
 public:
  string var;
  string op;

  Assign() = default;
  Assign(const Assign&) = default;
  Assign(Assign&&) = default;
  Assign& operator=(const Assign&) = default;
  Assign& operator=(Assign&&) = default;
  Assign(string label, string var, string op)
      : BaseExpr(move(label)), var(move(var)), op(move(op)) {}
  friend ostream& operator<<(ostream& os, const Assign& a);
  virtual ~Assign() = default;
};

class If : public BaseExpr {
 public:
  Stmt true_branch;
  string condition;

  If() = default;
  If(const If&) = default;
  If(If&&) = default;
  If& operator=(const If&) = default;
  If& operator=(If&&) = default;
  If(string label, string condition, Stmt true_branch = Stmt())
      : BaseExpr(move(label)),
        condition(move(condition)),
        true_branch(move(true_branch)) {
    true_branch.parent_stmt = this->parent_stmt;
  }
  friend ostream& operator<<(ostream& os, const If& a);
  virtual ~If() = default;
};

class While : public BaseExpr {
 public:
  Stmt body;
  string condition;

  While() = default;
  While(const While&) = default;
  While(While&&) = default;
  While& operator=(const While&) = default;
  While& operator=(While&&) = default;
  While(string label, Stmt body, string condition)
      : BaseExpr(move(label)), body(move(body)), condition(move(condition)) {
    body.parent_stmt = this->parent_stmt;
  }
  friend ostream& operator<<(ostream& os, const While& a);
  virtual ~While() = default;
};

class Print : public BaseExpr {
 public:
  string var;

  Print() = default;
  Print(const Print&) = default;
  Print(Print&&) = default;
  Print& operator=(const Print&) = default;
  Print& operator=(Print&&) = default;
  Print(string label, string var) : BaseExpr(move(label)), var(move(var)){};
  friend ostream& operator<<(ostream& os, const Print& a);
  virtual ~Print() = default;
};

class Goto : public BaseExpr {
 public:
  string dest;
  string condition;

  Goto() = default;
  Goto(const Goto&) = default;
  Goto(Goto&&) = default;
  Goto& operator=(const Goto&) = default;
  Goto& operator=(Goto&&) = default;
  Goto(string label, string condition, string dest = "")
      : BaseExpr(move(label)), condition(move(condition)), dest(move(dest)){};
  friend ostream& operator<<(ostream& os, const Goto& a);
  virtual ~Goto() = default;
};

/******************
 *  Declarations  *
 ******************/

typedef unordered_map<string, pair<Expr, string>> ParseTree;
ostream& operator<<(ostream& os, const statement::ParseTree& t);

/* string get_string(const Expr& v); */
ostream& operator<<(ostream& os, const Expr& v);
string get_label(const Expr& v);
Stmt* get_parent_stmt(const Expr& v);
Stmt* get_parent_stmt(const Stmt& v);
void set_parent_stmt(Expr& v, Stmt* parent_stmt);

Expr expressionFactory(string s);
Stmt statementFactory(list<string> s);

/**************************
 *  Statement's iterator  *
 **************************/

struct Stmt::Iterator {
  using iterator_category = std::forward_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using value_type = Expr;
  using iterator = list<Expr>::iterator;
  using reference = value_type&;

  Iterator(const iterator& it) : it(it){};
  Iterator(iterator&& it) : it(it){};
  Iterator(const Iterator&) = default;
  Iterator(Iterator&&) = default;
  Iterator& operator=(const Iterator&) = default;
  Iterator& operator=(Iterator&&) = default;
  Iterator& operator=(const iterator& it) {
    this->it = it;
    return *this;
  };
  Iterator& operator=(iterator&& it) {
    this->it = it;
    return *this;
  }

  reference operator*() const { return *it; }
  iterator operator->() { return it; }

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
  iterator it;
};
}  // namespace statement
