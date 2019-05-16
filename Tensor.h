#ifndef TENSOR_H_INCLUDED
#define TENSOR_H_INCLUDED

/*!
  \file Tensor.h
  \author gennadiy (gennadiy3.14@gmail.com)
  \brief Tensor of rank 2 definition.
*/

#include "Vector.h"
#include <iostream>

template<size_t N, class T = double> class Tensor
{
  T data[N][N];
  static_assert(N != 0, "Tensor of zero size is meaningless.");

public:
  template<class... Ts> constexpr explicit Tensor(const Ts&... as) noexcept;

  // access
  constexpr T* operator[](size_t i) noexcept { return data[i]; }
  constexpr const T* operator[](size_t i) const noexcept { return data[i]; }

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

  template<size_t I = N> // common case for N > 2
    constexpr std::enable_if_t<(I > 2), T> det_impl() const noexcept;

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
    n == 0 || n == 1 || n == N || n == N*N, "unsupported number of arguments.");
  init<n>({as...});
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
  out << '[' << A[0][0];
  for (size_t j = 1; j < N; ++j)
    out << ", " << A[0][j];

  for (size_t i = 1; i < N; ++i)
    for (size_t j = 0; j < N; ++j)
      out << ", " << A[i][j];

  return out << ']';
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
  constexpr std::enable_if_t<(I > 2), T> Tensor<N, T>::det_impl() const noexcept
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

// someday...

#endif // TENSOR_H_INCLUDED
