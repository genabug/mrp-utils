#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

/*!
  \file Utils.h
  \author gennadiy (gennadiy3.14@gmail.com)
  \brief Various utilities needed here and there all over the code.
*/

#include <cmath>
#include <limits>
#include <type_traits>

namespace Utils
{

  // constexpr version of std::sqrt for both integral and floating point types
  template<class T> constexpr std::enable_if_t<std::is_integral<T>::value, T> sqrt(T x);
  template<class T>
    constexpr std::enable_if_t<std::is_floating_point<T>::value, T> sqrt(T x);

  // floating-point comparison with specific epsilon
  template<class T>
    constexpr std::enable_if_t<!std::is_integral<T>::value, bool>
      almost_equal(T x, T y, int ulp = 1);

} // namespace Utils

/*---------------------------------------------------------------------------------------*/
/*------------------------------------ definition ---------------------------------------*/
/*---------------------------------------------------------------------------------------*/

namespace Utils
{
  namespace details
  {
    template<class T> constexpr T sqrt_int(T x, T lo, T hi)
    {
      auto mid = (lo + hi + 1) / 2;
      if (lo == hi)
        return lo;
      else
        return (x / mid < mid)? sqrt_int(x, lo, mid - 1) : sqrt_int(x, mid, hi);
    }

    template<class T> constexpr T sqrt_real(T x, T lo, T hi)
    {
      return (lo == hi) ? lo : sqrt_real(x, static_cast<T>(0.5) * (lo + x / lo), lo);
    }
  }
}

template<class T>
  constexpr std::enable_if_t<std::is_floating_point<T>::value, T> Utils::sqrt(T x)
{
  return Utils::details::sqrt_real(x, x, static_cast<T>(0));
}

template<class T>
  constexpr std::enable_if_t<std::is_integral<T>::value, T> Utils::sqrt(T x)
{
  return Utils::details::sqrt_int(x, static_cast<T>(0), x / 2 + static_cast<T>(1));
}

/*---------------------------------------------------------------------------------------*/

template<class T>
  constexpr std::enable_if_t<!std::is_integral<T>::value, bool>
    Utils::almost_equal(T x, T y, int ulp)
{
  // the machine epsilon has to be scaled to the magnitude of the values used
  // and multiplied by the desired precision in ULPs (units of least precision)
  return std::abs(x - y) < std::numeric_limits<T>::epsilon() * std::abs(x + y) * ulp
         // unless the result is subnormal
         || std::abs(x - y) < std::numeric_limits<T>::min();
}

#endif // UTILS_H_INCLUDED
