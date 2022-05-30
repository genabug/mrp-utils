#ifndef DERIVED22_HPP_INCLUDED
#define DERIVED22_HPP_INCLUDED

#include "base2.hpp"

struct LIB2_EXPORT Derived22 final : Base2
{
  ~Derived22() final = default;
  std::string id() const final { return name; }
  std::string addr() const final;

  static const std::string name;
};

#endif // DERIVED22_HPP_INCLUDED
