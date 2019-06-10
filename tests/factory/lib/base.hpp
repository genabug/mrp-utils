#ifndef BASE_HPP_INCLUDED
#define BASE_HPP_INCLUDED

#include <string>

struct Base
{
    virtual ~Base() = default;
    virtual std::string id() const = 0;
};

using BaseF = Base*();

#endif // BASE_HPP_INCLUDED
