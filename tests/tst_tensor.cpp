#include "utils/Tensor.h"
#include <gtest/gtest.h>
#include <sstream>

using T2d = Tensor<2>;
using T3d = Tensor<3>;
using T2i = Tensor<2, int>;
using T3i = Tensor<3, int>;

using V2i = Vector<2, int>;
using V3i = Vector<3, int>;


// init
constexpr T2i Z, E(1), t1(1, 2), t2(3, 4, 5, 6);

// access
static_assert(
  (Z[0][0] == 0) && (Z[0][1] == 0) && (Z[1][0] == 0) && (Z[1][1] == 0),
  "default init failed");

static_assert(
  (E[0][0] == 1) && (E[0][1] == 0) && (E[1][0] == 0) && (E[1][1] == 1),
  "single init failed");

static_assert(
  (t1[0][0] == 1) && (t1[0][1] == 0) && (t1[1][0] == 0) && (t1[1][1] == 2),
  "diagonal init failed");

static_assert(
  (t2[0][0] == 3) && (t2[0][1] == 4) && (t2[1][0] == 5) && (t2[1][1] == 6),
  "full init failed");

// conversion
constexpr T2d d(1., 2., 3., 4.);
constexpr T2i m = T2i(d);
static_assert(
  (m[0][0] == 1) && (m[0][1] == 2) && (m[1][0] == 3) && (m[1][1] == 4),
  "conversion d -> i failed");

// ops
static_assert(Z == Z, "t == t failed");
static_assert(Z != E, "t != t failed");

constexpr T2i t1_plus_t1(2, 4), t1_plus_t2(4, 4, 5, 8);
static_assert(Z + Z == Z, "Z + Z failed");
static_assert(Z + t1 == t1, "Z + t failed");
static_assert(t1 + Z == t1, "t + Z failed");
static_assert(t1 + t1 == t1_plus_t1, "t + t failed");
static_assert(t1 + t2 == t1_plus_t2, "t1 + t2 failed");

constexpr T2i t2_minus_t1(2, 4, 5, 4);
static_assert(t2 - t2 == Z, "t - t failed");
static_assert(t2 - Z == t2, "t - Z failed");
static_assert(Z - t2 == -t2, "Z - t failed");
static_assert(t2 - t1 == t2_minus_t1, "t2 - t1 failed");

static_assert(Z * t2 == Z, "Z * t failed");
static_assert(t2 * Z == Z, "t * Z failed");

static_assert(E * t2 == t2, "E * t failed");
static_assert(t2 * E == t2, "t * E failed");

constexpr T2i t3(3, 4, 10, 12), t4(3, 8, 5, 12);
static_assert(t1 * t2 == t3, "t * t failed");
static_assert(t2 * t1 == t4, "t * t failed");

constexpr T2i t5(2), t6(1);
static_assert(t6 * 2 == t5, "t * a failed");
static_assert(t5 / 2 == t6, "t / a failed");

// vector ops
constexpr V2i z(0), v(2, 3);
static_assert(t2 * z == z, "t * z failed");
static_assert(z * t2 == z, "z * t failed");
static_assert(E * v == v, "E * v failed");
static_assert(v * E == v, "v * E failed");

constexpr V2i v_mult_t(21, 26), t_mult_v(18, 28);
static_assert(v * t2 == v_mult_t, "v * t failed");
static_assert(t2 * v == t_mult_v, "t * v failed");

constexpr T2i v_diad_v(4, 6, 6, 9);
static_assert((v ^ v) == v_diad_v, "v ^ v failed");

// 2D vector ops
constexpr V2i t2_cross_v(1, 3);
static_assert(Z % v == z, "Z % v failed in 2D");
static_assert(t2 % z == z, "T % z failed in 2D");
static_assert(t2 % v == t2_cross_v, "t % v failed in 2D");
static_assert(v % t3 == ~t3 % (-v), "v % t failed in 2D");

// 3D vector ops
constexpr T3i ZZ(0), T(1, 2, 3, 4, 5, 6, 7, 8, 9);
constexpr V3i v1(1, 2, 3), v2(4, 5, 6), zz(0);
constexpr T3i T_cross_v1(0, 0, 0, 3, -6, 3, 6, -12, 6);
static_assert(~v1 * v2 == v1 % v2, "~v failed in 3D");
static_assert(ZZ % v1 == ZZ, "Z % v failed in 3D");
static_assert(T % zz == ZZ, "t % z failed in 3D");
static_assert(T % v1 == T_cross_v1, "t % v failed in 3D");
static_assert(v1 % T == ~(~T % (-v1)), "v % t failed in 3D");

// other methods
static_assert(E.det() == 1, "|E| failed");
static_assert(Z.det() == 0, "|Z| failed ");
static_assert(t1.det() == 2, "|t| failed");
static_assert(t1.trace() == 3, "tr(t) failed");
static_assert(E.invert() == E, "E^-1 failed");
static_assert(E == ~E, "E^T failed");

constexpr T2i t(2, 1, 3, 2), tT(2, 3, 1, 2), t_(2, -1, -3, 2);
static_assert(~t == tT, "t^T failed");
static_assert(t.invert() == t_, "t^-1 failed");
static_assert(t.invert() * t == E, "t^-1 * t failed");
static_assert(t * t.invert() == E, "t * t^-1 failed");


TEST(Tensor, init)
{
  T3i td; // default zero init
  EXPECT_TRUE(td[0][0] == 0 && td[0][1] == 0 && td[0][2] == 0 &&
              td[1][0] == 0 && td[1][1] == 0 && td[1][2] == 0 &&
              td[2][0] == 0 && td[2][1] == 0 && td[2][2] == 0);

  // single init -- assign diagonal elements to 1, rest are zeroes
  T3i t1(1);
  EXPECT_TRUE(t1[0][0] == 1 && t1[0][1] == 0 && t1[0][2] == 0 &&
              t1[1][0] == 0 && t1[1][1] == 1 && t1[1][2] == 0 &&
              t1[2][0] == 0 && t1[2][1] == 0 && t1[2][2] == 1);

  // diagonal init -- assign diag elements to the given ones, rest are 0
  T3i t2{1, 2, 3};
  EXPECT_TRUE(t2[0][0] == 1 && t2[0][1] == 0 && t2[0][2] == 0 &&
              t2[1][0] == 0 && t2[1][1] == 2 && t2[1][2] == 0 &&
              t2[2][0] == 0 && t2[2][1] == 0 && t2[2][2] == 3);

  // full init -- assign all elements to the given ones, by rows
  T3i t3{1, 2, 3, 4, 5, 6, 7, 8, 9};
  EXPECT_TRUE(t3[0][0] == 1 && t3[0][1] == 2 && t3[0][2] == 3 &&
              t3[1][0] == 4 && t3[1][1] == 5 && t3[1][2] == 6 &&
              t3[2][0] == 7 && t3[2][1] == 8 && t3[2][2] == 9);

  // copying
  T3i t1c(t1);
  EXPECT_TRUE(t1c[0][0] == 1 && t1c[0][1] == 0 && t1c[0][2] == 0 &&
              t1c[1][0] == 0 && t1c[1][1] == 1 && t1c[1][2] == 0 &&
              t1c[2][0] == 0 && t1c[2][1] == 0 && t1c[2][2] == 1);

  T3i t2c = t2;
  EXPECT_TRUE(t2c[0][0] == 1 && t2c[0][1] == 0 && t2c[0][2] == 0 &&
              t2c[1][0] == 0 && t2c[1][1] == 2 && t2c[1][2] == 0 &&
              t2c[2][0] == 0 && t2c[2][1] == 0 && t2c[2][2] == 3);

  T3i t3c{t3};
  EXPECT_TRUE(t3c[0][0] == 1 && t3c[0][1] == 2 && t3c[0][2] == 3 &&
              t3c[1][0] == 4 && t3c[1][1] == 5 && t3c[1][2] == 6 &&
              t3c[2][0] == 7 && t3c[2][1] == 8 && t3c[2][2] == 9);

  // conversion
  T3d tdd = T3d(t3);
  T3i ti = T3i(tdd);
  EXPECT_TRUE(ti[0][0] == 1 && ti[0][1] == 2 && ti[0][2] == 3 &&
              ti[1][0] == 4 && ti[1][1] == 5 && ti[1][2] == 6 &&
              ti[2][0] == 7 && ti[2][1] == 8 && ti[2][2] == 9);
}

auto getT2i() { return T2i(1, 2, 3, 4); }

TEST(Tensor, access)
{
  //auto row0 = T2i(1, 2, 3, 4)[0]; // COMPILE ERROR: use of deleted function
  //const auto row1 = T2i(4, 3, 2, 1)[1]; // same
}

TEST(Tensor, io_with_brackets_default_ok)
{
  std::stringstream ss;
  T2i t1(1, 2, 3, 4), t2;

  ss << t1;
  EXPECT_EQ(ss.str(), "[1, 2, 3, 4]");
  ss >> t2;
  EXPECT_EQ(t1, t2);
}

TEST(Tensor, io_with_brackets_explicit_mode_ok)
{
  std::stringstream ss;
  T2i t1(1, 2, 3, 4), t2;

  ss << Tensors::inBrackets << t1;
  EXPECT_EQ(ss.str(), "[1, 2, 3, 4]");
  ss >> t2;
  EXPECT_EQ(t1, t2);
}

TEST(Tensor, io_with_brackets_non_digit_chars_in_front_ok)
{
  std::stringstream ss;
  T2i t1(1, 2, 3, 4), t2;

  ss << Tensors::inBrackets << " ,!([ " << t1;
  EXPECT_EQ(ss.str(), " ,!([ [1, 2, 3, 4]");
  ss >> t2;
  EXPECT_EQ(t1, t2);
}

TEST(Tensor, io_with_brackets_digit_chars_in_front_error) // but currently OK
{
  std::stringstream ss;
  T2i t1(1, 2, 3, 4), t2;

  ss << Tensors::inBrackets << " 1,!([ " << t1;
  EXPECT_EQ(ss.str(), " 1,!([ [1, 2, 3, 4]");
  ss >> t2;
  EXPECT_NE(t1, t2);
  EXPECT_EQ(t2, T2i(1, 0, 0, 0));
  // TODO: the same shit as with Vector
}

TEST(Tensor, io_with_brackets_two_tensors_no_delimeter_ok)
{
  std::stringstream ss;
  T2i t1(1, 2, 3, 4), t2(5, 6, 7, 8);

  ss << Tensors::inBrackets << t1 << t2;
  EXPECT_EQ(ss.str(), "[1, 2, 3, 4][5, 6, 7, 8]");

  T2i t1r, t2r;
  ss >> t1r >> t2r;
  EXPECT_EQ(t1, t1r);
  EXPECT_EQ(t2, t2r);
}

TEST(Tensor, io_with_brackets_two_tensors_non_digit_delim_ok)
{
  std::stringstream ss;
  T2i t1(1, 2, 3, 4), t2(5, 6, 7, 8);

  ss << Tensors::inBrackets << t1 << "  ,;!ss " << t2;
  EXPECT_EQ(ss.str(), "[1, 2, 3, 4]  ,;!ss [5, 6, 7, 8]");

  T2i t1r, t2r;
  ss >> t1r >> t2r;
  EXPECT_EQ(t1, t1r);
  EXPECT_EQ(t2, t2r);
}

TEST(Tensor, io_with_bare_comps_explicit_mode_ok)
{
  std::stringstream ss;
  T2i t1(1, 2, 3, 4), t2;

  ss << Tensors::bareComponents << t1;
  EXPECT_EQ(ss.str(), "1 2 3 4");
  ss >> t2;
  EXPECT_EQ(t1, t2);
}

TEST(Tensor, io_with_bare_comps_non_digit_chars_in_front_ok)
{
  std::stringstream ss;
  T2i t1(1, 2, 3, 4), t2;

  ss << Tensors::bareComponents << " ,!([ " << t1;
  EXPECT_EQ(ss.str(), " ,!([ 1 2 3 4");
  ss >> t2;
  EXPECT_EQ(t1, t2);
}

TEST(Tensor, io_with_bare_comps_digit_chars_in_front_error) // but currently OK
{
  std::stringstream ss;
  T2i t1(1, 2, 3, 4), t2;

  ss << Tensors::bareComponents << " 1,!([ " << t1;
  EXPECT_EQ(ss.str(), " 1,!([ 1 2 3 4");
  ss >> t2;
  EXPECT_NE(t1, t2);
  EXPECT_EQ(t2, T2i(1, 0, 0, 0));
  // TODO: the same shit as with Vector
}

TEST(Tensor, io_with_bare_comps_two_tensors_no_delimeter_error) // but currently OK
{
  std::stringstream ss;
  T2i t1(1, 2, 3, 4), t2(5, 6, 7, 8);

  ss << Tensors::bareComponents << t1 << t2;
  EXPECT_EQ(ss.str(), "1 2 3 45 6 7 8");

  T2i t1r, t2r;
  ss >> t1r >> t2r;
  EXPECT_NE(t1, t1r);
  EXPECT_NE(t2, t2r);
  EXPECT_EQ(t1r, T2i(1, 2, 3, 45));
  EXPECT_EQ(t2r, T2i(6, 7, 8, 0));
  // TODO: fix it, t2r must throw an exception
}

TEST(Tensor, io_with_bare_comps_two_tensors_non_digit_delim_ok)
{
  std::stringstream ss;
  T2i t1(1, 2, 3, 4), t2(5, 6, 7, 8);

  ss << Tensors::bareComponents << t1 << "  ,;!ss " << t2;
  EXPECT_EQ(ss.str(), "1 2 3 4  ,;!ss 5 6 7 8");

  T2i t1r, t2r;
  ss >> t1r >> t2r;
  EXPECT_EQ(t1, t1r);
  EXPECT_EQ(t2, t2r);
}

TEST(Tensor, boolean_ops)
{
  T3i t11(1), t12{1}, t13{1};
  EXPECT_EQ(t11, t12);
  EXPECT_EQ(t12, t13);

  T3i t21(1, 2, 3), t22{1, 2, 3};
  EXPECT_EQ(t21, t22);
  EXPECT_NE(t21, t12);

  T3i t31(1, 2, 3, 4, 5, 6, 7, 8, 9), t32{1, 2, 3, 4, 5, 6, 7, 8, 9};
  EXPECT_EQ(t31, t32);
  EXPECT_NE(t31, t22);
}

TEST(Tensor, arithmetic_ops)
{
  T3i t0(0), t1(1), t2(2);
  EXPECT_EQ(t0 + t1, t1);
  EXPECT_EQ(t1 + t0, t1);
  EXPECT_EQ(t1 + t1, t2);

  EXPECT_EQ(t1 - t0, t1);
  EXPECT_EQ(t0 - t1, -t1);

  EXPECT_EQ(2 * t1, t2);
  EXPECT_EQ(t2 / 2, t1);
}

TEST(Tensor, vector_ops)
{
  T3i E(1, 1, 1);
  V3i v(1, 2, 3);
  EXPECT_EQ(E * v, v);
  EXPECT_EQ(v * E, v);

  T2i t(3, 4, 5, 6);
  V2i v2(2, 3);
  EXPECT_EQ(v2 * t, V2i(21, 26));
  EXPECT_EQ(t * v2, V2i(18, 28));
}

TEST(Tensor, assign_ops)
{
  T3i t3(1), t4(2);
  t3 += t3;
  EXPECT_EQ(t3, t4);
  t3 /= 2;
  EXPECT_EQ(t3, T3i(1));
  t3 *= 2;
  EXPECT_EQ(t3, t4);
  t3 -= t4;
  EXPECT_EQ(t3, T3i(0));
}

TEST(Tensor, methods)
{
  T3i E(1, 1, 1),
      t(1, 2, 3, 0, 1, 4, 5, 6, 0),
      T(1, 0, 5, 2, 1, 6, 3, 4, 0),
      R(-24, 18, 5, 20, -15, -4, -5, 4, 1);
  EXPECT_EQ(E, ~E);
  EXPECT_EQ(E.det(), 1);
  EXPECT_EQ(E.invert(), E);
  EXPECT_EQ(t.trace(), 2);
  EXPECT_EQ(t.transpose(), ~t);
  EXPECT_EQ(~t, T);
  EXPECT_EQ(t.invert() * t, E);
  EXPECT_EQ(t * t.invert(), E);
  EXPECT_EQ(t.invert(), R);

  using T4i = Tensor<4, int>;

  T4i t4(2, 3, 5, 2,
         6, 1, 8, 3,
         5, 4, 9, 2,
         1, 3, 5, 6);
  EXPECT_EQ(t4.det(), -1);

  T4i E4(1, 1, 1, 1);
  T4i t4_inverted(
      121,  28, -76, -29,
       88,  20, -55, -21,
     -113, -26,  71,  27,
       30,   7, -19,  -7);
  ASSERT_EQ(t4 * t4_inverted, E4);

  EXPECT_EQ(t4.invert(), t4_inverted);
}
