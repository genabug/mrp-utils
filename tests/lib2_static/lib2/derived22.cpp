#include "derived22.h"
#include "lib1/base1.h"
#include "factory/ObjectsFactory.h"

const std::string Derived22::name = "Derived22";
const ObjectsFactory<Base2F> reg_d22("Derived22", [](){ return new Derived22; });

std::string Derived22::addr() const
{
  auto *pd12 = ObjectsFactory<Base1F>::build("Derived12");
  return pd12->addr();
}
