#ifndef BASE2_HPP_INCLUDED
#define BASE2_HPP_INCLUDED

#include <string>
#include "lib2_export.h"

struct LIB2_EXPORT Base2
{
  virtual ~Base2() = default;
  virtual std::string id() const = 0;
  virtual std::string addr() const = 0;
};

using Base2F = Base2*();

#include "factory/ObjectsFactory.h"
extern template class LIB2_EXPORT ObjectsFactory<Base2F>;

#endif // BASE2_HPP_INCLUDED
