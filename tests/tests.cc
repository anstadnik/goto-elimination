#include "tests.h"

using namespace statement;
namespace tests {
void TestStmt::test_parent(const Stmt& s) {
  for (const auto& e : s.s) {
    assert(e.par_stmt == &s);
    if (holds_alternative<If>(e.contents)) {
      const auto& true_branch = *get<If>(e.contents).true_branch;
      assert(true_branch.par_expr == &e);
      test_parent(true_branch);
    } else if (holds_alternative<While>(e.contents)) {
      const auto& body = *get<While>(e.contents).body;
      assert(body.par_expr == &e);
      test_parent(body);
    }
  }
}

void TestStmt::run_all(const Stmt& s) {
  assert(!s.par_expr);
  test_parent(s); }
}  // namespace tests
