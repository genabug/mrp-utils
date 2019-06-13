/*!
  \file tst_vector.cpp
  \author gennadiy (gennadiy3.14@gmail.com)
  \brief Euclidian vector tests.
  \see Vector.h
*/

#include "doctest.h"
#include "utils/Vector.h"
#include <limits>
#include <sstream>

using V2d = Vector<2>;
using V3d = Vector<3>;
using V2i = Vector<2, int>;
using V3i = Vector<3, int>;

template<class T>
  constexpr std::enable_if_t<!std::is_integral<T>::value, bool>
    almost_equal(T x, T y, int ulp = 1)
{
  // the machine epsilon has to be scaled to the magnitude of the values used
  // and multiplied by the desired precision in ULPs (units of least precision)
  return std::abs(x - y) < std::numeric_limits<T>::epsilon() * std::abs(x + y) * ulp
         // unless the result is subnormal
         || std::abs(x - y) < std::numeric_limits<T>::min();
}

/*--------------------------------- compile-time tests ----------------------------------*/

namespace
{
  // init
  constexpr V2i x(4, -3), y(3, 4);
  constexpr V3i v, v1(1), v2(2), v3(v1 + v2), v4(4, 5, 6), v5(3, -6, 3);

  // access
  static_assert((v1[0] == 1) && (v1[1] == 1) && (v1[2] == 1), "single init failed");
  static_assert((v2[0] == 2) && (v2[1] == 2) && (v2[2] == 2), "single init failed");
  static_assert((v3[0] == 3) && (v3[1] == 3) && (v3[2] == 3), "single init failed");
  static_assert((v4[0] == 4) && (v4[1] == 5) && (v4[2] == 6), "full init failed");

  // conversion
  constexpr V2d xd = V2d(x);
  static_assert(almost_equal(xd[0], 4.), "vd = vi failed");
  static_assert(almost_equal(xd[1], -3.), "vd = vi failed");

  constexpr V2i xi = V2i(xd);
  static_assert(xi[0] == 4, "vi = vd failed");
  static_assert(xi[1] == -3, "vi = vd failed");

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
  static_assert(almost_equal(sqs(vd), 25.), "sqs failed");
  static_assert(almost_equal(fabs(vd), 5.), "abs failed");

  constexpr V2d ex(1, 0), ey(0, 1);
  static_assert(almost_equal(cos(ex, ex), 1.), "cos failed");
  static_assert(almost_equal(cos(ex, ey), 0.), "cos failed");
  static_assert(almost_equal(sin(ey, ey), 0.), "sin failed");
  static_assert(almost_equal(sin(ey, ex), 1.), "sin failed");

  static_assert(almost_equal(cos(vd, ey), .8), "cos failed");
  static_assert(almost_equal(sin(vd, ey), .6), "sin failed");
  static_assert(almost_equal(cos(vd, ex), .6), "cos failed");
  static_assert(almost_equal(sin(vd, ex), .8), "sin failed");

  constexpr Vector<2, float> vf(3, 4);
  static_assert(almost_equal(sqs(vf), 25.f), "sqs failed");
  static_assert(almost_equal(fabs(vf), 5.f), "abs failed");

  constexpr Vector<2, long> vl(3, 4);
  static_assert(sqs(vl) == 25, "sqs failed");
  static_assert(fabs(vl) == 5, "abs failed");
}

/*----------------------------------- run-time tests ------------------------------------*/

// not needed, it's fully tested in compile-time 8)
// OK, they are needed (for nice plots and stats) but later

union Test
{
  int i; // pod type
  V2i v; // type with non-trivial ctor
  Test() {}
};

TEST_CASE("union")
{
  Test t;
  t.v = V2i(1);
  t.i = 2;
  std::cerr << t.v << '\n';

  V2i v{10};
  std::cerr << v << '\n';

  v = V2i(1);
}
