#include "derived12.hpp"
#include <sstream>

const std::string Derived12::name = "Derived12";

std::string Derived12::addr() const
{
  std::stringstream ss;
  ss << this;
  return ss.str();
}
