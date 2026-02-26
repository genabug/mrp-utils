#include "utils/QState.h"
#include "utils/QTraits.h"
#include "utils/Vector.h"

#include <gtest/gtest.h>
#include <string>

using namespace Quantities;
using namespace std::string_literals;

using V3i = Vector<3, int>;

using ti_t = QTraits<int, 3, 1, 't','i'>;
using td_t = QTraits<double, 3, 1, 't','d'>;
using ts_t = QTraits<std::string, 0, 0, 't','s'>;
using tv_t = QTraits<V3i, 3, 3, 't','v'>;

static constexpr ti_t ti;
static constexpr td_t td;
static constexpr ts_t ts;
static constexpr tv_t tv;

// constexpr auto sc1 = make_state<ti_t, td_t>(1, 2.);
// constexpr auto sc2 = make_state<td_t, ti_t>(2., 1);
// static_assert(sc1 == sc2, "oops"); // fuck yeah!!!!

// constexpr auto sc1_plus_sc2 = make_state<ti_t, td_t>(2, 4.);
// static_assert(sc1 + sc2 == sc1_plus_sc2, "heck!");

// constexpr auto sc1_mult_2 = make_state<ti_t, td_t>(2, 4.);
// static_assert(sc1*2 == sc1_mult_2, "feck!");

// constexpr auto sc3 = make_state<ti_t, td_t>(2, 4.);
// constexpr auto sc3_div_2 = make_state<ti_t, td_t>(1, 2.);
// static_assert(sc3/2 == sc3_div_2, "fuck!");

TEST(State, with_vector_comps_compiled)
{
  using s = QState<ti_t, td_t, tv_t>;
  Vector<3, s> v;
}

// assumed index access works
TEST(State, init)
{
  QState<ti_t, ts_t, tv_t> s2(5, "bar"s, V3i(6, 7, 8));
  EXPECT_EQ(s2[ti], 5);
  EXPECT_EQ(s2[ts], "bar");
  EXPECT_EQ(s2[tv], V3i(6, 7, 8));
}

TEST(State, init_by_lvalue_refs)
{
  int i = 1;
  auto str = "bar"s;
  V3i v(2, 3, 4);
  QState<ti_t, ts_t, tv_t> s(i, str, v);
  EXPECT_EQ(s[ti], i);
  EXPECT_EQ(s[ts], str);
  EXPECT_EQ(s[tv], v);
}

TEST(State, copy_ctor)
{
  QState<ti_t, ts_t, tv_t> s(1, "foo"s, V3i(2, 3, 4));
  auto sc = s;
  EXPECT_EQ(sc, s);
}

TEST(State, access_by_index)
{
  QState<ti_t, ts_t, tv_t> s(1, "foo"s, V3i(3));
  EXPECT_EQ(s.get<0>(), 1);
  EXPECT_EQ(s.get<1>(), "foo");
  EXPECT_EQ(s.get<2>(), V3i(3));
}

TEST(State, access_by_type)
{
  QState<ti_t, ts_t, tv_t> s(1, "foo"s, V3i(3));
  EXPECT_EQ(s.get<ti_t>(), 1);
  EXPECT_EQ(s.get<ts_t>(), "foo");
  EXPECT_EQ(s.get<tv_t>(), V3i(3));
}

TEST(State, access_by_variable_index)
{
  QState<ti_t, ts_t, tv_t> s(1, "foo"s, V3i(3));
  EXPECT_EQ(s[ti], 1);
  EXPECT_EQ(s[ts], "foo");
  EXPECT_EQ(s[tv], V3i(3));
}

TEST(State, assign)
{
  int i = 1;
  auto str = "foo"s;
  V3i v(2, 3, 4);
  QState<ti_t, ts_t, tv_t> s(i, str, v);
  EXPECT_EQ(s[ti], i);
  EXPECT_EQ(s[ts], str);
  EXPECT_EQ(s[tv], v);
}

TEST(State, compare)
{
  double d = 1;
  int i = 1, i3 = 2;
  auto str = "foo"s;
  V3i v(3, 4, 5);

  QState<ti_t, ts_t, tv_t> s1(i, str, v);
  QState<tv_t, ti_t, ts_t, td_t> s2(v, i, str, d);
  QState<ti_t, ts_t, tv_t> s3(i3, str, v);

  EXPECT_EQ(s1, s1);
  EXPECT_EQ(s1, s2);
  //EXPECT_EQ(s2, s1); //<--- COMPILE ERROR!!!
  EXPECT_NE(s1, s3);
}

TEST(State, get_function_full)
{
  QState<ti_t, tv_t> s(1, V3i(2, 3, 4));
  auto sc = get(s);
  EXPECT_EQ(sc, s);
}

TEST(State, get_function_uno)
{
  int i = 1;
  QState<ti_t, tv_t> s(i, V3i(2, 3, 4));
  auto ic = get<ti_t>(s);
  EXPECT_EQ(ic, i);
}

TEST(State, get_function_slice)
{
  int i = 1;
  V3i v(2, 3, 4);
  QState<ti_t, ts_t, tv_t> s(i, "foo"s, v);
  auto ss = get<ti_t, tv_t>(s);
  EXPECT_EQ(ss, s);
}

void func_id(State auto &s)
{
  s.template get<ti_t>() = 1;
  s.template get<td_t>() = 2;
}

using st_idv = QState<ti_t, td_t, tv_t>;

st_idv func_copy(st_idv s) { s = 2; return s; }
void func_ref(st_idv &s) { s = 3; }
st_idv func_cref(const st_idv &s) { st_idv sc = s; return sc; }

TEST(State, func)
{
  st_idv s1; s1 = 1;
  st_idv s2 = func_copy(s1);
  st_idv s; s = 1;
  EXPECT_EQ(s1, s);
  s = 2;
  EXPECT_EQ(s2, s);

  func_ref(s2);
  s = 3;
  EXPECT_EQ(s2, s);

  st_idv s3 = func_cref(s2);
  EXPECT_EQ(s3, s2);

  func_id(s3);
  EXPECT_EQ(s3.get<ti_t>(), 1);
  EXPECT_EQ(s3.get<td_t>(), 2.0);
}

TEST(State, arithmetic)
{
  QState<ti_t, tv_t> s1(1, V3i(3, 5, 7));
  QState<tv_t, ti_t> s2(V3i(2, 4, 6), 4);
  QState<ti_t, td_t, tv_t> s3(1, 2., V3i(3, 4, 5));

  //auto s4 = s2 + s3; // OK
  //auto s5 = s3 + s2; // COMPILE ERROR!

  QState<ti_t, tv_t> s1_mult_by_2(2, V3i(6, 10, 14));
  EXPECT_EQ(2*s1, s1_mult_by_2);
  EXPECT_EQ(s1*2, s1_mult_by_2);

  QState<tv_t, ti_t> s2_div_by_2(V3i(1, 2, 3), 2);
  EXPECT_EQ(s2/2, s2_div_by_2);

  QState<ti_t, tv_t> s1_plus_s2(5, V3i(5, 9, 13));
  QState<tv_t, ti_t> s2_plus_s1(V3i(5, 9, 13), 5);
  EXPECT_EQ(s1 + s2, s1_plus_s2);
  EXPECT_EQ(s2 + s1, s2_plus_s1);

  QState<ti_t, tv_t> s1_minus_s2(-3, V3i(1, 1, 1));
  EXPECT_EQ(s1 - s2, s1_minus_s2);

  s1 *= 2;
  EXPECT_EQ(s1, s1_mult_by_2);

  s2 /= 2;
  EXPECT_EQ(s2, s2_div_by_2);

  QState<tv_t, ti_t> s2_plus_s3(V3i(4, 6, 8), 3);
  s2 += s3;
  EXPECT_EQ(s2, s2_plus_s3);

  QState<tv_t, ti_t> s1_equal_2(V3i(2), 2);
  s1 = 2;
  EXPECT_EQ(s1, s1_equal_2);
}

TEST(State, IO)
{
  QState<ti_t, tv_t, ts_t> s1(1, V3i(2, 3, 4), "foo"s);
  std::stringstream sstr;
  sstr << s1;
  EXPECT_EQ(sstr.str(), "1 (2, 3, 4) foo");

  QState<ti_t, tv_t, ts_t> s2;
  sstr >> s2;
  EXPECT_EQ(s1, s2);
}
