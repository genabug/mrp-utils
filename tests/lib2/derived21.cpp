#include "derived21.hpp"
#include "../lib1/base1.hpp"
#include "factory/ObjectsFactory.h"

const std::string Derived21::name = "Derived21";

std::string Derived21::addr() const
{
  auto *pd11 = ObjectsFactory<Base1F>::build("Derived11");
  return pd11->addr();
}
