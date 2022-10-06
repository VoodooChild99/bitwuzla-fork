#include "backtrack/vector.h"
#include "gtest/gtest.h"

namespace bzla::test {

class TestVector : public ::testing::Test
{
};

TEST_F(TestVector, ctor_dtor) { backtrack::vector<int> vec; }

TEST_F(TestVector, push_pop)
{
  backtrack::vector<int> vec;
  vec.push_back(0);
  vec.push_back(1);
  vec.push_back(2);
  vec.push();
  ASSERT_EQ(vec.size(), 3);
  vec.push_back(3);
  vec.push_back(4);
  ASSERT_EQ(vec.size(), 5);
  vec.pop();
  ASSERT_EQ(vec.size(), 3);
  ASSERT_EQ(vec[0], 0);
  ASSERT_EQ(vec[1], 1);
  ASSERT_EQ(vec[2], 2);
  ASSERT_DEATH(vec.pop(), "d_control.empty");
}

TEST_F(TestVector, mgr_multi)
{
  backtrack::BacktrackManager mgr;
  backtrack::vector<int> vec1(&mgr);
  mgr.push();
  {
    mgr.pop();
    backtrack::vector<int> vec2(&mgr);
    mgr.push();
  }
  mgr.pop();
}

TEST_F(TestVector, push_pop_mgr)
{
  backtrack::BacktrackManager mgr;
  backtrack::vector<int> vec1(&mgr);
  backtrack::vector<int> vec2(&mgr);
  vec1.push_back(0);
  vec1.push_back(1);
  vec1.push_back(2);
  mgr.push();
  ASSERT_EQ(vec1.size(), 3);
  vec1.push_back(3);
  vec1.push_back(4);
  ASSERT_EQ(vec1.size(), 5);
  mgr.pop();
  ASSERT_EQ(vec1.size(), 3);
  ASSERT_EQ(vec1[0], 0);
  ASSERT_EQ(vec1[1], 1);
  ASSERT_EQ(vec1[2], 2);
  ASSERT_DEATH(mgr.pop(), "d_scope_levels > 0");
}

TEST_F(TestVector, stress)
{
  backtrack::BacktrackManager mgr;
  backtrack::vector<size_t> vec(&mgr);

  mgr.push();
  for (size_t i = 0; i < 1000000; ++i)
  {
    vec.push_back(i);
  }
  mgr.pop();
}

}  // namespace bzla::test