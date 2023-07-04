#include "derived12.h"
#include "factory/ObjectsFactory.h"
#include <sstream>

const std::string Derived12::name = "Derived12";
const ObjectsFactory<Base1F> reg_d12("Derived12", [](){ return new Derived12; });

std::string Derived12::addr() const
{
  std::stringstream ss;
  ss << this;
  return ss.str();
}
