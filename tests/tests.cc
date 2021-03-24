#include "tests.h"

using namespace ast;
using namespace goto_elimination;
namespace tests {

/*
A:  a=0;
B:  b=0;
C:  a=a+1;
D:  b=b+1;
E:  if (C) { goto b<42; }
F:  cout << a << endl;
O:  if (b>=1000) {
Q:    if (b>100) {
J:      cout << "HELLO" << endl;
      }
M:    if (b>100) {
K:      cout << "WORLD" << endl;
      }
P:    if (D) { goto b>100; }
_P:   if (true) { goto Q; }
    }
G:  if (B) { goto !a%5==0; }
H:  if (E) { goto a<500; }
_H: if (true) { goto B; }
*/

TEST_F(StmtTest, testLoad) {
  ASSERT_FALSE(s->empty());
  ASSERT_FALSE(s->par_expr);
}
TEST_F(StmtTest, testOffset) {
  ASSERT_LT(offset(*a), offset(*b));
  ASSERT_EQ(offset(*h), 14);
}
TEST_F(StmtTest, testLevel) {
  ASSERT_EQ(level(*a),level(*b));
  ASSERT_EQ(level(*j),2);
}
TEST_F(StmtTest, testSiblings) {
  ASSERT_TRUE(siblings(a, b));
  ASSERT_FALSE(siblings(a, j));
  ASSERT_FALSE(siblings(q, k));
}
TEST_F(StmtTest, testDirectlyRelated) {
  ASSERT_TRUE(directly_related(j, a));
  ASSERT_FALSE(directly_related(j, k));
}
TEST_F(StmtTest, testIndirectlyRelated) {
  ASSERT_FALSE(indirectly_related(j, a));
  ASSERT_TRUE(indirectly_related(j, k));
}
TEST_F(StmtTest, testInsert) {
  size_t old_level = level(*p), old_offset = offset(*p);
  s->insert("P", Expr("Potato", If{make_unique<Stmt>(), "cond"}));
  ASSERT_EQ(old_level, level(*s->find("P")));
  ASSERT_EQ(old_offset+1, offset(*s->find("P")));
  ASSERT_NE(s->find("P"), s->end());
  ASSERT_NE(s->find("Potato"), s->end());
}
TEST_F(StmtTest, testExtract) {
  Stmt::ptr extracted = s->extract_from("M");
  ASSERT_EQ(s->find("P"), s->end());
  ASSERT_NE(extracted->find("P"), extracted->end());
  ASSERT_EQ(offset(*s->find("H")), 10);
}
TEST_F(StmtTest, testMoveOutward) {
  size_t old_level = level(*p), old_offset = offset(*p);
  move_outward(p);
  ASSERT_LT(level(*p), old_level);
  ASSERT_GT(offset(*p), old_offset);
}
TEST_F(StmtTest, testMoveInward) {
}

}  // namespace tests
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
