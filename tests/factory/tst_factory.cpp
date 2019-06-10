#include "doctest.h"
#include "factory/ObjectsFactory.h"

struct B { int d; B(int i) : d(i) {} };
struct D1 : B { D1(int i) : B(i) {} };
struct D2 : B { D2(int i, int j) : B(j) {} };

struct C { C(int, int) {} };

using FS = B(int);
ObjectsFactory<FS> of_b("B", [](int i){ return B(i); });
ObjectsFactory<FS> of_d1("D1", [](int i){ return D1(i); });
ObjectsFactory<FS> of_d20("D20", [](int i){ return D2(i, 0); });
ObjectsFactory<FS> of_d21("D21", [](int i){ return D2(i, 1); });

TEST_CASE("create")
{
  auto b = ObjectsFactory<FS>::build("B", 1);
  CHECK(b.d == 1);

  auto d1 = ObjectsFactory<FS>::build("D1", 2);
  CHECK(d1.d == 2);

  auto d2 = ObjectsFactory<FS>::build("D20", 2);
  CHECK(d2.d == 0);
}

