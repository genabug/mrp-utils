#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

/*!
  \file Vector.h
  \author gennadiy (gennadiy3.14@gmail.com)
  \brief Euclidian vector of arbitrary dimension, definition and documentation.
*/

#include "IOMode.h"

#include <cctype> // isspace
#include <cstddef> // size_t
#include <ostream>
#include <istream>
#include <type_traits> // enable_if_t

template<size_t N, class T = double, class ST = int> class Vector
{
  T data[N];
  static_assert(N != 0, "Vector of zero size is meaningless.");

public:
  // ctors
  constexpr explicit Vector() noexcept : data{} {}
  constexpr explicit Vector(const T &a) noexcept;
  template<class... Ts, class = std::enable_if_t<sizeof...(Ts) == N>>
    constexpr explicit Vector(Ts... as) noexcept : data{static_cast<T>(as)...} {}

  // converters
  template<class U> constexpr explicit Vector(const Vector<N, U, ST> &v) noexcept;
  template<class U> constexpr Vector& operator=(const Vector<N, U, ST> &v) noexcept;

  // access
  static constexpr size_t X = 0;
  static constexpr size_t Y = (N > 0)? 1 : X;
  static constexpr size_t Z = (N > 1)? 2 : Y;
  static constexpr size_t dim = N;

  constexpr T& operator[](size_t i) noexcept { return data[i]; }
  constexpr const T& operator[](size_t i) const noexcept { return data[i]; }

  // unary ops (NB! returns a copy!)
  constexpr Vector operator+() const noexcept { return *this; }
  constexpr Vector operator-() const noexcept { return static_cast<T>(-1) * (*this); }

  // assign-ops
  constexpr Vector& operator/=(const T &a) noexcept;
  constexpr Vector& operator*=(const T &a) noexcept;
  constexpr Vector& operator+=(const Vector &v) noexcept;
  constexpr Vector& operator-=(const Vector &v) noexcept;
}; // class Vector<N, T, ST>

/*---------------------------------------------------------------------------------------*/

// arithmetic ops
template<size_t N, class T, class ST>
  constexpr auto operator+(
    Vector<N, T, ST> v1, const Vector<N, T, ST> &v2) noexcept { v1 += v2; return v1; }

template<size_t N, class T, class ST>
  constexpr auto operator-(
    Vector<N, T, ST> v1, const Vector<N, T, ST> &v2) noexcept { v1 -= v2; return v1; }

template<size_t N, class T, class ST>
  constexpr auto operator*(const T &a, Vector<N, T, ST> v) noexcept { v *= a; return v; }

template<size_t N, class T, class ST>
  constexpr auto operator*(Vector<N, T, ST> v, const T &a) noexcept { v *= a; return v; }

template<size_t N, class T, class ST>
  constexpr auto operator/(Vector<N, T, ST> v, const T &a) noexcept { v /= a; return v; }

template<size_t N, class T, class ST>
  constexpr auto operator*(
    const Vector<N, T, ST> &v1, const Vector<N, T, ST> &v2) noexcept;

// boolean ops
template<size_t N, class T, class ST>
  constexpr bool operator==(
    const Vector<N, T, ST> &v1, const Vector<N, T, ST> &v2) noexcept;

template<size_t N, class T, class ST>
  constexpr bool operator!=(
    const Vector<N, T, ST> &v1, const Vector<N, T, ST> &v2) noexcept { return !(v1 == v2);}

// IO ops
template<size_t N, class T, class ST>
  std::istream& operator>>(std::istream &in, Vector<N, T, ST> &v) noexcept;

template<size_t N, class T, class ST>
  std::ostream& operator<<(std::ostream &out, const Vector<N, T, ST> &v) noexcept;

// useful functions
template<size_t N, class T, class ST>
  constexpr auto sqs(const Vector<N, T, ST> &v) noexcept { return v*v; }

template<size_t N, class T, class ST>
  constexpr auto fabs(const Vector<N, T, ST> &v) noexcept;

template<size_t N, class T, class ST>
  constexpr auto cos(const Vector<N, T, ST> &v1, const Vector<N, T, ST> &v2) noexcept;

template<size_t N, class T, class ST>
  constexpr auto sin(const Vector<N, T, ST> &v1, const Vector<N, T, ST> &v2) noexcept;

template<class T, class ST>
  constexpr auto operator%(
    const Vector<2, T, ST> &v1, const Vector<2, T, ST> &v2) noexcept;

template<class T, class ST>
  constexpr auto operator%(
    const Vector<3, T, ST> &v1, const Vector<3, T, ST> &v2) noexcept;

template<class T, class ST> constexpr auto operator~(const Vector<2, T, ST> &v) noexcept;

using Vector2D = Vector<2>;
using Vector3D = Vector<3>;

/*---------------------------------------------------------------------------------------*/
/*------------------------------------ definition ---------------------------------------*/
/*---------------------------------------------------------------------------------------*/

template<size_t N, class T, class ST>
  constexpr Vector<N, T, ST>::Vector(const T &a) noexcept : data{}
{
  for (auto &d : data)
    d = a;
}

/*---------------------------------------------------------------------------------------*/

template<size_t N, class T, class ST>
  template<class U>
    constexpr Vector<N, T, ST>::Vector(const Vector<N, U, ST> &v) noexcept : data{}
{
  for (size_t i = 0; i < N; ++i)
    data[i] = v[i];
}

/*---------------------------------------------------------------------------------------*/

template<size_t N, class T, class ST>
  template<class U>
    constexpr Vector<N, T, ST>& Vector<N, T, ST>::operator=(const Vector<N, U, ST> &v) noexcept
{
  for (size_t i = 0; i < N; ++i)
    data[i] = v[i];
}

/*---------------------------------------------------------------------------------------*/

template<size_t N, class T, class ST>
  constexpr Vector<N, T, ST>& Vector<N, T, ST>::operator/=(const T &a) noexcept
{
  for (auto &d : data)
    d /= a;
  return *this;
}

/*---------------------------------------------------------------------------------------*/

template<size_t N, class T, class ST>
  constexpr Vector<N, T, ST>& Vector<N, T, ST>::operator*=(const T &a) noexcept
{
  for (auto &d : data)
    d *= a;
  return *this;
}

/*---------------------------------------------------------------------------------------*/

template<size_t N, class T, class ST>
  constexpr Vector<N, T, ST>&
    Vector<N, T, ST>::operator+=(const Vector<N, T, ST> &v) noexcept
{
  for (size_t i = 0; i < N; ++i)
    data[i] += v[i];
  return *this;
}

/*---------------------------------------------------------------------------------------*/

template<size_t N, class T, class ST>
  constexpr Vector<N, T, ST>&
    Vector<N, T, ST>::operator-=(const Vector<N, T, ST> &v) noexcept
{
  for (size_t i = 0; i < N; ++i)
    data[i] -= v[i];
  return *this;
}

/*---------------------------------------------------------------------------------------*/

template<size_t N, class T, class ST>
  constexpr auto operator*(const Vector<N, T, ST> &v1, const Vector<N, T, ST> &v2) noexcept
{
  auto t = v1[0] * v2[0];
  for (size_t i = 1; i < N; ++i)
    t += v1[i]*v2[i];
  return t;
}

/*---------------------------------------------------------------------------------------*/

template<size_t N, class T, class ST>
  constexpr bool operator==(
    const Vector<N, T, ST> &v1, const Vector<N, T, ST> &v2) noexcept
{
  for (size_t i = 0; i < N; ++i)
    if (v1[i] != v2[i])
      return false;
  return true;
}

/*---------------------------------------------------------------------------------------*/

template<size_t N, class T, class ST>
  std::istream& operator>>(std::istream &in, Vector<N, T, ST> &v) noexcept
{
  char c;
  bool in_brackets = true;
  while (in.get(c) && c != '(')
    if (!std::isspace(c))
    {
      in.putback(c);
      in_brackets = false;
      break;
    }

  for (size_t i = 0; i < N; ++i)
  {
    while (in.get(c) && c != ',')
      if (!std::isspace(c))
      {
        in.putback(c);
        break;
      }
    in >> v[i];
  }

  while (in_brackets && in.get(c) && c != ')')
    if (!std::isspace(c))
    {
      in.putback(c);
      break;
    }

  return in;
}

/*---------------------------------------------------------------------------------------*/

template<size_t N, class T, class ST>
  std::ostream& operator<<(std::ostream &out, const Vector<N, T, ST> &v) noexcept
{
  const std::locale &loc = out.getloc();
  bool use_brackets =
    std::has_facet<IO_mode>(loc)?
      std::use_facet<IO_mode>(loc).use_brackets() : true;

  out << (use_brackets? "(" : "") << v[0];

  for (size_t i = 1; i < N; ++i)
    out << (use_brackets? ", " : " ") << v[i];

  return out << (use_brackets? ")" : "");
}

/*---------------------------------------------------------------------------------------*/

namespace details
{
  // constexpr version of std::sqrt for both integral and floating point types

  template<class T> static constexpr bool is_real_v = std::is_floating_point<T>::value;
  template<class T> static constexpr bool is_integer_v = std::is_integral<T>::value;

  template<class T> constexpr T sqrt_int(T x, T lo, T hi)
  {
    auto mid = (lo + hi + 1) / 2;
    if (lo == hi)
      return lo;
    else
      return (x/mid < mid)? sqrt_int(x, lo, mid - 1) : sqrt_int(x, mid, hi);
  }

  template<class T> constexpr T sqrt_real(T x, T lo, T hi)
  {
    return (lo == hi)? lo : sqrt_real(x, static_cast<T>(0.5) * (lo + x/lo), lo);
  }

  template<class T>
    constexpr std::enable_if_t<is_integer_v<T>, T> sqrt(T x)
  {
    return sqrt_int(x, static_cast<T>(0), x/2 + static_cast<T>(1));
  }

  template<class T>
    constexpr std::enable_if_t<is_real_v<T>, T> sqrt(T x)
  {
    return sqrt_real(x, x, static_cast<T>(0));
  }

} // namespace details

template<size_t N, class T, class ST>
  constexpr auto fabs(const Vector<N, T, ST> &v) noexcept
{
  static_assert(std::is_arithmetic<T>::value, "Component must has arithmetic type!");
  return details::sqrt(v*v);
}

/*---------------------------------------------------------------------------------------*/

template<size_t N, class T, class ST>
  constexpr auto cos(const Vector<N, T, ST> &v1, const Vector<N, T, ST> &v2) noexcept
{
  static_assert(std::is_arithmetic<T>::value, "Component must has arithmetic type!");
  return v1 * v2 / (fabs(v1) * fabs(v2));
}

/*---------------------------------------------------------------------------------------*/

template<size_t N, class T, class ST>
  constexpr auto sin(const Vector<N, T, ST> &v1, const Vector<N, T, ST> &v2) noexcept
{
  static_assert(std::is_arithmetic<T>::value, "Component must has arithmetic type!");
  auto x = cos(v1, v2);
  return details::sqrt(static_cast<T>(1) - x*x);
}

/*---------------------------------------------------------------------------------------*/

template<class T, class ST>
  constexpr auto operator%(const Vector<2, T, ST> &v1, const Vector<2, T, ST> &v2) noexcept
{
  return v1[0]*v2[1] - v1[1]*v2[0];
}

/*---------------------------------------------------------------------------------------*/

template<class T, class ST>
  constexpr auto operator%(const Vector<3, T, ST> &v1, const Vector<3, T, ST> &v2) noexcept
{
  return Vector<3, T, ST>(
    v1[1]*v2[2] - v2[1]*v1[2],
    v1[2]*v2[0] - v2[2]*v1[0],
    v1[0]*v2[1] - v2[0]*v1[1]);
}

/*---------------------------------------------------------------------------------------*/

template<class T, class ST> constexpr auto operator~(const Vector<2, T, ST> &v) noexcept
{
  return Vector<2, T, ST>(-v[1], v[0]);
}

/*---------------------------------------------------------------------------------------*/
/*----------------------------------- documentation -------------------------------------*/
/*---------------------------------------------------------------------------------------*/

/*!
  \class Vector
  \brief Euclidian vector of arbitrary dimension and type.
  \tparam N Number of components.
  \tparam T Type of the components.
  \tparam ST Subtype, used to distinguish vector value of various nature on the type-level.
    [[deprecated]]

  The vector values may have various physical nature and various behavior,
  such as sliding vectors, localized vectors etc. Moreover, the class can be also used
  for storage of sets of N numbers which, actually, are not the vectors...
  For developing functions which have different functionality for different subtypes
  of vectors, the parameter "ST" is added: for instance, next two function are different:
  \code
  void f(Vector<2, double, T1>)
  void f(Vector<2, double, T2>)
  \endcode
  Since this parameter is a type, not a value, it's possible to group different subtypes
  of vectors by introducing the template specializations:
  \code
  template<class T1> void f(Vector<2, double, T1>);
  \endcode
  handles the vector of any subtype, while
  \code
  template<class T2> class subtype;
  template<class T2> void f(Vector<2, double, subtype<T2>>)
  \endcode
  defines the alternative behavior for some subtypes of vector.

  NB! All operations are noexcept because I don't care about overflows! Just kidding!
  It's because there is no standard and cross-platform way to catch them in C++
  for types like int or double (which are the main type of the components IMO).
*/

/*!
  \fn constexpr explicit Vector::Vector(Ts... as) noexcept
  \brief Vector initialization.
  \tparam as Pack of the initial values

  Three variants of initialization are possible depends on the number of parameters:
  - 0 parameters: all components are set to zero;
  - 1 parameter: all components are set to the given value;
  - N parameters: all components are set to the given values.

  Only these combinations are possible, any other leads to a compilation error
  message about ambiguous number of the constructor arguments.
  \code
  using V2i = Vector<2, int>;
  V2i vd;       // (0, 0)
  V2i v1(1);    // (1, 1)
  V2i v2(1, 2); // (1, 2)
  \endcode

  Note that all constructors are explicit thus you should be specific in any operation
  with vectors!
*/

/*!
  \fn constexpr explicit Vector::Vector(const Vector<N, U, ST> &v) noexcept
  \brief Conversion constructor from a vector with arbitrary type of the components.
  \tparam U Type of the given vector' components.
  \param v Vector to be converted.

  If the specified type U leads to a narrowing conversion then there won't be any warnings.
  In other words, I suppose you know what you do.
*/

/*!
  \fn constexpr Vector& Vector::operator=(const Vector<N, U, ST> &v) noexcept
  \brief Conversion assignment from a vector with arbitrary type of the components.
  \tparam U Type of the given vector' components.
  \param t Vector to be converted.
*/

/*!
  \property static constexpr size_t Vector::X
  \brief Access shortcut to the first component of the vector
    in order to make expression look more mathematically.
*/

/*!
  \property static constexpr size_t Vector::Y
  \brief Access shortcut to the second (if presented) component of the vector
    in order to make expression look more mathematically. In case of 1D vectors
    it value coincides with Vector::X.
*/

/*!
  \property static constexpr size_t Vector::Z
  \brief Access shortcut to the third (if presented) component of the vector
    in order to make expression look more mathematically. In case of 2D vectors
    it value coincides with Vector::Y.
*/

/*!
  \property static constexpr size_t Vector::dim
  \brief Spatial dimension, number of vector components.
*/

#endif // VECTOR_H_INCLUDED
