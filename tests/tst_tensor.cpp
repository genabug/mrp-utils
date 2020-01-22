/*!
  \file tst_tensor.cpp
  \author gennadiy (gennadiy3.14@gmail.com)
  \brief Tensor of rank 2 tests.
  \see Tensor.h
*/

#include "doctest.h"
#include "utils/Tensor.h"
#include <sstream>

using T2d = Tensor<2>;
using T3d = Tensor<3>;
using T2i = Tensor<2, int>;
using T3i = Tensor<3, int>;

using V2i = Vector<2, int>;
using V3i = Vector<3, int>;

TEST_CASE("init")
{
  T3i td; // default zero init

  // single init -- assign all elements to 1
  T3i t1(1);
  CHECK((t1[0][0] == 1 && t1[0][1] == 1 && t1[0][2] == 1 &&
         t1[1][0] == 1 && t1[1][1] == 1 && t1[1][2] == 1 &&
         t1[2][0] == 1 && t1[2][1] == 1 && t1[2][2] == 1));

  // diagonal init -- assign diag elements to the given ones, rest are 0
  T3i t2{1, 2, 3};
  CHECK((t2[0][0] == 1 && t2[0][1] == 0 && t2[0][2] == 0 &&
         t2[1][0] == 0 && t2[1][1] == 2 && t2[1][2] == 0 &&
         t2[2][0] == 0 && t2[2][1] == 0 && t2[2][2] == 3));

  // full init -- assign all elements to the given ones, by rows
  T3i t3{1, 2, 3, 4, 5, 6, 7, 8, 9};
  CHECK((t3[0][0] == 1 && t3[0][1] == 2 && t3[0][2] == 3 &&
         t3[1][0] == 4 && t3[1][1] == 5 && t3[1][2] == 6 &&
         t3[2][0] == 7 && t3[2][1] == 8 && t3[2][2] == 9));

  // copying
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

  // conversion
  T3d tdd = T3d(t3);
  T3i ti = T3i(tdd);
  CHECK((ti[0][0] == 1 && ti[0][1] == 2 && ti[0][2] == 3 &&
         ti[1][0] == 4 && ti[1][1] == 5 && ti[1][2] == 6 &&
         ti[2][0] == 7 && ti[2][1] == 8 && ti[2][2] == 9));
}

TEST_CASE("io ops")
{
  std::stringstream ss;
  T2i t2(1, 2, 3, 4), t2r;
  ss << t2;
  CHECK(ss.str() == "[1, 2, 3, 4]");
  ss >> t2r;
  CHECK(t2 == t2r);

  ss.str("");
  ss << Tensors::bareComponents << t2;
  CHECK(ss.str() == "1 2 3 4");
  t2r = T2i(0);
  ss >> t2r;
  CHECK(t2r == t2);

  ss.str("");
  ss.clear(); // clear eof bit
  T3i t3(1, 2, 3, 4, 5, 6, 7, 8, 9), t3r;
  ss << t3;
  CHECK(ss.str() == "1 2 3 4 5 6 7 8 9");
  ss >> t3r;
  CHECK(t3 == t3r);

  ss.str("");
  ss.clear(); // clear eof bit
  ss << Tensors::inBrackets << t3;
  CHECK(ss.str() == "[1, 2, 3, 4, 5, 6, 7, 8, 9]");
  t3r = T3i(0);
  ss >> t3r;
  CHECK(t3 == t3r);
}

TEST_CASE("boolean ops")
{
  T3i t11(1), t12{1}, t13{1};
  CHECK(t11 == t12);
  CHECK(t12 == t13);

  T3i t21(1, 2, 3), t22{1, 2, 3};
  CHECK(t21 == t22);
  CHECK(t21 != t12);

  T3i t31(1, 2, 3, 4, 5, 6, 7, 8, 9), t32{1, 2, 3, 4, 5, 6, 7, 8, 9};
  CHECK(t31 == t32);
  CHECK(t31 != t22);
}

TEST_CASE("arithm ops")
{
  T3i t0(0), t1(1), t2(2);
  CHECK(t0 + t1 == t1);
  CHECK(t1 + t0 == t1);
  CHECK(t1 + t1 == t2);

  CHECK(t1 - t0 == t1);
  CHECK(t0 - t1 == -t1);

  CHECK(2 * t1 == t2);
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

TEST_CASE("assign ops")
{
  T3i t3(1), t4(2);
  t3 += t3;
  CHECK(t3 == t4);
  t3 /= 2;
  CHECK(t3 == T3i(1));
  t3 *= 2;
  CHECK(t3 == t4);
  t3 -= t4;
  CHECK(t3 == T3i(0));
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

  Tensor<4, int> E4(1, 1, 1, 1);
  Tensor<4, int> t4(2, 3, 5, 2,
                    6, 1, 8, 3,
                    5, 4, 9, 2,
                    1, 3, 5, 6);
  Tensor<4, int> t5( 121,  28, -76, -29,
                      88,  20, -55, -21,
                    -113, -26,  71,  27,
                      30,   7, -19,  -7);
  CHECK(t4.det() == -1);
  CHECK(t4.invert() == t5);
  CHECK(t4*t4.invert() == E4);
}
