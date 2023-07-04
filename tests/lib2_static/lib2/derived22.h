#ifndef DERIVED22_HPP_INCLUDED
#define DERIVED22_HPP_INCLUDED

#include "base2.h"

struct Derived22 final : Base2
{
  ~Derived22() final = default;
  std::string id() const final { return name; }
  std::string addr() const final;

private:
  static const std::string name;
};

#endif // DERIVED22_HPP_INCLUDED
