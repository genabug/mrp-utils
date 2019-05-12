#ifndef QSTATE_H_INCLUDED
#define QSTATE_H_INCLUDED

/*!
  \file QState.h
  \author gennadiy (gennadiy3.14@gmail.com)
  \brief Definition of class to represent vector of quantities (state).
*/

#include "QDetails.h"

namespace Quantities
{

  template<class... Qs> class QState
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
    > data;

  public:
    template<class... Args>
      constexpr QState(Args&&... args)
        : data(std::forward<Args>(args)...) {}

    template<class... Ts>
      constexpr QState(QState<Ts...> &&s)
        : data(std::move(s).template get<std::decay_t<Qs>>()...) {}

    template<class Arg>
      constexpr QState(Arg &&arg) { details::set_to(*this, std::forward<Arg>(arg)); }

    // copy ctors
    // WARNING: be careful when use them with auto due to specifics of implementation
    // (see make_state for details). For example:
    //   auto s = make_state<rho, w>(r, v); // r and v are lvalues => s : QState<rho&, w&>
    //   auto sc = s; // sc : QState<rho&, w&> => it is NOT a copy!
    //
    // It can be really confusing especially when compare with this code:
    //   auto s = make_state<rho, w>(1, 2); // arguments are rvalues => s : QState<rho, w>
    //   auto sc = s; // sc : QState<rho, w> => it is a copy!
    // So use 'auto' with care! Be verbose and specify the exact type:
    //   QState<rho, w> sc = s; // always copy!
    // or use the corresponding copy methods
    //   auto sc = s.copy();
    // in order to get a real copy.
    template<class... Ts>
      constexpr QState(QState<Ts...> &s)
        : data(s.template get<std::decay_t<Qs>>()...) {}

    template<class... Ts>
      constexpr QState(const QState<Ts...> &s)
        : data(s.template copy<std::decay_t<Qs>>()...) {}

    // access by reference using index (mainly to implement basic ops, see below)
    template<size_t I>
      constexpr decltype(auto) get() & noexcept { return std::get<I>(data); }
    template<size_t I>
      constexpr decltype(auto) get() const & noexcept { return std::get<I>(data); }

    template<size_t I>
      constexpr decltype(auto) get() && noexcept { return std::get<I>(std::move(data)); }
    template<size_t I>
      constexpr decltype(auto) get() const && noexcept { return std::get<I>(std::move(data)); }


    // TODO: consider less verbose access syntax, smth like s(q) or s[q]
    // is it possible at all? for example, using global constants, not types.
    constexpr decltype(auto) operator[](const char *id) noexcept
    {
      constexpr auto idx = details::index_by_id<Qs...>(id);
      static_assert(idx < sizeof...(Qs), "quantity is not presented in the state!");
      return std::get<idx>(data);
    }
    // DOES NOT WORK! Why? Because id is not a constant expression, thats why!
    // auto s = make_state<Q, ...>(q, ...);
    // s[Q::id] = 1; // COMPILE ERROR!
    // s["rho"] = 2; // same shit!
    // Why it's not a constant expression? Because id has external lifetime
    // __inside__ operator[] thus you can't prove it is a constant expression.
    //
    // Let me quote the standard:
    // 5.19 Constant expressions [expr.const]
    //
    // 2 A conditional-expression e is a core constant expression
    // unless the evaluation of e, following the rules of the abstract machine (1.9),
    // would evaluate one of the following expressions:
    // - an id-expression that refers to a variable or data member of reference type
    //   unless the reference has a preceding initialization and either
    // - it is initialized with a constant expression or
    // - it is a non-static data member of an object
    //   whose lifetime began within the evaluation of e; (sic!)
    //
    // This section has the following code example:
    //
    // constexpr int f1(int k)
    // {
    //   constexpr int x = k; // error: x is not initialized by a constant expression
    //                        // because lifetime of k began outside the initializer of x
    //   return x;
    // }
    //
    // Because x is not a constant expression,
    // you can't e.g. instantiate templates with either x or k inside f1.
    //
    // Back to the operator[], idx is not a constant expression because id is not!


    // access by reference using type-name
    // have to use common template pack with specialization for three cases
    // in order to simplify generic code
    template<class Q> constexpr auto& get() & noexcept;
    template<class Q> constexpr const auto& get() const & noexcept;
    template<class Q> constexpr auto&& get() && noexcept;
    // const && overload. yep, strange! see https://cplusplus.github.io/LWG/issue2485
    template<class Q> constexpr const auto&& get() const && noexcept;

    // refs to all elements
    template<class... Ts>
      constexpr std::enable_if_t<sizeof...(Ts) == 0,
        QState<std::decay_t<Qs>&...>> get() &;
    template<class... Ts>
      constexpr std::enable_if_t<sizeof...(Ts) == 0,
        QState<const std::decay_t<Qs>&...>> get() const &;
    template<class... Ts>
      constexpr std::enable_if_t<sizeof...(Ts) == 0,
        QState<std::decay_t<Qs>&...>&&> get() && = delete; // no refs to temporary objs!
    template<class... Ts>
      constexpr std::enable_if_t<sizeof...(Ts) == 0,
        QState<const std::decay_t<Qs>&...>&&> get() const && = delete; // --//--

    // refs to a slice
    template<class... Ts>
      constexpr std::enable_if_t<sizeof...(Ts) >= 2,
        QState<Ts&...>> get() &;
    template<class... Ts>
      constexpr std::enable_if_t<sizeof...(Ts) >= 2,
        QState<const Ts&...>> get() const &;
    template<class... Ts>
      constexpr std::enable_if_t<sizeof...(Ts) >= 2,
        QState<Ts&...>&&> get() && = delete; // --//--
    template<class... Ts>
      constexpr std::enable_if_t<sizeof...(Ts) >= 2,
        QState<const Ts&...>&&> get() const && = delete;

    // access by value using type-name
    // same shit about generic code here...
    template<class Q>
      constexpr auto copy() const { return get<Q>(); }

    template<class... Ts> // full copy
      constexpr std::enable_if_t<sizeof...(Ts) == 0,
        QState<std::decay_t<Qs>...>> copy() const;

    template<class... Ts> // partial copy (slice)
      constexpr std::enable_if_t<sizeof...(Ts) >= 2,
        QState<Ts...>> copy() const;

    // some ops
    template<class T> constexpr QState<Qs...>& operator=(T);
    template<class... Ts> constexpr QState<Qs...>& operator=(const QState<Ts...> &);
    template<class... Ts> constexpr QState<Qs...>& operator+=(const QState<Ts...> &);
    template<class... Ts> constexpr QState<Qs...>& operator-=(const QState<Ts...> &);

    constexpr QState<Qs...>& operator*=(double);
    constexpr QState<Qs...>& operator/=(double);

    constexpr QState<std::decay_t<Qs>...> operator-() const; // NB! returns a copy!
    constexpr QState<Qs...>& operator+() { return *this; }
    constexpr const QState<Qs...>& operator+() const { return *this; }

    // some traits
    template<class Q>
      constexpr size_t index() const { return details::index_by_type_v<Q, Qs...>; }

    constexpr size_t index(const char *name) const
    {
      return details::index_by_id<Qs...>(name);
    }

    static constexpr std::initializer_list<const char *> names =
      details::quantity_names<std::decay_t<Qs>...>;

    // use helper variables to guarantee correctness in case of nested states
    static constexpr int size = details::size_of_v<std::decay_t<Qs>...>;
    static constexpr int ncomps = details::ncomps_of_v<std::decay_t<Qs>...>;
  }; // class QState<Qs...>

  // a static member must be defined outside of the struct
  template<class... Qs>
    constexpr std::initializer_list<const char *> QState<Qs...>::names;

} //namespace Solvers

// IO operations

template<class... Qs>
  std::ostream& operator<<(std::ostream &, const Quantities::QState<Qs...> &);

template<class... Qs>
  std::istream& operator>>(std::istream &, Quantities::QState<Qs...> &);

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

template<class... Qs>
  constexpr auto operator*(const Quantities::QState<Qs...> &, double);

template<class... Qs>
  constexpr auto operator*(double v, const Quantities::QState<Qs...> &s) { return s * v; }

template<class... Qs>
  constexpr auto operator/(const Quantities::QState<Qs...> &, double);

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
  // helper function to create a state using given arguments.
  // in order to preserve the exact types of arguments (ref, const ref or simple one)
  // we transfer this information to the template arguments of the state
  // and then, inside the state, restore them to the final types
  // (see data definition in the class State<Qs...>)
  // Thus we get state with values tagges by the quantity types!
  // NB! It's a user responsibility to correctly match list of tags with list values!!!
  template<class... Qs, class... Args> constexpr auto make_state(Args&&... args)
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

  template<class... Qs>
    template<class Q> constexpr auto& QState<Qs...>::get() & noexcept
  {
    constexpr auto idx = details::index_by_type_v<std::decay_t<Q>, std::decay_t<Qs>...>;
    static_assert(idx < sizeof...(Qs), "quantity is not presented in the state!");
    return std::get<idx>(data);
  }

  template<class... Qs>
    template<class Q> constexpr const auto& QState<Qs...>::get() const & noexcept
  {
    constexpr auto idx = details::index_by_type_v<std::decay_t<Q>, std::decay_t<Qs>...>;
    static_assert(idx < sizeof...(Qs), "quantity is not presented in the state!");
    // it'll be good if name of non-presented quantity will be in the error message...
    // UPD: impossible due to second argument must be a string literal (standard requires)
    return std::get<idx>(data);
  }

  template<class... Qs>
    template<class Q> constexpr auto&& QState<Qs...>::get() && noexcept
  {
    constexpr auto idx = details::index_by_type_v<std::decay_t<Q>, std::decay_t<Qs>...>;
    static_assert(idx < sizeof...(Qs), "quantity is not presented in the state!");
    return std::get<idx>(std::move(data));
  }

  template<class... Qs>
    template<class Q> constexpr const auto&& QState<Qs...>::get() const && noexcept
  {
    constexpr auto idx = details::index_by_type_v<std::decay_t<Q>, std::decay_t<Qs>...>;
    static_assert(idx < sizeof...(Qs), "quantity is not presented in the state!");
    return std::get<idx>(std::move(data));
  }

/*---------------------------------------------------------------------------------------*/

  template<class... Qs> template<class... Ts>
    constexpr std::enable_if_t<sizeof...(Ts) >= 2,
      QState<Ts&...>> QState<Qs...>::get() &
  {
    return QState<Ts&...>(get<Ts>()...);
  }

  template<class... Qs> template<class... Ts>
    constexpr std::enable_if_t<sizeof...(Ts) >= 2,
      QState<const Ts&...>> QState<Qs...>::get() const &
  {
    return QState<const Ts&...>(get<Ts>()...);
  }

  template<class... Qs> template<class... Ts>
    constexpr std::enable_if_t<sizeof...(Ts) == 0,
      QState<std::decay_t<Qs>&...>> QState<Qs...>::get() &
  {
    return QState<std::decay_t<Qs>&...>(get<Qs>()...);
  }

  template<class... Qs> template<class... Ts>
    constexpr std::enable_if_t<sizeof...(Ts) == 0,
      QState<const std::decay_t<Qs>&...>> QState<Qs...>::get() const &
  {
    return QState<const std::decay_t<Qs>&...>(get<Qs>()...);
  }

/*---------------------------------------------------------------------------------------*/

  template<class... Qs> template<class... Ts>
    constexpr std::enable_if_t<sizeof...(Ts) == 0,
      QState<std::decay_t<Qs>...>> QState<Qs...>::copy() const
  {
    return QState<std::decay_t<Qs>...>(copy<std::decay_t<Qs>>()...);
  }

  template<class... Qs> template<class... Ts>
    constexpr std::enable_if_t<sizeof...(Ts) >= 2,
      QState<Ts...>> QState<Qs...>::copy() const
  {
    return QState<Ts...>(copy<Ts>()...);
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

  template<class... Qs>
    constexpr QState<Qs...>& QState<Qs...>::operator*=(double value)
  {
    details::mult_by(*this, value);
    return *this;
  }

/*---------------------------------------------------------------------------------------*/

  template<class... Qs>
    constexpr QState<Qs...>& QState<Qs...>::operator/=(double value)
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

template<class... Qs>
  constexpr auto operator*(const Quantities::QState<Qs...> &s, double v)
{
  auto r = s.copy();
  Quantities::details::mult_by(r, v);
  return r;
}

/*---------------------------------------------------------------------------------------*/

template<class... Qs>
  constexpr auto operator/(const Quantities::QState<Qs...> &s, double v)
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

#endif // QSTATE_H_INCLUDED
