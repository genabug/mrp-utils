#ifndef BASE2_HPP_INCLUDED
#define BASE2_HPP_INCLUDED

#include <string>

struct Base2
{
  virtual ~Base2() = default;
  virtual std::string id() const = 0;
  virtual std::string addr() const = 0;
};

using Base2F = Base2*();

#endif // BASE2_HPP_INCLUDED
