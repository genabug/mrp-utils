#ifndef QSTATE_H_INCLUDED
#define QSTATE_H_INCLUDED

/*!
  \file QState.h
  \author gennadiy (gennadiy3.14@gmail.com)
  \brief Vector/state of quantities with arbitrary types, definition and documentation.
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
    //  auto sc = make_state<rho, w>(1, 2); // r and v are rvalues => s : QState<rho, w>
    //  auto sr = make_state<rho, w>(r, v); // r and v are lvalues => s : QState<rho&, w&>
    //  auto c1 = sc; // c1 : QState<rho, w> => it is a copy!
    //  auto c2 = sr; // c2 : QState<rho&, w&> => it is NOT a copy!
    template<class... Ts>
      constexpr QState(const QState<Ts...> &s) noexcept : data(s.template get<Qs>()...) {}

    // access by index (mainly to implement basic ops, see details)
    template<size_t I> constexpr auto& get() & noexcept { return std::get<I>(data); }
    template<size_t I> constexpr auto& get() const & noexcept { return std::get<I>(data); }

    // access and slices by type-name (for generic code)
    template<class Q> constexpr auto& get() & noexcept;
    template<class Q> constexpr auto& get() const & noexcept;

    // slice/refs by type-name
    template<class... Ts>
      constexpr std::enable_if_t<sizeof...(Ts) == 0, QState<std::decay_t<Qs>&...>>
        get() & noexcept { return QState<std::decay_t<Qs>&...>(get<Qs>()...); }

    template<class... Ts>
      constexpr std::enable_if_t<sizeof...(Ts) >= 2, QState<std::decay_t<Ts>&...>>
        get() & noexcept { return QState<std::decay_t<Ts>&...>(get<Ts>()...); }

    // slice/copies by type-name
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

    // slice/refs by variable
    template<class... Ts, class = std::enable_if_t<sizeof...(Ts) != 0>>
      constexpr decltype(auto) get(Ts...) & noexcept { return get<Ts...>(); }

    // slice/copies by variable
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

    template<class Q> static constexpr size_t index_of()
    { return details::index_by_type_v<std::decay_t<Q>, std::decay_t<Qs>...>; }

    template<class Q> static constexpr bool has() { return index_of<Q>() < ncomps; }

    static constexpr auto names = details::quantity_names<std::decay_t<Qs>...>;

    // helpers for QTraits
    static constexpr int size = details::size_of_v<std::decay_t<Qs>...>;
    static constexpr int ncomps = sizeof...(Qs);
  }; // class QState<Qs...>

/*---------------------------------------------------------------------------------------*/

  // Helpers to get value(s) from a state using type-name(s).
  // Use in generic code when dealing with a state and would like to work
  // only with specific subset of its component(s), e.g.:
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
  // then the function below is what you need!
  // Just like the corresponding methods of QState class
  // get-function returns reference to a component or state of references
  // while copy-function returns copies (value or state of values).

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
// WARNING: operations are not symmetric, i.e. in general case l + r != r + l.
// This is done in order to do arithmetic with states of different sets of elements,
// when one of a set is a subset of the other, or when the order of elements is differ.
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
// WARNING: operations are not symmetric too, i.e. l == r does not mean that r == l.

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

  template<class... Qs> template<class Q>
    constexpr auto& QState<Qs...>::get() & noexcept
  {
    constexpr auto idx = index_of<Q>();
    static_assert(idx < ncomps, "quantity is not presented in the state!");
    return std::get<idx>(data);
  }

  template<class... Qs> template<class Q>
    constexpr auto& QState<Qs...>::get() const & noexcept
  {
    constexpr auto idx = index_of<Q>();
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
/*----------------------------------- documentation -------------------------------------*/
/*---------------------------------------------------------------------------------------*/

/*!
  \class Quantities::QState
  \tparam Qs Type-names (tags) of the data.
  \brief Vector/state of quantities with arbitrary types.

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
  The only requirement to a type-name (tag) is public field 'type' with value type.

  Class allows you to aggregate both copies and references to specified set of variables.
  State of copies is very similar to simple structure with a little bit more complex access
  (I'm working on that) but also with auto-generated set of basic operations.
  State of references is useful for vectorization of the computations (in a logical way),
  i.e. applying the same operation to a set of variables at once.
  \code
  double r = 1, t = 2;
  auto s1 = make_state<rho_t, T_t>(1, 2); // s1 : QState<rho_t, T_t>
  auto s2 = make_state<rho_t, T_t>(r, t); // s2 : QState<rho_t&, T_t&>
  s1[rho] = 2; // r == 1
  s2[rho] = 2; // r == 2
  \endcode

  The way the class aggregates values is effected on its type which leads to some specifics
  of copy constructor semantics. Namely the aggregated copies will be copied but references
  will remain the references to the original objects. Thus the resultant object of the copy
  constructor it's not a copy of the original object in general case but its duplicate.
  It may be really confusing especially in combination with auto keyword. To be assure
  that you get the copy of a state use special method/function 'copy' or specify exact type
  explicitely.
  \code
  // auto is dangerous!
  auto c1 = s1; // c1 : QState<rho, T> => it is a copy!
  auto c2 = s2; // c2 : QState<rho&, T&> => it is a duplicate!
  c1[T] = 3;    // s1[T] == 2
  c2[T] = 3;    // s2[T] == 3

  // get the real copy using special method
  auto c3 = s1.copy(); // c3 : QState<rho, T>
  auto c4 = copy(s2);  // c4 : QState<rho, T>

  // get the real copies by specifying exact type
  QState<rho, T> c5 = s1;
  QState<rho, T> c6 = s2;
  \endcode
  Note that both ways allow you to make more than just a copy! Namely you can slice
  the state i.e. make a state with specific subset of the data (see below).

  Currenlty there are two ways to get the components of a state: by type-name and
  by variable of the corresponding type-name. The last one is shorter and uses less
  parentheses but requires the global variables (see the unit tests file for examples).
  Type-name access is ugly, global variables are evil... Another way is needed!
  (I like this one https://stackoverflow.com/q/54617101/8802124 but it requires c++20.)
  \code
  s1[rho] = 4;
  s1.get<rho_t>() = 4;
  //s1.rho = 4; // this would be perfect: no globals, simple structure syntax.
  \endcode
  It's also possible to get several components at once by specifying their names to the
  corresponding methods (so called slicing). The result is a new state with copies
  or references (depends on method) of the original values.
  \code
  auto hd4 = hd1.get<rho_t, Ti_t, w_t>(); // hd3 : QState<rho_t&, Ti_t&, w_t&>
  auto hd5 = hd2.copy(rho, Ti, w); // hd5 : QState<rho_t, Ti_t, w_t>
  QState<rho_t, Ti_t, w_t> hd6 = hd2; // explicit way is also possible
  auto hd7 = hd2.copy(); // full copy
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
  most misusages lead to a compilation error! For example, access to a component
  which is not presented in a state, mixing states with unmatched list of quantities.

  \see QDetails.h for implementation details.
*/

#endif // QSTATE_H_INCLUDED
