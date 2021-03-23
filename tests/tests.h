#pragma once

#include "ast.h"
#include "goto_elimination.h"
#include <gtest/gtest.h>

namespace tests {

class StmtTest : public ::testing::Test {
 protected:
  StmtTest(){};
  ~StmtTest(){};

  void SetUp() override {
    s = ast::StatementFactory::gen_statement(
        readFileToList("inputs/complicated.txt"));
    a = s->find("A");
    b = s->find("B");
    o = s->find("O");
    q = s->find("Q");
    j = s->find("J");
    k = s->find("K");
    h = s->find("H");
  }

  void TearDown() override {}

  ast::Stmt::ptr s;
  ast::Stmt::Iterator a;
  ast::Stmt::Iterator b;
  ast::Stmt::Iterator o;
  ast::Stmt::Iterator q;
  ast::Stmt::Iterator j;
  ast::Stmt::Iterator k;
  ast::Stmt::Iterator h;
};
}  // namespace tests
