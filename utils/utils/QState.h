#ifndef QSTATE_H_INCLUDED
#define QSTATE_H_INCLUDED

/*!
  \file QState.h
  \author gennadiy
  \brief Vector of quantities with arbitrary types, definition and documentation.
*/

#include "QTraits.h"
#include "QDetails.h"

namespace Quantities
{
  template<Traits... Qs> class QState;
  
  namespace details
  {
    template<class>
    struct is_state : std::false_type {};
    
    template<class... Qs>
    struct is_state<QState<Qs...>> : std::true_type {};
  }
  
  template<class S> concept State = details::is_state<S>::value;
  template<class S> concept NotState = !details::is_state<S>::value;

  template<Traits... Qs> class QState
  {
    std::tuple<
      std::conditional_t<
        std::is_reference<Qs>::value,
        std::conditional_t<
          std::is_const<std::remove_reference_t<Qs>>::value,
          const typename std::decay_t<Qs>::type&,
          typename std::decay_t<Qs>::type&
        >,
        typename std::decay_t<Qs>::type
      >...
    > data = {};

  public:
    constexpr QState() noexcept = default;

    template<class... Args>
      constexpr explicit QState(Args&&... args) noexcept
        : data(std::forward<Args>(args)...) {}

    constexpr QState(State auto &&s) noexcept
      : data(std::move(s).template get<Qs>()...) {}

    // copy ctor
    // WARNING: be careful when use it with auto because exact types of data
    // (reference or not) are encoded in the QState type itself! Thus it's duplicate ctor
    // not copy! When data are themselfs copies then duplicate and copy are both the same
    // but when data are references they are not the same! For example:
    //  auto sc = make_state<rho, w>(1, 2); // r and v are rvalues => s : QState<rho, w>
    //  auto sr = make_state<rho, w>(r, v); // r and v are lvalues => s : QState<rho&, w&>
    //  auto c1 = sc; // c1 : QState<rho, w> => it is a copy!
    //  auto c2 = sr; // c2 : QState<rho&, w&> => it is NOT a copy!
    constexpr QState(const State auto &s) noexcept
      : data(s.template get<Qs>()...) {}

    // access by index (mainly to implement basic ops, see details)
    template<size_t I> constexpr auto& get() & noexcept { return std::get<I>(data); }
    template<size_t I> constexpr auto& get() const & noexcept { return std::get<I>(data); }

    // access and slice by type-name (for generic code)
    template<Traits Q> constexpr auto& get() & noexcept;
    template<Traits Q> constexpr auto& get() const & noexcept;

    // ref slice by type-name
    constexpr QState<std::decay_t<Qs>&...>
      get() & noexcept { return QState<std::decay_t<Qs>&...>(get<Qs>()...); }

    template<Traits... Ts> requires(sizeof...(Ts) > 1)
      constexpr QState<std::decay_t<Ts>&...>
        get() & noexcept { return QState<std::decay_t<Ts>&...>(get<Ts>()...); }

    constexpr QState<const std::decay_t<Qs>&...>
      get() const & noexcept { return QState<const std::decay_t<Qs>&...>(get<Qs>()...); }

    template<Traits... Ts> requires(sizeof...(Ts) > 1)
      constexpr QState<const std::decay_t<Ts>&...>
        get() const & noexcept { return QState<const std::decay_t<Ts>&...>(get<Ts>()...); }

    // copy slice by type-name
    template<Traits Q>
      constexpr auto copy() const noexcept { return get<Q>(); }

    constexpr QState<std::decay_t<Qs>...>
      copy() const noexcept { return *this; }

    template<Traits... Ts> requires(sizeof...(Ts) > 1)
      constexpr QState<std::decay_t<Ts>...> copy() const noexcept { return get<Ts...>(); }

    // access and slice by variable (for end-user code)
    template<Traits Q> constexpr auto& operator[](Q) & noexcept { return get<Q>(); }
    template<Traits Q> constexpr auto& operator[](Q) const & noexcept { return get<Q>(); }

    // ref slice by variable
    template<Traits... Ts> requires(sizeof...(Ts) > 0)
      constexpr decltype(auto) get(Ts...) & noexcept { return get<Ts...>(); }

    template<Traits... Ts> requires(sizeof...(Ts) > 0)
      constexpr decltype(auto) get(Ts...) const & noexcept { return get<Ts...>(); }

    // copy slice by variable
    template<Traits... Ts> requires(sizeof...(Ts) > 0)
      constexpr decltype(auto) copy(Ts...) const noexcept { return copy<Ts...>(); }

    // some ops
    template<NotState T>
      constexpr QState& operator=(T);
    constexpr QState& operator=(const State auto &);
    constexpr QState& operator+=(const State auto &);
    constexpr QState& operator-=(const State auto &);

    template<class T> constexpr QState& operator*=(T);
    template<class T> constexpr QState& operator/=(T);

    // unary ops return a copy!
    constexpr QState<std::decay_t<Qs>...> operator-() const;
    constexpr QState<std::decay_t<Qs>...> operator+() const { return *this; }

    // some traits
    template<size_t I> using type_of = details::type_by_index<I, std::decay_t<Qs>...>;

    template<Traits Q> static constexpr size_t index()
    { return details::index_by_type_v<std::decay_t<Q>, std::decay_t<Qs>...>; }

    static constexpr size_t index(const char *id)
    { return details::index_by_id<std::decay_t<Qs>...>(id); }

    static constexpr bool has(const char *id) { return index(id) < ncomps; }
    template<class Q> static constexpr bool has() { return index<Q>() < ncomps; }

    // helpers for QTraits
    static constexpr int size = (Qs::size + ...);
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

  // copy slice
  constexpr decltype(auto) copy(const State auto &s) noexcept { return s.copy(); }

  template<Traits... Qs, State S> requires(sizeof...(Qs) > 0)
    constexpr decltype(auto) copy(const S &s) noexcept { return s.template copy<Qs...>(); }

  // ref slice
  constexpr decltype(auto) get(State auto &s) noexcept { return s.get(); }
  constexpr decltype(auto) get(const State auto &s) noexcept { return s.get(); }

  template<Traits... Qs, State S> requires(sizeof...(Qs) > 0)
    constexpr decltype(auto) get(S &s) noexcept { return s.template get<Qs...>(); }

  template<Traits... Qs, State S> requires(sizeof...(Qs) > 0)
    constexpr decltype(auto) get(const S &s) noexcept { return s.template get<Qs...>(); }
} // namespace Quantities

// IO operations
std::istream& operator>>(std::istream &, Quantities::State auto &);
std::ostream& operator<<(std::ostream &, const Quantities::State auto &);

// binary arithmetic operations
// WARNING: operations are not symmetric, i.e. in general case l + r != r + l
// This is done in order to do arithmetic with states with different set of elements,
// when one of a set is a subset of the other, or when the order of elements is differ.
// For example:
// auto s1 = make_state<rho, Te, w>(...);
// auto s2 = make_state<rho, Te, w, Pi, B>(...);
// auto s3 = s1 + s2; // s3 type is State<rho, Te, w>
// auto s4 = s2 + s1; // COMPILE ERROR!!! s1 doesn't have Pi nor B
constexpr auto operator+(const Quantities::State auto &, const Quantities::State auto &);
constexpr auto operator-(const Quantities::State auto &, const Quantities::State auto &);

template<class T> constexpr auto operator*(const Quantities::State auto &, T);
template<class T> constexpr auto operator*(T v, const Quantities::State auto &s);
template<class T> constexpr auto operator/(const Quantities::State auto &, T);

// boolean operations
// WARNING: operations are not symmetric too, i.e. l == r does not mean that r == l.
constexpr bool operator==(const Quantities::State auto &, const Quantities::State auto &);
constexpr bool operator!=(const Quantities::State auto &l, const Quantities::State auto &r);

/*---------------------------------------------------------------------------------------*/
/*------------------------------------ definitions --------------------------------------*/
/*---------------------------------------------------------------------------------------*/

namespace Quantities
{
  // helper function to create a state using given arguments.
  // in order to preserve the exact types of arguments (ref, const ref or copy)
  // we transfer this information to the template arguments of the state
  // and then, inside the state, restore them to the final types
  // (see data definition in the class QState<Qs...>)
  // Thus we get state with values tagges by the quantity types!
  // NB! It's a user responsibility to correctly match list of tags with list of values!
  template<Traits... Qs, class... Args>
    constexpr auto make_state(Args&&... args) noexcept
  {
    static_assert(sizeof...(Qs) == sizeof...(Args), "incomplete state initialization!");
    return QState<
      std::conditional_t<
        std::is_reference<Args>::value,
        std::conditional_t<
          std::is_const<std::remove_reference_t<Args>>::value,
          const Qs&, Qs&
        >,
        Qs
      >...
    >(std::forward<Args>(args)...);
  }

/*---------------------------------------------------------------------------------------*/

  template<Traits... Qs> template<Traits Q>
    constexpr auto& QState<Qs...>::get() & noexcept
  {
    constexpr auto idx = index<Q>();
    static_assert(idx < ncomps, "quantity is not presented in the state!");
    // it'd be great if name of non-presented quantity will be in the error message...
    // UPD: impossible due to second argument must be a string literal (standard requires)
    return std::get<idx>(data);
  }

  template<Traits... Qs> template<Traits Q>
    constexpr auto& QState<Qs...>::get() const & noexcept
  {
    constexpr auto idx = index<Q>();
    static_assert(idx < ncomps, "quantity is not presented in the state!");
    return std::get<idx>(data);
  }

/*---------------------------------------------------------------------------------------*/

  template<Traits... Qs> template<NotState V>
    constexpr QState<Qs...>& QState<Qs...>::operator=(V value)
  {
    details::set_to(*this, value);
    return *this;
  }

/*---------------------------------------------------------------------------------------*/

  template<Traits... Qs>
    constexpr QState<Qs...>& QState<Qs...>::operator=(const State auto &other)
  {
    details::copy_to(*this, other);
    return *this;
  }

/*---------------------------------------------------------------------------------------*/

  template<Traits... Qs>
    constexpr QState<Qs...>& QState<Qs...>::operator+=(const State auto &other)
  {
    details::add_to(*this, other);
    return *this;
  }

/*---------------------------------------------------------------------------------------*/

  template<Traits... Qs>
    constexpr QState<Qs...>& QState<Qs...>::operator-=(const State auto &other)
  {
    details::sub_from(*this, other);
    return *this;
  }

/*---------------------------------------------------------------------------------------*/

  template<Traits... Qs> template<class T>
    constexpr QState<Qs...>& QState<Qs...>::operator*=(T value)
  {
    details::mult_by(*this, value);
    return *this;
  }

/*---------------------------------------------------------------------------------------*/

  template<Traits... Qs> template<class T>
    constexpr QState<Qs...>& QState<Qs...>::operator/=(T value)
  {
    details::div_by(*this, value);
    return *this;
  }

/*---------------------------------------------------------------------------------------*/

  template<Traits... Qs>
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
  auto s = l.copy();
  Quantities::details::add_to(s, r);
  return s;
}

/*---------------------------------------------------------------------------------------*/

constexpr auto operator-(const Quantities::State auto &l, const Quantities::State auto &r)
{
  auto s = l.copy();
  Quantities::details::sub_from(s, r);
  return s;
}

/*---------------------------------------------------------------------------------------*/

template<class T> constexpr auto operator*(const Quantities::State auto &s, T v)
{
  auto r = s.copy();
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
  auto r = s.copy();
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
  auto c1 = s1; // c1 : QState<rho_t, T_t> => it is a copy!
  auto c2 = s2; // c2 : QState<rho_t&, T_t&> => it is a duplicate!
  c1[T] = 3;    // s1[T] == 2
  c2[T] = 3;    // s2[T] == 3

  // get the real copy using special method
  auto c3 = s1.copy(); // c3 : QState<rho, T>
  auto c4 = copy(s2);  // c4 : QState<rho, T>

  // get the real copies by specifying exact type
  QState<rho_t, T_t> c5 = s1;
  QState<rho_t, T_t> c6 = s2;
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
  s1[rho] = 4;
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
  thus most misusages lead to a compilation error! For example, access to a component
  which is not presented in a state, mixing states with unmatched list of quantities,
  etc...

  \see QDetails.h for implementation details.
*/

#endif // QSTATE_H_INCLUDED
