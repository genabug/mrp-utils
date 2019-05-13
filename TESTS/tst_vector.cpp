/*!
  \file tst_vector.cpp
  \author gennadiy (gennadiy3.14@gmail.com)
  \brief Euclidian vector tests.
  \see Vector.h
*/

#include "doctest.h"
#include "Vector.h"
#include <limits>
#include <sstream>

using V2d = Vector<2>;
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
  constexpr V3i v, v1(1), v2(2), v3(3), v4(4, 5, 6);

  // access
  static_assert((v1[0] == 1) && (v1[1] == 1) && (v1[2] == 1), "single init failed");
  static_assert((v2[0] == 2) && (v2[1] == 2) && (v2[2] == 2), "single init failed");
  static_assert((v3[0] == 3) && (v3[1] == 3) && (v3[2] == 3), "single init failed");
  static_assert((v4[0] == 4) && (v4[1] == 5) && (v4[2] == 6), "full init failed");

  // ops
  static_assert(v == v, "v == v failed");
  static_assert(v != v1, "v != v failed");
  static_assert(v1*2 == v2, "v * a failed");
  static_assert(3*v1 == v3, "a * v failed");
  static_assert(v2/2 == v1, "v / a failed");
  static_assert(v2/2 == v3/3, "v / a failed");
  static_assert(v1 + v1 == v2, "v + v failed");
  static_assert(v1 + v2 == v3, "v + v failed");
  static_assert(v2 + v1 == v3, "v + v failed");
  static_assert(v3 - v1 == +v2, "v - v failed");
  static_assert(v1 - v3 == -v2, "v - v failed");
  static_assert(v1 * v2 == 6, "v * v failed");
  static_assert(v2 * v1 == 6, "v * v failed");
  static_assert(v2 * v4 == 30, "v * v failed");

  constexpr V2i vi(-3, -4);
  static_assert(sqs(vi) == 25, "sqs failed");
  static_assert(fabs(vi) == 5, "abs failed");

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

  constexpr V2i vi90(4, -3);
  static_assert(~ex == ey, "~v failed in 2D");
  static_assert(~ey == -ex, "~v failed in 2D");
  static_assert(~vi == vi90, "~v failed in 2D");

  static_assert(almost_equal(ex % ex, 0.), "v % v failed in 2D");

  constexpr V3i x(1, 0, 0), y(0, 1, 0), z(0, 0, 1);
  static_assert(x % y == z, "v % v failed in 3D");
  static_assert(y % x == -z, "v % v failed in 3D");
  static_assert(x % z == -y, "v % v failed in 3D");
  static_assert(z % x == y, "v % v failed in 3D");
  constexpr V3i v5(1, 2, 3), v6(4, 5, 6), v7(-3, 6, -3);
  static_assert(v5 % v6 == v7, "v % v failed in 3D");
  static_assert(v6 % v5 == -v7, "v % v failed in 3D");
}

/*----------------------------------- run-time tests ------------------------------------*/

// not needed, it's fully tested in compile-time 8)
// OK, they are needed (for nice plots and stats) but later
