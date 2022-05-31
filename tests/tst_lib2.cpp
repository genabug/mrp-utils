#include "lib2/derived21.hpp"
#include "lib2/derived22.hpp"
#include "doctest.h"
#include <string>
#include <vector>

TEST_CASE("lib2")
{
  Derived21 d21;
  CHECK_NOTHROW(d21.addr());

  Derived22 d22;
  CHECK_NOTHROW(d22.addr());
}
