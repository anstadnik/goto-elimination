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
K:    cout << "WORLD" << endl;
    }
G:  if (B) { goto !a%5==0; }
H:  if (E) { goto a<500; }
_H: if (B) { goto true; }
*/

TEST_F(StmtTest, testLoad) {
  ASSERT_FALSE(s->empty());
  ASSERT_FALSE(s->par_expr);
}
TEST_F(StmtTest, testOffset) {
  ASSERT_LT(offset(*a), offset(*b));
  ASSERT_EQ(offset(*h), 11);
}
TEST_F(StmtTest, testLevel) {
  ASSERT_EQ(level(*a),level(*b));
  ASSERT_EQ(level(*j),2);
}
TEST_F(StmtTest, testSiblings) {
  ASSERT_TRUE(siblings(a, b));
  ASSERT_FALSE(siblings(a, j));
  ASSERT_TRUE(siblings(q, k));
}
TEST_F(StmtTest, testDirectlyRelated) {
  ASSERT_TRUE(directly_related(j, a));
}
TEST_F(StmtTest, testIndirectlyRelated) {}

}  // namespace tests
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
