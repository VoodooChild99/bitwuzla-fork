#include "test.h"

namespace bzlals {
namespace test {

class TestBvOpInv : public TestBvOp
{
};

TEST_F(TestBvOpInv, add)
{
  test_binary<BitVectorAdd>(INV, ADD, 0);
  test_binary<BitVectorAdd>(INV, ADD, 1);
}

TEST_F(TestBvOpInv, and)
{
  test_binary<BitVectorAnd>(INV, AND, 0);
  test_binary<BitVectorAnd>(INV, AND, 1);
}

TEST_F(TestBvOpInv, concat)
{
  test_binary<BitVectorConcat>(INV, CONCAT, 0);
  test_binary<BitVectorConcat>(INV, CONCAT, 1);
}

TEST_F(TestBvOpInv, eq)
{
  test_binary<BitVectorEq>(INV, EQ, 0);
  test_binary<BitVectorEq>(INV, EQ, 1);
}

TEST_F(TestBvOpInv, mul)
{
  test_binary<BitVectorMul>(INV, MUL, 0);
  test_binary<BitVectorMul>(INV, MUL, 1);
}

}  // namespace test
}  // namespace bzlals