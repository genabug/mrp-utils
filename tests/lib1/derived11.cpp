#include "derived11.hpp"
#include <sstream>

const std::string Derived11::name = "Derived11";
ObjectsFactory<Base1F> reg_d11("Derived11", []() { return new Derived11; });

std::string Derived11::addr() const
{
  std::stringstream ss;
  ss << this;
  return ss.str();
};
