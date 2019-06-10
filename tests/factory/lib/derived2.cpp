#include "derived2.hpp"
#include "factory/ObjectsFactory.h"

const std::string Derived2::name("Derived2");

namespace
{
  ObjectsFactory<BaseF> reg_me("Derived2", [](){ return new Derived2; });
}
