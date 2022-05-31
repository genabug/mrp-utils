#include "derived22.hpp"
#include "../lib1/base1.hpp"
#include "factory/ObjectsFactory.h"

const std::string Derived22::name = "Derived22";

std::string Derived22::addr() const
{
  auto *pd12 = ObjectsFactory<Base1F>::build("Derived12");
  return pd12->addr();
}
