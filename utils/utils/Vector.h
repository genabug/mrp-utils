#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

/*!
  \file Vector.h
  \author gennadiy
  \brief Euclidian (or not) vector of arbitrary dimension, definition and documentation.
*/

#include "Utils.h"
#include "IOMode.h"
#include <cctype> // isspace
#include <cstddef> // size_t
#include <ostream>
#include <istream>
#include <type_traits> // enable_if_t

template<size_t N, class T = double, bool is_euclidian = true> class Vector
{
  T data[N] = {};
  static_assert(N != 0, "Vector of zero size is meaningless.");

  static_assert(
    std::is_default_constructible_v<T>,
    "Components must be default constructible");

public:
  // ctors
  constexpr Vector() noexcept = default;
  template<class U> constexpr explicit Vector(const U &a) noexcept;
  template<class... Ts, class = std::enable_if_t<sizeof...(Ts) == N>>
    constexpr explicit Vector(Ts... as) noexcept : data{static_cast<T>(as)...} {}

  // converters
  template<class U>
    constexpr explicit Vector(const Vector<N, U, is_euclidian> &v) noexcept;
  template<class U>
    constexpr Vector& operator=(const Vector<N, U, is_euclidian> &v) noexcept;

  // access
  static constexpr size_t X = 0;
  static constexpr size_t Y = (N > 0)? 1 : X;
  static constexpr size_t Z = (N > 1)? 2 : Y;
  static constexpr size_t dim = N;

  constexpr T operator[](size_t i) && noexcept { return data[i]; }
  constexpr T& operator[](size_t i) & noexcept { return data[i]; }
  constexpr const T& operator[](size_t i) const & noexcept { return data[i]; }

  // unary ops (NB! returns a copy!)
  constexpr Vector operator+() const noexcept { return *this; }
  constexpr Vector operator-() const noexcept { return static_cast<T>(-1) * (*this); }

  // assign-ops
  constexpr Vector& operator/=(const T &a) noexcept;
  constexpr Vector& operator*=(const T &a) noexcept;
  constexpr Vector& operator+=(const Vector &v) noexcept;
  constexpr Vector& operator-=(const Vector &v) noexcept;
}; // class Vector<N, T, is_euclidian>

using Vector2D = Vector<2>; //! Shortcut for 2D vector in euclidian space.
using Vector3D = Vector<3>; //! Shortcut for 3D vector in euclidian space.

//! Shortcut for non-euclidian vector
//! i.e. array with componentwise arithmetic, equality and IO operations.
template<size_t N, class T = double> using Array = Vector<N, T, false>;

/*---------------------------------------------------------------------------------------*/

// arithmetic ops
template<size_t N, class T, bool B>
  constexpr auto operator+(
    Vector<N, T, B> v1, const Vector<N, T, B> &v2) noexcept { v1 += v2; return v1; }

template<size_t N, class T, bool B>
  constexpr auto operator-(
    Vector<N, T, B> v1, const Vector<N, T, B> &v2) noexcept { v1 -= v2; return v1; }

template<size_t N, class T, bool B>
  constexpr auto operator*(const T &a, Vector<N, T, B> v) noexcept { v *= a; return v; }

template<size_t N, class T, bool B>
  constexpr auto operator*(Vector<N, T, B> v, const T &a) noexcept { v *= a; return v; }

template<size_t N, class T, bool B>
  constexpr auto operator/(Vector<N, T, B> v, const T &a) noexcept { v /= a; return v; }

// equality ops
template<size_t N, class T, bool B>
  constexpr bool operator==(const Vector<N, T, B> &v1, const Vector<N, T, B> &v2) noexcept;

template<size_t N, class T, bool B>
  constexpr bool operator!=(
    const Vector<N, T, B> &v1, const Vector<N, T, B> &v2) noexcept { return !(v1 == v2); }

// IO ops
// TODO: should throw an exception in case of unexpected format, symbols, ...
class Vectors : public Manipulators<Vectors> {};

template<size_t N, class T, bool B>
  std::istream& operator>>(std::istream &in, Vector<N, T, B> &v) noexcept;

template<size_t N, class T, bool B>
  std::ostream& operator<<(std::ostream &out, const Vector<N, T, B> &v) noexcept;

// useful functions for euclidian vector only! note the lack of third tparam
template<size_t N, class T>
  constexpr auto operator*(const Vector<N, T> &v1, const Vector<N, T> &v2) noexcept;

template<size_t N, class T>
  constexpr auto sqs(const Vector<N, T> &v) noexcept { return v*v; }

template<size_t N, class T>
  constexpr auto fabs(const Vector<N, T> &v) noexcept { return Utils::sqrt(v*v); }

template<size_t N, class T>
  constexpr auto cos(const Vector<N, T> &v1, const Vector<N, T> &v2) noexcept;

template<size_t N, class T>
  constexpr auto sin(const Vector<N, T> &v1, const Vector<N, T> &v2) noexcept;

template<class T>
  constexpr auto operator%(const Vector<2, T> &v1, const Vector<2, T> &v2) noexcept;

template<class T>
  constexpr auto operator%(const Vector<3, T> &v1, const Vector<3, T> &v2) noexcept;

template<class T> constexpr auto operator~(const Vector<2, T> &v) noexcept;

/*---------------------------------------------------------------------------------------*/
/*------------------------------------ definition ---------------------------------------*/
/*---------------------------------------------------------------------------------------*/

template<size_t N, class T, bool B>
  template<class U> constexpr Vector<N, T, B>::Vector(const U &a) noexcept
{
  for (auto &d : data)
    d = static_cast<T>(a);
}

/*---------------------------------------------------------------------------------------*/

template<size_t N, class T, bool B>
  template<class U>
    constexpr Vector<N, T, B>::Vector(const Vector<N, U, B> &v) noexcept
{
  for (size_t i = 0; i < N; ++i)
    data[i] = static_cast<T>(v[i]);
}

/*---------------------------------------------------------------------------------------*/

template<size_t N, class T, bool B>
  template<class U>
    constexpr Vector<N, T, B>&
      Vector<N, T, B>::operator=(const Vector<N, U, B> &v) noexcept
{
  for (size_t i = 0; i < N; ++i)
    data[i] = v[i];
}

/*---------------------------------------------------------------------------------------*/

template<size_t N, class T, bool B>
  constexpr Vector<N, T, B>& Vector<N, T, B>::operator/=(const T &a) noexcept
{
  for (auto &d : data)
    d /= a;
  return *this;
}

/*---------------------------------------------------------------------------------------*/

template<size_t N, class T, bool B>
  constexpr Vector<N, T, B>& Vector<N, T, B>::operator*=(const T &a) noexcept
{
  for (auto &d : data)
    d *= a;
  return *this;
}

/*---------------------------------------------------------------------------------------*/

template<size_t N, class T, bool B>
  constexpr Vector<N, T, B>& Vector<N, T, B>::operator+=(const Vector<N, T, B> &v) noexcept
{
  for (size_t i = 0; i < N; ++i)
    data[i] += v[i];
  return *this;
}

/*---------------------------------------------------------------------------------------*/

template<size_t N, class T, bool B>
  constexpr Vector<N, T, B>& Vector<N, T, B>::operator-=(const Vector<N, T, B> &v) noexcept
{
  for (size_t i = 0; i < N; ++i)
    data[i] -= v[i];
  return *this;
}

/*---------------------------------------------------------------------------------------*/

template<size_t N, class T, bool B>
  constexpr bool operator==(const Vector<N, T, B> &v1, const Vector<N, T, B> &v2) noexcept
{
  for (size_t i = 0; i < N; ++i)
    if (v1[i] != v2[i])
      return false;
  return true;
}

/*---------------------------------------------------------------------------------------*/

template<size_t N, class T, bool B>
  std::istream& operator>>(std::istream &in, Vector<N, T, B> &v) noexcept
{
  char c;
  bool in_brackets = true;
  while (in.get(c) && c != '(')
    if (std::isdigit(c))
    {
      in.putback(c);
      in_brackets = false;
      break;
    }

  for (size_t i = 0; i < N; ++i)
  {
    while (in.get(c) && c != ',')
      if (std::isdigit(c))
      {
        in.putback(c);
        break;
      }
    in >> v[i];
  }

  while (in_brackets && in.get(c) && c != ')')
    if (std::isdigit(c))
    {
      in.putback(c);
      break;
    }

  return in;
}

/*---------------------------------------------------------------------------------------*/

template<size_t N, class T, bool B>
  std::ostream& operator<<(std::ostream &out, const Vector<N, T, B> &v) noexcept
{
  const std::locale &loc = out.getloc();
  bool use_brackets =
    std::has_facet<IOMode<Vectors>>(loc)?
      std::use_facet<IOMode<Vectors>>(loc).use_brackets() : true;

  out << (use_brackets? "(" : "") << v[0];

  for (size_t i = 1; i < N; ++i)
    out << (use_brackets? ", " : " ") << v[i];

  return out << (use_brackets? ")" : "");
}

/*---------------------------------------------------------------------------------------*/

template<size_t N, class T>
constexpr auto operator*(const Vector<N, T> &v1, const Vector<N, T> &v2) noexcept
{
  auto t = v1[0] * v2[0];
  for (size_t i = 1; i < N; ++i)
    t += v1[i]*v2[i];
  return t;
}

/*---------------------------------------------------------------------------------------*/

template<size_t N, class T>
  constexpr auto cos(const Vector<N, T> &v1, const Vector<N, T> &v2) noexcept
{
  static_assert(std::is_arithmetic<T>::value, "Component must be of arithmetic type!");
  return v1 * v2 / (fabs(v1) * fabs(v2));
}

/*---------------------------------------------------------------------------------------*/

template<size_t N, class T>
  constexpr auto sin(const Vector<N, T> &v1, const Vector<N, T> &v2) noexcept
{
  static_assert(std::is_arithmetic<T>::value, "Component must be of arithmetic type!");
  auto x = cos(v1, v2);
  return Utils::sqrt(static_cast<T>(1) - x*x);
}

/*---------------------------------------------------------------------------------------*/

template<class T>
  constexpr auto operator%(const Vector<2, T> &v1, const Vector<2, T> &v2) noexcept
{
  return v1[0]*v2[1] - v1[1]*v2[0];
}

/*---------------------------------------------------------------------------------------*/

template<class T>
  constexpr auto operator%(const Vector<3, T> &v1, const Vector<3, T> &v2) noexcept
{
  return Vector<3, T>(
    v1[1]*v2[2] - v2[1]*v1[2],
    v1[2]*v2[0] - v2[2]*v1[0],
    v1[0]*v2[1] - v2[0]*v1[1]);
}

/*---------------------------------------------------------------------------------------*/

template<class T> constexpr auto operator~(const Vector<2, T> &v) noexcept
{
  return Vector<2, T>(-v[1], v[0]);
}

/*---------------------------------------------------------------------------------------*/
/*----------------------------------- documentation -------------------------------------*/
/*---------------------------------------------------------------------------------------*/

/*!
  \class Vectors
  \brief Specialization of generic IO manipulators for the Vector objects.
*/

/*!
  \class Vector
  \brief Euclidian (or not) vector of arbitrary dimension and type.
  \tparam N Number of components.
  \tparam T Type of the components.
  \tparam is_euclidian Boolean flag, used to distinguish vector in euclidian space (true)
    from an array of components with the same type (false) and a subset of operatrions.

  If vector is not euclidian one then only minimal, componentwise set of operations
  are defined for it, like multiplication and division by scalar, addition to
  and substruction from another non-euclidian vector, as well as IO and equality ops.
  For an euclidian vector additional operations are defined, e.g. dot and curl product,
  magnitude, etc. Also several mixed operations with tensors/matricies are defined,
  \see Tensor.

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
  \fn constexpr explicit Vector::Vector(const Vector &v) noexcept
  \brief Conversion constructor from a vector with arbitrary type of the components.
  \tparam U Type of the given vector' components.
  \param v Vector to be converted.

  If the specified type U leads to a narrowing conversion then there won't be any warnings.
  In other words, I suppose you know what you do.
*/

/*!
  \fn constexpr Vector& Vector::operator=(const Vector &v) noexcept
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

/*!
  \fn constexpr T& Vector::operator[](size_t i) noexcept
  \brief Get the i-th component of a vector.
  \param i Number of component.
  \return Reference to the i-th component of a vector.
*/

/*!
  \fn constexpr const T& Vector::operator[](size_t i) const noexcept
  \brief Get the i-th component of a vector (constant version).
  \param i Number of component.
  \return Constant reference to the i-th component of a vector.
*/

/*!
  \fn constexpr Vector Vector::operator+() const noexcept
  \brief Unary plus.
  \return Copy of the given vector.
*/

/*!
  \fn constexpr Vector Vector::operator-() const noexcept
  \brief Unary minus.
  \return Copy of the given vector with negated components.
*/

/*!
  \fn constexpr Vector& Vector::operator/=(const T &a) noexcept
  \brief Division assignment by scalar
  \param a Divider, a value of the same type as vector components (T).
  \return The vector with all its components divided by "a" value.
*/

/*!
  \fn constexpr Vector& Vector::operator*=(const T &a) noexcept
  \brief Mutliplication assignment by scalar.
  \param a Factor, a value of the same type as vector components (T).
  \return The vector with all its components multiplied by "a" value.
*/

/*!
  \fn constexpr Vector& Vector::operator+=(const Vector &v) noexcept
  \brief Addition assignment with vector.
  \param v Addhend, a vector of the same size (N) and type (T) of the components.
  \return The sum of the given vector with the addhend "v".
*/

/*!
  \fn constexpr Vector& Vector::operator-=(const Vector &v) noexcept
  \brief Substruction assignment with vector.
  \param v Substrahend, a vector of the same size (N) and type (T) of the components.
  \return The difference of the given vector and the substrahend "v".
*/

/*!
  \fn constexpr auto operator+(Vector v1, const Vector &v2) noexcept
  \brief Component-wise addition of two vectors with the same dimension and type.
  \param v1 Left term, augend.
  \param v2 Right term, addhend.
  \return Vector "r" with components r[i] == v1[i] + v2[i]
*/

/*!
  \fn constexpr auto operator-(Vector v1, const Vector &v2) noexcept
  \brief Component-wise substruction of two vectors with the same dimension and type.
  \param v1 Left term, minuend.
  \param v2 Right term, substrahend.
  \return Vector "r" with components r[i] == v1[i] - v2[i]
*/

/*!
  \fn constexpr auto operator*(const T &a, Vector v) noexcept
  \brief Component-wise vector multiplication by scalar on the left.
  \param a Left term, scalar multiplier.
  \param v Right term, vector multiplicand.
  \return Vector "r" with components r[i] == a * r[i]
*/

/*!
  \fn constexpr auto operator*(Vector v, const T &a) noexcept
  \brief Component-wise vector multiplication by scalar on the right.
  \param v Left term, vector multiplicand.
  \param a Right term, scalar multiplier.
  \return Vector "r" with components r[i] == v[i] * a
*/

/*!
  \fn constexpr auto operator/(Vector v, const T &a) noexcept
  \brief Component-wise vector division by scalar.
  \param v Left term, vector divident.
  \param a Right term, scalar divider.
  \return Vector "r" with components r[i] == v[i] / a
*/

/*!
  \fn constexpr auto operator*(const Vector &v1, const Vector &v2) noexcept
  \brief Dot product of two vectors of the same size and type.
  \param v1 Left term, multiplier.
  \param v2 Right term, multiplicand.
  \return Dot product of the two given vectors.
*/

/*!
  \fn constexpr bool operator==(const Vector &v1, const Vector &v2) noexcept
  \brief Component-wise equality comparison of two vectors.
  \param v1 Left term.
  \param v2 Right term.
  \return true if all components of the left term equals to the corresponding components
    of the right term, i.e. v1[i] == v2[i] for i = 0..(N-1)
*/

/*!
  \fn constexpr bool operator!=(const Vector &v1, const Vector &v2) noexcept
  \brief Component-wise inequality comparison of two vectors.
  \param v1 Left term.
  \param v2 Right term.
  \return true if at least one component of the left term is not equal
    to the corresponding component of the right term, i.e. exists i: v1[i] != v2[i]
*/

/*!
  \fn std::istream& operator>>(std::istream &in, Vector &v) noexcept
  \brief Read a vector from input stream.
  \param in Input stream from which tensor is read.
  \param v Vector where read object will be stored.
  \return Input stream after reading the tensor from it.
  \see IOMode

  Vector may be represented in two formats -- w/ and w/o brackets, e.g.
  "(1, 2, 3)" and "1 2 3" both represent the same vector.
  This function is able to read in both formats, none additional actions needed
  in order to successfully read a vector.
*/

/*!
  \fn std::ostream& operator<<(std::ostream &out, const Vector &v) noexcept
  \brief Write a vector to output stream.
  \param out Output stream where tensor is written.
  \param v Vector which will be written to the output stream.
  \return Output stream after writing the tensor to it.
  \see IOMode

  Vector may be represented in two formats -- w/ and w/o brackets, e.g.
  "(1, 2, 3)" and "1 2 3" both represent the same vector.
  You can choose the format using special manipulators inBrackets() and bareComponents(),
  the default is inBrackets. The second one may be useful for e.g. backup purposes.
*/

/*!
  \fn constexpr auto sqs(const Vector &v) noexcept
  \brief Alias for dot product of a vector by itself, i.e. sqs(v) == v*v
  \param v Vector
  \return Dot product of the given vector by itself.
*/

/*!
  \fn constexpr auto fabs(const Vector &v) noexcept
  \brief Get vector's magnitude.
  \param v Vector
  \return Magnitude of the given vector.
*/

/*!
  \fn constexpr auto cos(const Vector &v1, const Vector &v2) noexcept
  \brief Get cosine of the angle between two vectors.
  \param v1 Left term.
  \param v2 Right term.
  \return Cosine of the angle between two given vectors.
*/

/*!
  \fn constexpr auto sin(const Vector &v1, const Vector &v2) noexcept
  \brief Get sine of the angle between two vectors.
  \param v1 Left term.
  \param v2 Right term.
  \return Sine of the angle between two given vectors.
*/

/*!
  \fn constexpr auto operator%(const Vector &v1, const Vector &v2) noexcept
  \brief Cross product in 2D i.e. signed(!) area of a parallelogram formed by two vectors.
  \param v1 Left term.
  \param v2 Right term.
  \return Signed(!) area of a parallelogram formed by the two given vectors.
*/

/*!
  \fn constexpr auto operator%(const Vector &v1, const Vector &v2) noexcept
  \brief Cross product in 3D.
  \param v1 Left term.
  \param v2 Right term.
  \return A vector that is orthogonal to both given vectors, with a direction
    given by the right-hand rule and a magnitude equal to the area of the parallelogram
    that the vectors span.
*/

/*!
  \fn constexpr auto operator~(const Vector &v) noexcept
  \brief Counterclockwise rotation of a 2D vector.
  \param v Vector
  \return A vector rotated by 90 degrees in the positive direction (counterclockwise)
    relative to the given one.
*/

#endif // VECTOR_H_INCLUDED
