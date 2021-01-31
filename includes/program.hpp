#include <cctype>
#include <variant>
#include <chrono>
#include <fstream>
#include <io/io.hpp>
#include <iostream>
#include <sstream>
#include <thread>
#include <unordered_map>
#include <variant>
#include <memory>
#include <algorithm>

using namespace std;

namespace program 
{
  class Expression {
   private:
    const string label;
  
   public:
    typedef std::shared_ptr<Expression> ptr;
    Expression(const string);
    virtual ~Expression() = 0;
    friend ostream& operator<<(ostream& os, Expression::ptr a);
  };

  class Arbitrary: public Expression {
    private:
      const string code;

   public:
    Arbitrary(const string, const string);
    virtual ~Arbitrary();
  };
  
  class Assignment : public Expression {
   private:
    const string var;
    const string op;
  
   public:
    Assignment(const string, const string, const string);
    virtual ~Assignment();
  };
  
  class Conditional : public Expression {
   private:
   public:
    Conditional(const string&);
    virtual ~Conditional();
  };
  
  class WhileLoop : public Expression {
   private:
   public:
    WhileLoop(const string&);
    virtual ~WhileLoop();
  };

  class Program {
   private:
    unordered_map<string, variant<Expression::ptr>> p;
  
   public:
    Program(list<string>);
    virtual ~Program();
  };
  
  Expression::ptr expressionFactory(string s);
  pair<Expression::ptr, Expression::ptr> splitConnection(const string& s);
} /*  program  */ 
