#include "lib2/derived21.h"
#include "lib2/derived22.h"
#include <gtest/gtest.h>

TEST(lib2_static, basic1)
{
  Derived21 d21;
  EXPECT_NO_THROW(d21.addr());
}

TEST(lib2_static, basic2)
{
  Derived22 d22;
  EXPECT_NO_THROW(d22.addr());
}
