#include "tests.h"
#include "statement_factory.h"

using namespace ast;
using namespace goto_elimination;

namespace backward {
backward::SignalHandling sh;
}  // namespace backward

namespace tests {

/* TEST_F(TestTransformations, testLoad) { */
/*   ASSERT_FALSE(s->empty()); */
/*   ASSERT_FALSE(s->par_expr); */
/* } */
/* TEST_F(TestTransformations, testOffset) { */
/*   ASSERT_LT(offset(*a), offset(*b)); */
/*   ASSERT_EQ(offset(*h), 14); */
/* } */
/* TEST_F(TestTransformations, testLevel) { */
/*   ASSERT_EQ(level(*a),level(*b)); */
/*   ASSERT_EQ(level(*j),2); */
/* } */
/* TEST_F(TestTransformations, testSiblings) { */
/*   ASSERT_TRUE(siblings(a, b)); */
/*   ASSERT_FALSE(siblings(a, j)); */
/*   ASSERT_FALSE(siblings(q, k)); */
/* } */
/* TEST_F(TestTransformations, testDirectlyRelated) { */
/*   ASSERT_TRUE(directly_related(j, a)); */
/*   ASSERT_FALSE(directly_related(j, k)); */
/* } */
/* TEST_F(TestTransformations, testIndirectlyRelated) { */
/*   ASSERT_FALSE(indirectly_related(j, a)); */
/*   ASSERT_TRUE(indirectly_related(j, k)); */
/* } */
/* TEST_F(TestTransformations, testErase) { */
/*   s->erase(s->find("P")); */
/*   ASSERT_EQ(s->find("P"), s->end()); */
/* } */
/* TEST_F(TestTransformations, testInsert) { */
/*   size_t old_level = level(*p), old_offset = offset(*p); */
/*   p->par_stmt->insert(p, Expr("Potato", If{make_unique<Stmt>(), "cond"})); */
/*   ASSERT_EQ(old_level, level(*s->find("P"))); */
/*   ASSERT_EQ(old_offset+1, offset(*s->find("P"))); */
/*   ASSERT_NE(s->find("P"), s->end()); */
/*   ASSERT_NE(s->find("Potato"), s->end()); */
/* } */
/* TEST_F(TestTransformations, testExtract) { */
/*   Stmt::ptr extracted = m->par_stmt->extract_from(m); */
/*   ASSERT_EQ(s->find("P"), s->end()); */
/*   ASSERT_NE(extracted->find("P"), extracted->end()); */
/*   ASSERT_EQ(offset(*s->find("H")), 10); */
/* } */
/* TEST_F(TestTransformations, testMoveOutward) { */
/*   size_t old_level = level(*p), old_offset = offset(*p); */
/*   p = move_outward(p); */
/*   ASSERT_LT(level(*p), old_level); */
/*   ASSERT_GT(offset(*p), old_offset); */
/* } */
/* TEST_F(TestTransformations, testMoveInward) { */
/*   size_t old_level = level(*e), old_offset = offset(*e); */
/*   move_inward(e, o); */
/*   ASSERT_GT(level(*p), old_level); */
/*   ASSERT_GT(offset(*p), old_offset); */
/* } */

class LoggerTesting {
  void dump(Stmt& s) {
    s.dump("/tmp/current.cc");
    assert(!system("g++ /tmp/current.cc -o /tmp/current"));
    assert(!system("/tmp/current > /dev/null"));
  }

 public:
  LoggerTesting(Stmt& s) {
    dump(s);
    rename("/tmp/current", "/tmp/previous");
    rename("/tmp/current.cc", "/tmp/previous.cc");
  }
  template <typename T>
  ostream operator<<(const T& v) {
    (void)v;
    return ostream(0);
  }
  ostream operator<<(Stmt& s) {
    dump(s);
    assert(!system("diff <(/tmp/current) <(/tmp/previous)"));
    rename("/tmp/current", "/tmp/previous");
    rename("/tmp/current.cc", "/tmp/previous.cc");
    return ostream(0);
  }
};

TEST_P(TestGotoElimination, TestEqOutput) {
  s->dump("/tmp/original.cc");
  auto logger = LoggerTesting(*s);
  s = goto_elimination::eliminateGoto(move(s), logger);
  s->dump("/tmp/changed.cc");

  ASSERT_EQ(s->find_type<Goto>(), s->end());

  ASSERT_EQ(system("g++ /tmp/original.cc -o /tmp/original"), 0);
  ASSERT_EQ(system("/tmp/original > /dev/null"), 0);

  ASSERT_EQ(system("g++ /tmp/changed.cc -o /tmp/changed"), 0);
  ASSERT_EQ(system("/tmp/changed > /dev/null"), 0);

  ASSERT_EQ(system("diff <(/tmp/original) <(/tmp/changed)"), 0);
}

INSTANTIATE_TEST_SUITE_P(
    TestEqOutput, TestGotoElimination,
    testing::Values("inputs/elim_while.txt", "inputs/out.txt", "inputs/lift.txt",
                    "inputs/indirectly_related.txt", "inputs/working_complicated.txt"),

    [](testing::TestParamInfo<TestGotoElimination::ParamType> s)
        -> std::string {
      std::string s_(s.param);
      return s_.erase(s_.size() - 4, 4).erase(0, 7);
    });

}  // namespace tests
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
