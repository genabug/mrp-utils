#include <gtest/gtest.h>
#include "lib1/base1.hpp"
#include "lib2/base2.hpp"
#include "factory/ObjectsFactory.h"

#include <string>
#include <vector>

TEST(factory, dump_names_ok)
{
  std::vector<std::string> names;
  ObjectsFactory<Base1F>::dump_names(names);
  ASSERT_EQ(names.size(), 2);
  EXPECT_EQ(names[0], "Derived12");
  EXPECT_EQ(names[1], "Derived11");
}

TEST(factory, create_objects_lib1_ok)
{
  auto *d11 = ObjectsFactory<Base1F>::build("Derived11");
  auto *d12 = ObjectsFactory<Base1F>::build("Derived12");

  ASSERT_NE(d11, nullptr);
  EXPECT_EQ(d11->id(), "Derived11");

  ASSERT_NE(d12, nullptr);
  EXPECT_EQ(d12->id(), "Derived12");
}

TEST(factory, create_not_registered_object_throws)
{
  EXPECT_THROW(ObjectsFactory<Base1F>::build("NotRegistered"), std::runtime_error);
  EXPECT_THROW(ObjectsFactory<Base2F>::build("NotRegistered"), std::runtime_error);
}
