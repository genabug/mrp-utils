#ifndef TENSOR_H_INCLUDED
#define TENSOR_H_INCLUDED

/*!
  \file Tensor.h
  \author gennadiy (gennadiy3.14@gmail.com)
  \brief Tensor of rank 2 definition.
*/

#include "Vector.h"
#include <cassert>

template<size_t N, class T = double> class Tensor
{
  T data[N][N];
  static_assert(N != 0, "Tensor of zero size is meaningless.");

public:
  template<class... Ts> constexpr explicit Tensor(const Ts&... as) noexcept;

  // converters
  template<class U> constexpr explicit Tensor(const Tensor<N, U> &t) noexcept;
  template<class U> constexpr Tensor& operator=(const Tensor<N, U> &t) noexcept;

  // access
  static constexpr size_t dim = N;
  constexpr T* operator[](size_t i) noexcept { assert(i < N); return data[i]; }
  constexpr const T* operator[](size_t i) const noexcept { assert(i < N); return data[i]; }

  // unary ops (NB! returns a copy!)
  constexpr Tensor operator-() const noexcept;
  constexpr Tensor operator+() const noexcept { return *this; }
  constexpr Tensor operator~() const noexcept { return transpose(); }

  // assign with op
  constexpr Tensor& operator*=(const T &a) noexcept;
  constexpr Tensor& operator/=(const T &a) noexcept;
  constexpr Tensor& operator+=(const Tensor &A) noexcept;
  constexpr Tensor& operator-=(const Tensor &A) noexcept;
  constexpr Tensor& operator*=(const Tensor &A) noexcept;
  constexpr Tensor& operator/=(const Tensor &A) noexcept { return *this *= A.invert(); }

  // other useful ops
  constexpr T det() const noexcept { return det_impl(); }
  constexpr T trace() const noexcept;
  constexpr Tensor invert() const noexcept;
  constexpr Tensor transpose() const noexcept;

private:
  constexpr Tensor<N-1, T> M(size_t I, size_t J) const noexcept;

  // helper to calculate the determinant
  template<size_t I = N> // stop case for N = 1
    constexpr std::enable_if_t<I == 1, T> det_impl() const noexcept { return data[0][0]; }

  template<size_t I = N> // direct calculation for N = 2
    constexpr std::enable_if_t<I == 2, T> det_impl() const noexcept;

  template<size_t I = N> // direct calculation for N = 3
    constexpr std::enable_if_t<I == 3, T> det_impl() const noexcept;

  template<size_t I = N> // common case for N > 3
    constexpr std::enable_if_t<(I > 3), T> det_impl() const noexcept;

  // init helper struct
  // had to use it instead of std::array
  // because the last one doesn't have constexpr operator[] until c++17
  template<size_t I = N, class U = T> struct array
  {
    U data[I];
    constexpr const T& operator[](size_t i) const noexcept { return data[i]; }
  };

  // init helpers
  template<size_t I, class = std::enable_if_t<I == 0>> // default init
    constexpr void init(const array<0> &) noexcept {}

  template<size_t I, class = std::enable_if_t<I == 1>> // single init
    constexpr void init(const array<1> &arr) noexcept;

  template<size_t I, class = std::enable_if_t<I == N>> // diagonal init
    constexpr void init(const array<I> &arr) noexcept;

  template<size_t I, class = std::enable_if_t<I == N*N>, class = T> // full init
    constexpr void init(const array<I> &arr) noexcept;
}; // class Tensor<N, T>

/*---------------------------------------------------------------------------------------*/

// arithmetic ops
template<size_t N, class T>
  constexpr auto
    operator+(Tensor<N, T> A, const Tensor<N, T> &B) noexcept { A += B; return A; }

template<size_t N, class T>
  constexpr auto
    operator-(Tensor<N, T> A, const Tensor<N, T> &B) noexcept { A -= B; return A; }

template<size_t N, class T>
  constexpr auto
    operator*(Tensor<N, T> A, const Tensor<N, T> &B) noexcept { A *= B; return A; }

template<size_t N, class T>
  constexpr auto operator*(Tensor<N, T> A, const T &a) noexcept { A *= a; return A; }

template<size_t N, class T>
  constexpr auto operator*(const T &a, Tensor<N, T> A) noexcept { A *= a; return A; }

template<size_t N, class T>
  constexpr auto operator/(Tensor<N, T> A, const T &a) noexcept { A /= a; return A; }

template<size_t N, class T>
  constexpr auto
    operator/(Tensor<N, T> A, const Tensor<N, T> &B) noexcept { A /= B; return A; }

// boolean ops
template<size_t N, class T>
  constexpr bool operator==(const Tensor<N, T> &A, const Tensor<N, T> &B) noexcept;

template<size_t N, class T>
  constexpr bool
    operator!=(const Tensor<N, T> &A, const Tensor<N, T> &B) noexcept { return !(A == B); }

// io ops
template<size_t N, class T>
  std::istream& operator>>(std::istream &in, Tensor<N, T> &A) noexcept;

template<size_t N, class T>
  std::ostream& operator<<(std::ostream &out, const Tensor<N, T> &A) noexcept;

// ops with vectors
template<size_t N, class T>
  constexpr Vector<N, T>& operator*=(Vector<N, T> &v, const Tensor<N, T> &A) noexcept;

template<size_t N, class T>
  constexpr auto
    operator*(Vector<N, T> v, const Tensor<N, T> &A) noexcept { v *= A; return v; }

template<size_t N, class T>
  constexpr auto
    operator*(const Tensor<N, T> &A, Vector<N, T> v) noexcept { v *= ~A; return v; }

template<size_t N, class T>
  constexpr Vector<N, T>&
    operator/=(Vector<N, T> &v, const Tensor<N, T> &A) noexcept { return v *= A.invert(); }

template<size_t N, class T>
  constexpr auto
    operator/(Vector<N, T> v, const Tensor<N, T> &A) noexcept { v /= A; return v; }

template<size_t N, class T>
  constexpr auto operator^(const Vector<N, T> &v1, const Vector<N, T> &v2) noexcept;

// ops with 2D vectors
template<class T>
  constexpr auto operator%(const Tensor<2, T> &A, const Vector<2, T> &v) noexcept;

template<class T>
  constexpr auto operator%(const Vector<2, T> &v, const Tensor<2, T> &A) noexcept;

template<class T>
  constexpr auto operator%(const Tensor<2, T> &A, const Tensor<2, T> &B) noexcept;

// ops with 3D vectors
template<class T> constexpr Tensor<3, T> operator~(const Vector<3, T> &v) noexcept;

template<class T>
  constexpr auto operator%(const Tensor<3, T> &A, const Vector<3, T> &v) noexcept;

template<class T>
  constexpr auto operator%(const Vector<3, T> &v, const Tensor<3, T> &A) noexcept;

/*---------------------------------------------------------------------------------------*/
/*------------------------------------ definition ---------------------------------------*/
/*---------------------------------------------------------------------------------------*/

template<size_t N, class T> template<class... Ts>
  constexpr Tensor<N, T>::Tensor(const Ts&... as) noexcept : data{}
{
  constexpr auto n = sizeof...(Ts);
  static_assert(
    n == 0 || n == 1 || n == N || n == N*N, "Ambiguous number of arguments.");
  init<n>({as...});
}

/*---------------------------------------------------------------------------------------*/

template<size_t N, class T>
  template<class U>
    constexpr Tensor<N, T>::Tensor(const Tensor<N, U> &t) noexcept : data{}
{
  for (size_t i = 0; i < N; ++i)
    for (size_t j = 0; j < N; ++j)
      data[i][j] = static_cast<T>(t[i][j]);
}

/*---------------------------------------------------------------------------------------*/

template<size_t N, class T>
  template<class U>
    constexpr Tensor<N, T>& Tensor<N, T>::operator=(const Tensor<N, U> &t) noexcept
{
  for (size_t i = 0; i < N; ++i)
    for (size_t j = 0; j < N; ++j)
      data[i][j] = static_cast<T>(t[i][j]);
}

/*---------------------------------------------------------------------------------------*/

template<size_t N, class T>
  constexpr Tensor<N, T> Tensor<N, T>::operator-() const noexcept
{
  Tensor<N, T> A;
  for (size_t i = 0; i < N; ++i)
    for (size_t j = 0; j < N; ++j)
      A[i][j] = -data[i][j];
  return A;
}

/*---------------------------------------------------------------------------------------*/

template<size_t N, class T>
  constexpr Tensor<N, T>& Tensor<N, T>::operator*=(const T &a) noexcept
{
  for (size_t i = 0; i < N; ++i)
    for (size_t j = 0; j < N; ++j)
      data[i][j] *= a;
  return *this;
}

/*---------------------------------------------------------------------------------------*/

template<size_t N, class T>
  constexpr Tensor<N, T>& Tensor<N, T>::operator/=(const T &a) noexcept
{
  for (size_t i = 0; i < N; ++i)
    for (size_t j = 0; j < N; ++j)
      data[i][j] /= a;
  return *this;
}

/*---------------------------------------------------------------------------------------*/

template<size_t N, class T>
  constexpr Tensor<N, T>& Tensor<N, T>::operator+=(const Tensor<N, T> &A) noexcept
{
  for (size_t i = 0; i < N; ++i)
    for (size_t j = 0; j < N; ++j)
      data[i][j] += A[i][j];
  return *this;
}

/*---------------------------------------------------------------------------------------*/

template<size_t N, class T>
  constexpr Tensor<N, T>& Tensor<N, T>::operator-=(const Tensor<N, T> &A) noexcept
{
  for (size_t i = 0; i < N; ++i)
    for (size_t j = 0; j < N; ++j)
      data[i][j] -= A[i][j];
  return *this;
}

/*---------------------------------------------------------------------------------------*/

template<size_t N, class T>
  constexpr Tensor<N, T>& Tensor<N, T>::operator*=(const Tensor<N, T> &A) noexcept
{
  auto B = *this;
  for (size_t i = 0; i < N; ++i)
    for (size_t j = 0; j < N; ++j)
    {
      data[i][j] = 0;
      for (size_t k = 0; k < N; ++k)
        data[i][j] += B[i][k]*A[k][j];
    }
  return *this;
}

/*---------------------------------------------------------------------------------------*/

template<size_t N, class T> constexpr T Tensor<N, T>::trace() const noexcept
{
  T tr = 0;
  for (size_t i = 0; i < N; ++i)
    tr += data[i][i];
  return tr;
}

/*---------------------------------------------------------------------------------------*/

template<size_t N, class T> constexpr Tensor<N, T> Tensor<N, T>::invert() const noexcept
{
  T d = det();
  if (d == static_cast<T>(0))
    return Tensor<N, T>(0); // inverse matrix doesn't exist, return 0

  Tensor<N, T> A;
  T c = static_cast<T>(1) / d;
  for (size_t i = 0; i < N; ++i)
    for (size_t j = 0; j < N; ++j)
      A[j][i] = ( ((i + j) & 1)? -c : c ) * M(i, j).det();

  return A;
}

/*---------------------------------------------------------------------------------------*/

template<size_t N, class T> constexpr Tensor<N, T> Tensor<N, T>::transpose() const noexcept
{
  Tensor<N, T> A;
  for (size_t i = 0; i < N; ++i)
    for (size_t j = 0; j < N; ++j)
      A[i][j] = data[j][i];
  return A;
}

/*---------------------------------------------------------------------------------------*/

template<size_t N, class T>
  constexpr Tensor<N-1, T> Tensor<N, T>::M(size_t I, size_t J) const noexcept
{
  Tensor<N-1, T> A;
  for (size_t i = 0; i < N-1; ++i)
    for (size_t j = 0; j < N-1; ++j)
    {
      size_t im = (i < I)? i : i+1;
      size_t jm = (j < J)? j : j+1;
      A[i][j] = data[im][jm];
    }
  return A;
}

/*---------------------------------------------------------------------------------------*/

template<size_t N, class T>
  constexpr Vector<N, T>& operator*=(Vector<N, T> &v, const Tensor<N, T> &A) noexcept
{
  auto c = v;
  for (size_t i = 0; i < N; ++i)
  {
    v[i] = 0;
    for (size_t j = 0; j < N; ++j)
      v[i] += c[j] * A[j][i];
  }
  return v;
}

/*---------------------------------------------------------------------------------------*/

template<size_t N, class T>
  constexpr bool operator==(const Tensor<N, T> &A, const Tensor<N, T> &B) noexcept
{
  for (size_t i = 0; i < N; ++i)
    for (size_t j = 0; j < N; ++j)
      if (A[i][j] != B[i][j])
        return false;
  return true;
}

/*---------------------------------------------------------------------------------------*/

template<size_t N, class T>
  std::istream& operator>>(std::istream &in, Tensor<N, T> &A) noexcept
{
  char c;
  bool in_brackets = true;
  while (in.get(c) && c != '[')
    if (!std::isspace(c))
    {
      in.putback(c);
      in_brackets = false;
      break;
    }

  for (size_t i = 0; i < N; ++i)
    for (size_t j = 0; j < N; ++j)
    {
      while (in.get(c) && c != ',')
        if (!std::isspace(c))
        {
          in.putback(c);
          break;
        }
      in >> A[i][j];
    }

  while (in_brackets && in.get(c) && c != ']')
    if (!std::isspace(c))
    {
      in.putback(c);
      break;
    }

  return in;
}

/*---------------------------------------------------------------------------------------*/

template<size_t N, class T>
  std::ostream& operator<<(std::ostream &out, const Tensor<N, T> &A) noexcept
{
  const std::locale &loc = out.getloc();
  bool use_brackets =
    std::has_facet<IO_mode>(loc)?
      std::use_facet<IO_mode>(loc).use_brackets() : true;

  out << (use_brackets? "[" : "") << A[0][0];
  for (size_t j = 1; j < N; ++j)
    out << (use_brackets? ", " : " ") << A[0][j];

  for (size_t i = 1; i < N; ++i)
    for (size_t j = 0; j < N; ++j)
      out << (use_brackets? ", " : " ") << A[i][j];

  return out << (use_brackets? "]" : "");
}

/*---------------------------------------------------------------------------------------*/

template<size_t N, class T>
  constexpr auto operator^(const Vector<N, T> &v1, const Vector<N, T> &v2) noexcept
{
  Tensor<N, T> t;
  for (size_t i = 0; i < N; ++i)
    for (size_t j = 0; j < N; ++j)
      t[i][j] = v1[i] * v2[j];
  return t;
}

/*---------------------------------------------------------------------------------------*/

template<class T>
  constexpr auto operator%(const Tensor<2, T> &A, const Vector<2, T> &v) noexcept
{
  return Vector<2, T>(A[0][0] * v[1] - A[0][1] * v[0],
                      A[1][0] * v[1] - A[1][1] * v[0]);
}

/*---------------------------------------------------------------------------------------*/

template<class T>
  constexpr auto operator%(const Vector<2, T> &v, const Tensor<2, T> &A) noexcept
{
  return Vector<2, T>(v[0] * A[1][0] - v[1] * A[0][0],
                      v[0] * A[1][1] - v[1] * A[0][1]);
}

/*---------------------------------------------------------------------------------------*/

template<class T>
  constexpr auto operator%(const Tensor<2, T> &A, const Tensor<2, T> &B) noexcept
{
  Tensor<2, T> R;
  R[0][0] = A[0][0] * B[1][0] - A[0][1] * B[0][0];
  R[0][1] = A[0][0] * B[1][1] - A[0][1] * B[0][1];
  R[1][0] = A[1][0] * B[1][0] - A[1][1] * B[0][0];
  R[1][1] = A[1][0] * B[1][1] - A[1][1] * B[0][1];
  return R;
}

/*---------------------------------------------------------------------------------------*/

template<class T> constexpr Tensor<3, T> operator~(const Vector<3, T> &v) noexcept
{
  return Tensor<3, T>(    0, -v[2],  v[1],
                       v[2],     0, -v[0],
                      -v[1],  v[0],     0);
}

/*---------------------------------------------------------------------------------------*/

template<class T>
  constexpr auto operator%(const Tensor<3, T> &A, const Vector<3, T> &v) noexcept
{
  Tensor<3, T> R;
  for (size_t i = 0; i < 3; ++i)
  {
    R[i][0] = A[i][1] * v[2] - A[i][2] * v[1];
    R[i][1] = A[i][2] * v[0] - A[i][0] * v[2];
    R[i][2] = A[i][0] * v[1] - A[i][1] * v[0];
  }
  return R;
}

/*---------------------------------------------------------------------------------------*/

template<class T>
  constexpr auto operator%(const Vector<3, T> &v, const Tensor<3, T> &A) noexcept
{
  Tensor<3, T> R;
  for (size_t i = 0; i < 3; ++i)
  {
    R[0][i] = v[1] * A[2][i] - v[2] * A[1][i];
    R[1][i] = v[2] * A[0][i] - v[0] * A[2][i];
    R[2][i] = v[0] * A[1][i] - v[1] * A[0][i];
  }
  return R;
}

/*---------------------------------------------------------------------------------------*/

template<size_t N, class T> template<size_t I>
  constexpr std::enable_if_t<I == 2, T> Tensor<N, T>::det_impl() const noexcept
{
  return data[0][0]*data[1][1] - data[0][1]*data[1][0];
}

template<size_t N, class T> template<size_t I>
  constexpr std::enable_if_t<I == 3, T> Tensor<N, T>::det_impl() const noexcept
{
  return data[0][0] * (data[1][1]*data[2][2] - data[1][2]*data[2][1]) +
         data[0][1] * (data[1][2]*data[2][0] - data[1][0]*data[2][2]) +
         data[0][2] * (data[1][0]*data[2][1] - data[1][1]*data[2][0]);
}

template<size_t N, class T> template<size_t I>
  constexpr std::enable_if_t<(I > 3), T> Tensor<N, T>::det_impl() const noexcept
{
  T d = 0;
  for (size_t j = 0; j < N; ++j)
    d += ( (j & 1)? -1 : 1 ) * data[0][j] * M(0, j).det();
  return d;
}

/*---------------------------------------------------------------------------------------*/

template<size_t N, class T> template<size_t, class> // single init
  constexpr void Tensor<N, T>::init(const Tensor<N, T>::array<1> &arr) noexcept
{
  for (size_t i = 0; i < N; ++i)
    for (size_t j = 0; j < N; ++j)
      data[i][j] = arr[0];
}

template<size_t N, class T> template<size_t I, class> // diagonal init
  constexpr void Tensor<N, T>::init(const Tensor<N, T>::array<I> &arr) noexcept
{
  for (size_t i = 0; i < N; ++i)
    for (size_t j = 0; j < N; ++j)
      data[i][j] = (i == j)? arr[i] : 0;
}

template<size_t N, class T> template<size_t I, class, class> // full init
  constexpr void Tensor<N, T>::init(const Tensor<N, T>::array<I> &arr) noexcept
{
  for (size_t i = 0; i < N; ++i)
    for (size_t j = 0; j < N; ++j)
      data[i][j] = arr[i*N + j];
}

/*---------------------------------------------------------------------------------------*/
/*----------------------------------- documentation -------------------------------------*/
/*---------------------------------------------------------------------------------------*/

/*!
  \class Tensor.
  \brief Tensor of rank 2.
  \tparam N Spatial dimension (total number of its components is N*N).
  \tparam T Type of the components.

  ...
*/

/*!
  \fn constexpr explicit Tensor::Tensor(const Ts&... as) noexcept
  \brief Tensor initialization.
  \tparam as Pack of the initial values

  Four variants of initialization are possible depends on the number of parameters:
  - no parameters: all components are set to zero;
  - one parameter: all components are set to the given value;
  - N parameters: diagonal components are set to the given values,
    the rest are set to zero;
  - N*N parameters: all components are set to the given values, row by row.

  Only these combinations are possible, any other leads to a compilation error
  message about ambiguous number of the constructor arguments.

  \code
  using T2i = Tensor<2, int>;
  T2i td;             // [0 0]
                      // [0 0]

  T2i t1(1);          // [1 1]
                      // [1 1]

  T2i t2(1, 2);       // [1 0]
                      // [0 2]

  T2i t3(1, 2, 3, 4); // [1 2]
                      // [3 4]
  \endcode

  Note that all constructors are explicit thus you should be specific in any operation
  with tensors. For example, if

  If deduced types/size of the terms aren't match than compile error occur. Be explicit.

*/

/*!
  \fn constexpr explicit Tensor::Tensor(const Tensor<N, U> &t) noexcept
  \brief Conversion constructor from a tensor with arbitrary type of the components.
  \tparam U Type of the given tensor' components.
  \param t Tensor to be converted.

  If the specified type U leads to a narrowing conversion then there won't be any warnings.
  In other words, I suppose you know what you're going to do.
*/

/*!
  \fn constexpr Tensor& Tensor::operator=(const Tensor<N, U> &t) noexcept
  \brief Conversion assignment from a tensor with arbitrary type of the components.
  \tparam U Type of the given tensor' components.
  \param t Tensor to be converted.
*/

/*!
  \property static constexpr size_t Tensor::dim
  \brief Spatial dimension (size of vector field).
*/

/*!
  \fn constexpr T* Tensor::operator[](size_t i) noexcept
  \brief Returns the specified row of a tensor.
  \param i Row number.
  \return Specified row of a tensor.
*/

/*!
  \fn constexpr const T* Tensor::operator[](size_t i) const noexcept
  \param i Row number.
  \return Specified row of a tensor.
  \brief Returns the specified row of a tensor. Constant version.
*/

/*!
  \fn constexpr Tensor Tensor::operator-() const noexcept
  \brief Unary minus.
  \return A copy of the given tensor with all its components multiplied by -1.
*/

/*!
  \fn constexpr Tensor Tensor::operator+() const noexcept
  \brief Unary plus.
  \return A copy of the given tensor.
*/

/*!
  \fn constexpr Tensor operator~() const noexcept
  \brief Transposition.
  \return Copy of the given tensor with flipped components values,\f$a_{i,j} = a_{j,i}\f$
*/

/*!
  \fn constexpr Tensor& Tensor::operator*=(const T &a) noexcept
  \brief Mutliplication by scalar.
  \param a Factor, a value of the same type as tensor components (T).
  \return The tensor with all its components multiplied by "a" value.
*/

/*!
  \fn constexpr Tensor& Tensor::operator/=(const T &a) noexcept
  \brief Division by scalar.
  \param a Divider, a value of the same type as tensor components (T).
  \return The tensor with all its components divided by "a" value.
*/

/*!
  \fn constexpr Tensor& Tensor::operator+=(const Tensor &A) noexcept
  \brief Addition with tensor.
  \param A Addhend, a tensor of the same size (N) and type (T) of the components.
  \return The sum of the given tensor with the addhend A.
*/

/*!
  \fn constexpr Tensor& Tensor::operator-=(const Tensor &A) noexcept
  \brief Substruction with tensor.
  \param A Substrahend, a tensor of the same size (N) and type (T) of the components.
  \return The difference of the given tensor and the substrahend A.
*/

/*!
  \fn constexpr Tensor& Tensor::operator*=(const Tensor &A) noexcept
  \brief Multiplication by tensor.
  \param A Factor, a tensor of the same size (N) and type (T) of the components.
  \return The multiplication of the given tensor and the factor A.
*/

/*!
  \fn constexpr Tensor& Tensor::operator/=(const Tensor &A) noexcept
  \brief A shortcut for multiplication by inverse tensor, \f$A /= B \equiv A *= B^{-1}\f$.
  \param A Factor, a tensor of the same size (N) and type (T) of the components.
  \return The multiplication of the given tensor and the inverse factor A.
*/

/*!
  \fn constexpr T Tensor::det() const noexcept
  \brief Compute the tensor' determinant.
  \return Determinant of the given tensor.
*/

/*!
  \fn constexpr T Tensor::trace() const noexcept
  \brief Compute the tensor' trace.
  \return Trace of the given tensor.
*/

/*!
  \fn constexpr Tensor Tensor::invert() const noexcept
  \brief Get the inverse tensor.
  \return The inverse tensor to the given one.
*/

/*!
  \fn constexpr Tensor Tensor::transpose() const noexcept
  \brief Get the transposed tensor.
  \return Tensor with flipped components, i.e. \f$a_{i,j} = a_{j,i}\f$.
  \see Tensor::operator~()
*/

/*!
  \fn constexpr auto operator+(Tensor A, const Tensor &B) noexcept
  \brief Component-wise addition of two tensors with the same dimension and type.
  \param A Left term, augend.
  \param B Right term, addhend.
  \return Tensor with components \f$C_{i,j} = A_{i,j} + B_{i,j}\f$.
*/

/*!
  \fn constexpr auto operator-(Tensor A, const Tensor &B) noexcept
  \brief Component-wise substruction of two tensors with the same dimension and type.
  \param A Left term, minuend.
  \param B Right term, substrahend.
  \return Tensor with components \f$C_{i,j} = A_{i,j} - B_{i,j}\f$.
*/

/*!
  \fn constexpr auto operator*(Tensor A, const Tensor &B) noexcept
  \brief Matrix multiplication of two tensors with the same dimension and type.
  \param A Left term, multiplicand.
  \param B Right term, multiplier.
  \return Tensor with components \f$C_{i,j} = \Sigma_{k=1}^N A_{i,k} \cdot B_{k,j}\f$.
*/

/*!
  \fn constexpr auto operator*(Tensor A, const T &a) noexcept
  \brief Component-wise tensor multiplication by scalar on the right.
  \param A Left term, tensor multiplicand.
  \param a Right term, scalar multiplier.
  \return Tensor with components \f$C_{i,j} = A_{i,j} \cdot a\f$.
*/

/*!
  \fn constexpr auto operator*(const T &a, Tensor A) noexcept
  \brief Component-wise tensor multiplication by scalar on the left.
  \param a Left term, scalar multiplier.
  \param A Right term, tensor multiplicand.
  \return Tensor with components \f$C_{i,j} = a \cdot A_{i,j}\f$.
*/

/*!
  \fn constexpr auto operator/(Tensor A, const T &a) noexcept
  \brief Component-wise tensor division by scalar.
  \param A Left term, tensor divident.
  \param a Right term, scalar divider.
  \return Tensor with components \f$C_{i,j} = A_{i,j} / a\f$.
*/

/*!
  \fn constexpr auto operator/(Tensor A, const Tensor &B) noexcept
  \brief A shortcut for tensor multiplication by inverse one,
    \f$A/B \equiv A\cdot B^{-1}\f$.
  \param A Left term, divident.
  \param B Right term, divider.
  \return The multiplication of the left term by the inverse right term.
*/

/*!
  \fn constexpr bool operator==(const Tensor &A, const Tensor &B) noexcept
  \brief Component-wise equality comparison of two tensors.
  \param A Left term.
  \param B Right term.
  \return true if all components of the left term equals to the corresponding components
    of the right term, i.e. \f$A_{i,j} = B_{i,j}\f$.
*/

/*!
  \fn constexpr bool operator!=(const Tensor &A, const Tensor &B) noexcept
  \brief Component-wise inequality comparison of two tensors.
  \param A Left term.
  \param B Right term.
  \return true if any component of the left term not equal to the corresponding component
    of the right term, i.e. \f$\exists i, j: A_{i,j} \neq B_{i,j}\f$.
*/

/*!
  \fn std::istream& operator>>(std::istream &in, Tensor &A) noexcept
  \brief
  \param in
  \param A
  \return
  \see IO_Mode
*/

/*
template<size_t N, class T>
std::ostream& operator<<(std::ostream &out, const Tensor<N, T> &A) noexcept;

// ops with vectors
template<size_t N, class T>
constexpr Vector<N, T>& operator*=(Vector<N, T> &v, const Tensor<N, T> &A) noexcept;

template<size_t N, class T>
constexpr auto
  operator*(Vector<N, T> v, const Tensor<N, T> &A) noexcept { v *= A; return v; }

template<size_t N, class T>
constexpr auto
  operator*(const Tensor<N, T> &A, Vector<N, T> v) noexcept { v *= ~A; return v; }

template<size_t N, class T>
constexpr Vector<N, T>&
  operator/=(Vector<N, T> &v, const Tensor<N, T> &A) noexcept { return v *= A.invert(); }

template<size_t N, class T>
constexpr auto
  operator/(Vector<N, T> v, const Tensor<N, T> &A) noexcept { v /= A; return v; }

template<size_t N, class T>
constexpr auto operator^(const Vector<N, T> &v1, const Vector<N, T> &v2) noexcept;

// ops with 2D vectors
template<class T>
constexpr auto operator%(const Tensor<2, T> &A, const Vector<2, T> &v) noexcept;

template<class T>
constexpr auto operator%(const Vector<2, T> &v, const Tensor<2, T> &A) noexcept;

template<class T>
constexpr auto operator%(const Tensor<2, T> &A, const Tensor<2, T> &B) noexcept;

// ops with 3D vectors
template<class T> constexpr Tensor<3, T> operator~(const Vector<3, T> &v) noexcept;

template<class T>
constexpr auto operator%(const Tensor<3, T> &A, const Vector<3, T> &v) noexcept;

template<class T>
constexpr auto operator%(const Vector<3, T> &v, const Tensor<3, T> &A) noexcept;
*/

#endif // TENSOR_H_INCLUDED
