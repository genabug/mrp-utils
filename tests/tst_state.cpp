#include "quantities/State.h"

#include <gtest/gtest.h>

using namespace Quantities;

using ti_t = Traits<int, 3, "ti">;
using td_t = Traits<double, 3, "td">;

constexpr ti_t ti;
constexpr td_t td;

// assumed index access works
// most of the operations are tested on compile-time
// but I'd like to have a good old runtime testsfor both debugging and as examples
TEST(State, init_with_rvalues)
{
  State<ti_t, td_t> s(1, 2);
  EXPECT_EQ(s[ti], 1);
  EXPECT_EQ(s[td], 2);
}

TEST(State, init_with_lvalues)
{
  int i = 1;
  double d = 2;
  State<ti_t, td_t> s(i, d);
  EXPECT_EQ(s[ti], i);
  EXPECT_EQ(s[td], d);
}

TEST(State, copy_ctor)
{
  State<ti_t, td_t> s(1, 2);
  auto sc = s;
  EXPECT_EQ(sc, s);
}

TEST(State, access_by_index)
{
  State<ti_t, td_t> s(1, 2);
  EXPECT_EQ(s.get<0>(), 1);
  EXPECT_EQ(s.get<1>(), 2);
}

TEST(State, access_by_type)
{
  State<ti_t, td_t> s(1, 2);
  EXPECT_EQ(s.get<ti_t>(), 1);
  EXPECT_EQ(s.get<td_t>(), 2);
}

TEST(State, access_by_variable)
{
  State<ti_t, td_t> s(1, 2);
  EXPECT_EQ(s[ti], 1);
  EXPECT_EQ(s[td], 2);
}

TEST(State, assign_by_components)
{
  State<ti_t, td_t> s(1, 2);
  s[ti] = 3;
  s[td] = 4;
  EXPECT_EQ(s[ti], 3);
  EXPECT_EQ(s[td], 4);
}

TEST(State, assign_all_components)
{
  State<ti_t, td_t> s, expected(1, 1);
  s = 1;
  EXPECT_EQ(s, expected);
}

TEST(State, assign_state_with_same_order_of_comps)
{
  State<ti_t, td_t> s1, s2(1, 2);
  s1 = s2;
  EXPECT_EQ(s1[ti], s2[ti]);
  EXPECT_EQ(s1[td], s2[td]);
}

TEST(State, assign_state_with_different_order_of_comps)
{
  State<ti_t, td_t> s1;
  State<td_t, ti_t> s2(3, 4);
  s1 = s2;
  EXPECT_EQ(s1[ti], s2[ti]);
  EXPECT_EQ(s1[td], s2[td]);
}

TEST(State, assign_state_with_subset_of_comps)
{
  State<ti_t> s1;
  State<td_t, ti_t> s2(5, 6);
  s1 = s2;
  EXPECT_EQ(s1[ti], s2[ti]);
}

TEST(State, assign_sum_with_same_order_of_comps)
{
  State<ti_t, td_t> s1(1, 2), s2(3, 4);
  s1 += s2;
  EXPECT_EQ(s1[ti], 4);
  EXPECT_EQ(s1[td], 6);
}

TEST(State, assign_sum_with_different_order_of_comps)
{
  State<ti_t, td_t> s1(1, 2);
  State<td_t, ti_t> s2(3, 4);
  s1 += s2;
  EXPECT_EQ(s1[ti], 5);
  EXPECT_EQ(s1[td], 5);
}

TEST(State, assign_sum_with_subset_of_comps)
{
  State<ti_t, td_t> s1(1, 2);
  State<td_t, ti_t> s2(3, 4);
  s1 += s2;
  EXPECT_EQ(s1[ti], 5);
  EXPECT_EQ(s1[td], 5);
}

TEST(State, assign_sub_with_same_order_of_comps)
{
  State<ti_t, td_t> s1(3, 4), s2(1, 2);
  s1 -= s2;
  EXPECT_EQ(s1[ti], 2);
  EXPECT_EQ(s1[td], 2);
}

TEST(State, assign_sub_with_different_order_of_comps)
{
  State<ti_t, td_t> s1(3, 4);
  State<td_t, ti_t> s2(1, 2);
  s1 -= s2;
  EXPECT_EQ(s1[ti], 1);
  EXPECT_EQ(s1[td], 3);
}

TEST(State, assign_sub_with_subset_of_comps)
{
  State<ti_t, td_t> s1(3, 4);
  State<td_t, ti_t> s2(1, 2);
  s1 -= s2;
  EXPECT_EQ(s1[ti], 1);
  EXPECT_EQ(s1[td], 3);
}

TEST(State, assign_mult_state_by_coeff)
{
  State<ti_t, td_t> s(1, 2);
  s *= 2;
  EXPECT_EQ(s[ti], 2);
  EXPECT_EQ(s[td], 4);
}

TEST(State, assign_div_state_by_coeff)
{
  State<ti_t, td_t> s(4, 6);
  s /= 2;
  EXPECT_EQ(s[ti], 2);
  EXPECT_EQ(s[td], 3);
}

TEST(State, compare_states_with_same_order_of_comps)
{
  State<ti_t, td_t> s(1, 2);
  EXPECT_EQ(s, s);
}

TEST(State, compare_states_with_different_order_of_comps)
{
  int i = 1;
  double d = 2;
  State<ti_t, td_t> s1(i, d);
  State<td_t, ti_t> s2(d, i);
  EXPECT_EQ(s1, s2);
}

TEST(State, compare_states_with_subset_of_comps)
{
  int i = 1;
  double d = 2;
  State<ti_t> s1(i);
  State<td_t, ti_t> s2(d, i);
  EXPECT_EQ(s1, s2);
}

TEST(State, unary_plus_doesnot_change_state)
{
  State<ti_t, td_t> s1(1, 2);
  auto sc = s1;
  auto s2 = s1;
  EXPECT_EQ(s1, sc);
  EXPECT_EQ(s1, s2);
}

TEST(State, unary_minus_doesnot_change_state)
{
  State<ti_t, td_t> s1(1, 2);
  auto sc = s1;
  auto s2 = -s1;
  EXPECT_EQ(s1, sc);
  EXPECT_EQ(s2[ti], -1);
  EXPECT_EQ(s2[td], -2);
}

TEST(State, sum_states_with_same_order_of_comps)
{
  State<ti_t, td_t> s1(1, 2), s2(3, 4), expected(4, 6);
  auto sum = s1 + s2;
  EXPECT_EQ(sum, expected);
}

TEST(State, sum_states_with_different_order_of_comps)
{
  State<ti_t, td_t> s1(2, 3);
  State<td_t, ti_t> s2(4, 5), expected(7, 7);
  auto sum = s1 + s2;
  EXPECT_EQ(sum, expected);
}

TEST(State, sum_states_with_subset_of_comps)
{
  State<ti_t, td_t> s1(2, 1), expected(5, 5);
  State<td_t, ti_t> s2(4, 3);
  auto sum = s1 + s2;
  EXPECT_EQ(sum, expected);
}

TEST(State, sub_states_with_same_order_of_comps)
{
  State<ti_t, td_t> s1(4, 3), s2(1, 2), expected(3, 1);
  auto sum = s1 - s2;
  EXPECT_EQ(sum, expected);
}

TEST(State, sub_states_with_different_order_of_comps)
{
  State<ti_t, td_t> s1(3, 4);
  State<td_t, ti_t> s2(1, 2), expected(3, 1);
  auto sum = s1 - s2;
  EXPECT_EQ(sum, expected);
}

TEST(State, sub_states_with_subset_of_comps)
{
  State<ti_t, td_t> s1(3, 4), expected(1, 3);
  State<td_t, ti_t> s2(1, 2);
  auto sum = s1 - s2;
  EXPECT_EQ(sum, expected);
}

TEST(State, mult_state_by_coeff)
{
  int coeff = 2;
  State<ti_t, td_t> s(1, 2), expected(2, 4);
  EXPECT_EQ(s*coeff, expected);
  EXPECT_EQ(coeff*s, expected);
}

TEST(State, div_state_by_coeff)
{
  int coeff = 2;
  State<ti_t, td_t> s(4, 6), expected(2, 3);
  EXPECT_EQ(s/coeff, expected);
}

TEST(State, output_default_in_brackets)
{
  State<ti_t, td_t> s(1, 2);
  std::stringstream sstr;
  sstr << s;
  EXPECT_EQ(sstr.str(), "{ti 1, td 2}");
}

TEST(State, output_in_brackets)
{
  State<ti_t, td_t> s(1, 2);
  std::stringstream sstr;
  sstr << IO::inBrackets << s;
  EXPECT_EQ(sstr.str(), "{ti 1, td 2}");
}

TEST(State, output_bare_comps)
{
  State<ti_t, td_t> s(-1, 2);
  std::stringstream sstr;
  sstr << IO::bareComps << s;
  EXPECT_EQ(sstr.str(), "-1 2");
}

TEST(State, input_in_brackets)
{
  std::stringstream sstr("{ti 3, td 4}");
  State<ti_t, td_t> s, expected(3, 4);
  sstr >> s;
  EXPECT_EQ(s, expected);
}

TEST(State, input_in_brackets_unexpected_name)
{
  State<ti_t, td_t> s;
  std::stringstream sstr("{td 3, ti 4}");
  EXPECT_TRUE(!(sstr >> s));
}

TEST(State, input_in_brackets_missing_closing_bracket)
{
  State<ti_t, td_t> s;
  std::stringstream sstr("{ti 3, td 4");
  EXPECT_TRUE(!(sstr >> s));
}

TEST(State, input_in_brackets_extra_comma)
{
  State<ti_t, td_t> s;
  std::stringstream sstr("{ti 3, td 4,");
  EXPECT_TRUE(!(sstr >> s));
}

TEST(State, input_in_brackets_missing_values)
{
  State<ti_t, td_t> s;
  std::stringstream sstr("{ti 3}");
  EXPECT_TRUE(!(sstr >> s));
}

TEST(State, input_in_brackets_extra_values)
{
  State<ti_t, td_t> s;
  std::stringstream sstr("{ti 3, td 4, tf 5}");
  EXPECT_TRUE(!(sstr >> s));
}

TEST(State, input_bare_comps)
{
  std::stringstream sstr("+3 4");
  State<ti_t, td_t> s, expected(3, 4);
  sstr >> s;
  EXPECT_EQ(s, expected);
}

TEST(State, input_bare_comps_missing_value)
{
  State<ti_t, td_t> s;
  std::stringstream sstr("+3");
  EXPECT_TRUE(!(sstr >> s));
}

TEST(State, roundtrip_io_in_brackets)
{
  State<ti_t, td_t> s1(-5, +6), s2;
  std::stringstream sstr;
  sstr << s1;
  sstr >> s2;
  EXPECT_EQ(s1, s2);
}

TEST(State, roundtrip_io_bare_comps)
{
  State<ti_t, td_t> s1(+7, -8), s2;
  std::stringstream sstr;
  sstr << IO::bareComps << s1;
  sstr >> s2;
  EXPECT_EQ(s1, s2);
}
