#ifndef QSTATE_H_INCLUDED
#define QSTATE_H_INCLUDED

/*!
  \file QState.h
  \author gennadiy
  \brief Vector of quantities with arbitrary types, definition, documentation and basic tests.
*/

#include "QTraits.h"
#include "QDetails.h"

namespace Quantities
{
  template<Traits... Qs> class QState;

  namespace details
  {
    template<class> struct is_state : std::false_type {};
    template<class... Qs> struct is_state<QState<Qs...>> : std::true_type {};
  }

  template<class S> constexpr bool is_state_v = details::is_state<S>::value;
  template<class S> concept State = is_state_v<S>;

  template<Traits... Qs> class QState
  {
    static_assert(sizeof...(Qs) > 0, "state with zero traits is not allowed"); // why?
    static_assert(details::are_unique<Qs...>, "traits must be unique in a state");
    std::tuple<typename Qs::type...> data = {};

  public:
    // QTraits
    static constexpr int size = sizeof...(Qs);

    // helpers
    template<size_t I> using type_of = details::type_of<I, Qs...>;
    template<Traits Q> static constexpr auto index_of = details::index_of<Q, Qs...>;
    template<Traits Q> static constexpr bool has = index_of<Q> < size;

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
    template<size_t I> requires(I < size) constexpr auto& get() & noexcept { return std::get<I>(data); }
    template<size_t I> requires(I < size) constexpr auto& get() const & noexcept { return std::get<I>(data); }
    template<size_t I> requires(I < size) constexpr auto&& get() && noexcept { return std::get<I>(std::move(data)); }
    template<size_t I> requires(I < size) constexpr auto&& get() const && noexcept { return std::get<I>(std::move(data)); }

    // access by type-name (for generic code)
    template<Traits Q> requires(has<Q>) constexpr auto& get() & noexcept { return get<index_of<Q>>(); };
    template<Traits Q> requires(has<Q>) constexpr auto& get() const & noexcept { return get<index_of<Q>>(); };
    template<Traits Q> requires(has<Q>) constexpr auto&& get() && noexcept { return std::move(*this).template get<index_of<Q>>(); };
    template<Traits Q> requires(has<Q>) constexpr auto&& get() const && noexcept { return std::move(*this).template get<index_of<Q>>(); };

    // index access by variable (for end-user code)
    template<Traits Q> constexpr auto& operator[](Q) & noexcept { return get<Q>(); }
    template<Traits Q> constexpr auto& operator[](Q) const & noexcept { return get<Q>(); }
    template<Traits Q> constexpr auto&& operator[](Q) && noexcept { return std::move(*this).template get<Q>(); }
    template<Traits Q> constexpr auto&& operator[](Q) const && noexcept { return std::move(*this).template get<Q>(); }

    // TODO: member access, see https://stackoverflow.com/q/54617101/8802124
  }; // class QState<Qs...>

  // Helper to get value(s) from a state using type-name(s).
  // Use in generic code when dealing with a state or not-a-state
  // and would like to work only with specific subset of its component(s), e.g.:
  //   QState<A,B,C> s;
  //   auto a = get(s);      // a: QState<A,B,C>
  //   auto b = get<A>(s);   // b : A::type
  //   auto c = get<A,B>(s); // c : QState<A,B>
  // NB! it's most probably an error to try getting any component from not-a-state
  template<Traits... Qs, class T> requires(!is_state_v<T>)
    constexpr decltype(auto) get(T t) noexcept
  {
    static_assert(sizeof...(Qs) == 0, "access a state' component(s) of not-a-state object is suspicious");
    return t;
  }

  template<Traits... Qs, class S> requires(is_state_v<S>)
    constexpr decltype(auto) get(S s) noexcept
  {
    constexpr size_t slice_sz = sizeof...(Qs);
    if constexpr (slice_sz == 0)
      return s;
    else if constexpr (slice_sz == 1)
      return s.template get<Qs...>();
    else
      return QState<Qs...>(s);
  }

  // IO operations
  std::istream& operator>>(std::istream &, State auto &);
  std::ostream& operator<<(std::ostream &, const State auto &);

  // arithmetic operations
  // NB! operations are asymmetric, right-hand operand must has all the components
  // of the left-hand operand, otherwise an operation is not compilable.
  constexpr auto operator+(const State auto &, const State auto &) noexcept;
  constexpr auto operator-(const State auto &, const State auto &) noexcept;

  constexpr auto operator*(const State auto &, auto) noexcept;
  constexpr auto operator*(auto, const State auto &) noexcept;
  constexpr auto operator/(const State auto &, auto) noexcept;

  // boolean operations
  // NB! operations are asymmetric.
  constexpr bool operator==(const State auto &, const State auto &) noexcept;
  constexpr bool operator!=(const State auto &, const State auto &) noexcept;
} // namespace Quantities

/*---------------------------------------------------------------------------------------*/
/*------------------------------------ definitions --------------------------------------*/
/*---------------------------------------------------------------------------------------*/

namespace Quantities
{
  template<Traits... Qs>
    constexpr QState<Qs...>& QState<Qs...>::operator=(auto v) noexcept
  {
    if constexpr (is_state_v<decltype(v)>)
      details::set_to_state(*this, v);
    else
      details::set_to_value(*this, v);
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

/*---------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------*/

  std::ostream& operator<<(std::ostream &ostr, const State auto &s)
  {
    details::write_state(ostr, s);
    return ostr;
  }

/*---------------------------------------------------------------------------------------*/

  std::istream& operator>>(std::istream &istr, State auto &s)
  {
    details::read_state(istr, s);
    return istr;
  }

/*---------------------------------------------------------------------------------------*/

  constexpr auto operator+(const State auto &l, const State auto &r) noexcept
  {
    auto s = l;
    details::add_to(s, r);
    return s;
  }

/*---------------------------------------------------------------------------------------*/

  constexpr auto operator-(const State auto &l, const State auto &r) noexcept
  {
    auto s = l;
    details::sub_from(s, r);
    return s;
  }

/*---------------------------------------------------------------------------------------*/

  constexpr auto operator*(const State auto &s, auto v) noexcept
  {
    auto r = s;
    details::mult_by(r, v);
    return r;
  }

  constexpr auto operator*(auto v, const State auto &s) noexcept
  {
    return s * v;
  }

/*---------------------------------------------------------------------------------------*/

  constexpr auto operator/(const State auto &s, auto v) noexcept
  {
    auto r = s;
    details::div_by(r, v);
    return r;
  }

/*---------------------------------------------------------------------------------------*/

  constexpr bool operator==(const State auto &l, const State auto &r) noexcept
  {
    return details::equal(l, r);
  }

  constexpr bool operator!=(const State auto &l, const State auto &r) noexcept
  {
    return !(l == r);
  }
} // namespace Quantities

/*---------------------------------------------------------------------------------------*/
/*--------------------------------------- tests -----------------------------------------*/
/*---------------------------------------------------------------------------------------*/

namespace Quantities::tests
{
  using t1 = QTraits<int, 3, "ti">;
  using t2 = QTraits<double, 3, "td">;

  constexpr t1 ti;
  constexpr t2 td;

  using S = QState<t1, t2>;
  using t3 = QTraits<float, 3, "tf">;
  static_assert(S::has<t1>);
  static_assert(S::has<t2>);
  static_assert(!S::has<t3>);
  static_assert(S::index_of<t1> == 0);
  static_assert(S::index_of<t2> == 1);
  static_assert(S::index_of<t3> == 2);
  static_assert(std::is_same_v<t1, S::type_of<0>>);
  static_assert(std::is_same_v<t2, S::type_of<1>>);
  
  constexpr QState<t1, t2> s(1, 2);
  static_assert(s.get<0>() == 1 && s.get<1>() == 2);
  static_assert(s.get<t1>() == 1 && s.get<t2>() == 2);
  static_assert(s[ti] == 1 && s[td] == 2);

  static_assert(+QState<t1, t2>(1, 2) == QState<t1, t2>(+1, +2));
  static_assert(-QState<t1, t2>(1, 2) == QState<t1, t2>(-1, -2));

  static_assert(QState<t1, t2>(1, 2) * 2 == QState<t1, t2>(2, 4));
  static_assert(2 * QState<t1, t2>(3, 4) == QState<t1, t2>(6, 8));
  static_assert(QState<t1, t2>(2, 4) / 2 == QState<t1, t2>(1, 2));

  static_assert(QState<t1, t2>(1, 2) == QState<t1, t2>(1, 2));
  static_assert(QState<t1, t2>(1, 2) == QState<t2, t1>(2, 1));
  static_assert(QState<t1>(1) == QState<t2, t1>(2, 1));
  static_assert(QState<t1>(2) != QState<t2, t1>(2, 1));

  static_assert(QState<t1, t2>(1, 2) + QState<t1, t2>(3, 4) == QState<t1, t2>(4, 6));
  static_assert(QState<t1, t2>(1, 2) + QState<t2, t1>(3, 4) == QState<t1, t2>(5, 5));
  static_assert(QState<t1>(1) + QState<t1, t2>(3, 4) == QState<t1>(4));

  static_assert(QState<t1, t2>(3, 4) - QState<t1, t2>(2, 1) == QState<t1, t2>(1, 3));
  static_assert(QState<t1, t2>(3, 4) - QState<t2, t1>(2, 1) == QState<t1, t2>(2, 2));
  static_assert(QState<t1>(2) - QState<t1, t2>(1, 3) == QState<t1>(1));
} // namespace Quantities::tests

/*---------------------------------------------------------------------------------------*/
/*----------------------------------- documentation -------------------------------------*/
/*---------------------------------------------------------------------------------------*/

/*!
  \class Quantities::QState
  \tparam Qs Type-names (traits) of the data.
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
  //s1.rho = 4; // this would be perfect: no globals, simple structure syntax.
  \endcode
  It's possible to get subset of components at once by specifying their names
  or simply assigning a state to another state with a subset of quantities.
  The result is a new state with copies of the original values, for example
  all of following expressions are equivalent to each other:
  \code
  auto hd4 = hd1.get(rho, w, Te); // hd6 : QState<rho_t, w_t, Te_t>
  QState<rho_t, Ti_t, w_t> hd5 = hd1;
  auto hd6 = hd1.get<rho_t, Ti_t, w_t>(); // hd6 : QState<rho_t, Ti_t, w_t>
  \endcode

  Class is accompanying by a set of arithmetic, logical and IO operations.
  Note that both arithmetic and logical operations are not symmetric!
  This is done in order to allow these operations on states with different sets of components,
  when one set is a subset of the other, or when the order of components is differ.
  \code
  hd3 = (hd1 + hd2) / 2; // hd{1,2,3} are the same type, HD2T_s
  hd4 = (hd1 + hd2) / 2; // it's also OK, both hd{1,2} have all needed components
  //hd1 = hd4; // COMPILE ERROR: Te_t is not presented in the hd4 state
  \endcode

  Also note that all operations with states are constexpr and can be done in compile-time,
  thus most of the time misusage of working with states leads to a compilation error.
  For example, access to a component which is not presented in a state,
  mixing states with unmatched list of quantities, etc...

  \see QDetails.h for implementation details.
*/

#endif // QSTATE_H_INCLUDED
