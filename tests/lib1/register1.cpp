#include "derived11.hpp"
#include "derived12.hpp"
#include "factory/ObjectsFactory.h"

const std::string Derived11::name = "Derived11";
const std::string Derived12::name = "Derived12";

//namespace
//{
  LIB1_EXPORT ObjectsFactory<Base1F> reg_d11(Derived11::name, [](){ return new Derived11; });
  LIB1_EXPORT ObjectsFactory<Base1F> reg_d12(Derived12::name, [](){ return new Derived12; });
//}
