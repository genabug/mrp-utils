#ifndef DERIVED21_HPP_INCLUDED
#define DERIVED21_HPP_INCLUDED

#include "base2.hpp"

struct LIB2_EXPORT Derived21 final : Base2
{
  ~Derived21() final = default;
  std::string id() const final { return name; }
  std::string addr() const final;

  static const std::string name;
};

#endif // DERIVED21_HPP_INCLUDED
