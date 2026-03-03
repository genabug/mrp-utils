#include "utils/Vector.h"

#include <gtest/gtest.h>
#include <sstream>

using V2d = Vector<2>;
using V3d = Vector<3>;
using V2i = Vector<2, int>;
using V3i = Vector<3, int>;

TEST(Vector, access)
{
  //int& i = V2i(3, 4)[1]; // compile error: cannot bind lvalue to rvalue
  const int &a = V2i(1, 2)[0];
  EXPECT_EQ(a, 1);
}

TEST(Vector, assign_ops)
{
  // TODO
}

TEST(Vector, io_with_brackets_default_ok)
{
  std::stringstream ss;
  V3i v1(1, 2, 3), v2;

  ss << v1;
  EXPECT_EQ(ss.str(), "(1, 2, 3)");
  ss >> v2;
  EXPECT_EQ(v1, v2);
}

TEST(Vector, io_with_brackets_explicit_mode_ok)
{
  std::stringstream ss;
  V3i v1(3, 2, 1), v2;

  ss << VectorManip::inBrackets << v1;
  EXPECT_EQ(ss.str(), "(3, 2, 1)");
  ss >> v2;
  EXPECT_EQ(v2, v1);
}

TEST(Vector, io_with_brackets_with_non_digit_chars_ok)
{
  std::stringstream ss;
  V3i v1(1, 2, 3), v2;

  ss << VectorManip::inBrackets << ", " << v1 << "   ";
  EXPECT_EQ(ss.str(), ", (1, 2, 3)   ");
  ss >> v2;
  EXPECT_EQ(v1, v2);
}

TEST(Vector, io_with_brackets_with_digit_chars_in_front_error) // should be but currently OK
{
  std::stringstream ss;
  V3i v1(2, 3, 4), v2;

  ss << VectorManip::inBrackets << " 1 (,! " << v1;
  EXPECT_EQ(ss.str(), " 1 (,! (2, 3, 4)");
  ss >> v2;
  EXPECT_NE(v2, v1);
  EXPECT_EQ(v2, V3i(1, 0, 0));
  // '1' is interpreted as beginning of a new vector in bareComponents mode
  // (even though we explicitly set IO mode with brackets)
  // and then the algorithm stucks on , (comma) and the rest components become 0.
}

TEST(Vector, io_with_brackets_no_closing_bracket_error) // should be but currently OK
{
  std::stringstream ss;
  V3i v1(2, 3, 4), v2;

  ss << VectorManip::inBrackets << v1;
  EXPECT_EQ(ss.str(), "(2, 3, 4)");
  ss.str("(2, 3, 4");
  ss >> v2;
  EXPECT_EQ(v1, v2);
}

TEST(Vector, io_with_brackets_two_consequent_vectors_ok)
{
  std::stringstream ss;
  V2i v1(1, 2), v1r;
  V3i v2(3, 4, 5), v2r;

  ss << VectorManip::inBrackets << v1 << v2;
  EXPECT_EQ(ss.str(), "(1, 2)(3, 4, 5)");
  ss >> v1r >> v2r;
  EXPECT_EQ(v1, v1r);
  EXPECT_EQ(v2, v2r);
}

TEST(Vector, io_with_bare_components_ok)
{
  std::stringstream ss;
  V3i v1(1, 2, 3), v2;

  ss << VectorManip::bareComponents << v1;
  EXPECT_EQ(ss.str(), "1 2 3");
  ss >> v2;
  EXPECT_EQ(v1, v2);
}

TEST(Vector, io_with_bare_components_with_non_digit_chars_in_front_ok)
{
  std::stringstream ss;
  V3i v1(1, 2, 3), v2;

  ss << VectorManip::bareComponents << ", " << v1 << " 4  ";
  EXPECT_EQ(ss.str(), ", 1 2 3 4  ");
  ss >> v2;
  EXPECT_EQ(v1, v2);
}

TEST(Vector, io_with_bare_components_with_digit_chars_in_front_ok)
{
  std::stringstream ss;
  V3i v1(2, 3, 4), v2;

  ss << VectorManip::bareComponents << " ,1  " << v1 << " 5  ";
  EXPECT_EQ(ss.str(), " ,1  2 3 4 5  ");
  ss >> v2;
  EXPECT_NE(v1, v2);
  EXPECT_EQ(v2, V3i(1, 2, 3));
}

TEST(Vector, io_with_bare_components_two_vectors_nospace_error)
{
  std::stringstream ss;
  V2i v1(1, 2), v1r;
  V3i v2(3, 4, 5), v2r;

  ss << VectorManip::bareComponents << v1 << v2;
  EXPECT_EQ(ss.str(), "1 23 4 5");
  ss >> v1r >> v2r;
  EXPECT_NE(v1, v1r);
  EXPECT_NE(v2, v2r);
  EXPECT_EQ(v1r, V2i(1, 23));
  EXPECT_EQ(v2r, V3i(4, 5, 0));
  // TODO: fix it, exception must be thrown for v2r
}

TEST(Vector, io_with_bare_components_two_vectors_non_digit_delim_ok)
{
  std::stringstream ss;
  V2i v1(1, 2), v1r;
  V3i v2(3, 4, 5), v2r;

  ss << VectorManip::bareComponents << v1 << " , [ d  " << v2;
  EXPECT_EQ(ss.str(), "1 2 , [ d  3 4 5");
  ss >> v1r >> v2r;
  EXPECT_EQ(v1, v1r);
  EXPECT_EQ(v2, v2r);
}

TEST(Vector, io_with_brackets_signed_components_ok)
{
  std::stringstream ss;
  V3i v1(-1, +2, -3), v2;

  ss << VectorManip::inBrackets << v1;
  EXPECT_EQ(ss.str(), "(-1, 2, -3)");
  ss >> v2;
  EXPECT_EQ(v1, v2);
}
