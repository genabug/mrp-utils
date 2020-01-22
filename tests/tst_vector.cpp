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

using V3d = Vector<3>;
using V3i = Vector<3, int>;

union Test
{
  int i; // pod type
  V3i v; // type with non-trivial ctor
  Test() {}
};

TEST_CASE("union")
{
  Test t;
  t.v = V3i(1);
  t.i = 2;
//  std::cerr << t.v << '\n';

  V3i v{10};
//  std::cerr << v << '\n';

  v = V3i(1);
}

TEST_CASE("io ops")
{
  std::stringstream ss;

  ss.str("");
  V3i v3(1, 2, 3), v4;

  ss.str("");
  ss << v3;
  CHECK(ss.str() == "(1, 2, 3)");
  ss >> v4;
  CHECK(v3 == v4);

  ss.str("");
  //ss.clear(); // doesn't needed because ss is not in eof state and contains ')'
  ss << Vectors::bareComponents << v3;
  CHECK(ss.str() == "1 2 3");
  v4 = V3i(0);
  ss >> v4;
  CHECK(v3 == v4);

  ss.str("");
  ss.clear(); // clear eof bit
  V3i v1(1), v2;
  ss << v1;
  CHECK(ss.str() == "1 1 1");
  ss >> v2;
  CHECK(v2 == v1);

  ss.str("");
  ss.clear(); // clear eof bit
  ss << Vectors::inBrackets << v1;
  CHECK(ss.str() == "(1, 1, 1)");
  v2 = V3i(0);
  ss >> v2;
  CHECK(v2 == v1);
}
