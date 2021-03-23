#pragma once

#include "ast.h"

namespace tests {
class TestStmt {
 private:
  TestStmt();

 public:
  static void test_parent(const statement::Stmt& s);
  static void run_all(const statement::Stmt& s);
};
}  // namespace tests
