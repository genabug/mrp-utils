#ifndef QSTATE_H_INCLUDED
#define QSTATE_H_INCLUDED

/*!
  \file QState.h
  \author gennadiy (gennadiy3.14@gmail.com)
  \brief Vector (state) of quantities with arbitrary types, definition and documentation.
*/

#include "QDetails.h"

namespace Quantities
{

  template<class... Qs> class QState
  {
    static_assert(sizeof...(Qs) != 0, "empty state is useless");

    std::tuple<
      std::conditional_t<
        std::is_reference<Qs>::value,
        std::conditional_t<
          std::is_const<std::remove_reference_t<Qs>>::value,
          typename std::decay_t<Qs>::type,
          typename std::decay_t<Qs>::type&
        >,
        typename std::decay_t<Qs>::type
      >...
    > data;

  public:
    template<class... Args>
      constexpr explicit QState(Args &&... args) noexcept
        : data(std::forward<Args>(args)...) {}

    // copy ctor
    // WARNING: be careful when use it with auto due to the fact that exact types of data
    // (reference or not) are encoded in the QState type itself! Thus it's duplicate ctor
    // not copy! When data are themselfs copies then duplicate and copy are both the same
    // but when data are references they are not the same! For example:
    //   auto sc = make_state<rho, w>(1, 2); // r and v are rvalues => s : QState<rho, w>
    //   auto sr = make_state<rho, w>(r, v); // r and v are lvalues => s : QState<rho&, w&>
    //   auto c2 = sc; // c2 : QState<rho, w> => it is a copy!
    //   auto c1 = sr; // c1 : QState<rho&, w&> => it is NOT a copy!
    template<class... Ts>
      constexpr QState(const QState<Ts...> &s) noexcept : data(s.template get<Qs>()...) {}

    // access by index (mainly to implement basic ops, see details)
    template<size_t I> constexpr auto& get() & noexcept { return std::get<I>(data); }
    template<size_t I> constexpr auto& get() const & noexcept { return std::get<I>(data); }

    // access and slices by type-name (for generic code)
    template<class Q> constexpr auto& get() & noexcept;
    template<class Q> constexpr auto& get() const & noexcept;

    // slice/refs
    template<class... Ts>
      constexpr std::enable_if_t<sizeof...(Ts) == 0, QState<std::decay_t<Qs>&...>>
        get() & noexcept { return QState<std::decay_t<Qs>&...>(get<Qs>()...); }

    template<class... Ts>
      constexpr std::enable_if_t<sizeof...(Ts) >= 2, QState<std::decay_t<Ts>&...>>
        get() & noexcept { return QState<std::decay_t<Ts>&...>(get<Ts>()...); }

    // slice/copies
    template<class Q> constexpr auto copy() const noexcept { return get<Q>(); }

    template<class... Ts>
      constexpr std::enable_if_t<sizeof...(Ts) == 0, QState<std::decay_t<Qs>...>>
        copy() const noexcept { return *this; }

    template<class... Ts>
      constexpr std::enable_if_t<sizeof...(Ts) >= 2, QState<std::decay_t<Ts>...>>
        copy() const noexcept { return *this; }

    // access and slices by variable (for end-user code)
    template<class Q> constexpr auto& operator[](Q) & noexcept { return get<Q>(); }
    template<class Q> constexpr auto& operator[](Q) const & noexcept { return get<Q>(); }

    // slice/refs
    template<class... Ts, class = std::enable_if_t<sizeof...(Ts) != 0>>
      constexpr decltype(auto) get(Ts...) & noexcept { return get<Ts...>(); }

    // slice/copies
    template<class... Ts, class = std::enable_if_t<sizeof...(Ts) != 0>>
      constexpr decltype(auto) copy(Ts...) const noexcept { return copy<Ts...>(); }

    // some arithmetics
    template<class T> constexpr QState<Qs...>& operator=(T);
    template<class... Ts> constexpr QState<Qs...>& operator=(const QState<Ts...> &);
    template<class... Ts> constexpr QState<Qs...>& operator+=(const QState<Ts...> &);
    template<class... Ts> constexpr QState<Qs...>& operator-=(const QState<Ts...> &);

    template<class T> constexpr QState<Qs...>& operator*=(T);
    template<class T> constexpr QState<Qs...>& operator/=(T);

    // unary ops return the copy!
    constexpr QState<std::decay_t<Qs>...> operator-() const;
    constexpr QState<std::decay_t<Qs>...> operator+() const { return *this; }

    // some traits
    template<size_t I> using type_of = details::type_by_index<I, std::decay_t<Qs>...>;

    template<class Q>
      static constexpr size_t index_of =
        details::index_by_type_v<std::decay_t<Q>, std::decay_t<Qs>...>;

    static constexpr auto names = details::quantity_names<std::decay_t<Qs>...>;

    // helpers for QTraits
    static constexpr int size = details::size_of_v<std::decay_t<Qs>...>;
    static constexpr int ncomps = sizeof...(Qs);
  }; // class QState<Qs...>

/*---------------------------------------------------------------------------------------*/

  // Helpers to get value(s) from a state using type-name(s).
  // In generic code when dealing with compound state and would like to work
  // only with specific component(s), e.g.:
  //   QState<A,B,C,D> c;
  //   auto a = get(c);       // a : QState<A,B,C,D>
  //   auto a = get<A>(c);    // a : A::type
  //   auto a = get<A,B>(c);  // a : QState<A,B>
  // Generic is generic thus 'c' may be not a state at all! In that case:
  //   C c;
  //   auto a = get(c);       // a : C
  //   auto a = get<A>(c);    // a : C
  //   auto a = get<A,B>(c);  // a : C
  // If in all these cases we want the type of 'a' be the same as mentioned
  // in the corresponding comment then the function below is what you need!

  // Just like the corresponding methods of QState class:
  // get-function returns reference(s) to the component(s)
  // while copy-function returns value(s) i.e. copies.

  // slice/copies
  template<class..., class T>
    constexpr std::enable_if_t<!is_state_v<T>, T> copy(T &&s) noexcept { return s; }

  template<class... Qs, class... Ts>
    constexpr decltype(auto) copy(const QState<Ts...> &s) noexcept
  {
    return s.template copy<Qs...>();
  }

  // slice/refs
  template<class..., class T>
    constexpr std::enable_if_t<!is_state_v<T>, T&> get(T &s) noexcept { return s; }

  template<class... Qs, class... Ts>
    constexpr decltype(auto) get(QState<Ts...> &s) noexcept
  {
    return s.template get<Qs...>();
  }

} // namespace Quantities

// IO operations

template<class... Qs>
  std::istream& operator>>(std::istream &, Quantities::QState<Qs...> &);

template<class... Qs>
  std::ostream& operator<<(std::ostream &, const Quantities::QState<Qs...> &);

// binary arithmetic operations
// WARNING: operations are not symmetric, i.e. in general case l + r != r + l
// This is done in order to do arithmetic with states with different set of elements,
// when one of a set is subset of the other, or when the order of elements is differ.
// For example:
// auto s1 = make_state<rho, Te, w>(...);
// auto s2 = make_state<rho, Te, w, Pi, B>(...);
// auto s3 = s1 + s2; // s3 type is State<rho, Te, w>
// auto s4 = s2 + s1; // COMPILE ERROR!!! s1 doesn't have needed variables

template<class... Ls, class... Rs>
  constexpr auto operator+(
    const Quantities::QState<Ls...> &, const Quantities::QState<Rs...> &);

template<class... Ls, class... Rs>
  constexpr auto operator-(
    const Quantities::QState<Ls...> &, const Quantities::QState<Rs...> &);

template<class T, class... Qs>
  constexpr auto operator*(const Quantities::QState<Qs...> &, T);

template<class T, class... Qs>
  constexpr auto operator*(T v, const Quantities::QState<Qs...> &s) { return s * v; }

template<class T, class... Qs>
  constexpr auto operator/(const Quantities::QState<Qs...> &, T);

// boolean operations
// WARNING: operations are not symmetric, i.e. l == r does not mean that r == l
// This is done in order to compare states with different set of elements,
// when one of a set is subset of the other or when order of elements is differ.
// For example, state<rho, Te, w> may be equal to the state<rho, Te, w, Pe, B>
// but not vise versa (COMPILE ERROR!!!)

template<class... Ls, class... Rs>
  constexpr bool operator==(
    const Quantities::QState<Ls...> &, const Quantities::QState<Rs...> &);

template<class... Ls, class... Rs>
  constexpr bool operator!=(
    const Quantities::QState<Ls...> &l, const Quantities::QState<Rs...> &r)
{
    return !(l == r);
}

/*---------------------------------------------------------------------------------------*/
/*------------------------------------ definition ---------------------------------------*/
/*---------------------------------------------------------------------------------------*/

namespace Quantities
{
  // Helper function to create a state using given arguments.
  // In order to preserve the exact types of arguments (ref or copy)
  // we transfer this information to the template arguments of the state
  // and then, inside the state, restore them to the final types
  // (see data definition in the class QState<Qs...>).
  // Thus we get state with values tagged by the quantity types.
  // NB! It's a user responsibility to correctly match list of tags with list values!
  template<class... Qs, class... Args> constexpr auto make_state(Args&&... args) noexcept
  {
    static_assert(sizeof...(Qs) == sizeof...(Args), "incomplete state initialization!");
    return QState<
      std::conditional_t<
        std::is_reference<Args>::value,
        std::conditional_t<
          std::is_const<std::remove_reference_t<Args>>::value,
          Qs, Qs&
        >,
        Qs
      >...
    >(std::forward<Args>(args)...);
  }

/*---------------------------------------------------------------------------------------*/

  template<class... Qs>
    template<class Q> constexpr auto& QState<Qs...>::get() & noexcept
  {
    constexpr auto idx = index_of<Q>;
    static_assert(idx < ncomps, "quantity is not presented in the state!");
    return std::get<idx>(data);
  }

  template<class... Qs>
    template<class Q> constexpr auto& QState<Qs...>::get() const & noexcept
  {
    constexpr auto idx = index_of<Q>;
    static_assert(idx < ncomps, "quantity is not presented in the state!");
    // it'll be good if name of non-presented quantity will be in the error message...
    // UPD: impossible due to second argument must be a string literal (standard requires)
    return std::get<idx>(data);
  }

/*---------------------------------------------------------------------------------------*/

  template<class... Qs> template<class T>
    constexpr QState<Qs...>& QState<Qs...>::operator=(T value)
  {
    details::set_to(*this, value);
    return *this;
  }

/*---------------------------------------------------------------------------------------*/

  template<class... Qs> template<class... Ts>
    constexpr QState<Qs...>& QState<Qs...>::operator=(const QState<Ts...> &other)
  {
    details::copy_to(*this, other);
    return *this;
  }

/*---------------------------------------------------------------------------------------*/

  template<class... Qs> template<class... Ts>
    constexpr QState<Qs...>& QState<Qs...>::operator+=(const QState<Ts...> &other)
  {
    details::add_to(*this, other);
    return *this;
  }

/*---------------------------------------------------------------------------------------*/

  template<class... Qs> template<class... Ts>
    constexpr QState<Qs...>& QState<Qs...>::operator-=(const QState<Ts...> &other)
  {
    details::sub_from(*this, other);
    return *this;
  }

/*---------------------------------------------------------------------------------------*/

  template<class... Qs> template<class T>
    constexpr QState<Qs...>& QState<Qs...>::operator*=(T value)
  {
    details::mult_by(*this, value);
    return *this;
  }

/*---------------------------------------------------------------------------------------*/

  template<class... Qs> template<class T>
    constexpr QState<Qs...>& QState<Qs...>::operator/=(T value)
  {
    details::div_by(*this, value);
    return *this;
  }

/*---------------------------------------------------------------------------------------*/

  template<class... Qs>
    constexpr QState<std::decay_t<Qs>...> QState<Qs...>::operator-() const
  {
    auto s = copy();
    details::mult_by(s, -1);
    return s;
  }

} // namespace Quantities

/*---------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------*/

template<class... Qs>
  std::ostream& operator<<(std::ostream &ostr, const Quantities::QState<Qs...> &s)
{
  Quantities::details::write_state(ostr, s);
  return ostr;
}

/*---------------------------------------------------------------------------------------*/

template<class... Qs>
  std::istream& operator>>(std::istream &istr, Quantities::QState<Qs...> &s)
{
  Quantities::details::read_state(istr, s);
  return istr;
}

/*---------------------------------------------------------------------------------------*/

template<class... Ls, class... Rs>
  constexpr auto operator+(
    const Quantities::QState<Ls...> &l, const Quantities::QState<Rs...> &r)
{
  auto s = l.copy();
  Quantities::details::add_to(s, r);
  return s;
}

/*---------------------------------------------------------------------------------------*/

template<class... Ls, class... Rs>
  constexpr auto operator-(
    const Quantities::QState<Ls...> &l, const Quantities::QState<Rs...> &r)
{
  auto s = l.copy();
  Quantities::details::sub_from(s, r);
  return s;
}

/*---------------------------------------------------------------------------------------*/

template<class T, class... Qs>
  constexpr auto operator*(const Quantities::QState<Qs...> &s, T v)
{
  auto r = s.copy();
  Quantities::details::mult_by(r, v);
  return r;
}

/*---------------------------------------------------------------------------------------*/

template<class T, class... Qs>
  constexpr auto operator/(const Quantities::QState<Qs...> &s, T v)
{
  auto r = s.copy();
  Quantities::details::div_by(r, v);
  return r;
}

/*---------------------------------------------------------------------------------------*/

template<class... Ls, class... Rs>
  constexpr bool operator==(
    const Quantities::QState<Ls...> &l, const Quantities::QState<Rs...> &r)
{
  return Quantities::details::equal(l, r);
}

/*---------------------------------------------------------------------------------------*/
/*--------------------------------------- tests -----------------------------------------*/
/*---------------------------------------------------------------------------------------*/

namespace qstate_tests
{
  // someday...
}

/*---------------------------------------------------------------------------------------*/
/*----------------------------------- documentation -------------------------------------*/
/*---------------------------------------------------------------------------------------*/

/*!
  \class Quantities::QState
  \tparam Qs Type-names (tags) of the data.
  \author gennadiy (gennadiy3.14@gmail.com)
  \brief
*/

#endif // QSTATE_H_INCLUDED
