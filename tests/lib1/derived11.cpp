#include "derived11.hpp"
#include <sstream>

//const std::string Derived11::name = "Derived11";

std::string Derived11::addr() const
{
  std::stringstream ss;
  ss << this;
  return ss.str();
};
