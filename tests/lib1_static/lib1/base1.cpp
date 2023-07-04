#include "base1.h"

template class ObjectsFactory<Base1F>;

namespace
{
struct Base1Logger : IO::FactoryLogger
{
  Base1Logger(std::ostream &out)
  {
    IO::print_factory<Base1F>(out, "Base1", "Base1 logger");
  }
};

ObjectsFactory<IO::FactoryLoggerF> reg_b1("Base1", [](std::ostream &out){ return Base1Logger(out); });
}
