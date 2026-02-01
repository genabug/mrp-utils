#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

/*!
  \file Utils.h
  \author gennadiy
  \brief Various utilities needed here and there all over the code.
*/

#include <limits>
#include <cstddef>
#include <concepts>
#include <type_traits>

namespace Utils
{
  // constexpr version of std::abs
  // TODO: remove after switching to c++23
  constexpr auto abs(auto x) noexcept { return (x < 0)? -x : x; }

  // constexpr version of std::sqrt for both integral and floating point types
  // TODO: remove after switching to c++26
  constexpr auto sqrt(std::integral auto x) noexcept;
  constexpr auto sqrt(std::floating_point auto x) noexcept;

  // floating-point comparison with specific epsilon
  template<std::floating_point T>
    constexpr T fp_equal(T x, T y, size_t ulp = 1) noexcept;

  // equality for c-strings
  constexpr bool cstr_equal(const char *a, const char *b) noexcept;
} // namespace Utils

/*---------------------------------------------------------------------------------------*/
/*------------------------------------ definitions --------------------------------------*/
/*---------------------------------------------------------------------------------------*/

namespace Utils::details
{
  template<std::integral T> constexpr T sqrt_int(T x, T lo, T hi) noexcept
  {
    auto mid = (lo + hi + 1) / 2;
    if (lo == hi)
      return lo;
    else
      return (x / mid < mid)? sqrt_int(x, lo, mid - 1) : sqrt_int(x, mid, hi);
  }
  template<std::floating_point T> constexpr T sqrt_real(T x, T lo, T hi) noexcept
  {
    return (lo == hi) ? lo : sqrt_real(x, T{0.5} * (lo + x / lo), lo);
  }
}

constexpr auto Utils::sqrt(std::floating_point auto x) noexcept
{
  using type = std::decay_t<decltype(x)>;
  return (x < type{0})? type{-1} : Utils::details::sqrt_real(x, x, type{0});
}

constexpr auto Utils::sqrt(std::integral auto x) noexcept
{
  using type = std::decay_t<decltype(x)>;
  return (x < type{0})? type{-1} : Utils::details::sqrt_int(x, type{0}, x / 2 + type{1});
}

/*---------------------------------------------------------------------------------------*/

template<std::floating_point T>
  constexpr T Utils::fp_equal(T x, T y, size_t ulp) noexcept
{
  // the machine epsilon has to be scaled to the magnitude of the values used
  // and multiplied by the desired precision in ULPs (units of least precision)
  return Utils::abs(x - y) < std::numeric_limits<T>::epsilon() * Utils::abs(x + y) * ulp
         // unless the result is subnormal
         || Utils::abs(x - y) < std::numeric_limits<T>::min();
}

/*---------------------------------------------------------------------------------------*/

constexpr bool Utils::cstr_equal(const char *a, const char *b) noexcept
{
  return (*a && *b)? (*a == *b && Utils::cstr_equal(a + 1, b + 1)) : (!*a && !*b);
}

/*---------------------------------------------------------------------------------------*/
/*--------------------------------------- tests -----------------------------------------*/
/*---------------------------------------------------------------------------------------*/

static_assert(Utils::abs(-1) == 1, "abs failed");
static_assert(Utils::abs(1.) == 1., "abs failed");

static_assert(Utils::sqrt(-1) == -1, "sqrt failed");
static_assert(Utils::sqrt(-2.) == -1., "sqrt failed");
static_assert(Utils::sqrt(-3.f) == -1.f, "sqrt failed");
static_assert(Utils::sqrt(0) == 0, "sqrt failed");
static_assert(Utils::sqrt(0.) == 0., "sqrt failed");
static_assert(Utils::sqrt(0.f) == 0.f, "sqrt failed");
static_assert(Utils::sqrt(1) == 1, "sqrt failed");
static_assert(Utils::sqrt(1.) == 1., "sqrt failed");
static_assert(Utils::sqrt(1.f) == 1.f, "sqrt failed");
static_assert(Utils::sqrt(4) == 2, "sqrt failed");
static_assert(Utils::sqrt(4.) == 2., "sqrt failed");
static_assert(Utils::sqrt(4.f) == 2.f, "sqrt failed");

static_assert(Utils::fp_equal(6.022140857e+23, 6.022140857e+23 + 2e8), "fp equal failed");
static_assert(!Utils::fp_equal(6.022140857e+23, 6.022140857e+23 + 3e8), "fp equal failed");

static_assert(Utils::cstr_equal("", ""), "cstr equal failed");
static_assert(!Utils::cstr_equal("one", ""), "cstr equal failed");
static_assert(!Utils::cstr_equal("", "two"), "cstr equal failed");
static_assert(Utils::cstr_equal("one", "one"), "cstr equal failed");
static_assert(!Utils::cstr_equal("one", "two"), "cstr equal failed");

#endif // UTILS_H_INCLUDED
