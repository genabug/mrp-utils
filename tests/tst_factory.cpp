#include "doctest.h"
#include "lib1/base1.hpp"
#include "lib2/base2.hpp"
#include "factory/ObjectsFactory.h"

#include <string>
#include <vector>

TEST_CASE("factory")
{
  std::vector<std::string> names;
  ObjectsFactory<Base1F>::dump_names(names);
  REQUIRE(names.size() > 1);
  CHECK(names[0] == "Derived12");
  CHECK(names[1] == "Derived11");

  auto *d1f = ObjectsFactory<Base1F>::find("Derived11");
  auto *d2f = ObjectsFactory<Base1F>::find("Derived12");
  auto *err = ObjectsFactory<Base1F>::find("Derived13");

  CHECK(d1f != nullptr);
  CHECK(d2f != nullptr);
  CHECK(err == nullptr);

  auto *d11 = ObjectsFactory<Base1F>::build("Derived11");
  auto *d12 = ObjectsFactory<Base1F>::build("Derived12");
  CHECK_THROWS(ObjectsFactory<Base1F>::build("Derived13"));

  CHECK(d11->id() == "Derived11");
  CHECK(d12->id() == "Derived12");

  if (ObjectsFactory<Base2F>::find("Derived21"))
  {
    auto *d21 = ObjectsFactory<Base2F>::build("Derived21");
    std::cerr << "d11: " << d11->addr() << "\nd21: " << d21->addr() << '\n';
  }
  else
    std::cerr << "Derived21 not found!\n";
}
