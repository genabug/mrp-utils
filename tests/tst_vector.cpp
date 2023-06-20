#include <gtest/gtest.h>
#include "utils/Vector.h"
#include <limits>
#include <sstream>

using V2d = Vector<2>;
using V3d = Vector<3>;
using V2i = Vector<2, int>;
using V3i = Vector<3, int>;

// init
constexpr V3i v, v1(1), v2(2), v3(v1 + v2), v4(4, 5, 6), v5(3, -6, 3);
constexpr V2i x(4, -3), y(3, 4);

// access
static_assert((v1[0] == 1) && (v1[1] == 1) && (v1[2] == 1), "single init failed");
static_assert((v2[0] == 2) && (v2[1] == 2) && (v2[2] == 2), "single init failed");
static_assert((v3[0] == 3) && (v3[1] == 3) && (v3[2] == 3), "copy init failed");
static_assert((v4[0] == 4) && (v4[1] == 5) && (v4[2] == 6), "full init failed");

// conversion
constexpr V2d xd = V2d(x);
static_assert(Utils::fp_equal(xd[0], 4.) && Utils::fp_equal(xd[1], -3.),
              "explicit conversion i->d failed");

constexpr V2i xi = V2i(xd);
static_assert((xi[0] == 4) && (xi[1] == -3), "explicit conversion d->i failed");

// ops
static_assert(v == v, "v == v failed");
static_assert(v != v1, "v != v failed");
static_assert(v1*2 == v2, "v * a failed");
static_assert(3*v1 == v3, "a * v failed");
static_assert(v2/2 == v1, "v / a failed");
static_assert(v1 + v1 == v2, "v + v failed");
static_assert(v1 + v2 == v3, "v + v failed");
static_assert(v3 - v1 == +v2, "v - v failed");
static_assert(v1 - v3 == -v2, "v - v failed");
static_assert(v1 * v2 == 6, "v * v failed");
static_assert(v2 * v4 == 30, "v * v failed");
static_assert(x % y == 25, "v % v failed in 2D");
static_assert(v3 % v4 == v5, "v % v failed in 3D");
static_assert(~x == y, "~v failed in 2D");

// op properties
constexpr V3i z(0), a(1, 2, 3), b(4, 5, 6), c(7, 8, 9);
static_assert(a + z == a, "v + 0 failed");
static_assert(a + b == b + a, "v + v commutative failed");
static_assert((a + b) + c == a + (b + c), "v + v associative failed");

static_assert(a * b == b * a, "v * v commutative failed");
static_assert((2*a) * b == 2*(a * b), "v * v distributive failed");
static_assert((a + b) * c == a * c + b * c, "v * v associative failed");

static_assert(x % x == 0, "v % v failed");
static_assert(a % a == z, "v % v failed");
static_assert(a % b == -(b % a), "v % v anticommutative failed");
static_assert(x % y == -(y % x), "v % v anticommutative failed");
static_assert(a % (b + c) == a % b + a % c, "v % v distributive failed");
static_assert((2*a) % b == 2*(a % b), "v % v associative failed");
static_assert((a % b) * c ==  a * (b % c), "Jacobi's identity failed");
static_assert(a % (b % c) + b % (c % a) + c % (a % b) == z, "triple product failed");
static_assert(a % (b % c) == b * (a * c) - c * (a * b), "Lagrange's identity failed");

// methods
static_assert(sqs(-y) == 25, "sqs failed");
static_assert(sqs(y) == sqs(-y), "sqs failed");
static_assert(fabs(-y) == 5, "abs failed");
static_assert(fabs(x) == fabs(-x), "abs failed");

constexpr V2d vd(3, 4);
static_assert(Utils::fp_equal(sqs(vd), 25.), "sqs failed");
static_assert(Utils::fp_equal(fabs(vd), 5.), "abs failed");

constexpr V2d ex(1, 0), ey(0, 1);
static_assert(Utils::fp_equal(cos(ex, ex), 1.), "cos failed");
static_assert(Utils::fp_equal(cos(ex, ey), 0.), "cos failed");
static_assert(Utils::fp_equal(sin(ey, ey), 0.), "sin failed");
static_assert(Utils::fp_equal(sin(ey, ex), 1.), "sin failed");

static_assert(Utils::fp_equal(cos(vd, ey), .8), "cos failed");
static_assert(Utils::fp_equal(sin(vd, ey), .6), "sin failed");
static_assert(Utils::fp_equal(cos(vd, ex), .6), "cos failed");
static_assert(Utils::fp_equal(sin(vd, ex), .8), "sin failed");

constexpr Vector<2, float> vf(3, 4);
static_assert(Utils::fp_equal(sqs(vf), 25.f), "sqs failed");
static_assert(Utils::fp_equal(fabs(vf), 5.f), "abs failed");

constexpr Vector<2, long> vl(3, 4);
static_assert(sqs(vl) == 25, "sqs failed");
static_assert(fabs(vl) == 5, "abs failed");

union VectorAsUnionMemberTest // must compile
{
  int i; // pod type
  V3i v; // type with non-trivial ctor
  VectorAsUnionMemberTest() {}
};

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
  //ss << Vectors::inBrackets; // this is the default mode
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

  ss << Vectors::inBrackets << v1;
  EXPECT_EQ(ss.str(), "(3, 2, 1)");
  ss >> v2;
  EXPECT_EQ(v2, v1);
}

TEST(Vector, io_with_brackets_with_non_digit_chars_ok)
{
  std::stringstream ss;
  V3i v1(1, 2, 3), v2;

  ss << Vectors::inBrackets << ", " << v1 << "   ";
  EXPECT_EQ(ss.str(), ", (1, 2, 3)   ");
  ss >> v2;
  EXPECT_EQ(v1, v2);
}

TEST(Vector, io_with_brackets_with_digit_chars_in_front_error) // should be but currently OK
{
  std::stringstream ss;
  V3i v1(2, 3, 4), v2;

  ss << Vectors::inBrackets << " 1 (,! " << v1;
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

  ss << Vectors::inBrackets << v1;
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

  ss << Vectors::inBrackets << v1 << v2;
  EXPECT_EQ(ss.str(), "(1, 2)(3, 4, 5)");
  ss >> v1r >> v2r;
  EXPECT_EQ(v1, v1r);
  EXPECT_EQ(v2, v2r);
}

TEST(Vector, io_with_bare_components_ok)
{
  std::stringstream ss;
  V3i v1(1, 2, 3), v2;

  ss << Vectors::bareComponents << v1;
  EXPECT_EQ(ss.str(), "1 2 3");
  ss >> v2;
  EXPECT_EQ(v1, v2);
}

TEST(Vector, io_with_bare_components_with_non_digit_chars_in_front_ok)
{
  std::stringstream ss;
  V3i v1(1, 2, 3), v2;

  ss << Vectors::bareComponents << ", " << v1 << " 4  ";
  EXPECT_EQ(ss.str(), ", 1 2 3 4  ");
  ss >> v2;
  EXPECT_EQ(v1, v2);
}

TEST(Vector, io_with_bare_components_with_digit_chars_in_front_ok)
{
  std::stringstream ss;
  V3i v1(2, 3, 4), v2;

  ss << Vectors::bareComponents << " ,1  " << v1 << " 5  ";
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

  ss << Vectors::bareComponents << v1 << v2;
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

  ss << Vectors::bareComponents << v1 << " , [ d  " << v2;
  EXPECT_EQ(ss.str(), "1 2 , [ d  3 4 5");
  ss >> v1r >> v2r;
  EXPECT_EQ(v1, v1r);
  EXPECT_EQ(v2, v2r);
}
