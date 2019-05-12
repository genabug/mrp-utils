#ifndef TENSOR_H_INCLUDED
#define TENSOR_H_INCLUDED

/*!
  \file Tensor.h
  \author gennadiy (gennadiy3.14@gmail.com)
  \brief Tensor of rank 2 definition.
*/

#include "Vector.h"

template<size_t N, class T = double> class Tensor
{
  T data[N][N];
  static_assert(N != 0, "Tensor of zero size is meaningless.");

public:
  template<class... Ts>
    constexpr Tensor(const Ts&... as) noexcept : data{}
  {
    constexpr auto n = sizeof...(Ts);
    static_assert(
      n == 0 || n == 1 || n == N || n == N*N, "unsupported number of arguments.");
    init<n>({as...});
  }

  // access
  constexpr T* operator[](size_t i) noexcept { return data[i]; }
  constexpr const T* operator[](size_t i) const noexcept { return data[i]; }

  // unary ops (NB! returns a copy!)
  constexpr Tensor operator-() const noexcept;
  constexpr Tensor operator+() const noexcept { return *this; }
  constexpr Tensor operator~() const noexcept { return transpose(); }

  // assign
  constexpr Tensor& operator=(const T &a) noexcept;
  constexpr Tensor& operator=(const Tensor &) noexcept = default;

  // assign with op
  constexpr Tensor& operator+=(const T &a) noexcept;
  constexpr Tensor& operator-=(const T &a) noexcept;
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
  // because the last one doesn't have constexpr operator[] until in c++17
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
// addition (+)
template<size_t N, class T>
  constexpr Tensor<N, T>
    operator+(Tensor<N, T> A, const Tensor<N, T> &B) noexcept { return A += B; }

template<size_t N, class T>
  constexpr Tensor<N, T> operator+(Tensor<N, T> A, const T &a) noexcept { return A += a; }

template<size_t N, class T>
  constexpr Tensor<N, T> operator+(const T &a, Tensor<N, T> A) noexcept { return A += a; }

// substruction (-)
template<size_t N, class T>
  constexpr Tensor<N, T>
    operator-(Tensor<N, T> A, const Tensor<N, T> &B) noexcept { return A -= B; }

template<size_t N, class T>
  constexpr Tensor<N, T> operator-(Tensor<N, T> A, const T &a) noexcept { return A -= a; }

// multiplication (*)
template<size_t N, class T>
  constexpr Tensor<N, T>
    operator*(Tensor<N, T> A, const Tensor<N, T> &B) noexcept { return A *= B; }

template<size_t N, class T>
  constexpr Tensor<N, T> operator*(Tensor<N, T> A, const T &a) noexcept { return A *= a; }

template<size_t N, class T>
  constexpr Tensor<N, T> operator*(const T &a, Tensor<N, T> A) noexcept { return A *= a; }

// division (/)
template<size_t N, class T>
  constexpr Tensor<N, T> operator/(Tensor<N, T> A, const T &a) noexcept { return A /= a; }

template<size_t N, class T>
  constexpr Tensor<N, T>
    operator/(Tensor<N, T> A, const Tensor<N, T> &B) noexcept { return A /= B; }

// ops with vector
template<size_t N, class T>
  constexpr Vector<N, T>& operator*=(Vector<N, T> &x, const Tensor<N, T> &A) noexcept;

template<size_t N, class T>
  constexpr Vector<N, T>
    operator*(Vector<N, T> x, const Tensor<N, T> &A) noexcept { return x *= A; }

template<size_t N, class T>
  constexpr Vector<N, T>
    operator*(const Tensor<N, T> &A, Vector<N, T> x) noexcept { return x *= ~A; }

template<size_t N, class T>
  constexpr Vector<N, T>&
    operator/=(Vector<N, T> &x, const Tensor<N, T> &A) noexcept { return x *= A.invert(); }

template<size_t N, class T>
  constexpr Vector<N, T>
    operator/(Vector<N, T> x, const Tensor<N, T> &A) noexcept { return x /= A; }

template<size_t N, class T>
  constexpr Tensor<N, T> operator~(const Vector<N, T> &x) noexcept;

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

/*---------------------------------------------------------------------------------------*/
/*------------------------------------ definition ---------------------------------------*/
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
  constexpr Tensor<N, T>& Tensor<N, T>::operator=(const T &a) noexcept
{
  for (size_t i = 0; i < N; ++i)
    data[i][i] = a;
  return *this;
}

/*---------------------------------------------------------------------------------------*/

template<size_t N, class T>
  constexpr Tensor<N, T>& Tensor<N, T>::operator+=(const T &a) noexcept
{
  for (size_t i = 0; i < N; ++i)
    data[i][i] += a;
  return *this;
}

/*---------------------------------------------------------------------------------------*/

template<size_t N, class T>
  constexpr Tensor<N, T>& Tensor<N, T>::operator-=(const T &a) noexcept
{
  for (size_t i = 0; i < N; ++i)
    data[i][i] -= a;
  return *this;
}

/*---------------------------------------------------------------------------------------*/

template<size_t N, class T>
  constexpr Tensor<N, T>& Tensor<N, T>::operator*=(const T &a) noexcept
{
  for (size_t i = 0; i < N; ++i)
    data[i][i] *= a;
  return *this;
}

/*---------------------------------------------------------------------------------------*/

template<size_t N, class T>
  constexpr Tensor<N, T>& Tensor<N, T>::operator/=(const T &a) noexcept
{
  for (size_t i = 0; i < N; ++i)
    data[i][i] /= a;
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
  Tensor<N, T> A;
  T d = det();
  if (d == static_cast<T>(0))
    return (A = 0); // inverse matrix doesn't exist, return 0

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
  constexpr Vector<N, T>& operator*=(Vector<N, T> &x, const Tensor<N, T> &A) noexcept
{
  auto y = x; x = 0;
  for (size_t i = 0; i < N; ++i)
    for (size_t j = 0; j < N; ++j)
      x[i] += y[j] * A[j][i];
  return x;
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
