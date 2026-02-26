#ifndef QSTATE_H_INCLUDED
#define QSTATE_H_INCLUDED

/*!
  \file QState.h
  \author gennadiy
  \brief Vector of quantities with arbitrary types, definition and documentation.
*/

#include "QFwds.h"
#include "QDetails.h"

namespace Quantities
{
  template<Traits... Qs> class QState
  {
    static_assert(sizeof...(Qs) > 0, "QState can not be empty");
    // TODO: static_assert on traits uniqueness
    std::tuple<typename Qs::type...> data = {};

  public:
    constexpr QState() noexcept = default;
    constexpr QState(State auto &&s) noexcept : data(std::move(s).template get<Qs>()...) {}
    constexpr QState(const State auto &s) noexcept : data(s.template get<Qs>()...) {}

    template<class... Args> requires(sizeof...(Args) == sizeof...(Qs))
      constexpr explicit QState(Args... args) noexcept : data(args...) {}

    constexpr QState& operator=(auto) noexcept;
    constexpr QState& operator*=(auto) noexcept;
    constexpr QState& operator/=(auto) noexcept;
    constexpr QState& operator+=(const State auto &) noexcept;
    constexpr QState& operator-=(const State auto &) noexcept;

    constexpr QState operator-() const noexcept;
    constexpr QState operator+() const noexcept { return *this; }

    // access by index (mainly to implement basic ops, see details)
    template<size_t I> constexpr auto& get() & noexcept { return std::get<I>(data); }
    template<size_t I> constexpr auto& get() const & noexcept { return std::get<I>(data); }

    // access by type-name (for generic code)
    template<Traits Q> requires(details::has<Q, Qs...>) constexpr auto& get() & noexcept;
    template<Traits Q> requires(details::has<Q, Qs...>) constexpr auto& get() const & noexcept;

    // index access by variable (for end-user code)
    template<Traits Q> constexpr auto& operator[](Q) & noexcept { return get<Q>(); }
    template<Traits Q> constexpr auto& operator[](Q) const & noexcept { return get<Q>(); }

    // TODO: member access, see https://stackoverflow.com/q/54617101/8802124

    // helpers for QTraits
    static constexpr int size = (Qs::size + ...);
    static constexpr int ncomps = sizeof...(Qs);

    // some traits
    template<size_t I> using type_of = details::type_of<I, Qs...>;
    template<Traits Q> static constexpr auto index_of = details::index_of<Q, Qs...>;
    template<Traits Q> static constexpr bool has = details::has<Q, Qs...>;
  }; // class QState<Qs...>

  // Helper to get value(s) from a state using type-name(s).
  // Use in generic code when dealing with a state and would like to work
  // only with specific subset of its component(s), e.g.:
  //   QState<A,B,C,D> c;
  //   auto a = get<A>(c);    // a : A::type
  //   auto a = get<A,B>(c);  // a : QState<A,B>

  template<Traits... Qs, State S>
    constexpr decltype(auto) get(S s) noexcept
  {
    constexpr size_t slice_sz = sizeof...(Qs);
    if constexpr (slice_sz == 0)
      return s;
    else if constexpr (slice_sz == 1)
      return s.template get<0>();
    else
      return QState<Qs...>(s);
  }

} // namespace Quantities

// IO operations
std::istream& operator>>(std::istream &, Quantities::State auto &);
std::ostream& operator<<(std::ostream &, const Quantities::State auto &);

// binary arithmetic operations
// WARNING: operations + and - are not symmetric, it's assumed that right-side operand
// has all the components of the left-side operand, otherwise the operation is not compilable.
// This is done in order to do arithmetic on states with different set of elements,
// when one of a set is a subset of the other, or when the order of elements is different.
// For example:
// auto s1 = QState<rho, Te, w>(...);
// auto s2 = QState<rho, Te, w, Pi, B>(...);
// auto s3 = s1 + s2; // s3 type is QState<rho, Te, w>
// auto s4 = s2 + s1; // COMPILE ERROR!!! s1 doesn't have Pi nor B
constexpr auto operator+(const Quantities::State auto &, const Quantities::State auto &);
constexpr auto operator-(const Quantities::State auto &, const Quantities::State auto &);

template<class T> constexpr auto operator*(const Quantities::State auto &, T);
template<class T> constexpr auto operator*(T v, const Quantities::State auto &s);
template<class T> constexpr auto operator/(const Quantities::State auto &, T);

// boolean operations
// WARNING: operations are not symmetric too.
constexpr bool operator==(const Quantities::State auto &, const Quantities::State auto &);
constexpr bool operator!=(const Quantities::State auto &l, const Quantities::State auto &r);

/*---------------------------------------------------------------------------------------*/
/*------------------------------------ definitions --------------------------------------*/
/*---------------------------------------------------------------------------------------*/

namespace Quantities
{
  template<Traits... Qs> template<Traits Q> requires(details::has<Q, Qs...>)
    constexpr auto& QState<Qs...>::get() & noexcept
  {
    constexpr auto idx = details::index_of<Q, Qs...>;
    return std::get<idx>(data);
  }

  template<Traits... Qs> template<Traits Q> requires(details::has<Q, Qs...>)
    constexpr auto& QState<Qs...>::get() const & noexcept
  {
    constexpr auto idx = details::index_of<Q, Qs...>;
    return std::get<idx>(data);
  }

/*---------------------------------------------------------------------------------------*/

  template<Traits... Qs>
    constexpr QState<Qs...>& QState<Qs...>::operator=(auto v) noexcept
  {
    details::set_to(*this, v);
    return *this;
  }

/*---------------------------------------------------------------------------------------*/

  template<Traits... Qs>
    constexpr QState<Qs...>& QState<Qs...>::operator+=(const State auto &s) noexcept
  {
    details::add_to(*this, s);
    return *this;
  }

/*---------------------------------------------------------------------------------------*/

  template<Traits... Qs>
    constexpr QState<Qs...>& QState<Qs...>::operator-=(const State auto &s) noexcept
  {
    details::sub_from(*this, s);
    return *this;
  }

/*---------------------------------------------------------------------------------------*/

  template<Traits... Qs> template<class T>
    constexpr QState<Qs...>& QState<Qs...>::operator*=(T value) noexcept
  {
    details::mult_by(*this, value);
    return *this;
  }

/*---------------------------------------------------------------------------------------*/

  template<Traits... Qs> template<class T>
    constexpr QState<Qs...>& QState<Qs...>::operator/=(T value) noexcept
  {
    details::div_by(*this, value);
    return *this;
  }

/*---------------------------------------------------------------------------------------*/

  template<Traits... Qs>
    constexpr QState<Qs...> QState<Qs...>::operator-() const noexcept
  {
    auto s = *this;
    details::mult_by(s, -1);
    return s;
  }

} // namespace Quantities

/*---------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------*/

std::ostream& operator<<(std::ostream &ostr, const Quantities::State auto &s)
{
  Quantities::details::write_state(ostr, s);
  return ostr;
}

/*---------------------------------------------------------------------------------------*/

std::istream& operator>>(std::istream &istr, Quantities::State auto &s)
{
  Quantities::details::read_state(istr, s);
  return istr;
}

/*---------------------------------------------------------------------------------------*/

constexpr auto operator+(const Quantities::State auto &l, const Quantities::State auto &r)
{
  auto s = l;
  Quantities::details::add_to(s, r);
  return s;
}

/*---------------------------------------------------------------------------------------*/

constexpr auto operator-(const Quantities::State auto &l, const Quantities::State auto &r)
{
  auto s = l;
  Quantities::details::sub_from(s, r);
  return s;
}

/*---------------------------------------------------------------------------------------*/

template<class T> constexpr auto operator*(const Quantities::State auto &s, T v)
{
  auto r = s;
  Quantities::details::mult_by(r, v);
  return r;
}

template<class T> constexpr auto operator*(T v, const Quantities::State auto &s)
{
  return s * v;
}

/*---------------------------------------------------------------------------------------*/

template<class T> constexpr auto operator/(const Quantities::State auto &s, T v)
{
  auto r = s;
  Quantities::details::div_by(r, v);
  return r;
}

/*---------------------------------------------------------------------------------------*/

constexpr bool operator==(const Quantities::State auto &l, const Quantities::State auto &r)
{
  return Quantities::details::equal(l, r);
}

constexpr bool operator!=(const Quantities::State auto &l, const Quantities::State auto &r)
{
  return !(l == r);
}

/*---------------------------------------------------------------------------------------*/
/*----------------------------------- documentation -------------------------------------*/
/*---------------------------------------------------------------------------------------*/

/*!
  \class Quantities::QState
  \tparam Qs Type-names (tags) of the data.
  \brief Vector of quantities with arbitrary types.

  Class is aimed for aggregation and processing an arbitrary set of heterogeneous
  values as a single structure with a set of basic operations (arithmetic, logical, IO)
  on objects of such classes. Set of values of physical nature can be considered
  as a state of matter at some spatial point thus the class is called QState.
  Basic arithmetic operations on such states allows us to consider them as vectors,
  for example, for flux computations. However the class doesn't limited to work only
  with physical/arithmetic values and may be considered as heterogeneous collection
  of data with name access to the elements (named tuple).
  \code
  using HD2T_s = QState<rho_t, Te_t, Ti_t, w_t>;
  HD2T_s hd1(1e-6, 1e-3, 1e-3, V3d(0));
  auto hd2 = 2*hd1;
  hd1[rho] = 2e-6;
  auto hd3 = (hd1 + hd2) / 2;
  std::cerr << hd3 << '\n';
  \endcode

  There are two ways to get the components of a state: by type-name and by variable
  of the corresponding type-name. The last one is shorter and uses less parentheses
  but requires the global variables (see the unit tests file for examples).
  \code
  s1[rho] = 4;
  s1[rho] = 4;
  //s1.rho = 4; // this would be perfect: no globals, simple structure syntax.
  \endcode
  It's possible to get subset of components at once by specifying their names
  or simply assigning a state to another state with a subset of quantities.
  The result is a new state with copies of the original values, for example
  all of them are equivalent to each other:
  \code
  auto hd4 = hd1.get(rho, Ti, w);
  QState<rho_t, Ti_t, w_t> hd5 = hd1;
  auto hd6 = hd1.get<rho_t, Ti_t, w_t>(); // hd3 : QState<rho_t, Ti_t, w_t>
  \endcode

  Class is accompanying by a set of arithmetic, logical and IO operations. Note that
  both arithmetic and logical operations are not symmetric! This is done in order to allow
  these operations on states with different sets of components, when one set is a subset
  of the other, or when the order of components is differ.
  \code
  hd3 = (hd1 + hd2) / 2; // hd{1,2,3} are the same type, HD2T_s
  hd4 = (hd1 + hd2) / 2; // it's also OK, both hd{1,2} have all needed components
  //hd1 = hd4; // COMPILE ERROR: Te_t is not presented in the hd4 state
  \endcode

  Note that all operations with states are constexpr and can be done in compile-time,
  thus most of the time misusage of working with states leads to a compilation error.
  For example, access to a component which is not presented in a state,
  mixing states with unmatched list of quantities, etc...

  \see QDetails.h for implementation details.
*/

#endif // QSTATE_H_INCLUDED
