#include <set>
#include <list>
#include <memory>
#include <unordered_map>
#include <cassert>

#include "3rd_party/termcolor.hpp"

using namespace std;

namespace statement {

/******************
 *  Main classes  *
 ******************/

enum class Condition { None, True, False };

class Expression {
 public:
  const string label;
  typedef std::shared_ptr<Expression> ptr;
  Expression(string label);
  Expression() = default;
  virtual operator string();
  virtual ~Expression() = default;
  /* friend ostream& operator<<(ostream& os, Expression::ptr a); */
};

class Statement {
 private:
  list<Expression::ptr> s;

 public:
  typedef std::shared_ptr<Statement> ptr;
  /* Statement() = default; */
  Statement(list<Expression::ptr>);
  void add(Expression::ptr& left, Expression::ptr& right, Condition cond);
  void replace(string pattern, Expression::ptr& replacement);
  operator string();
  virtual ~Statement() = default;
  /* friend ostream& operator<<(ostream& os, Statement::ptr a); */
};

/*******************************
 *  Expression specifications  *
 *******************************/

class Assignment : public Expression {
 public:
  const string var;
  const string op;

  Assignment() = default;
  Assignment(string label, string var, string op);
  virtual operator string() final override;
  virtual ~Assignment() = default;
};

class Conditional : public Expression {
 public:
  Statement::ptr true_branch;
  Statement::ptr false_branch;
  string condition;

  Conditional() = default;
  Conditional(string label, string condition,
              Statement::ptr true_branch = nullptr,
              Statement::ptr false_branch = nullptr);
  virtual operator string() final override;
  virtual ~Conditional() = default;
};

class WhileLoop : public Expression {
 public:
  Statement::ptr body;
  string condition;

  WhileLoop() = default;
  WhileLoop(string label, Statement::ptr body, string condition);
  virtual operator string() final override;
  virtual ~WhileLoop() = default;
};

class Print : public Expression {
 public:
  string var;

  Print() = default;
  Print(string label, string var);
  virtual operator string() final override;
  virtual ~Print() = default;
};

class Goto : public Expression {
 public:
  string dest;
  string condition;

  Goto() = default;
  Goto(string label, string condition, string dest="");
  virtual operator string() final override;
  virtual ~Goto() = default;
};

Expression::ptr expressionFactory(string s);
Statement::ptr statementFactory(list<string> s);
}  // namespace statement
