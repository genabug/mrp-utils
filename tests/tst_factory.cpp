#include <gtest/gtest.h>
#include "lib1/base1.hpp"
#include "lib2/base2.hpp"
#include "factory/ObjectsFactory.h"

#include <string>
#include <vector>
#include <iostream>

TEST(factory, basic)
{
  std::vector<std::string> names;
  ObjectsFactory<Base1F>::dump_names(names);
  ASSERT_GT(names.size(), 1);
  EXPECT_EQ(names[0], "Derived12");
  EXPECT_EQ(names[1], "Derived11");

  auto *d1f = ObjectsFactory<Base1F>::find("Derived11");
  auto *d2f = ObjectsFactory<Base1F>::find("Derived12");
  auto *err = ObjectsFactory<Base1F>::find("Derived13");

  EXPECT_NE(d1f, nullptr);
  EXPECT_NE(d2f, nullptr);
  EXPECT_EQ(err, nullptr);

  auto *d11 = ObjectsFactory<Base1F>::build("Derived11");
  auto *d12 = ObjectsFactory<Base1F>::build("Derived12");
  EXPECT_THROW(ObjectsFactory<Base1F>::build("Derived13"), std::runtime_error);

  EXPECT_EQ(d11->id(), "Derived11");
  EXPECT_EQ(d12->id(), "Derived12");

  if (ObjectsFactory<Base2F>::find("Derived21"))
  {
    auto *d21 = ObjectsFactory<Base2F>::build("Derived21");
    std::cerr << "d11: " << d11->addr() << "\nd21: " << d21->addr() << '\n';
  }
  else
    std::cerr << "Derived21 not found!\n";
}
