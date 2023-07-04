#ifndef DERIVED12_HPP_INCLUDED
#define DERIVED12_HPP_INCLUDED

#include "base1.h"

struct Derived12 final : Base1
{
  ~Derived12() final = default;
  std::string id() const final { return name; }
  std::string addr() const final;

private:
  static const std::string name;
};

#endif // DERIVED12_HPP_INCLUDED
