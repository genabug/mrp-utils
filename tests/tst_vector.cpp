#include "math/Vector.h"

#include <gtest/gtest.h>
#include <sstream>

using namespace Math;

using V2d = Vector<2>;
using V3d = Vector<3>;
using V2i = Vector<2, int>;
using V3i = Vector<3, int>;

TEST(Vector, assign_add)
{
  V3i v(1, 2, 3);
  v += V3i(1, 1, 1);
  EXPECT_EQ(v, V3i(2, 3, 4));
}

TEST(Vector, assign_sub)
{
  V3i v(2, 3, 4);
  v -= V3i(1, 1, 1);
  EXPECT_EQ(v, V3i(1, 2, 3));
}

TEST(Vector, assign_mul)
{
  V3i v(1, 2, 3);
  v *= 2;
  EXPECT_EQ(v, V3i(2, 4, 6));
}

TEST(Vector, assign_div)
{
  V3i v(2, 4, 6);
  v /= 2;
  EXPECT_EQ(v, V3i(1, 2, 3));
}

TEST(Vector, addition)
{
  V3i v1(1, 2, 3), v2(4, 5, 6), z(0);
  EXPECT_EQ(v1 + v2, V3i(5, 7, 9));
  EXPECT_EQ(v1 + z, v1);
  EXPECT_EQ(z + v1, v1);
}

TEST(Vector, subtraction)
{
  V3i v1(1, 2, 3), v2(4, 5, 6), z(0);
  EXPECT_EQ(v2 - v1, V3i(3, 3, 3));
  EXPECT_EQ(v1 - z, v1);
  EXPECT_EQ(z - v1, -v1);
}

TEST(Vector, scalar_multiplication)
{
  V3i v(1, 2, 3);
  EXPECT_EQ(v * 2, V3i(2, 4, 6));
  EXPECT_EQ(2 * v, V3i(2, 4, 6));
}

TEST(Vector, scalar_division)
{
  V3i v(2, 4, 6);
  EXPECT_EQ(v / 2, V3i(1, 2, 3));
}

TEST(Vector, unary_plus)
{
  V3i v(1, 2, 3);
  EXPECT_EQ(+v, v);
}

TEST(Vector, unary_minus)
{
  V3i v(1, 2, 3);
  EXPECT_EQ(-v, V3i(-1, -2, -3));
}

TEST(Vector, dot_product)
{
  V3i v1(1, 2, 3), v2(4, 5, 6);
  EXPECT_EQ(v1 * v2, 32);
  EXPECT_EQ(v1 * v1, 14);
}

TEST(Vector, cross_product_base_vectors_2d)
{
  V2i ex(1, 0), ey(0, 1);
  EXPECT_EQ(ex % ey, 1);
  EXPECT_EQ(ey % ex, -1);
  EXPECT_EQ(ex % ex, 0);
}

TEST(Vector, cross_product_arbitrary_vectors_2d)
{
  V2i v1(3, 4), v2(4, -3);
  EXPECT_EQ(v1 % v2, -25);
  EXPECT_EQ(v2 % v1, 25);
  EXPECT_EQ(v1 % v1, 0);
}

TEST(Vector, cross_product_base_vectors_3d)
{
  V3i ex(1, 0, 0), ey(0, 1, 0), ez(0, 0, 1);
  EXPECT_EQ(ex % ey, ez);
  EXPECT_EQ(ey % ex, -ez);
  EXPECT_EQ(ex % ex, V3i(0, 0, 0));
}

TEST(Vector, cross_product_arbitrary_vectors_3d)
{
  V3i a(1, 2, 3), b(4, 5, 6);
  EXPECT_EQ(a % b, V3i(-3, 6, -3));
}

TEST(Vector, square_of_vector)
{
  V2i v(3, 4);
  EXPECT_EQ(sqs(v), 25);
}

TEST(Vector, magnitude)
{
  V2i v(3, 4);
  EXPECT_EQ(fabs(v), 5);
  EXPECT_EQ(fabs(-v), 5);
}

TEST(Vector, cos_sin_angle_base_vectors)
{
  V2d ex(1, 0), ey(0, 1);
  EXPECT_DOUBLE_EQ(cos(ex, ey), 0.0);
  EXPECT_DOUBLE_EQ(sin(ex, ey), 1.0);
  EXPECT_DOUBLE_EQ(cos(ex, ex), 1.0);
  EXPECT_DOUBLE_EQ(sin(ex, ex), 0.0);
}

TEST(Vector, cos_sin_angle_arbitrary_vectors)
{
  V2d ex(1, 0), ey(0, 1), v(3, 4);
  EXPECT_DOUBLE_EQ(cos(v, ex), 0.6);
  EXPECT_DOUBLE_EQ(sin(v, ex), 0.8);
  EXPECT_DOUBLE_EQ(cos(v, ey), 0.8);
  EXPECT_DOUBLE_EQ(sin(v, ey), 0.6);
}

TEST(Vector, rotation_2d)
{
  V2i v(1, 0);

  // ~ rotates 90 degrees counterclockwise
  EXPECT_EQ(~v, V2i(0, 1));
  EXPECT_EQ(~V2i(0, 1), V2i(-1, 0));
  EXPECT_EQ(~V2i(-1, 0), V2i(0, -1));
  EXPECT_EQ(~V2i(0, -1), V2i(1, 0));

  // full rotation
  EXPECT_EQ(~(~(~(~v))), v);
}

TEST(Vector, output_default_in_brackets)
{
  std::stringstream ss;
  ss << V3i(1, 2, 3);
  EXPECT_EQ(ss.str(), "(1, 2, 3)");
}

TEST(Vector, output_in_brackets)
{
  std::stringstream ss;
  ss << IO::inBrackets << V3i(3, 2, 1);
  EXPECT_EQ(ss.str(), "(3, 2, 1)");
}

TEST(Vector, output_in_brackets_signed)
{
  std::stringstream ss;
  ss << IO::inBrackets << V3i(-1, +2, -3);
  EXPECT_EQ(ss.str(), "(-1, 2, -3)");
}

TEST(Vector, output_bare_comps)
{
  std::stringstream ss;
  ss << IO::bareComps << V3i(1, 2, 3);
  EXPECT_EQ(ss.str(), "1 2 3");
}

TEST(Vector, input_in_brackets)
{
  std::stringstream ss("(1, 2, 3)");
  V3i v;
  ss >> v;
  EXPECT_EQ(v, V3i(1, 2, 3));
}

TEST(Vector, input_in_brackets_signed)
{
  std::stringstream ss("(-1, 2, -3)");
  V3i v;
  ss >> v;
  EXPECT_EQ(v, V3i(-1, 2, -3));
}

TEST(Vector, input_in_brackets_missing_closing_bracket)
{
  V3i v;
  std::stringstream ss("(2, 3, 4");
  EXPECT_TRUE(!(ss >> v));
}

TEST(Vector, input_in_brackets_extra_comma)
{
  V3i v;
  std::stringstream ss("(2, 3, 4,)");
  EXPECT_TRUE(!(ss >> v));
}

TEST(Vector, input_in_brackets_missing_values)
{
  V3i v;
  std::stringstream ss("(3, 4)");
  EXPECT_TRUE(!(ss >> v));
}

TEST(Vector, input_in_brackets_extra_values)
{
  V3i v;
  std::stringstream ss("(4, 5, 6, 7)");
  EXPECT_TRUE(!(ss >> v));
}

TEST(Vector, input_bare_comps)
{
  std::stringstream ss("  1 2 3 ");
  V3i v;
  ss >> v;
  EXPECT_EQ(v, V3i(1, 2, 3));
}

TEST(Vector, input_bare_comps_signed)
{
  std::stringstream ss("-1 2 -3");
  V3i v;
  ss >> v;
  EXPECT_EQ(v, V3i(-1, 2, -3));
}

TEST(Vector, input_bare_comps_missing_values)
{
  V3i v;
  std::stringstream ss("3 4");
  EXPECT_TRUE(!(ss >> v));
}

TEST(Vector, roundtrip_in_brackets)
{
  V3i v1(3, -2, 1), v2;
  std::stringstream ss;
  ss << IO::inBrackets << v1;
  ss >> v2;
  EXPECT_EQ(v1, v2);
}

TEST(Vector, roundtrip_bare_comps)
{
  V3i v1(7, 8, 9), v2;
  std::stringstream ss;
  ss << IO::bareComps << v1;
  ss >> v2;
  EXPECT_EQ(v1, v2);
}
