#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "lib1/base1.h"
#include "lib2/base2.h"
#include "factory/ObjectsFactory.h"

#include <string>
#include <vector>
#include <iostream>

TEST(factory_static, lib1)
{
  std::vector<std::string> names, expected{"Derived11", "Derived12"};
  ObjectsFactory<Base1F>::dump_names(names);
  ASSERT_THAT(names, ::testing::ContainerEq(expected));

  auto *d1f = ObjectsFactory<Base1F>::find("Derived11");
  auto *d2f = ObjectsFactory<Base1F>::find("Derived12");
  auto *err = ObjectsFactory<Base1F>::find("Derived13");

  ASSERT_NE(d1f, nullptr);
  ASSERT_NE(d2f, nullptr);
  EXPECT_EQ(err, nullptr);

  auto *d11 = ObjectsFactory<Base1F>::build("Derived11");
  auto *d12 = ObjectsFactory<Base1F>::build("Derived12");
  EXPECT_THROW(ObjectsFactory<Base1F>::build("Derived13"), std::runtime_error);

  EXPECT_EQ(d11->id(), "Derived11");
  EXPECT_EQ(d12->id(), "Derived12");
}

TEST(factory_static, lib2)
{
  std::vector<std::string> names, expected{"Derived21", "Derived22"};
  ObjectsFactory<Base2F>::dump_names(names);
  ASSERT_THAT(names, ::testing::ContainerEq(expected));

  auto *d1f = ObjectsFactory<Base2F>::find("Derived21");
  auto *d2f = ObjectsFactory<Base2F>::find("Derived22");
  auto *err = ObjectsFactory<Base2F>::find("Derived23");

  ASSERT_NE(d1f, nullptr);
  ASSERT_NE(d2f, nullptr);
  EXPECT_EQ(err, nullptr);

  auto *d21 = ObjectsFactory<Base2F>::build("Derived21");
  auto *d22 = ObjectsFactory<Base2F>::build("Derived22");
  EXPECT_THROW(ObjectsFactory<Base2F>::build("Derived13"), std::runtime_error);

  EXPECT_EQ(d21->id(), "Derived21");
  EXPECT_EQ(d22->id(), "Derived22");
}
