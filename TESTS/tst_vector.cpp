/*!
  \file tst_vector.cpp
  \author gennadiy (gennadiy3.14@gmail.com)
  \brief Euclidian vector tests.
  \see Vector.h
*/

#include "doctest.h"
#include "../Vector.h"
#include <sstream>

using V2d = Vector<2>;
using V2i = Vector<2, int>;
using V3i = Vector<3, int>;

namespace
{
  // init
  constexpr V3i v, v1(1), v2(2), v3(3), v4(4, 5, 6);

  // access
  static_assert((v1[0] == 1) && (v1[0] == v1[1]) && (v1[0] == v1[2]), "access failed");
  static_assert((v2[0] == 2) && (v2[0] == v2[1]) && (v2[0] == v2[2]), "access failed");
  static_assert((v3[0] == 3) && (v2[0] == v2[1]) && (v2[0] == v2[2]), "access failed");
  static_assert((v4[0] == 4) && (v4[1] == 5) && (v4[2] == 6), "access failed");

  // ops
  static_assert(v1 == v1, "v == v failed");
  static_assert(v1 != v2, "v != v failed");
  static_assert(v1 + v1 == v2, "v + v failed");
  static_assert(v1 + v2 == v3, "v + v failed");
  static_assert(v3 - v1 == +v2, "v - v failed");
  static_assert(v1 - v3 == -v2, "v - v failed");
  static_assert(v1*2 == v2, "v * a failed");
  static_assert(3*v1 == v3, "a * v failed");
  static_assert(v2/2 == v1, "v / a failed");
  static_assert(v2/2 == v3/3, "v / a failed");

  constexpr V2i vi(3, 4);
  static_assert(fabs(vi) == 5, "|v| failed");

  constexpr V2d vd(3, 4);
  static_assert(fabs(vd) == 5., "|v| failed");

  constexpr V2d ex(1, 0), ey(0, 1);
  static_assert(sin(ex, ey) == 1., "sin failed");
  static_assert(sin(ey, ex) == 1., "sin failed");
  static_assert(cos(ex, ey) == 0., "cos failed");
  static_assert(cos(ex, ey) == 0., "cos failed");
}

TEST_CASE("create")
{
  V3i v1, v2(2);
  CHECK(v1[0] == 0);
  CHECK(v2[1] == 2);
}
