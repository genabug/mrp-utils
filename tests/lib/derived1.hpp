#ifndef DERIVED1_HPP_INCLUDED
#define DERIVED1_HPP_INCLUDED

#include "base.hpp"

struct Derived1 final : Base
{
    ~Derived1() final = default;
    std::string id() const final { return name; }
private:
    static const std::string name;
};

#endif // DERIVED1_HPP_INCLUDED
