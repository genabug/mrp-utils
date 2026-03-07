#include "utils/QState.h"

#include <gtest/gtest.h>
#include <string>

using std::string;
using namespace Quantities;

using ti_t = QTraits<int, 3, "ti">;
using td_t = QTraits<double, 3, "td">;
using ts_t = QTraits<string, 0, "ts">;

constexpr ti_t ti;
constexpr td_t td;
constexpr ts_t ts;

// assumed index access works
// most of the operations are tested on compile-time
// but I'd like to have a good old one runtime tests
// for both debugging and as examples
TEST(State, init_with_rvalues)
{
  QState<ti_t, td_t, ts_t> s(1, 2, "bar");
  EXPECT_EQ(s[ti], 1);
  EXPECT_EQ(s[td], 2);
  EXPECT_EQ(s[ts], "bar");
}

TEST(State, init_with_lvalues)
{
  int i = 1;
  double d = 2;
  string str = "bar";
  QState<ti_t, td_t, ts_t> s(i, d, str);
  EXPECT_EQ(s[ti], i);
  EXPECT_EQ(s[td], d);
  EXPECT_EQ(s[ts], str);
}

TEST(State, copy_ctor)
{
  QState<ti_t, td_t, ts_t> s(1, 2, "foo");
  auto sc = s;
  EXPECT_EQ(sc, s);
}

TEST(State, access_by_index)
{
  QState<ti_t, td_t, ts_t> s(1, 2, "foo");
  EXPECT_EQ(s.get<0>(), 1);
  EXPECT_EQ(s.get<1>(), 2);
  EXPECT_EQ(s.get<2>(), "foo");
}

TEST(State, access_by_type)
{
  QState<ti_t, td_t, ts_t> s(1, 2, "foo");
  EXPECT_EQ(s.get<ti_t>(), 1);
  EXPECT_EQ(s.get<td_t>(), 2);
  EXPECT_EQ(s.get<ts_t>(), "foo");
}

TEST(State, access_by_variable)
{
  QState<ti_t, td_t, ts_t> s(1, 2, "foo");
  EXPECT_EQ(s[ti], 1);
  EXPECT_EQ(s[td], 2);
  EXPECT_EQ(s[ts], "foo");
}

TEST(State, assign_by_components)
{
  QState<ti_t, td_t, ts_t> s(1, 2, "foo");
  s[ti] = 3;
  s[td] = 4;
  s[ts] = "bar";
  EXPECT_EQ(s[ti], 3);
  EXPECT_EQ(s[td], 4);
  EXPECT_EQ(s[ts], "bar");
}

TEST(State, assign_all_components)
{
  QState<ti_t, td_t> s, expected(1, 1);
  s = 1;
  EXPECT_EQ(s, expected);
}

TEST(State, assign_state_with_same_order_of_comps)
{
  QState<ti_t, td_t, ts_t> s1, s2(1, 2, "foo");
  s1 = s2;
  EXPECT_EQ(s1[ti], s2[ti]);
  EXPECT_EQ(s1[td], s2[td]);
  EXPECT_EQ(s1[ts], s2[ts]);
}

TEST(State, assign_state_with_different_order_of_comps)
{
  QState<ti_t, td_t, ts_t> s1;
  QState<ts_t, td_t, ti_t> s2("bar", 3, 4);
  s1 = s2;
  EXPECT_EQ(s1[ti], s2[ti]);
  EXPECT_EQ(s1[td], s2[td]);
  EXPECT_EQ(s1[ts], s2[ts]);
}

TEST(State, assign_state_with_subset_of_comps)
{
  QState<ti_t, td_t> s1;
  QState<ts_t, td_t, ti_t> s2("baz", 5, 6);
  s1 = s2;
  EXPECT_EQ(s1[ti], s2[ti]);
  EXPECT_EQ(s1[td], s2[td]);
}

TEST(State, assign_sum_with_same_order_of_comps)
{
  QState<ti_t, td_t> s1(1, 2), s2(3, 4);
  s1 += s2;
  EXPECT_EQ(s1[ti], 4);
  EXPECT_EQ(s1[td], 6);
}

TEST(State, assign_sum_with_different_order_of_comps)
{
  QState<ti_t, td_t> s1(1, 2);
  QState<td_t, ti_t> s2(3, 4);
  s1 += s2;
  EXPECT_EQ(s1[ti], 5);
  EXPECT_EQ(s1[td], 5);
}

TEST(State, assign_sum_with_subset_of_comps)
{
  QState<ti_t, td_t> s1(1, 2);
  QState<ts_t, td_t, ti_t> s2("str", 3, 4);
  s1 += s2;
  EXPECT_EQ(s1[ti], 5);
  EXPECT_EQ(s1[td], 5);
}

TEST(State, assign_sub_with_same_order_of_comps)
{
  QState<ti_t, td_t> s1(3, 4), s2(1, 2);
  s1 -= s2;
  EXPECT_EQ(s1[ti], 2);
  EXPECT_EQ(s1[td], 2);
}

TEST(State, assign_sub_with_different_order_of_comps)
{
  QState<ti_t, td_t> s1(3, 4);
  QState<td_t, ti_t> s2(1, 2);
  s1 -= s2;
  EXPECT_EQ(s1[ti], 1);
  EXPECT_EQ(s1[td], 3);
}

TEST(State, assign_sub_with_subset_of_comps)
{
  QState<ti_t, td_t> s1(3, 4);
  QState<ts_t, td_t, ti_t> s2("str", 1, 2);
  s1 -= s2;
  EXPECT_EQ(s1[ti], 1);
  EXPECT_EQ(s1[td], 3);
}

TEST(State, assign_mult_state_by_coeff)
{
  QState<ti_t, td_t> s(1, 2);
  s *= 2;
  EXPECT_EQ(s[ti], 2);
  EXPECT_EQ(s[td], 4);
}

TEST(State, assign_div_state_by_coeff)
{
  QState<ti_t, td_t> s(4, 6);
  s /= 2;
  EXPECT_EQ(s[ti], 2);
  EXPECT_EQ(s[td], 3);
}

TEST(State, compare_states_with_same_order_of_comps)
{
  QState<ti_t, td_t, ts_t> s(1, 2, "foo");
  EXPECT_EQ(s, s);
}

TEST(State, compare_states_with_different_order_of_comps)
{
  int i = 1;
  double d = 2;
  string str = "foo";
  QState<ti_t, ts_t, td_t> s1(i, str, d);
  QState<td_t, ti_t, ts_t> s2(d, i, str);
  EXPECT_EQ(s1, s2);
}

TEST(State, compare_states_with_subset_of_comps)
{
  int i = 1;
  double d = 2;
  string str = "foo";
  QState<ti_t, ts_t> s1(i, str);
  QState<td_t, ti_t, ts_t> s2(d, i, str);
  EXPECT_EQ(s1, s2);
}

TEST(State, unary_plus_doesnot_change_state)
{
  QState<ti_t, td_t> s1(1, 2);
  auto sc = s1;
  auto s2 = s1;
  EXPECT_EQ(s1, sc);
  EXPECT_EQ(s1, s2);
}

TEST(State, unary_minus_doesnot_change_state)
{
  QState<ti_t, td_t> s1(1, 2);
  auto sc = s1;
  auto s2 = -s1;
  EXPECT_EQ(s1, sc);
  EXPECT_EQ(s2[ti], -1);
  EXPECT_EQ(s2[td], -2);
}

TEST(State, sum_states_with_same_order_of_comps)
{
  QState<ti_t, td_t> s1(1, 2), s2(3, 4), expected(4, 6);
  auto sum = s1 + s2;
  EXPECT_EQ(sum, expected);
}

TEST(State, sum_states_with_different_order_of_comps)
{
  QState<ti_t, td_t> s1(2, 3);
  QState<td_t, ti_t> s2(4, 5), expected(7, 7);
  auto sum = s1 + s2;
  EXPECT_EQ(sum, expected);
}

TEST(State, sum_states_with_subset_of_comps)
{
  QState<ti_t, td_t> s1(2, 1), expected(5, 5);
  QState<td_t, ti_t, ts_t> s2(4, 3, "foo");
  auto sum = s1 + s2;
  EXPECT_EQ(sum, expected);
}

TEST(State, sub_states_with_same_order_of_comps)
{
  QState<ti_t, td_t> s1(4, 3), s2(1, 2), expected(3, 1);
  auto sum = s1 - s2;
  EXPECT_EQ(sum, expected);
}

TEST(State, sub_states_with_different_order_of_comps)
{
  QState<ti_t, td_t> s1(3, 4);
  QState<td_t, ti_t> s2(1, 2), expected(3, 1);
  auto sum = s1 - s2;
  EXPECT_EQ(sum, expected);
}

TEST(State, sub_states_with_subset_of_comps)
{
  QState<ti_t, td_t> s1(3, 4), expected(1, 3);
  QState<td_t, ti_t, ts_t> s2(1, 2, "foo");
  auto sum = s1 - s2;
  EXPECT_EQ(sum, expected);
}

TEST(State, mult_state_by_coeff)
{
  int coeff = 2;
  QState<ti_t, td_t> s(1, 2), expected(2, 4);
  EXPECT_EQ(s*coeff, expected);
  EXPECT_EQ(coeff*s, expected);
}

TEST(State, div_state_by_coeff)
{
  int coeff = 2;
  QState<ti_t, td_t> s(4, 6), expected(2, 3);
  EXPECT_EQ(s/coeff, expected);
}

TEST(State, serialize)
{
  QState<ti_t, td_t, ts_t> s(1, 2, "foo");
  std::stringstream sstr;
  sstr << s;
  EXPECT_EQ(sstr.str(), "1 2 foo");
}

TEST(State, deserialize)
{
  std::stringstream sstr("3 4 baz");
  QState<ti_t, td_t, ts_t> s, expected(3, 4, "baz");
  sstr >> s;
  EXPECT_EQ(s, expected);
}

TEST(State, pretty_print)
{
  QState<ti_t, td_t, ts_t> s(1, 2, "foo");
  std::stringstream sstr;
  details::print_state(sstr, s);
  EXPECT_EQ(sstr.str(), "{ti: 1, td: 2, ts: foo}");
}

TEST(State, get_function_not_a_state)
{
  int i = 1;
  auto s = get(i);
  EXPECT_EQ(s, i);
}

TEST(State, get_function_full_state)
{
  QState<ti_t, td_t, ts_t> s(1, 2, "foo");
  auto sc = get(s);
  EXPECT_EQ(sc, s);
}

TEST(State, get_function_one_comp)
{
  int i = 1;
  double d = 2;
  string str = "foo";
  QState<ti_t, td_t, ts_t> s(i, d, str);
  EXPECT_EQ(get<ti_t>(s), i);
  EXPECT_EQ(get<td_t>(s), d);
  EXPECT_EQ(get<ts_t>(s), str);
}

TEST(State, get_function_slice)
{
  int i = 1;
  double d = 2;
  QState<ti_t, td_t, ts_t> s(i, d, "foo");
  EXPECT_EQ((get<ti_t, td_t>(s)), s);
  EXPECT_EQ((get<ti_t, ts_t>(s)), s);
  EXPECT_EQ((get<ts_t, td_t>(s)), s);
}
