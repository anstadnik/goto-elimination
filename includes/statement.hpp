#include <set>
#include <list>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <cassert>
#include <variant>
#include <algorithm>
#include <utility>
#include <type_traits>

#include "3rd_party/termcolor.hpp"

using namespace std;

namespace statement {

/******************
 *  Main classes  *
 ******************/

enum class Cond { None, True, False };

class BaseExpr;
class Stmt;
class Assign;
class If;
class While;
class Print;
class Goto;

typedef variant<BaseExpr, Assign, If, While, Print, Goto> Expr;

class BaseExpr {
 public:
  string label;
  BaseExpr() = default;
  BaseExpr(const BaseExpr&) = default;
  BaseExpr(BaseExpr&&) = default;
  BaseExpr& operator= (const BaseExpr&) = default;
  BaseExpr& operator= (BaseExpr&&) = default;
  BaseExpr(string label);
  virtual operator string() const;
  virtual ~BaseExpr() = default;
  /* friend ostream& operator<<(ostream& os, Expression::ptr a); */
};

class Stmt {
 private:
  // Possible UB, Expr is not defined
  list<Expr> s;

 public:
  Stmt() = default;
  Stmt(list<Expr>);
  Stmt(const Stmt&) = default;
  Stmt(Stmt&&) = default;
  Stmt& operator= (const Stmt&) = default;
  Stmt& operator= (Stmt&&) = default;
  void insert(string parent, Expr& expr);
  void push_back(Expr expr);
  /* void move_to_cond(string cond_label, list<Expr>::iterator begin, list<Expr>::iterator end); */
  void replace(string pattern, Expr& replacement);
  operator string() const;
  bool empty() const;
  virtual ~Stmt() = default;
  /* friend ostream& operator<<(ostream& os, Statement::ptr a); */
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
  Assign(string label, string var, string op);
  virtual operator string() const final override;
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
  If(string label, string condition, Stmt true_branch = Stmt());
  virtual operator string() const final override;
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
  While(string label, Stmt body, string condition);
  virtual operator string() const final override;
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
  Print(string label, string var);
  virtual operator string() const final override;
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
  Goto(string label, string condition, string dest="");
  virtual operator string() const final override;
  virtual ~Goto() = default;
};

typedef unordered_map<string, pair<Expr, string>> ParseTree;

string get_string(const Expr& v);
string get_label(const Expr& v);

Expr expressionFactory(string s);
Stmt statementFactory(list<string> s);
}  // namespace statement
