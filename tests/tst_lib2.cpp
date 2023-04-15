#include "lib2/derived21.hpp"
#include "lib2/derived22.hpp"
#include <gtest/gtest.h>
#include <string>
#include <vector>

TEST(lib2, basic)
{
  Derived21 d21;
  EXPECT_NO_THROW(d21.addr());

  Derived22 d22;
  EXPECT_NO_THROW(d22.addr());
}
