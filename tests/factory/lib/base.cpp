#include "base.hpp"
#include "factory/ObjectsFactory.h"

struct BaseLogger final : IO::FactoryLogger
{
    BaseLogger(std::ostream &out)
    {
      IO::print_factory<BaseF>(out, "Base", "Hierarchy Base");
    }
};

namespace
{
  ObjectsFactory<IO::FactoryLoggerF>
    reg_base_logger("Base", [](std::ostream &out){ return BaseLogger(out); });
}
