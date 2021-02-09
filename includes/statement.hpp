#include <set>
#include <list>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <cassert>

#include "3rd_party/termcolor.hpp"

using namespace std;

namespace statement {

/******************
 *  Main classes  *
 ******************/

enum class Cond { None, True, False };

class Expr {
 public:
  const string label;
  typedef std::shared_ptr<Expr> ptr;
  Expr(string label);
  Expr() = default;
  virtual operator string();
  virtual ~Expr() = default;
  /* friend ostream& operator<<(ostream& os, Expression::ptr a); */
};

class Stmt {
 private:
  list<Expr::ptr> s;

 public:
  typedef std::shared_ptr<Stmt> ptr;
  Stmt() = default;
  Stmt(list<Expr::ptr>);
  void add(Expr::ptr& left, Expr::ptr& right, Cond cond);
  void replace(string pattern, Expr::ptr& replacement);
  operator string();
  virtual ~Stmt() = default;
  /* friend ostream& operator<<(ostream& os, Statement::ptr a); */
};

/*******************************
 *  Expression specifications  *
 *******************************/

class Assign : public Expr {
 public:
  const string var;
  const string op;

  Assign() = default;
  Assign(string label, string var, string op);
  virtual operator string() final override;
  virtual ~Assign() = default;
};

class If : public Expr {
 public:
  Stmt::ptr true_branch;
  Stmt::ptr false_branch;
  string condition;

  If() = default;
  If(string label, string condition,
              Stmt::ptr true_branch = nullptr,
              Stmt::ptr false_branch = nullptr);
  virtual operator string() final override;
  virtual ~If() = default;
};

class While : public Expr {
 public:
  Stmt::ptr body;
  string condition;

  While() = default;
  While(string label, Stmt::ptr body, string condition);
  virtual operator string() final override;
  virtual ~While() = default;
};

class Print : public Expr {
 public:
  string var;

  Print() = default;
  Print(string label, string var);
  virtual operator string() final override;
  virtual ~Print() = default;
};

class Goto : public Expr {
 public:
  string dest;
  string condition;

  Goto() = default;
  Goto(string label, string condition, string dest="");
  virtual operator string() final override;
  virtual ~Goto() = default;
};

typedef unordered_map<string, pair<Expr::ptr, string>> ParseTree;
Expr::ptr expressionFactory(string s);
Stmt::ptr statementFactory(list<string> s);
}  // namespace statement
