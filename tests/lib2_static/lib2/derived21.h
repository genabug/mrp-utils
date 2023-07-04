#ifndef DERIVED21_HPP_INCLUDED
#define DERIVED21_HPP_INCLUDED

#include "base2.h"

struct Derived21 final : Base2
{
  ~Derived21() final = default;
  std::string id() const final { return name; }
  std::string addr() const final;

private:
  static const std::string name;
};

#endif // DERIVED21_HPP_INCLUDED
