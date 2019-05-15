/*!
  \file tst_tensor.cpp
  \author gennadiy (gennadiy3.14@gmail.com)
  \brief Tensor of rank 2 tests.
  \see Tensor.h
*/

#include "doctest.h"
#include "Tensor.h"
#include <sstream>

using T2i = Tensor<2, int>;
using T3i = Tensor<3, int>;

using V2i = Vector<2, int>;
using V3i = Vector<3, int>;

/*--------------------------------- compile-time tests ----------------------------------*/

namespace
{
  // init
  constexpr T2i td, Z(0), E(1, 1), t1(1, 2), t2(3, 4, 5, 6);

  // access
  static_assert((Z[0][0] == 0) && (Z[0][1] == 0) &&
                (Z[1][0] == 0) && (Z[1][1] == 0), "single init failed");

  static_assert((t1[0][0] == 1) && (t1[0][1] == 0) &&
                (t1[1][0] == 0) && (t1[1][1] == 2), "diagonal init failed");

  static_assert((t2[0][0] == 3) && (t2[0][1] == 4) &&
                (t2[1][0] == 5) && (t2[1][1] == 6), "full init failed");

  // ops
  static_assert(td == td, "t == t failed");
  static_assert(td != t1, "t != t failed");

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

  static_assert(Z + 1 == E, "Z + 1 failed");
  static_assert(E - 1 == Z, "E - 1 failed");
  static_assert(1 - E == Z, "1 - E failed");

  constexpr T2i t5(2), t6(1, 2, 2, 1), t7(1);
  static_assert(t5 - 1 == t6, "t - a failed");
  static_assert(1 - t5 == -t6, "a - t failed");
  static_assert(t6 + 1 == t5, "t + a failed");
  static_assert(1 + t6 == t5, "a + t failed");
  static_assert(t7 * 2 == t5, "t * a failed");
  static_assert(t5 / 2 == t7, "t / a failed");

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
}

/*----------------------------------- run-time tests ------------------------------------*/

TEST_CASE("init")
{
  T3i td; // default init

  // single init -- assign all elements to 1
  T3i t1 = 1;
  CHECK((t1[0][0] == 1 && t1[0][1] == 1 && t1[0][2] == 1 &&
         t1[1][0] == 1 && t1[1][1] == 1 && t1[1][2] == 1 &&
         t1[2][0] == 1 && t1[2][1] == 1 && t1[2][2] == 1));

  // diagonal init -- assign diag elements to the given ones, rest are 0
  T3i t2{1, 2, 3};
  CHECK((t2[0][0] == 1 && t2[0][1] == 0 && t2[0][2] == 0 &&
         t2[1][0] == 0 && t2[1][1] == 2 && t2[1][2] == 0 &&
         t2[2][0] == 0 && t2[2][1] == 0 && t2[2][2] == 3));

  // full init -- assign all elements to the given ones, by rows
  T3i t3 = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  CHECK((t3[0][0] == 1 && t3[0][1] == 2 && t3[0][2] == 3 &&
         t3[1][0] == 4 && t3[1][1] == 5 && t3[1][2] == 6 &&
         t3[2][0] == 7 && t3[2][1] == 8 && t3[2][2] == 9));

  T3i t1c(t1);
  CHECK((t1c[0][0] == 1 && t1c[0][1] == 1 && t1c[0][2] == 1 &&
         t1c[1][0] == 1 && t1c[1][1] == 1 && t1c[1][2] == 1 &&
         t1c[2][0] == 1 && t1c[2][1] == 1 && t1c[2][2] == 1));

  T3i t2c = t2;
  CHECK((t2c[0][0] == 1 && t2c[0][1] == 0 && t2c[0][2] == 0 &&
         t2c[1][0] == 0 && t2c[1][1] == 2 && t2c[1][2] == 0 &&
         t2c[2][0] == 0 && t2c[2][1] == 0 && t2c[2][2] == 3));

  T3i t3c{t3};
  CHECK((t3c[0][0] == 1 && t3c[0][1] == 2 && t3c[0][2] == 3 &&
         t3c[1][0] == 4 && t3c[1][1] == 5 && t3c[1][2] == 6 &&
         t3c[2][0] == 7 && t3c[2][1] == 8 && t3c[2][2] == 9));
}

TEST_CASE("io ops")
{
  std::stringstream ss;
  T2i t2(1, 2, 3, 4), t2_;
  ss << t2;
  CHECK(ss.str() == "[1, 2, 3, 4]");
  ss >> t2_;
  CHECK(t2 == t2_);

  ss.str("");
  T3i t3(1, 2, 3, 4, 5, 6, 7, 8, 9), t3r;
  ss << t3;
  CHECK(ss.str() == "[1, 2, 3, 4, 5, 6, 7, 8, 9]");
  ss >> t3r;
  CHECK(t3 == t3r);
}

TEST_CASE("boolean ops")
{
  T3i t11(1), t12 = 1, t13{1};
  CHECK(t11 == t12);
  CHECK(t12 == t13);

  T3i t21(1, 2, 3), t22{1, 2, 3};
  CHECK(t21 == t22);
  CHECK(t21 != t12);

  T3i t31(1, 2, 3, 4, 5, 6, 7, 8, 9), t32{1, 2, 3, 4, 5, 6, 7, 8, 9};
  CHECK(t31 == t32);
  CHECK(t31 != t22);
}

TEST_CASE("assign ops")
{
  T3i t(0);
  t = 1;
  CHECK((t[0][0] == 1 && t[0][1] == 0 && t[0][2] == 0 &&
         t[1][0] == 0 && t[1][1] == 1 && t[1][2] == 0 &&
         t[2][0] == 0 && t[2][1] == 0 && t[2][2] == 1));

  T3i t1 = t, t11(2, 2, 2);
  t1 += 1;
  CHECK(t1 == t11);

  T3i t2 = t1;
  t2 -= 1;
  CHECK(t2 == t);

  T3i t3(1), t4(2);
  t3 *= 2;
  CHECK(t3 == t4);

  T3i t5(1);
  t4 /= 2;
  CHECK(t4 == t5);
}

TEST_CASE("arithm ops")
{
  T3i t0(0), t1(1), t2(2);
  CHECK(t0 + t1 == t1);
  CHECK(t1 + t0 == t1);
  CHECK(t1 + t1 == t2);

  CHECK(t1 - t0 == t1);
  CHECK(t0 - t1 == -t1);

  T3i t3(1, 2, 2, 2, 1, 2, 2, 2, 1);
  CHECK(t2 - 1 == t3);
  CHECK(t3 + 1 == t2);
  CHECK(t1 * 2 == t2);
  CHECK(t2 / 2 == t1);
}

TEST_CASE("vector ops")
{
  T3i E(1, 1, 1);
  V3i v(1, 2, 3);
  CHECK(E * v == v);
  CHECK(v * E == v);

  T2i t(3, 4, 5, 6);
  V2i v2(2, 3);
  CHECK(v2 * t == V2i(21, 26));
  CHECK(t * v2 == V2i(18, 28));
}

TEST_CASE("methods")
{
  T3i E(1, 1, 1),
      t(1, 2, 3, 0, 1, 4, 5, 6, 0),
      T(1, 0, 5, 2, 1, 6, 3, 4, 0),
      R(-24, 18, 5, 20, -15, -4, -5, 4, 1);
  CHECK(E == ~E);
  CHECK(E.det() == 1);
  CHECK(E.invert() == E);
  CHECK(t.trace() == 2);
  CHECK(t.transpose() == ~t);
  CHECK(~t == T);
  CHECK(t.invert() * t == E);
  CHECK(t * t.invert() == E);
  CHECK(t.invert() == R);

  Tensor<4, int> t4(2, 3, 5, 2,
                    6, 1, 8, 3,
                    5, 4, 9, 2,
                    1, 3, 5, 6);
  Tensor<4, int> t5( 121,  28, -76, -29,
                      88,  20, -55, -21,
                    -113, -26,  71,  27,
                      30,   7, -19,  -7);
  Tensor<4, int> E4(1, 1, 1, 1);
  CHECK(t4.det() == -1);
  CHECK(t4.invert() == t5);
  CHECK(t4*t4.invert() == E4);
}
