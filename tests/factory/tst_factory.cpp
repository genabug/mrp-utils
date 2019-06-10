#include "doctest.h"
#include "lib/base.hpp"
#include "factory/ObjectsFactory.h"

#include <string>
#include <vector>

TEST_CASE("factory")
{
  std::vector<std::string> names;
  ObjectsFactory<BaseF>::dump_names(names);
  CHECK(names.size() > 1);
  CHECK(names[0] == "Derived2");
  CHECK(names[1] == "Derived1");

  auto *d1f = ObjectsFactory<BaseF>::find("Derived1");
  auto *d2f = ObjectsFactory<BaseF>::find("Derived2");
  auto *err = ObjectsFactory<BaseF>::find("Derived3");

  CHECK(d1f != nullptr);
  CHECK(d2f != nullptr);
  CHECK(err == nullptr);

  auto *d1 = ObjectsFactory<BaseF>::build("Derived1");
  auto *d2 = ObjectsFactory<BaseF>::build("Derived2");
  CHECK_THROWS(ObjectsFactory<BaseF>::build("Derived3"));

  CHECK(d1->id() == "Derived1");
  CHECK(d2->id() == "Derived2");
}

