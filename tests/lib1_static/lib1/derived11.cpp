#include "derived11.h"
#include "factory/ObjectsFactory.h"
#include <sstream>

const std::string Derived11::name = "Derived11";
const ObjectsFactory<Base1F> reg_d11("Derived11", []() { return new Derived11; });

std::string Derived11::addr() const
{
  std::stringstream ss;
  ss << this;
  return ss.str();
};
