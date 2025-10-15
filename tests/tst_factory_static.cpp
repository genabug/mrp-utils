#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "lib1/base1.h"
#include "lib2/base2.h"
#include "factory/ObjectsFactory.h"

#include <string>
#include <vector>

TEST(factory_static, dump_names_lib1_ok)
{
  std::vector<std::string> names;
  ObjectsFactory<Base1F>::dump_names(names);
  EXPECT_EQ(names.size(), 2);
  EXPECT_THAT(names, ::testing::Contains("Derived11"));
  EXPECT_THAT(names, ::testing::Contains("Derived12"));
}

TEST(factory_static, create_objects_lib1_ok)
{
  auto *d11 = ObjectsFactory<Base1F>::build("Derived11");
  auto *d12 = ObjectsFactory<Base1F>::build("Derived12");

  EXPECT_EQ(d11->id(), "Derived11");
  EXPECT_EQ(d12->id(), "Derived12");
}

TEST(factory_static, dump_names_lib2_ok)
{
  std::vector<std::string> names;
  ObjectsFactory<Base2F>::dump_names(names);
  EXPECT_EQ(names.size(), 2);
  EXPECT_THAT(names, ::testing::Contains("Derived21"));
  EXPECT_THAT(names, ::testing::Contains("Derived22"));
}

TEST(factory_static, create_objects_lib2_ok)
{
  auto *d21 = ObjectsFactory<Base2F>::build("Derived21");
  auto *d22 = ObjectsFactory<Base2F>::build("Derived22");
  
  EXPECT_EQ(d21->id(), "Derived21");
  EXPECT_EQ(d22->id(), "Derived22");
}

TEST(factory_static, create_not_registered_object_throws)
{
  EXPECT_THROW(ObjectsFactory<Base1F>::build("Nonexistent"), std::runtime_error);
  EXPECT_THROW(ObjectsFactory<Base2F>::build("Derived13"), std::runtime_error);
}
