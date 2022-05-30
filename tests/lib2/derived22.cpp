#include "derived22.hpp"
#include "../lib1/base1.hpp"
#include "factory/ObjectsFactory.h"

std::string Derived22::addr() const
{
  auto *pd12 = ObjectsFactory<Base1F>::build("derived12");
  return pd12->addr();
}
