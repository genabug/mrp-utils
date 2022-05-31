#include "derived11.hpp"
#include "derived12.hpp"
#include "factory/ObjectsFactory.h"

namespace
{
  ObjectsFactory<Base1F> reg_d11("Derived11", []() { return new Derived11; });
  ObjectsFactory<Base1F> reg_d12("Derived12", []() { return new Derived12; });
}
