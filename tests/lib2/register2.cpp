#include "derived21.hpp"
#include "derived22.hpp"
#include "factory/ObjectsFactory.h"

const std::string Derived21::name = "Derived21";
const std::string Derived22::name = "Derived22";

//namespace
//{
  LIB2_EXPORT ObjectsFactory<Base2F> reg_d21(Derived21::name, [](){ return new Derived21; });
  LIB2_EXPORT ObjectsFactory<Base2F> reg_d22(Derived22::name, [](){ return new Derived22; });
//}
