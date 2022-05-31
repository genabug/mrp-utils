#include "derived21.hpp"
#include "derived22.hpp"
#include "factory/ObjectsFactory.h"

namespace
{
  ObjectsFactory<Base2F> reg_d21("Derived21", []() { return new Derived21; });
  ObjectsFactory<Base2F> reg_d22("Derived22", []() { return new Derived22; });
}
