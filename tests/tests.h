#pragma once

#include <gtest/gtest.h>

#include "ast.h"
#include "goto_elimination.h"

namespace tests {

class TestTransformations : public ::testing::Test {
 protected:
  void SetUp() override {
    s = ast::StatementFactory::gen_statement(
        algs::io::readFileToList("inputs/complicated.txt"));
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

class TestGotoElimination : public ::testing::TestWithParam<const char*> {
 protected:
  void SetUp() override {
    s = ast::StatementFactory::gen_statement(
        algs::io::readFileToList(GetParam()));
  }

  ast::Stmt::ptr s;
};
}  // namespace tests
