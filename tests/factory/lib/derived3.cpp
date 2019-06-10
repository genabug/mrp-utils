#include "derived3.h"
#include "factory/ObjectsFactory.h"

namespace
{
  ObjectsFactory<BaseF> reg_me("Derived3", [](){ return new Derived3(); });
}
