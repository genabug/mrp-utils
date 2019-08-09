#ifndef QDETAILS_H_INCLUDED
#define QDETAILS_H_INCLUDED

/*!
  \file QDetails.h
  \author gennadiy (gennadiy3.14@gmail.com)
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

  template<class> struct is_state : std::false_type {};
  template<class... Qs> struct is_state<QState<Qs...>> : std::true_type {};
  template<class T> static constexpr bool is_state_v = is_state<std::decay_t<T>>::value;

  namespace details
  {
    // implementation of helper indexers
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

    // total number of components of the given quantities
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
    template<size_t I, class... Qs>
      std::enable_if_t<I == sizeof...(Qs)>
        print_state(std::ostream &, const QState<Qs...> &) {} // stop

    template<size_t I = 0, class... Qs>
      std::enable_if_t<I != sizeof...(Qs)>
        print_state(std::ostream &ostr, const QState<Qs...> &s)
    {
      using Q = type_by_index<I, Qs...>;
      ostr << (I? "" : "state:\n") << Q::id << ": " << s.template get<I>() << '\n';
      print_state<I + 1>(ostr, s);
    }

/*---------------------------------------------------------------------------------------*/

    template<size_t I, class... Qs>
      std::enable_if_t<I == sizeof...(Qs)>
        write_state(std::ostream &, const QState<Qs...> &) {} // stop

    template<size_t I = 0, class... Qs>
      std::enable_if_t<I != sizeof...(Qs)>
        write_state(std::ostream &ostr, const QState<Qs...> &s)
    {
      ostr << (I? " " : "") << s.template get<I>();
      write_state<I + 1>(ostr, s);
    }

/*---------------------------------------------------------------------------------------*/

    template<size_t I, class... Qs>
      std::enable_if_t<I == sizeof...(Qs)>
        read_state(std::istream &, QState<Qs...> &) {} // stop

    template<size_t I = 0, class... Qs>
      std::enable_if_t<I != sizeof...(Qs)>
        read_state(std::istream &istr, QState<Qs...> &s)
    {
      istr >> s.template get<I>();
      read_state<I + 1>(istr, s);
    }

/*---------------------------------------------------------------------------------------*/

    template<size_t I, class... Ls, class... Rs>
      std::enable_if_t<I == sizeof...(Ls)>
        constexpr add_to(QState<Ls...> &, const QState<Rs...> &) {} // stop

    template<size_t I = 0, class... Ls, class... Rs>
      std::enable_if_t<I != sizeof...(Ls)>
        constexpr add_to(QState<Ls...> &self, const QState<Rs...> &other)
    {
      using L = type_by_index<I, Ls...>;
      static_assert(other.template has<L>(), "other state doesn't have enough quantities");
      self.template get<L>() += other.template get<L>();
      add_to<I + 1>(self, other);
    }

/*---------------------------------------------------------------------------------------*/

    template<size_t I, class... Ls, class... Rs>
      constexpr std::enable_if_t<I == sizeof...(Ls)>
        sub_from(QState<Ls...> &, const QState<Rs...> &) {} // stop

    template<size_t I = 0, class... Ls, class... Rs>
      constexpr std::enable_if_t<I != sizeof...(Ls)>
        sub_from(QState<Ls...> &self, const QState<Rs...> &other)
    {
      using L = type_by_index<I, Ls...>;
      static_assert(other.template has<L>(), "other state doesn't have enough quantities");
      self.template get<L>() -= other.template get<L>();
      sub_from<I + 1>(self, other);
    }

/*---------------------------------------------------------------------------------------*/

    template<size_t I, class... Ls, class... Rs>
      constexpr std::enable_if_t<I == sizeof...(Ls)>
        copy_to(QState<Ls...> &, const QState<Rs...> &) {} // stop

    template<size_t I = 0, class... Ls, class... Rs>
      constexpr std::enable_if_t<I != sizeof...(Ls)>
        copy_to(QState<Ls...> &self, const QState<Rs...> &other)
    {
      using L = type_by_index<I, Ls...>;
      static_assert(other.template has<L>(), "other state doesn't have emough quantities");
      self.template get<L>() = other.template get<L>();
      copy_to<I + 1>(self, other);
    }

/*---------------------------------------------------------------------------------------*/

    template<size_t I, class Coeff, class... Qs>
      constexpr std::enable_if_t<I == sizeof...(Qs)>
        mult_by(QState<Qs...> &, const Coeff &) {} // stop

    template<size_t I = 0, class Coeff, class... Qs>
      constexpr std::enable_if_t<I != sizeof...(Qs)>
        mult_by(QState<Qs...> &self, const Coeff &coeff)
    {
      self.template get<I>() *= coeff;
      mult_by<I + 1>(self, coeff);
    }

/*---------------------------------------------------------------------------------------*/

    template<size_t I, class Coeff, class... Qs>
      constexpr std::enable_if_t<I == sizeof...(Qs)>
        div_by(QState<Qs...> &, Coeff) {} // stop

    template<size_t I = 0, class Coeff, class... Qs>
      constexpr std::enable_if_t<I != sizeof...(Qs)>
        div_by(QState<Qs...> &self, Coeff coeff)
    {
      self.template get<I>() /= coeff;
      div_by<I + 1>(self, coeff);
    }

/*---------------------------------------------------------------------------------------*/

    template<size_t I, class Value, class... Qs>
      constexpr std::enable_if_t<I == sizeof...(Qs)>
        set_to(QState<Qs...> &, Value) {} // stop

    template<size_t I = 0, class Value, class... Qs>
      constexpr std::enable_if_t<I != sizeof...(Qs)>
        set_to(QState<Qs...> &s, Value value)
    {
      using Q = typename type_by_index<I, Qs...>::type;
      s.template get<I>() = Q{value};
      set_to<I + 1>(s, value);
    }

/*---------------------------------------------------------------------------------------*/

    template<size_t I, class... Ls, class... Rs>
      constexpr std::enable_if_t<I == sizeof...(Ls), bool>
        equal(const QState<Ls...> &, const QState<Rs...> &) { return true; } // stop

    template<size_t I = 0, class... Ls, class... Rs>
      constexpr std::enable_if_t<I != sizeof...(Ls), bool>
        equal(const QState<Ls...> &l, const QState<Rs...> &r)
    {
      using L = type_by_index<I, Ls...>;
      static_assert(r.template has<L>(), "right state doesn't have enough quantities");
      return (l.template get<L>() == r.template get<L>()) && equal<I + 1>(l, r);
    }

  } // namespace details
} // namespace Quantities

#endif // QDETAILS_H_INCLUDED
