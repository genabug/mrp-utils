#include "doctest.h"

#include "utils/QState.h"
#include "utils/QTraits.h"
#include "utils/Vector.h"
#include <string>

using namespace Quantities;
using namespace std::string_literals;
namespace qdetails = Quantities::details;

using V3i = Vector<3, int>;

using ti = QTraits<int, 3, 1, 't','i'>;
using td = QTraits<double, 3, 1, 't','d'>;
using ts = QTraits<std::string, 0, 0, 't','s'>;
using tv = QTraits<V3i, 3, 3, 't','v'>;

template<class T> void WTF(T);

//TEST_SUITE_BEGIN("state");

TEST_CASE("create")
{
  auto s = make_state<ti, ts, tv>(1, "foo"s, V3i(2, 3, 4));

  CHECK(s.get<0>() == 1);
  CHECK(s.get<1>() == "foo");
  CHECK(s.get<2>() == V3i(2, 3, 4));

  CHECK(s.get<ti>() == 1);
  CHECK(s.get<ts>() == "foo");
  CHECK(s.get<tv>() == V3i(2, 3, 4));
}

TEST_CASE("create_ref")
{
  int i = 1;
  auto str = "bar"s;
  V3i v(2, 3, 4);
  auto s = make_state<ti, ts, tv>(i, str, v);

  CHECK(s.get<0>() == i);
  CHECK(s.get<1>() == str);
  CHECK(s.get<2>() == v);

  CHECK(s.get<ti>() == i);
  CHECK(s.get<ts>() == str);
  CHECK(s.get<tv>() == v);
}

TEST_CASE("create_cref")
{
  const int i = 1;
  const auto str = "bar"s;
  const auto v = V3i(2, 3, 4);
  auto s = make_state<ti, ts, tv>(i, str, v);

  CHECK(s.get<0>() == 1);
  CHECK(s.get<1>() == "bar");
  CHECK(s.get<2>() == V3i(2, 3, 4));

  CHECK(s.get<ti>() == 1);
  CHECK(s.get<ts>() == "bar");
  CHECK(s.get<tv>() == V3i(2, 3, 4));

//  s.get<ti>() = 2;      // COMPILE ERROR: can't assign to const
//  s.get<ts>() = "foo";  // COMPILE ERROR: can't assign to const
//  s.get<tv>() = V3i(3); // COMPILE ERROR: can't assign to const

  int j = s.get<ti>();
  CHECK(j == i);
//  std::string &ss = s.get<ts>(); // COMPILE ERROR: can't bind non-const ref to a constant
//  ss = "foo";
  auto vv = s.get<tv>();
  vv = V3i(0);
  CHECK(s.get<tv>() == V3i(2, 3, 4));
}

TEST_CASE("access")
{
  auto s = make_state<ti, ts, tv>(1, "foo"s, V3i(3));

  SUBCASE("by value")
  {
    auto j = s.get<ti>(); // j is not a ref, thus it's a copy!
    auto k = s.copy<ti>();
    j *= 2; k += j + 1;
    CHECK(s.get<ti>() == 1);
  }

  SUBCASE("by ref")
  {
    auto &i = s.get<ti>();
    i = 2;
    CHECK(s.get<ti>() == 2);
    s.get<ti>() = 3;
    CHECK(i == 3);
    CHECK(s.get<ti>() == 3);
  }
}

TEST_CASE("assign")
{
  auto s = make_state<ti, ts, tv>(1, "foo"s, V3i(3));
  s.get<ti>() = 2;
  s.get<ts>() = "bar";
  s.get<tv>() = V3i(1, 2, 3);

  CHECK(s.get<ti>() == 2);
  CHECK(s.get<ts>() == "bar");
  CHECK(s.get<tv>() == V3i(1, 2, 3));
}

TEST_CASE("assign_ref")
{
  int i = 0;
  auto str = ""s;
  V3i v(0);
  auto s = make_state<ti, ts, tv>(i, str, v);

  s.get<ti>() = 1;
  s.get<ts>() = "foo";
  s.get<tv>() = V3i(2, 3, 4);
  CHECK(i == 1);
  CHECK(str == "foo");
  CHECK(v == V3i(2, 3, 4));
}

TEST_CASE("copy")
{
  auto s = make_state<ti, ts, tv>(1, "bar"s, V3i(2, 3, 4));

  int i = s.get<ti>();
  auto j = s.get<ti>();
  auto k = s.copy<ti>();
  //int &r = s.copy<ti>(); // COMPILE ERROR: non-const ref to a temporary obj
  //int &r = s.get<ti>(); // OK, but it's not a copy
  CHECK(i == 1);
  CHECK(j == 1);
  CHECK(k == 1);
  i = 2; j = i + 1; k = j + 1;
  CHECK(s.get<ti>() == 1);

//  s has type QState<ti, ts, tv> thus all these variants are make copies
  auto c1 = s;
  auto c2 = s.copy();
  QState<ti, ts, tv> c3 = s;
  CHECK(c1 == s);
  CHECK(c2 == s);
  CHECK(c3 == s);
  c1.get<ti>() = 2;
  CHECK(s.get<ti>() == 1);
  c2.get<ti>() = 3;
  CHECK(s.get<ti>() == 1);
  c3.get<ti>() = 4;
  CHECK(s.get<ti>() == 1);

  // copy of a temporary object --> OK
  auto c4 = make_state<ti, ts, tv>(1, "foo"s, V3i(2, 3, 4)).copy();
  CHECK(c4.get<ti>() == 1);
  CHECK(c4.get<ts>() == "foo");
  CHECK(c4.get<tv>() == V3i(2, 3, 4));
}

TEST_CASE("copy_ref")
{
  int a = 1;
  auto str = "bar"s;
  V3i v = V3i(2, 3, 4);
  auto s = make_state<ti, ts, tv>(a, str, v);

  int i = s.get<ti>();
  auto j = s.get<ti>();
  auto k = s.copy<ti>();
  //auto &r = s.copy<ti>(); // COMPILE ERROR: non-const ref to a temporary
  //auto &r = s.get<ti>(); // OK, but it's not a copy
  CHECK(i == 1);
  CHECK(j == 1);
  CHECK(k == 1);
  i = 2; j = i + 1; k = j + 1;
  CHECK(s.get<ti>() == 1);

//  s has type QState<ti&, ts&, tv&> thus the variants below are NOT the same
  auto c1 = s; // contains references to a, str and v, not copies!!!
  auto c2 = s.copy(); // to get real copies you need to be more specific, this way
  QState<ti, ts, tv> c3 = s; // or that way

  CHECK(c1 == s);
  CHECK(c2 == s);
  CHECK(c3 == s);
  c1.get<ti>() = 7;
  CHECK(a == 7);
  c2.get<ti>() = 8;
  CHECK(a == 7);
  c3.get<ti>() = 9;
  CHECK(a == 7);

  // copy of a temporary object --> OK
  auto c4 = make_state<ti, ts, tv>(a, str, v).copy();
  CHECK(c4.get<ti>() == a);
  CHECK(c4.get<ts>() == str);
  CHECK(c4.get<tv>() == v);
}

TEST_CASE("ref")
{
  auto s = make_state<ti, ts, tv>(1, "foo"s, V3i(3));

  auto &r1 = s;
  auto r2 = s.get();
  auto r3 = qdetails::get(s);
  CHECK(r1 == s);
  CHECK(r2 == s);
  CHECK(r3 == s);
  r1.get<ti>() = 2;
  CHECK(s.get<ti>() == 2);
  r2.get<ts>() = "bar";
  CHECK(s.get<ts>() == "bar");
  r3.get<tv>() = V3i(1);
  CHECK(s.get<tv>() == V3i(1));

  // COMPILE ERROR: reference to a temporary is not a good idea!
  //auto &r = make_state<ti, ts, tv>(1, "foo"s, V3i(3));
  //auto r = make_state<ti, ts, tv>(1, "foo"s, V3i(3)).get();
  //auto r = qdetails::get(make_state<ti, ts, tv>(1, "foo"s, V3i(3)));

  // it's easy for one component
  int i = make_state<ti, ts, tv>(1, "foo"s, V3i(3)).get<ti>();
  CHECK(i == 1);
}

TEST_CASE("ref_ref")
{
  int i = 1;
  auto str = "foo"s;
  V3i v = V3i(2, 3, 4);
  auto s = make_state<ti, ts, tv>(i, str, v);

  auto &r1 = s;
  auto r2 = s.get();
  auto r3 = qdetails::get(s);
  CHECK(r1 == s);
  CHECK(r2 == s);
  CHECK(r3 == s);
  r1.get<ti>() = 2;
  CHECK(i == 2);
  r2.get<ts>() = "bar";
  CHECK(str == "bar");
  r3.get<tv>() = V3i(1);
  CHECK(s.get<tv>() == V3i(1));

  // COMPILE ERROR: for the sake of consistency!
  // In case when arguments are not temporary, it could be possible to create state
  // using get() method or function, but... Consistency, CARL! Seriously, be consistent!
  //auto &r = make_state<ti, ts, tv>(i, str, v);
  //auto r = make_state<ti, ts, tv>(i, str, v).get();
  //auto r = qdetails::get(make_state<ti, ts, tv>(i, str, v));

  // it is easier for one component
  make_state<ti, ts, tv>(i, str, v).get<ti>() = 2;
  CHECK(i == 2);
  // but not for several
  //make_state<ti, ts, tv>(i, str, v).get<ti, tv>() = 3;
}

TEST_CASE("slice_copy")
{
  auto s = make_state<ti, ts, tv>(1, "foo"s, V3i(3));

  auto c1 = s.copy<tv, ti>();
  QState<tv, ti> c2 = s;
  CHECK(c1 == s);
  CHECK(c2 == s);
  c1.get<ti>() = 2;
  c2.get<ti>() = 3;
  CHECK(s.get<ti>() == 1);

  // get copies of the temporary objs --> OK!
  // clang doesn't like this...
//  auto c = make_state<ti, ts, td>(1, "bar", 2).copy<ti, td>();
//  c.get<ti>() = 3;
//  CHECK(c.get<ti>() == 3);

  // get references to the temporary objs --> NOT OK!
  // COMPILE ERROR: Seriously, do not use references to the temporary objects!
//  auto s4 = make_state<ti, ts, td>(1, "bar", 2).get<ti, td>();
}

TEST_CASE("slice_ref")
{
  int i = 1;
  auto str = ""s;
  V3i v = V3i(2, 3, 4);
  auto s = make_state<ti, ts, tv>(i, str, v);

  auto s1 = s.get<ti, ts>();
  QState<ti&, ts&> s2 = s;
  CHECK(s1 == s);
  CHECK(s2 == s);
  s1.get<ts>() = "foo";
  CHECK(str == "foo");
  s2.get<ts>() = "bar";
  CHECK(str == "bar");

  // get references to the real objects using temporary state --> Well... OK! But later!
//  auto s3 = make_state<ti, ts, td>(i, str, v).get<ti, td>();
//  s3.get<ti>() = 3;
//  CHECK(i == 3);

  // get copies of the real objs pointed by references using temporary state --> OK!
  // Copies are always OK
  auto s3 = make_state<ti, ts, tv>(i, str, v).copy<ti, tv>();
  CHECK(s3.get<ti>() == i);
  s3.get<ti>() = 2;
  CHECK(i == 1);
}

using st = QState<ti, tv>;

st func_copy(st s) { s = 2; return s; }
void func_ref(st &s) { s = 3; }
st func_cref(const st &s) { st sc = s; return sc; }

TEST_CASE("func")
{
  st s1; s1 = 1;
  st s2 = func_copy(s1);
  st s; s = 1;
  CHECK((s1 == s));
  s = 2;
  CHECK((s2 == s));

  func_ref(s2);
  s = 3;
  CHECK((s2 == s));

  st s3 = func_cref(s2);
  CHECK((s3 == s2));
}

template<class... Ts, class A> decltype(auto) copy_t(size_t i, A &a)
{
  return a[i].template copy<Ts...>();
}

template<class... Ts, class A> decltype(auto) get_t(size_t i, A &a)
{
  return a[i].template get<Ts...>();
}

TEST_CASE("templates")
{
  std::array<st, 3> arr;
  arr[0] = 1;
  arr[1] = 2;
  arr[2] = 3;

  SUBCASE("copy")
  {
    decltype(auto) s1 = copy_t(0, arr);
    CHECK(s1 == arr[0]);

    decltype(auto) s2 = copy_t<ti>(1, arr);
    CHECK(s2 == 2);

    decltype(auto) s3 = copy_t<tv>(2, arr);
    CHECK(s3 == V3i(3));
  }

  SUBCASE("get")
  {
    decltype(auto) s1 = get_t(0, arr);
    CHECK(s1 == arr[0]);

    get_t<ti>(1, arr) = 4;
    CHECK(arr[1].get<ti>() == 4);

    get_t<tv>(2, arr) = V3i(5);
    CHECK(arr[2].get<tv>() == V3i(5));
  }
}


constexpr auto sc1 = make_state<ti, td>(1, 2.);
constexpr auto sc2 = make_state<td, ti>(2., 1);
static_assert(sc1 == sc2, "oops"); // fuck yeah!!!!

constexpr auto sc1_plus_sc2 = make_state<ti, td>(2, 4.);
static_assert(sc1 + sc2 == sc1_plus_sc2, "heck!");

constexpr auto sc1_mult_2 = make_state<ti, td>(2, 4.);
static_assert(sc1*2 == sc1_mult_2, "feck!");

constexpr auto sc3 = make_state<ti, td>(2, 4.);
constexpr auto sc3_div_2 = make_state<ti, td>(1, 2.);
static_assert(sc3/2 == sc3_div_2, "fuck!");



TEST_CASE("boolean")
{
  auto s1 = make_state<ti, ts, tv>(1, "foo"s, V3i(3));
  auto s2 = make_state<tv, ti, ts, td>(V3i(3), 1, "foo"s, 2.);
  auto s3 = make_state<ti, ts, tv>(2, "bar"s, V3i(1));

  CHECK(s1 == s1);
  CHECK(s1 == s2);
  //CHECK(s2 == s1); //<--- COMPILE ERROR!!!
  CHECK(s1 != s3);
}

TEST_CASE("arithmetic")
{
  auto s1 = make_state<ti, tv>(1, V3i(3, 5, 7));
  auto s2 = make_state<tv, ti>(V3i(2, 4, 6), 4);
  auto s3 = make_state<ti, td, tv>(1, 2., V3i(3, 4, 5));

//  auto s4 = s2 + s3;
//  auto s5 = s3 + s2; //<--- COMPILE ERROR!!!

  auto s1_mult_by_2 = make_state<ti, tv>(2, V3i(6, 10, 14));
  CHECK(2*s1 == s1_mult_by_2);
  CHECK(s1*2 == s1_mult_by_2);

  auto s2_div_by_2 = make_state<tv, ti>(V3i(1, 2, 3), 2);
  CHECK(s2/2 == s2_div_by_2);

  auto s1_plus_s2 = make_state<ti, tv>(5, V3i(5, 9, 13));
  auto s2_plus_s1 = make_state<tv, ti>(V3i(5, 9, 13), 5);
  CHECK(s1 + s2 == s1_plus_s2);
  CHECK(s2 + s1 == s2_plus_s1);

  auto s1_minus_s2 = make_state<ti, tv>(-3, V3i(1, 1, 1));
  CHECK(s1 - s2 == s1_minus_s2);

  s1 *= 2;
  CHECK(s1 == s1_mult_by_2);

  s2 /= 2;
  CHECK(s2 == s2_div_by_2);

  auto s2_plus_s3 = make_state<tv, ti>(V3i(4, 6, 8), 3);
  s2 += s3;
  CHECK(s2 == s2_plus_s3);

  auto s1_equal_2 = make_state<tv, ti>(V3i(2), 2);
  s1 = 2;
  CHECK(s1 == s1_equal_2);
}

TEST_CASE("arithmetic_ref")
{
  int i1 = 1, i2 = 2, i3 = 3;
  double d1 = 4, d2 = 5, d3 = 6;
  V3i v1(7), v2(8), v3(9);

  auto s1 = make_state<ti, tv>(i1, v1);
  auto s2 = make_state<tv, ti>(v2, i2);
  auto s3 = make_state<ti, td, tv>(i3, d3, v3);

  auto s4 = s2 + s3;
  CHECK(v2 == V3i(8));
  CHECK(i2 == 2);
  CHECK(i3 == 3);
  CHECK(d3 == 6);
  CHECK(v3 == V3i(9));
//  auto s5 = s3 + s2; //<--- COMPILE ERROR!!!

  s1 *= 2;
  CHECK(i1 == 2);
  CHECK(v1 == V3i(14));

  s2 /= 2;
  CHECK(v2 == V3i(4));
  CHECK(i2 == 1);

  s1 -= s2;
  CHECK(i1 == 1);
  CHECK(v1 == V3i(10));

  s2 += s3;
  CHECK(v2 == V3i(13));
  CHECK(i2 == 4);
}

TEST_CASE("io")
{
  auto s1 = make_state<ti, tv, ts>(1, V3i(1, 2, 3), "foo"s);
  std::stringstream sstr;
  sstr << s1;
  CHECK(sstr.str() == "1 (1, 2, 3) foo "); // NB! space at end

  QState<ti, tv, ts> s2;
  sstr >> s2;
  CHECK(s1 == s2);
}

TEST_CASE("array")
{
  using s = QState<ti, ts, tv>;
  std::array<s, 3> arr;
  arr[0] = make_state<ti, ts, tv>(1, "2"s, V3i(3));

  auto s1 = make_state<ti, ts, tv>(1, "foo"s, V3i(1, 2, 3));
  arr[1] = s1;
  CHECK(arr[1] == s1);

  s1.get<ti>() = 4;
  CHECK(arr[1].get<ti>() == 1);

  std::vector<s> vs;
  vs.push_back(s1);
  CHECK(vs[0] == s1);

  s1.get<ti>() = 5;
  CHECK(vs[0].get<ti>() == 4);
}

template<class F, class S> static constexpr bool is_same_v = std::is_same<F, S>::value;

TEST_CASE("qdetails::get(not_a_state)")
{
  struct S {int i; std::string s; V3i v; };
  S s{1, "2", V3i(3)};
  static_assert(is_same_v<std::decay_t<decltype(qdetails::get(s))>, S>, "HECK!");
  static_assert(is_same_v<std::decay_t<decltype(qdetails::get<ti>(s))>, S>, "HECK!");
  static_assert(is_same_v<std::decay_t<decltype(qdetails::get<ti, ts>(s))>, S>, "HECK!");

  auto sc = qdetails::get(s); // copy
  sc.i = 4; sc.s = "5"; sc.v = V3i(6);
  CHECK(s.i == 1);
  CHECK(s.s == "2");
  CHECK(s.v == V3i(3));

  auto &sr = qdetails::get(s); // ref (watch for type)
  sr.i = 4; sr.s = "5"; sr.v = V3i(6);
  CHECK(s.i == 4);
  CHECK(s.s == "5");
  CHECK(s.v == V3i(6));
}

TEST_CASE("qdetails::get(state)")
{
  // strings are strange. remove s and you'll see...
  auto a = make_state<ti, ts, tv>(1, "2"s, V3i(3));
  CHECK(qdetails::get(a) == a);

  CHECK(qdetails::get<ti>(a) == 1);
  CHECK(qdetails::get<ts>(a) == "2");
  CHECK(qdetails::get<tv>(a) == V3i(3));

  auto b = qdetails::get<ts, ti>(a);
  CHECK(qdetails::get<ts>(b) == qdetails::get<ts>(a));
  CHECK(qdetails::get<ti>(a) == qdetails::get<ti>(a));
  b.get<ts>() = "4";
  b.get<ti>() = 5;
  CHECK(qdetails::get<ti>(a) == 5);
  CHECK(qdetails::get<ts>(a) == "4");

//  auto c = qdetails::get<ti, td>(make_state<td, ti, tv>(1, 2, V3i(3)));
  auto c1 = qdetails::get<ti, ts>(a);
}

//TEST_SUITE_END();
