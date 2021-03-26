#include "tests.h"
#include "statement_factory.h"

using namespace ast;
using namespace goto_elimination;
namespace tests {

TEST_F(TestTransformations, testLoad) {
  ASSERT_FALSE(s->empty());
  ASSERT_FALSE(s->par_expr);
}
TEST_F(TestTransformations, testOffset) {
  ASSERT_LT(offset(*a), offset(*b));
  ASSERT_EQ(offset(*h), 14);
}
TEST_F(TestTransformations, testLevel) {
  ASSERT_EQ(level(*a),level(*b));
  ASSERT_EQ(level(*j),2);
}
TEST_F(TestTransformations, testSiblings) {
  ASSERT_TRUE(siblings(a, b));
  ASSERT_FALSE(siblings(a, j));
  ASSERT_FALSE(siblings(q, k));
}
TEST_F(TestTransformations, testDirectlyRelated) {
  ASSERT_TRUE(directly_related(j, a));
  ASSERT_FALSE(directly_related(j, k));
}
TEST_F(TestTransformations, testIndirectlyRelated) {
  ASSERT_FALSE(indirectly_related(j, a));
  ASSERT_TRUE(indirectly_related(j, k));
}
TEST_F(TestTransformations, testRemove) {
  s->remove("P");
  ASSERT_EQ(s->find("P"), s->end());
}
TEST_F(TestTransformations, testInsert) {
  size_t old_level = level(*p), old_offset = offset(*p);
  p->par_stmt->insert(p, Expr("Potato", If{make_unique<Stmt>(), "cond"}));
  ASSERT_EQ(old_level, level(*s->find("P")));
  ASSERT_EQ(old_offset+1, offset(*s->find("P")));
  ASSERT_NE(s->find("P"), s->end());
  ASSERT_NE(s->find("Potato"), s->end());
}
TEST_F(TestTransformations, testExtract) {
  Stmt::ptr extracted = m->par_stmt->extract_from(m);
  ASSERT_EQ(s->find("P"), s->end());
  ASSERT_NE(extracted->find("P"), extracted->end());
  ASSERT_EQ(offset(*s->find("H")), 10);
}
TEST_F(TestTransformations, testMoveOutward) {
  size_t old_level = level(*p), old_offset = offset(*p);
  p = move_outward(p);
  ASSERT_LT(level(*p), old_level);
  ASSERT_GT(offset(*p), old_offset);
}
TEST_F(TestTransformations, testMoveInward) {
  size_t old_level = level(*e), old_offset = offset(*e);
  move_inward(e, o);
  ASSERT_GT(level(*p), old_level);
  ASSERT_GT(offset(*p), old_offset);
}
TEST_P(TestGotoElimination, TestEqOutput) {
  s->dump("/tmp/original.cc");
  /* std::cout << *s << std::endl; */
  s = goto_elimination::eliminateGoto(move(s));
  /* std::cout << *s << std::endl; */
  s->dump("/tmp/changed.cc");

  ASSERT_EQ(s->find_type<Goto>(), s->end());

  ASSERT_EQ(system("g++ /tmp/original.cc -o /tmp/original"), 0);
  std::cout << "Original output:" << std::endl;
  ASSERT_EQ(system("/tmp/original"), 0);

  ASSERT_EQ(system("g++ /tmp/changed.cc -o /tmp/changed"), 0);
  std::cout << "Changed output:" << std::endl;
  ASSERT_EQ(system("/tmp/changed"), 0);

  ASSERT_EQ(system("diff <(/tmp/original) <(/tmp/changed)"), 0);
}

INSTANTIATE_TEST_SUITE_P(
    TestEqOutput, TestGotoElimination,
    testing::Values("inputs/lift.txt", "inputs/elim_if.txt",
                    "inputs/normal.txt", "inputs/elim_while.txt"),
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
