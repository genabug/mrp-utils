#ifndef DERIVED2_HPP_INCLUDED
#define DERIVED2_HPP_INCLUDED

#include "base.hpp"

struct Derived2 final : Base
{
    ~Derived2() final = default;
    std::string id() const final { return name; }
private:
    static const std::string name;
};

#endif // DERIVED2_HPP_INCLUDED
