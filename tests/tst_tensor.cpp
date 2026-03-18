#include "math/Tensor.h"
#include <gtest/gtest.h>
#include <sstream>

using namespace Math;

using T2d = Tensor<2>;
using T3d = Tensor<3>;
using T2i = Tensor<2, int>;
using T3i = Tensor<3, int>;

using V2i = Vector<2, int>;
using V3i = Vector<3, int>;

TEST(Tensor, init_default_to_zero)
{
  T3i t;
  EXPECT_TRUE(t[0][0] == 0 && t[0][1] == 0 && t[0][2] == 0 &&
              t[1][0] == 0 && t[1][1] == 0 && t[1][2] == 0 &&
              t[2][0] == 0 && t[2][1] == 0 && t[2][2] == 0);
}

TEST(Tensor, init_with_single_sets_diagonal)
{
  T3i t(1);
  EXPECT_TRUE(t[0][0] == 1 && t[0][1] == 0 && t[0][2] == 0 &&
              t[1][0] == 0 && t[1][1] == 1 && t[1][2] == 0 &&
              t[2][0] == 0 && t[2][1] == 0 && t[2][2] == 1);
}

TEST(Tensor, init_with_diagonal_sets_diagonal)
{
  T3i t{1, 2, 3};
  EXPECT_TRUE(t[0][0] == 1 && t[0][1] == 0 && t[0][2] == 0 &&
              t[1][0] == 0 && t[1][1] == 2 && t[1][2] == 0 &&
              t[2][0] == 0 && t[2][1] == 0 && t[2][2] == 3);
  }

TEST(Tensor, init_with_full_sets_all)
{
  T3i t{1, 2, 3, 4, 5, 6, 7, 8, 9};
  EXPECT_TRUE(t[0][0] == 1 && t[0][1] == 2 && t[0][2] == 3 &&
              t[1][0] == 4 && t[1][1] == 5 && t[1][2] == 6 &&
              t[2][0] == 7 && t[2][1] == 8 && t[2][2] == 9);
}

TEST(Tensor, copy_constructor)
{
  T3i t1(1);
  T3i t1c(t1);
  EXPECT_EQ(t1c, t1);
}

TEST(Tensor, copy_assignment)
{
  T3i t1(1, 2, 3);
  T3i t1c = t1;
  EXPECT_EQ(t1c, t1);
}

TEST(Tensor, brace_copy)
{
  T3i t1{1, 2, 3, 4, 5, 6, 7, 8, 9};
  T3i t1c{t1};
  EXPECT_EQ(t1c, t1);
}

TEST(Tensor, conversion_int_to_double)
{
  T3i ti{1, 2, 3, 4, 5, 6, 7, 8, 9};
  T3d td = T3d(ti);
  EXPECT_DOUBLE_EQ(td[0][0], 1.0);
  EXPECT_DOUBLE_EQ(td[1][1], 5.0);
  EXPECT_DOUBLE_EQ(td[2][2], 9.0);
}

TEST(Tensor, conversion_double_to_int)
{
  T3d td{1.9, 2.1, 3.5, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};
  T3i ti = T3i(td);
  EXPECT_EQ(ti[0][0], 1);
  EXPECT_EQ(ti[0][1], 2);
  EXPECT_EQ(ti[0][2], 3);
}

TEST(Tensor, access)
{
  T2i t(1, 2, 3, 4);
  EXPECT_EQ(t[0][0], 1);
  EXPECT_EQ(t[0][1], 2);
  EXPECT_EQ(t[1][0], 3);
  EXPECT_EQ(t[1][1], 4);

  t[0][0] = 10;
  EXPECT_EQ(t[0][0], 10);

  // Note: accessing rows from rvalue tensors is intentionally disabled
  // to prevent dangling references. The following would cause compile errors:
  //   auto row0 = T2i(1, 2, 3, 4)[0];
  //   const auto row1 = T2i(4, 3, 2, 1)[1];
}

TEST(Tensor, output_with_brackets_default)
{
  std::stringstream ss;
  ss << T2i(1, 2, 3, 4);
  EXPECT_EQ(ss.str(), "[1, 2, 3, 4]");
}

TEST(Tensor, output_with_brackets_explicit)
{
  std::stringstream ss;
  ss << IO::inBrackets << T2i(1, 2, 3, 4);
  EXPECT_EQ(ss.str(), "[1, 2, 3, 4]");
}

TEST(Tensor, output_with_brackets_signed_components)
{
  std::stringstream ss;
  ss << IO::inBrackets << T2i(-1, +2, -3, +4);
  EXPECT_EQ(ss.str(), "[-1, 2, -3, 4]");
}

TEST(Tensor, output_bare_components)
{
  std::stringstream ss;
  ss << IO::bareComps << T2i(1, 2, 3, 4);
  EXPECT_EQ(ss.str(), "1 2 3 4");
}

TEST(Tensor, output_bare_components_two_tensors_nospace)
{
  std::stringstream ss;
  ss << IO::bareComps << T2i(1, 2, 3, 4) << T2i(5, 6, 7, 8);
  EXPECT_EQ(ss.str(), "1 2 3 45 6 7 8");
}

TEST(Tensor, input_with_brackets)
{
  std::stringstream ss("[1, 2, 3, 4]");
  T2i t;
  ss >> t;
  EXPECT_EQ(t, T2i(1, 2, 3, 4));
}

TEST(Tensor, input_with_brackets_signed_components)
{
  std::stringstream ss("[-1, 2, -3, 4]");
  T2i t;
  ss >> t;
  EXPECT_EQ(t, T2i(-1, 2, -3, 4));
}

TEST(Tensor, input_with_brackets_non_digit_chars_in_front_fails)
{
  std::stringstream ss(" ,!([ [1, 2, 3, 4]");
  T2i t;
  ss >> t;
  EXPECT_TRUE(ss.fail());
}

TEST(Tensor, input_with_brackets_junk_after_digit_fails)
{
  std::stringstream ss(" 1,!([ [1, 2, 3, 4]");
  T2i t;
  ss >> t;
  EXPECT_TRUE(ss.fail()); // ',' is not a valid int, fails on second component
}

TEST(Tensor, input_with_brackets_two_tensors_no_delimeter)
{
  std::stringstream ss("[-1, +2, -3, +4][+5, -6, +7, -8]");
  T2i t1, t2;
  ss >> t1 >> t2;
  EXPECT_EQ(t1, T2i(-1, +2, -3, +4));
  EXPECT_EQ(t2, T2i(+5, -6, +7, -8));
}

TEST(Tensor, input_with_brackets_two_tensors_non_digit_delim_fails)
{
  std::stringstream ss("[1, 2, 3, 4]  ,;!ss [5, 6, 7, 8]");
  T2i t1, t2;
  ss >> t1 >> t2;
  EXPECT_EQ(t1, T2i(1, 2, 3, 4));
  EXPECT_EQ(t2, T2i(0));
  EXPECT_TRUE(ss.fail()); // ',;!ss' is junk before second tensor
}

TEST(Tensor, input_bare_components)
{
  std::stringstream ss("1 2 3 4");
  T2i t;
  ss >> t;
  EXPECT_EQ(t, T2i(1, 2, 3, 4));
}

TEST(Tensor, input_bare_components_signed_components)
{
  std::stringstream ss("-1 2 -3 4");
  T2i t;
  ss >> t;
  EXPECT_EQ(t, T2i(-1, 2, -3, 4));
}

TEST(Tensor, input_bare_components_junk_in_front_fails)
{
  std::stringstream ss(" ,!([ 1 2 3 4");
  T2i t;
  ss >> t;
  EXPECT_TRUE(ss.fail());
}

TEST(Tensor, input_bare_components_two_tensors_not_enough_data_fails)
{
  std::stringstream ss("1 2 3 45 6 7 8");
  T2i t1, t2;
  ss >> t1 >> t2;
  EXPECT_EQ(t1, T2i(1, 2, 3, 45));
  EXPECT_EQ(t2, T2i(6, 7, 8, 0));
  EXPECT_TRUE(ss.fail()); // only 3 values left, need 4
}

TEST(Tensor, input_bare_components_two_tensors_non_digit_delim_fails)
{
  std::stringstream ss("-1 +2 -3 +4  ,;!ss +5 -6 +7 -8");
  T2i t1, t2;
  ss >> t1 >> t2;
  EXPECT_EQ(t1, T2i(-1, +2, -3, +4));
  EXPECT_EQ(t2, T2i(0));
  EXPECT_TRUE(ss.fail()); // ',;!ss' is junk before second tensor
}

TEST(Tensor, equality)
{
  T3i t1(1), t2(1);
  EXPECT_EQ(t1, t2);
}

TEST(Tensor, inequality)
{
  T3i t1(1), t2(2);
  EXPECT_NE(t1, t2);
}

TEST(Tensor, addition)
{
  T3i t0(0), t1(1), t2(2);
  EXPECT_EQ(t0 + t1, t1);
  EXPECT_EQ(t1 + t0, t1);
  EXPECT_EQ(t1 + t1, t2);
}

TEST(Tensor, subtraction)
{
  T3i t0(0), t1(1);
  EXPECT_EQ(t1 - t0, t1);
  EXPECT_EQ(t0 - t1, -t1);
}

TEST(Tensor, scalar_multiplication)
{
  T3i t1(1), t2(2);
  EXPECT_EQ(2 * t1, t2);
}

TEST(Tensor, scalar_division)
{
  T3i t1(1), t2(2);
  EXPECT_EQ(t2 / 2, t1);
}

TEST(Tensor, unary_minus)
{
  T3i t1(1);
  EXPECT_EQ(-(-t1), t1);
}

TEST(Tensor, tensor_times_vector)
{
  T2i t(3, 4, 5, 6);
  V2i v(2, 3);
  EXPECT_EQ(t * v, V2i(18, 28));
}

TEST(Tensor, vector_times_tensor)
{
  T2i t(3, 4, 5, 6);
  V2i v(2, 3);
  EXPECT_EQ(v * t, V2i(21, 26));
}

TEST(Tensor, identity_times_vector)
{
  T3i E(1, 1, 1);
  V3i v(1, 2, 3);
  EXPECT_EQ(E * v, v);
  EXPECT_EQ(v * E, v);
}

TEST(Tensor, assign_add)
{
  T3i t(1);
  t += t;
  EXPECT_EQ(t, T3i(2));
}

TEST(Tensor, assign_sub)
{
  T3i t(2);
  t -= T3i(1);
  EXPECT_EQ(t, T3i(1));
}

TEST(Tensor, assign_mul)
{
  T3i t(1);
  t *= 2;
  EXPECT_EQ(t, T3i(2));
}

TEST(Tensor, assign_div)
{
  T3i t(2);
  t /= 2;
  EXPECT_EQ(t, T3i(1));
}

TEST(Tensor, transpose)
{
  T3i t(1, 2, 3, 4, 5, 6, 7, 8, 9);
  T3i tT(1, 4, 7, 2, 5, 8, 3, 6, 9);
  EXPECT_EQ(~t, tT);
  EXPECT_EQ(t.transpose(), ~t);
}

TEST(Tensor, transpose_identity)
{
  T3i E(1, 1, 1);
  EXPECT_EQ(~E, E);
}

TEST(Tensor, trace)
{
  T3i t(1, 2, 3, 0, 1, 4, 5, 6, 0);
  EXPECT_EQ(t.trace(), 2);
}

TEST(Tensor, determinant_identity)
{
  T3i E(1, 1, 1);
  EXPECT_EQ(E.det(), 1);
}

TEST(Tensor, determinant_zero)
{
  T3i Z(0);
  EXPECT_EQ(Z.det(), 0);
}

TEST(Tensor, determinant_3x3)
{
  T3i t(1, 2, 3, 0, 1, 4, 5, 6, 0);
  EXPECT_EQ(t.det(), 1);
}

TEST(Tensor, determinant_4x4)
{
  using T4i = Tensor<4, int>;
  T4i t4(2, 3, 5, 2,
         6, 1, 8, 3,
         5, 4, 9, 2,
         1, 3, 5, 6);
  EXPECT_EQ(t4.det(), -1);
}

TEST(Tensor, invert_identity)
{
  T3i E(1, 1, 1);
  EXPECT_EQ(E.invert(), E);
}

TEST(Tensor, invert_3x3)
{
  T3i E(1, 1, 1);
  T3i t(1, 2, 3, 0, 1, 4, 5, 6, 0);
  T3i R(-24, 18, 5, 20, -15, -4, -5, 4, 1);
  EXPECT_EQ(t.invert(), R);
  EXPECT_EQ(t.invert() * t, E);
  EXPECT_EQ(t * t.invert(), E);
}

TEST(Tensor, invert_4x4)
{
  using T4i = Tensor<4, int>;
  T4i E4(1, 1, 1, 1);
  T4i t4(2, 3, 5, 2,
         6, 1, 8, 3,
         5, 4, 9, 2,
         1, 3, 5, 6);
  T4i t4_inverted(
      121,  28, -76, -29,
       88,  20, -55, -21,
     -113, -26,  71,  27,
       30,   7, -19,  -7);
  EXPECT_EQ(t4.invert(), t4_inverted);
  EXPECT_EQ(t4 * t4_inverted, E4);
}
