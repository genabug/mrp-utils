#ifndef QDETAILS_H_INCLUDED
#define QDETAILS_H_INCLUDED

/*!
  \file QDetails.h
  \author gennadiy
  \brief A bunch of helpers to implement Quantities::QState.
*/

#include "Utils.h"

#include <tuple>
#include <ostream>
#include <istream>
#include <type_traits>

namespace Quantities::details
{
  // implementation of a helper indexers
  template<size_t N, class T, class... Ts>
  struct index_by_type_impl {
    static constexpr size_t value = N; }; // stop

  template<size_t I, class T, class Ti, class... Ts>
    struct index_by_type_impl<I, T, Ti, Ts...> {
      static constexpr size_t value =
        std::is_same_v<T, Ti>? I : index_by_type_impl<I + 1, T, Ts...>::value; };

  // helper structure to enumerate quantities in a state (Ts...)
  // and get the index of a given one (T)
  template<class T, class... Ts>
    struct index_by_type {
      static constexpr size_t value = index_by_type_impl<0, T, Ts...>::value; };
  template<class T, class... Ts>
    static constexpr size_t index_by_type_v = index_by_type<T, Ts...>::value;

  // just a useful shortcut for symmetry -- get i-th type in a pack
  template<size_t I, class... Ts>
    using type_by_index = std::tuple_element_t<I, std::tuple<Ts...>>;

/*---------------------------------------------------------------------------------------*/

  template<size_t N> constexpr size_t index_by_id_impl(const char *) { return N; }

  template<size_t I, class Q, class... Qs>
    constexpr size_t index_by_id_impl(const char *s)
  {
    return Utils::cstr_equal(s, Q::id)? I : index_by_id_impl<I+1, Qs...>(s);
  }

  // This function may be used to implement access by name (not type-name)
  // though it's not practical/useful I think. Indeed, the name should be constexpr
  // (e.g. you can't use name read from a file) and thus there is no big difference
  // with the type-name. But who knows, may be someone find such access convenient.
  template<class... Qs> constexpr size_t index_by_id(const char *s)
  {
    return index_by_id_impl<0, Qs...>(s);
    }

  // helper variable to obtain list of state components
  template<class... Qs>
    static constexpr std::initializer_list<const char *> quantity_names = {Qs::id...};

/*---------------------------------------------------------------------------------------*/

  // pretty printer
  template<size_t I = 0, class S>
    void print_state(std::ostream &out, const S &s)
  {
    if constexpr (I == s.ncomps)
    {
      out << '}';
      return;
    }

    using Q = typename S::template type_of<I>;
    out << (I? ", " : "{") << Q::id << ": " << s.template get<I>();
    print_state<I + 1>(out, s);
  }

/*---------------------------------------------------------------------------------------*/

  template<size_t I = 0, class S>
    void write_state(std::ostream &out, const S &s)
  {
    if constexpr (I != S::ncomps)
    {
      out << (I? " " : "") << s.template get<I>();
      write_state<I + 1>(out, s);
    }
  }

/*---------------------------------------------------------------------------------------*/

  template<size_t I = 0, class S>
    void read_state(std::istream &in, S &s)
  {
    if constexpr (I != S::ncomps)
    {
      in >> s.template get<I>();
      read_state<I + 1>(in, s);
    }
  }

/*---------------------------------------------------------------------------------------*/

  template<size_t I = 0, class L, class R>
    void add_to(L &l, const R &r)
  {
    if constexpr (I != L::ncomps)
    {
      using T = typename L::template type_of<I>;
      static_assert(R::template has<T>(), "right-hand state doesn't have a quantity");
      l.template get<T>() += r.template get<T>();
      add_to<I + 1>(l, r);
    }
  }

/*---------------------------------------------------------------------------------------*/

  template<size_t I = 0, class L, class R>
    void sub_from(L &l, const R &r)
  {
    if constexpr (I != L::ncomps)
    {
      using T = typename L::template type_of<I>;
      static_assert(R::template has<T>(), "right-hand state doesn't have a quantity");
      l.template get<T>() -= r.template get<T>();
      sub_from<I + 1>(l, r);
    }
  }

/*---------------------------------------------------------------------------------------*/

  template<size_t I = 0, class L, class R>
    void copy_to(L &l, const R &r)
  {
    if constexpr (I != L::ncomps)
    {
      using T = typename L::template type_of<I>;
      static_assert(R::template has<T>(), "right-hand state doesn't have a quantity");
      l.template get<T>() = r.template get<T>();
      copy_to<I + 1>(l, r);
    }
  }

/*---------------------------------------------------------------------------------------*/

  template<size_t I = 0, class S, class T>
    void mult_by(S &s, T coeff)
  {
    if constexpr (I != S::ncomps)
    {
      s.template get<I>() *= coeff;
      mult_by<I + 1>(s, coeff);
    }
  }

/*---------------------------------------------------------------------------------------*/

  template<size_t I = 0, class S, class T>
    void div_by(S &s, T coeff)
  {
    if constexpr (I != S::ncomps)
    {
      s.template get<I>() /= coeff;
      div_by<I + 1>(s, coeff);
    }
  }

/*---------------------------------------------------------------------------------------*/

  template<size_t I = 0, class S, class T>
    void set_to(S &s, T value)
  {
    if constexpr (I != S::ncomps)
    {
      using Q = typename S::template type_of<I>::type;
      s.template get<I>() = static_cast<Q>(value);
      set_to<I + 1>(s, value);
    }
  }

/*---------------------------------------------------------------------------------------*/

  template<size_t I = 0, class L, class R>
    bool equal(const L &l, const R &r)
  {
    if constexpr (I != L::ncomps)
    {
      using T = typename L::template type_of<I>;
      static_assert(R::template has<T>(), "right-hand state doesn't have a quantity");
      return (l.template get<T>() == r.template get<T>()) && equal<I + 1>(l, r);
    }
    else
      return true;
  }

} // namespace Quantities::details

#endif // QDETAILS_H_INCLUDED
