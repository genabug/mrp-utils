#ifndef BASE1_HPP_INCLUDED
#define BASE1_HPP_INCLUDED

#include <string>
#include "lib1_export.h"

struct LIB1_EXPORT Base1
{
  virtual ~Base1() = default;
  virtual std::string id() const = 0;
  virtual std::string addr() const = 0;
};

using Base1F = Base1*();

#endif // BASE1_HPP_INCLUDED
