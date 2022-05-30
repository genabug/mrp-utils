#include "doctest.h"

#include "utils/QState.h"
#include "utils/QTraits.h"
#include "utils/Vector.h"
#include <string>
#include <array>

using namespace Quantities;
using namespace std::string_literals;
namespace qdetails = Quantities::details;

using V3i = Vector<3, int>;

using ti_t = QTraits<int, 3, 1, 't','i'>;
using td_t = QTraits<double, 3, 1, 't','d'>;
using ts_t = QTraits<std::string, 0, 0, 't','s'>;
using tv_t = QTraits<V3i, 3, 3, 't','v'>;

static constexpr ti_t ti;
static constexpr td_t td;
static constexpr ts_t ts;
static constexpr tv_t tv;

template<class T> void WTF(T);


constexpr auto sc1 = make_state<ti_t, td_t>(1, 2.);
constexpr auto sc2 = make_state<td_t, ti_t>(2., 1);
//static_assert(sc1 == sc2, "oops"); // fuck yeah!!!!

constexpr auto sc1_plus_sc2 = make_state<ti_t, td_t>(2, 4.);
//static_assert(sc1 + sc2 == sc1_plus_sc2, "heck!");

constexpr auto sc1_mult_2 = make_state<ti_t, td_t>(2, 4.);
//static_assert(sc1*2 == sc1_mult_2, "feck!");

constexpr auto sc3 = make_state<ti_t, td_t>(2, 4.);
constexpr auto sc3_div_2 = make_state<ti_t, td_t>(1, 2.);
//static_assert(sc3/2 == sc3_div_2, "fuck!");

TEST_CASE("vector")
{
  using s = QState<ti_t, td_t, tv_t>;
  Vector<3, s> v;
}

TEST_CASE("init")
{
  // assume that access by index works

  SUBCASE("by rvalues") // i.e. store the copies
  {
    auto s1 = make_state<ti_t, ts_t, tv_t>(1, "foo"s, V3i(2, 3, 4));
    CHECK(s1.get<0>() == 1);
    CHECK(s1.get<1>() == "foo");
    CHECK(s1.get<2>() == V3i(2, 3, 4));

    // explicit way to make a state of copies
    QState<ti_t, ts_t, tv_t> s2(5, "bar"s, V3i(6, 7, 8));
    CHECK(s2.get<0>() == 5);
    CHECK(s2.get<1>() == "bar");
    CHECK(s2.get<2>() == V3i(6, 7, 8));
  }

  SUBCASE("by lvalues") // i.e. store the references
  {
    int i = 1;
    auto str = "bar"s;
    V3i v(2, 3, 4);
    auto s = make_state<ti_t, ts_t, tv_t>(i, str, v);
    CHECK(s.get<0>() == i);
    CHECK(s.get<1>() == str);
    CHECK(s.get<2>() == v);

    i = 2;
    str = "baz";
    v = V3i(5, 6, 7);
    CHECK(s.get<0>() == i);
    CHECK(s.get<1>() == str);
    CHECK(s.get<2>() == v);

    // explicit way to make a state of references
    // NOTE! without amps it means copy!
    QState<ti_t&, ts_t&, tv_t&> ss(i, str, v);
    s.get<0>() = 3;
    s.get<1>() = "foo";
    s.get<2>() = V3i(1);
    CHECK(i == 3);
    CHECK(str == "foo");
    CHECK(v == V3i(1));
  }

  SUBCASE("by const lvalues") // i.e. store the copies
  {
    const int ci = 5;
    const auto cstr = "baz"s;
    const auto cv = V3i(6, 7, 8);
    auto s = make_state<ti_t, ts_t, tv_t>(ci, cstr, cv);
    CHECK(s.get<0>() == ci);
    CHECK(s.get<1>() == cstr);
    CHECK(s.get<2>() == cv);

    // explicit way to make a state of copies
    // NOTE! amps in template arguments is not allowed here (COMPILE ERROR)
    QState<ti_t, ts_t, tv_t> s2(ci, cstr, cv);
    CHECK(s.get<0>() == ci);
    CHECK(s.get<1>() == cstr);
    CHECK(s.get<2>() == cv);
  }
}


TEST_CASE("copy ctor")
{
  int i = 1;
  auto str = "foo"s;
  V3i v(2, 3, 4);
  auto sr = make_state<ti_t, ts_t, tv_t>(i, str, v);
  auto sc = make_state<ti_t, ts_t, tv_t>(5, "bar"s, V3i(6, 7, 8));

  SUBCASE("auto")
  {
    auto cc = sc; // sc : QState<ti, ts, tv> thus auto is OK to make a copy
    auto cr = sr; // sr : QState<ti&, ts&, tv&> thus it's NOT a copy, it's a duplicate
    CHECK(cc == sc);
    CHECK(cr == sr);
    cc[ti] = 9;
    CHECK(sc[ti] == 5);
    cr[ti] = 9;
    CHECK(sr[ti] == 9);
    sr[ti] = 1;
  }

  SUBCASE("explicit")
  {
    QState<ti_t, ts_t, tv_t> cc = sc;
    QState<ti_t, ts_t, tv_t> cr = sr;
    CHECK(cc == sc);
    CHECK(cr == sr);
    cc[ti] = 2;
    CHECK(sc[ti] == 5);
    cr[ti] = 4;
    CHECK(sr[ti] == 1);
  }
}


TEST_CASE("access")
{
  auto s = make_state<ti_t, ts_t, tv_t>(1, "foo"s, V3i(3));

  SUBCASE("by index")
  {
    CHECK(s.get<0>() == 1);
    CHECK(s.get<1>() == "foo");
    CHECK(s.get<2>() == V3i(3));
  }

  SUBCASE("by type")
  {
    CHECK(s.get<ti_t>() == 1);
    CHECK(s.get<ts_t>() == "foo");
    CHECK(s.get<tv_t>() == V3i(3));
  }

  SUBCASE("by var/[]")
  {
    CHECK(s[ti] == 1);
    CHECK(s[ts] == "foo");
    CHECK(s[tv] == V3i(3));
  }
}


TEST_CASE("assign")
{
  SUBCASE("for rvalues")
  {
    auto s = make_state<ti_t, ts_t, tv_t>(1, "foo"s, V3i(3));
    s[ti] = 2;
    s[ts] = "baz";
    s[tv] = V3i(1, 2, 3);
    CHECK(s[ti] == 2);
    CHECK(s[ts] == "baz");
    CHECK(s[tv] == V3i(1, 2, 3));
  }

  SUBCASE("for lvalues")
  {
    int i = 0;
    auto str = ""s;
    V3i v(0);
    auto s = make_state<ti_t, ts_t, tv_t>(i, str, v);
    s[ti] = 1;
    s[ts] = "bar";
    s[tv] = V3i(1, 2, 3);
    CHECK(s[ti] == i);
    CHECK(s[ts] == str);
    CHECK(s[tv] == v);
  }

  SUBCASE("for const lvalues")
  {
    const int ci = 3;
    const auto cstr = "boo"s;
    const V3i cv(2);
    auto s = make_state<ti_t, ts_t, tv_t>(ci, cstr, cv);
    // COMPILE ERROR! Assign to read-only
//    s[ti] = 4;
//    s[ts] = "fac";
//    s[tv] = V3i(3, 2, 1);
  }
}


TEST_CASE("compare")
{
  SUBCASE("rvalues")
  {
    auto s1 = make_state<ti_t, ts_t, tv_t>(1, "foo"s, V3i(3));
    auto s2 = make_state<tv_t, ti_t, ts_t, td_t>(V3i(3), 1, "foo"s, 2.);
    auto s3 = make_state<ti_t, ts_t, tv_t>(2, "bar"s, V3i(1));

    CHECK(s1 == s1);
    CHECK(s1 == s2);
//    CHECK(s2 == s1); //<--- COMPILE ERROR!!!
    CHECK(s1 != s3);
  }

  SUBCASE("lvalues")
  {
    double d = 1;
    int i = 1, i3 = 2;
    auto str = "foo"s;
    V3i v(3, 4, 5);
    auto s1 = make_state<ti_t, ts_t, tv_t>(i, str, v);
    auto s2 = make_state<tv_t, ti_t, ts_t, td_t>(v, i, str, d);
    auto s3 = make_state<ti_t, ts_t, tv_t>(i3, str, v);

    CHECK(s1 == s1);
    CHECK(s1 == s2);
//    CHECK(s2 == s1); //<--- COMPILE ERROR!!!
    CHECK(s1 != s3);
  }
}


TEST_CASE("copy method")
{
  int i = 1;
  auto str = "foo"s;
  V3i v(2, 3, 4);
  auto sr = make_state<ti_t, ts_t, tv_t>(i, str, v);
  auto sc = make_state<ti_t, ts_t, tv_t>(5, "bar"s, V3i(6, 7, 8));

  SUBCASE("full")
  {
    auto cc = sc.copy();
    auto cr = sr.copy();
    CHECK(cc == sc);
    CHECK(cr == sr);
    cc[ti] = cr[ti] = 9;
    CHECK(sc[ti] == 5);
    CHECK(sr[ti] == 1);
  }

  SUBCASE("uno/type")
  {
    auto cc = sc.copy<ti_t>();
    auto cr = sr.copy<ti_t>();
    cc = cr = 9;
    CHECK(sc[ti] == 5);
    CHECK(sr[ti] == 1);
  }

  SUBCASE("uno/var")
  {
    auto cc = sc.copy(ti);
    auto cr = sr.copy(ti);
    cc = cr = 9;
    CHECK(sc[ti] == 5);
    CHECK(sr[ti] == 1);
  }

  SUBCASE("slice/type")
  {
    auto cc = sc.copy<ti_t, ts_t>();
    auto cr = sr.copy<ti_t, tv_t>();
    cc[ts] = "baz";
    cr[tv] = V3i(0);
    CHECK(sc[ts] == "bar");
    CHECK(sr[tv] == v);
  }

  SUBCASE("slice/var")
  {
    auto cc = sc.copy(ti, ts);
    auto cr = sr.copy(ti, tv);
    cc[ts] = "baz";
    cr[tv] = V3i(0);
    CHECK(sc[ts] == "bar");
    CHECK(sr[tv] == v);
  }
}


TEST_CASE("copy function")
{
  int i = 1;
  auto str = "foo"s;
  V3i v(2, 3, 4);
  auto sr = make_state<ti_t, ts_t, tv_t>(i, str, v);
  auto sc = make_state<ti_t, ts_t, tv_t>(5, "bar"s, V3i(6, 7, 8));

  SUBCASE("full")
  {
    auto cc = copy(sc);
    auto cr = copy(sr);
    CHECK(cc == sc);
    CHECK(cr == sr);
    cc[ti] = cr[ti] = 9;
    CHECK(sc[ti] == 5);
    CHECK(sr[ti] == 1);
  }

  SUBCASE("uno/type")
  {
    auto cc = copy<ti_t>(sc);
    auto cr = copy<ti_t>(sr);
    cc = cr = 9;
    CHECK(sc[ti] == 5);
    CHECK(sr[ti] == 1);
  }

  SUBCASE("partial/type")
  {
    auto cc = copy<ti_t, ts_t>(sc);
    auto cr = copy<ti_t, tv_t>(sr);
    cc[ts] = "baz";
    cr[tv] = V3i(0);
    CHECK(sc[ts] == "bar");
    CHECK(sr[tv] == v);
  }

  SUBCASE("not-a-state")
  {
    struct S {int i; std::string s; V3i v; };
    S s{1, "2", V3i(3)};

    auto sc = copy(s);
    sc.i = 4; sc.s = "5"; sc.v = V3i(6);
    CHECK(s.i == 1);
    CHECK(s.s == "2");
    CHECK(s.v == V3i(3));
  }
}


TEST_CASE("get method")
{
  int i = 1;
  auto str = "foo"s;
  V3i v(2, 3, 4);
  auto sr = make_state<ti_t, ts_t, tv_t>(i, str, v);
  auto sc = make_state<ti_t, ts_t, tv_t>(5, "bar"s, V3i(6, 7, 8));

  SUBCASE("full")
  {
    auto rc = sc.get();
    auto rr = sr.get();
    CHECK(rc == sc);
    CHECK(rr == sr);
    rc[ti] = 1;
    rr[ts] = "baz";
    CHECK(sc[ti] == 1);
    CHECK(sr[ts] == str);
  }

  SUBCASE("uno/type")
  {
    // NB! without amp these are copies!
    auto &rc = sc.get<ti_t>();
    auto &rr = sr.get<ts_t>();
    CHECK(rc == sc[ti]);
    CHECK(rr == sr[ts]);
    rc = 2;
    rr = "fuz";
    CHECK(sc[ti] == 2);
    CHECK(sr[ts] == "fuz");
  }

  SUBCASE("uno/var")
  {
    // NB! without amp these are copies!
    auto &rc = sc.get(ti);
    auto &rr = sr.get(ts);
    CHECK(rc == sc[ti]);
    CHECK(rr == sr[ts]);
    rc = 3;
    rr = "biz";
    CHECK(sc[ti] == 3);
    CHECK(sr[ts] == "biz");
  }

  SUBCASE("slice/type")
  {
    auto rc = sc.get<ti_t, ts_t>();
    auto rr = sr.get<ti_t, tv_t>();
    rc[ts] = "baz";
    rr[tv] = V3i(0);
    CHECK(sc[ts] == "baz");
    CHECK(sr[tv] == v);
  }

  SUBCASE("slice/var")
  {
    auto rc = sc.get(ti, ts);
    auto rr = sr.get(ti, tv);
    rc[ts] = "baz";
    rr[tv] = V3i(1);
    CHECK(sc[ts] == "baz");
    CHECK(sr[tv] == v);
  }
/*
  SUBCASE("slice/state")
  {
    using iv_s = QState<ti_t, tv_t>;
    sc.get<iv_s>()[ti] = 3;
    sr.get<iv_s>()[tv] = V3i(2);
    CHECK(sc[ti] == 3);
    CHECK(sr[tv] == v);
  }
*/
}


TEST_CASE("get function")
{
  int i = 1;
  auto str = "foo"s;
  V3i v(2, 3, 4);
  auto sr = make_state<ti_t, ts_t, tv_t>(i, str, v);
  auto sc = make_state<ti_t, ts_t, tv_t>(5, "bar"s, V3i(6, 7, 8));

  SUBCASE("full")
  {
    auto rc = get(sc);
    auto rr = get(sr);
    CHECK(rc == sc);
    CHECK(rr == sr);
    rc[ti] = 1;
    rr[ts] = "baz";
    CHECK(sc[ti] == 1);
    CHECK(sr[ts] == str);
  }

  SUBCASE("uno/type")
  {
    // NB! without amp these are copies!
    auto &rc = get<ti_t>(sc);
    auto &rr = get<ts_t>(sr);
    CHECK(rc == sc[ti]);
    CHECK(rr == sr[ts]);
    rc = 2;
    rr = "fuz";
    CHECK(sc[ti] == 2);
    CHECK(sr[ts] == "fuz");
  }

  SUBCASE("slice/type")
  {
    auto rc = sc.get<ti_t, ts_t>();
    auto rr = sr.get<ti_t, tv_t>();
    rc[ts] = "baz";
    rr[tv] = V3i(0);
    CHECK(sc[ts] == "baz");
    CHECK(sr[tv] == v);
  }

  SUBCASE("not-a-state")
  {
    struct S {int i; std::string s; V3i v; };
    S s{1, "2", V3i(3)};

    auto sc = get(s); // copy
    sc.i = 4; sc.s = "5"; sc.v = V3i(6);
    CHECK(s.i == 1);
    CHECK(s.s == "2");
    CHECK(s.v == V3i(3));

    auto &sr = get(s); // ref (look at type)
    sr.i = 4; sr.s = "5"; sr.v = V3i(6);
    CHECK(s.i == 4);
    CHECK(s.s == "5");
    CHECK(s.v == V3i(6));
  }
}


using st_idv = QState<ti_t, td_t, tv_t>;

st_idv func_copy(st_idv s) { s = 2; return s; }
void func_ref(st_idv &s) { s = 3; }
st_idv func_cref(const st_idv &s) { st_idv sc = s; return sc; }

template<class S> void func_id(S &s)
{
  s.template get<ti_t>() = 1;
  s.template get<td_t>() = 2;
}

TEST_CASE("func")
{
  st_idv s1; s1 = 1;
  st_idv s2 = func_copy(s1);
  st_idv s; s = 1;
  CHECK((s1 == s));
  s = 2;
  CHECK((s2 == s));

  func_ref(s2);
  s = 3;
  CHECK((s2 == s));

  st_idv s3 = func_cref(s2);
  CHECK((s3 == s2));

  func_id(s3);
  CHECK(s3.get<ti_t>() == 1);
  CHECK(s3.get<td_t>() == 2.0);
}


TEST_CASE("arithmetic")
{
  SUBCASE("rvalues")
  {
    auto s1 = make_state<ti_t, tv_t>(1, V3i(3, 5, 7));
    auto s2 = make_state<tv_t, ti_t>(V3i(2, 4, 6), 4);
    auto s3 = make_state<ti_t, td_t, tv_t>(1, 2., V3i(3, 4, 5));

//    auto s4 = s2 + s3; // OK
//    auto s5 = s3 + s2; // COMPILE ERROR!

    auto s1_mult_by_2 = make_state<ti_t, tv_t>(2, V3i(6, 10, 14));
    CHECK(2*s1 == s1_mult_by_2);
    CHECK(s1*2 == s1_mult_by_2);

    auto s2_div_by_2 = make_state<tv_t, ti_t>(V3i(1, 2, 3), 2);
    CHECK(s2/2 == s2_div_by_2);

    auto s1_plus_s2 = make_state<ti_t, tv_t>(5, V3i(5, 9, 13));
    auto s2_plus_s1 = make_state<tv_t, ti_t>(V3i(5, 9, 13), 5);
    CHECK(s1 + s2 == s1_plus_s2);
    CHECK(s2 + s1 == s2_plus_s1);

    auto s1_minus_s2 = make_state<ti_t, tv_t>(-3, V3i(1, 1, 1));
    CHECK(s1 - s2 == s1_minus_s2);

    s1 *= 2;
    CHECK(s1 == s1_mult_by_2);

    s2 /= 2;
    CHECK(s2 == s2_div_by_2);

    auto s2_plus_s3 = make_state<tv_t, ti_t>(V3i(4, 6, 8), 3);
    s2 += s3;
    CHECK(s2 == s2_plus_s3);

    auto s1_equal_2 = make_state<tv_t, ti_t>(V3i(2), 2);
    s1 = 2;
    CHECK(s1 == s1_equal_2);
  }

  SUBCASE("lvalues")
  {
    double d3 = 1;
    int i1 = 2, i2 = 4, i3 = 3;
    V3i v1(5), v2(6), v3(7);

    auto s1 = make_state<ti_t, tv_t>(i1, v1);
    auto s2 = make_state<tv_t, ti_t>(v2, i2);
    auto s3 = make_state<ti_t, td_t, tv_t>(i3, d3, v3);

//    auto s4 = s2 + s3; // OK
//    auto s5 = s3 + s2; // COMPILE ERROR!

    s1 *= 2;
    CHECK(i1 == 4);
    CHECK(v1 == V3i(10));

    s2 /= 2;
    CHECK(v2 == V3i(3));
    CHECK(i2 == 2);

    s1 -= s2;
    CHECK(i1 == 2);
    CHECK(v1 == V3i(7));

    s2 += s3;
    CHECK(v2 == V3i(10));
    CHECK(i2 == 5);
  }
}

TEST_CASE("io")
{
  auto s1 = make_state<ti_t, tv_t, ts_t>(1, V3i(1, 2, 3), "foo"s);
  std::stringstream sstr;
  sstr << s1;
  CHECK(sstr.str() == "1 (1, 2, 3) foo");

  QState<ti_t, tv_t, ts_t> s2;
  sstr >> s2;
  CHECK(s1 == s2);
}
