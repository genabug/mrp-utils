#include "derived22.hpp"
#include "lib1/base1.hpp"

const std::string Derived22::name = "Derived22";
ObjectsFactory<Base2F> reg_d22("Derived22", []() { return new Derived22; });

std::string Derived22::addr() const
{
  auto *pd12 = ObjectsFactory<Base1F>::build("Derived12");
  return pd12->addr();
}
