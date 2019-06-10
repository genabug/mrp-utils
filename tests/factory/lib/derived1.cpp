#include "derived1.hpp"
#include "factory/ObjectsFactory.h"

const std::string Derived1::name("Derived1");

namespace
{
  const ObjectsFactory<BaseF> reg_me("Derived1", [](){ return new Derived1; });
}
