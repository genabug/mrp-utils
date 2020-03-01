#ifndef QDETAILS_H_INCLUDED
#define QDETAILS_H_INCLUDED

/*!
  \file QDetails.h
  \author gennadiy
  \brief A bunch of helpers to implement quantities (well, mostly Quantities::QState).
*/

#include "Utils.h"
#include <tuple>
#include <utility>
#include <ostream>
#include <istream>
#include <type_traits>

namespace Quantities
{
  template<class... Qs> class QState; // fwd declaration

  template<class...> struct is_state : std::false_type {};
  template<class... Qs> struct is_state<QState<Qs...>> : std::true_type {};
  template<class T> static constexpr bool is_state_v = is_state<std::decay_t<T>>::value;

  template<class...> struct enable_if {};
  template<class T> struct enable_if<T> : std::enable_if<is_state_v<T>> {};
  template<class L, class R>
    struct enable_if<L, R> : std::enable_if<is_state_v<L> && is_state_v<R>> {};

  template<class... Ts> using enable_if_s = typename enable_if<Ts...>::type;

  // workaround for buggy icc17
  template<class T> static constexpr bool is_not_state_v = !is_state_v<T>;
  template<class T> using enable_if_not_s = std::enable_if_t<is_not_state_v<T>>;

  namespace details
  {
    // implementation of a helper indexers
    template<size_t N, class T, class... Ts>
      struct index_by_type_impl {
        static constexpr size_t value = N; }; // stop

    template<size_t I, class T, class Ti, class... Ts>
      struct index_by_type_impl<I, T, Ti, Ts...> {
        static constexpr size_t value =
          std::is_same<T, Ti>::value? I : index_by_type_impl<I + 1, T, Ts...>::value; };

    // helper structure to enumerate quantities in a state (Ts...)
    // and get the index of a given one (T)
    template<class T, class... Ts>
      struct index_by_type {
        static constexpr size_t value = index_by_type_impl<0, T, Ts...>::value; };

    // helper variable -- index of a given quantity (T) in a state (Ts...)
    template<class T, class... Ts>
      static constexpr size_t index_by_type_v = index_by_type<T, Ts...>::value;

    // just a useful shortcut -- get i-th type in a pack
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
    // with the type-name. But who knows, may be someone find such access more pleasant...
    template<class... Qs> constexpr size_t index_by_id(const char *s)
    {
      return index_by_id_impl<0, Qs...>(s);
    }

    // helper variable to obtain list of state components
    template<class... Qs>
      static constexpr std::initializer_list<const char *> quantity_names = {Qs::id...};

/*---------------------------------------------------------------------------------------*/

    // compute total number of components of the given quantities
    template<class... Qs> struct ncomps_of { static constexpr size_t value = 0; };

    template<class Q, class... Qs> struct ncomps_of<Q, Qs...> {
      static constexpr size_t value = Q::ncomps + ncomps_of<Qs...>::value; };

    template<class... Qs> static constexpr size_t ncomps_of_v = ncomps_of<Qs...>::value;


    // total size of the given quantities
    template<class... Qs> struct size_of { static constexpr size_t value = 0; };

    template<class Q, class... Qs> struct size_of<Q, Qs...> {
      static constexpr size_t value = Q::size + size_of<Qs...>::value; };

    template<class... Qs> static constexpr size_t size_of_v = size_of<Qs...>::value;

/*---------------------------------------------------------------------------------------*/

    // pretty printer
    template<size_t I, class S, class = enable_if_s<S>>
      std::enable_if_t<I == S::ncomps>
        print_state(std::ostream &out, const S &) { out << "}"; } // stop

    template<size_t I = 0, class S, class = enable_if_s<S>>
      std::enable_if_t<I != S::ncomps>
        print_state(std::ostream &out, const S &s)
    {
      using Q = typename S::template type_of<I>;
      out << (I? ", " : "{") << Q::id << ": " << s.template get<I>();
      print_state<I + 1>(out, s);
    }

/*---------------------------------------------------------------------------------------*/

    template<size_t I, class S, class = enable_if_s<S>>
      std::enable_if_t<I == S::ncomps>
        write_state(std::ostream &, const S &) {} // stop

    template<size_t I = 0, class S, class = enable_if_s<S>>
      std::enable_if_t<I != S::ncomps>
        write_state(std::ostream &out, const S &s)
    {
      out << (I? " " : "") << s.template get<I>();
      write_state<I + 1>(out, s);
    }

/*---------------------------------------------------------------------------------------*/

    template<size_t I, class S, class = enable_if_s<S>>
      std::enable_if_t<I == S::ncomps>
        read_state(std::istream &, S &) {} // stop

    template<size_t I = 0, class S, class = enable_if_s<S>>
      std::enable_if_t<I != S::ncomps>
        read_state(std::istream &in, S &s)
    {
      in >> s.template get<I>();
      read_state<I + 1>(in, s);
    }

/*---------------------------------------------------------------------------------------*/

    template<size_t I, class L, class R, class = enable_if_s<L, R>>
      constexpr std::enable_if_t<I == L::ncomps>
        add_to(L &, const R &) {} // stop

    template<size_t I = 0, class L, class R, class = enable_if_s<L, R>>
      constexpr std::enable_if_t<I != L::ncomps>
        add_to(L &l, const R &r)
    {
      using T = typename L::template type_of<I>;
      static_assert(R::template has<T>(), "other state doesn't have enough quantities");
      l.template get<T>() += r.template get<T>();
      add_to<I + 1>(l, r);
    }

/*---------------------------------------------------------------------------------------*/

    template<size_t I, class L, class R, class = enable_if_s<L, R>>
      constexpr std::enable_if_t<I == L::ncomps>
        sub_from(L &, const R &) {} // stop

    template<size_t I = 0, class L, class R, class = enable_if_s<L, R>>
      constexpr std::enable_if_t<I != L::ncomps>
        sub_from(L &l, const R &r)
    {
      using T = typename L::template type_of<I>;
      static_assert(R::template has<T>(), "other state doesn't have enough quantities");
      l.template get<T>() -= r.template get<T>();
      sub_from<I + 1>(l, r);
    }

/*---------------------------------------------------------------------------------------*/

    template<size_t I, class L, class R, class = enable_if_s<L, R>>
      constexpr std::enable_if_t<I == L::ncomps>
        copy_to(L &, const R &) {} // stop

    template<size_t I = 0, class L, class R, class = enable_if_s<L, R>>
      constexpr std::enable_if_t<I != L::ncomps>
        copy_to(L &l, const R &r)
    {
      using T = typename L::template type_of<I>;
      static_assert(R::template has<T>(), "other state doesn't have enough quantities");
      l.template get<T>() = r.template get<T>();
      copy_to<I + 1>(l, r);
    }

/*---------------------------------------------------------------------------------------*/

    template<size_t I, class S, class T, class = enable_if_s<S>>
      constexpr std::enable_if_t<I == S::ncomps>
        mult_by(S &, T) {} // stop

    template<size_t I = 0, class S, class T, class = enable_if_s<S>>
      constexpr std::enable_if_t<I != S::ncomps>
        mult_by(S &s, T coeff)
    {
      s.template get<I>() *= coeff;
      mult_by<I + 1>(s, coeff);
    }

/*---------------------------------------------------------------------------------------*/

    template<size_t I, class S, class T, class = enable_if_s<S>>
      constexpr std::enable_if_t<I == S::ncomps>
        div_by(S &, T) {} // stop

    template<size_t I = 0, class S, class T, class = enable_if_s<S>>
      constexpr std::enable_if_t<I != S::ncomps>
        div_by(S &s, T coeff)
    {
      s.template get<I>() /= coeff;
      div_by<I + 1>(s, coeff);
    }

/*---------------------------------------------------------------------------------------*/

    template<size_t I, class S, class T, class = enable_if_s<S>>
      constexpr std::enable_if_t<I == S::ncomps>
        set_to(S &, T) {} // stop

    template<size_t I = 0, class S, class T, class = enable_if_s<S>>
      constexpr std::enable_if_t<I != S::ncomps>
        set_to(S &s, T value)
    {
      using Q = typename S::template type_of<I>::type;
      s.template get<I>() = static_cast<Q>(value);
      set_to<I + 1>(s, value);
    }

/*---------------------------------------------------------------------------------------*/

    template<size_t I, class L, class R, class = enable_if_s<L, R>>
      constexpr std::enable_if_t<I == L::ncomps, bool>
        equal(const L &, const R &) { return true; } // stop

    template<size_t I = 0, class L, class R, class = enable_if_s<L, R>>
      constexpr std::enable_if_t<I != L::ncomps, bool>
        equal(const L &l, const R &r)
    {
      using T = typename L::template type_of<I>;
      static_assert(R::template has<T>(), "right state doesn't have enough quantities");
      return (l.template get<T>() == r.template get<T>()) && equal<I + 1>(l, r);
    }

  } // namespace details
} // namespace Quantities

#endif // QDETAILS_H_INCLUDED
