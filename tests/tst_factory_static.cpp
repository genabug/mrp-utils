#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "lib1/base1.h"
#include "lib2/base2.h"
#include "factory/ObjectsFactory.h"

#include <string>
#include <vector>

TEST(factory_static, lib1)
{
  std::vector<std::string> names;
  ObjectsFactory<Base1F>::dump_names(names);
  ASSERT_THAT(names, ::testing::Contains("Derived11"));
  ASSERT_THAT(names, ::testing::Contains("Derived12"));

  ASSERT_NE(ObjectsFactory<Base1F>::find("Derived11"), nullptr);
  ASSERT_NE(ObjectsFactory<Base1F>::find("Derived12"), nullptr);
  EXPECT_EQ(ObjectsFactory<Base1F>::find("Nonexistent"), nullptr);

  auto *d11 = ObjectsFactory<Base1F>::build("Derived11");
  auto *d12 = ObjectsFactory<Base1F>::build("Derived12");
  EXPECT_THROW(ObjectsFactory<Base1F>::build("Nonexistent"), std::runtime_error);

  EXPECT_EQ(d11->id(), "Derived11");
  EXPECT_EQ(d12->id(), "Derived12");
}

TEST(factory_static, lib2)
{
  std::vector<std::string> names;
  ObjectsFactory<Base2F>::dump_names(names);
  ASSERT_THAT(names, ::testing::Contains("Derived21"));
  ASSERT_THAT(names, ::testing::Contains("Derived22"));

  ASSERT_NE(ObjectsFactory<Base2F>::find("Derived21"), nullptr);
  ASSERT_NE(ObjectsFactory<Base2F>::find("Derived22"), nullptr);
  EXPECT_EQ(ObjectsFactory<Base2F>::find("Nonexistent"), nullptr);

  auto *d21 = ObjectsFactory<Base2F>::build("Derived21");
  auto *d22 = ObjectsFactory<Base2F>::build("Derived22");
  EXPECT_THROW(ObjectsFactory<Base2F>::build("Derived13"), std::runtime_error);

  EXPECT_EQ(d21->id(), "Derived21");
  EXPECT_EQ(d22->id(), "Derived22");
}
