#ifndef DERIVED3_H_INCLUDED
#define DERIVED3_H_INCLUDED

#include "base.hpp"

struct Derived3 final : Base
{
    ~Derived3() final = default;
    const char* id() const final { return "Derived3"; }
};

#endif // DERIVED3_H_INCLUDED
