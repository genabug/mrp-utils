#ifndef DERIVED11_HPP_INCLUDED
#define DERIVED11_HPP_INCLUDED

#include "base1.h"

struct Derived11 final : Base1
{
  ~Derived11() final = default;
  std::string id() const final { return name; }
  std::string addr() const final;

private:
  static const std::string name;
};

#endif // DERIVED11_HPP_INCLUDED
