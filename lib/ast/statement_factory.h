#pragma once

#include "parse_tree.h"
#include "statement.h"

namespace ast {
Stmt::ptr statementFactory(list<string> s);

class StatementFactory {
 private:
  StatementFactory();
  virtual ~StatementFactory();

  static void fix_parents(Stmt& s);
  static Stmt::ptr parse_tree_to_statement(ParseTree& t, const string& first,
                                           unordered_set<string>& keys);
  static Stmt::ptr parse_tree_to_statement(ParseTree& t, const string& first);

 public:
  static Stmt::ptr gen_statement(list<string> s);
};
}  // namespace ast
