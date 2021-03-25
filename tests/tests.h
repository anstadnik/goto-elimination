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
    e = s->find("E");
    o = s->find("O");
    q = s->find("Q");
    j = s->find("J");
    m = s->find("M");
    k = s->find("K");
    p = s->find("P");
    h = s->find("H");
  }

  void TearDown() override {}

  ast::Stmt::ptr s;
  ast::Stmt::Iterator a;
  ast::Stmt::Iterator b;
  ast::Stmt::Iterator e;
  ast::Stmt::Iterator o;
  ast::Stmt::Iterator q;
  ast::Stmt::Iterator j;
  ast::Stmt::Iterator m;
  ast::Stmt::Iterator k;
  ast::Stmt::Iterator p;
  ast::Stmt::Iterator h;
};
}  // namespace tests
