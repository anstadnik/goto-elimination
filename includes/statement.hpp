#include <list>
#include <memory>
#include <unordered_map>

#include "3rd_party/termcolor.hpp"

using namespace std;

namespace statement {

/******************
 *  Main classes  *
 ******************/

class Expression {
 private:
  const string label;

 public:
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
  operator string();
  virtual ~Statement() = default;
  /* friend ostream& operator<<(ostream& os, Statement::ptr a); */
};

/*******************************
 *  Expression specifications  *
 *******************************/

class Assignment : public Expression {
 private:
  const string var;
  const string op;

 public:
  Assignment() = default;
  Assignment(string label, string var, string op);
  virtual operator string() final override;
  virtual ~Assignment() = default;
};

class Conditional : public Expression {
 private:
  Statement::ptr true_branch;
  Statement::ptr false_branch;
  string condition;

 public:
  Conditional() = default;
  Conditional(string label, string condition,
              Statement::ptr true_branch = nullptr,
              Statement::ptr false_branch = nullptr);
  virtual operator string() final override;
  virtual ~Conditional() = default;
};

class WhileLoop : public Expression {
 private:
  Statement::ptr body;
  string condition;

 public:
  WhileLoop() = default;
  WhileLoop(string label, Statement::ptr body, string condition);
  virtual operator string() final override;
  virtual ~WhileLoop() = default;
};

class Print : public Expression {
 private:
  string var;

 public:
  Print() = default;
  Print(string label, string var);
  virtual operator string() final override;
  virtual ~Print() = default;
};

class Goto : public Expression {
 private:
  string dest;
  string condition;

 public:
  Goto() = default;
  Goto(string label, string condition, string dest);
  virtual operator string() final override;
  virtual ~Goto() = default;
};

Expression::ptr expressionFactory(string s);
Statement::ptr statementFactory(list<string> s);
}  // namespace statement
