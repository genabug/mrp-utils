#ifndef QDETAILS_H_INCLUDED
#define QDETAILS_H_INCLUDED

/*!
  \file QDetails.h
  \author gennadiy (gennadiy3.14@gmail.com)
  \brief A bunch of helpers to implement quantities (well, mostly Quantities::QState).
*/

#include <tuple>
#include <utility>
#include <ostream>
#include <istream>
#include <type_traits>

namespace Quantities
{
  template<class... Qs> class QState; // fwd declaration

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
      using type_by_index = std::decay_t<std::tuple_element_t<I, std::tuple<Ts...>>>;

/*---------------------------------------------------------------------------------------*/

    constexpr bool cstr_eq(const char *a, const char *b)
    {
      return (*a && *b)? (*a == *b && cstr_eq(a + 1, b + 1)) : (!*a && !*b);
    }

    static_assert(cstr_eq("one", "one"), "HECK!");
    static_assert(!cstr_eq("one", "two"), "FECK!");

    template<size_t N> constexpr size_t index_by_id_impl(const char *) { return N; }

    template<size_t I, class Q, class... Qs>
      constexpr size_t index_by_id_impl(const char *s)
    {
      return cstr_eq(s, Q::id)? I : index_by_id_impl<I+1, Qs...>(s);
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

    template<class> struct is_state : std::false_type {};
    template<class... Qs> struct is_state<QState<Qs...>> : std::true_type {};
    template<class T> static constexpr bool is_state_v = is_state<std::decay_t<T>>::value;

/*---------------------------------------------------------------------------------------*/

    // compute total number of components of the given quantities
    template<class... Qs> struct ncomps_of { static constexpr size_t value = 0; };

    template<class Q, class... Qs> struct ncomps_of<Q, Qs...> {
      static constexpr size_t value = Q::ncomps + ncomps_of<Qs...>::value; };

    template<class... Qs>
      static constexpr size_t ncomps_of_v = ncomps_of<Qs...>::value;

    // get number of components: if Q is a state then get its ncomps else return 1
    template<class Q>
      constexpr std::enable_if_t<!is_state_v<Q>, size_t> ncomps() { return 1; }
    template<class Q>
      constexpr std::enable_if_t<is_state_v<Q>, size_t> ncomps() { return Q::ncomps; }


    // total size of the given quantities
    template<class... Qs> struct size_of {
      static constexpr size_t value = 0; };

    template<class Q, class... Qs> struct size_of<Q, Qs...> {
      static constexpr size_t value = Q::size + size_of<Qs...>::value; };

    template<class... Qs>
      static constexpr size_t size_of_v = size_of<Qs...>::value;

/*---------------------------------------------------------------------------------------*/

    // Helper structure to get value(s) (by reference!) from a state using type-name(s).
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

    template<class... Qs, class T, class = std::enable_if_t<!is_state_v<T>>>
      constexpr decltype(auto) get(T &&t) noexcept { return std::forward<T>(t); }

    template<class... Qs, class... Ts>
      constexpr decltype(auto) get(QState<Ts...> &t) noexcept {
        return t.template get<Qs...>(); }

    template<class... Qs, class... Ts>
      constexpr decltype(auto) get(const QState<Ts...> &t) noexcept {
        return t.template get<Qs...>(); }

    template<class... Qs, class... Ts>
      constexpr decltype(auto) get(QState<Ts...> &&t) noexcept = delete;

    template<class... Qs, class... Ts>
      constexpr decltype(auto) get(const QState<Ts...> &&t) noexcept = delete;


    template<class... Qs, class T, class = std::enable_if_t<!is_state_v<T>>>
      constexpr T copy(T &&t) noexcept { return t; }

    template<class... Qs, class... Ts>
      constexpr auto copy(QState<Ts...> &t) noexcept {
        return t.template copy<Qs...>(); }

    template<class... Qs, class... Ts>
      constexpr auto copy(const QState<Ts...> &t) noexcept {
        return t.template copy<Qs...>(); }

    // and similar tool to get just a type (Do I really need it???)

    template<class T, class... Qs> struct get_type {
      using type = std::conditional_t<is_state_v<T>, QState<Qs...>, std::decay_t<T>>; };

    template<class T> struct get_type<T> { using type = std::decay_t<T>; };

    template<class T, class Q> struct get_type<T, Q> {
      using type = std::conditional_t<is_state_v<T>, typename Q::type, std::decay_t<T>>; };

    template<class T, class... Qs>
      using get_t = typename get_type<std::decay_t<T>, Qs...>::type;

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
      ostr << s.template get<I>() << ' ';
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

    // State<Rs...> = Ca*State<As...> + Cb*State<Bs...>
    template<size_t I, class Ca, class Cb, class... Rs, class... As, class... Bs>
      constexpr std::enable_if_t<I == sizeof...(Rs)>
        linear_combination(
          QState<Rs...> &,
          Ca, const QState<As...> &,
          Cb, const QState<Bs...> &) {} // stop

    template<size_t I = 0, class Ca, class Cb, class... Rs, class... As, class... Bs>
      constexpr std::enable_if_t<I != sizeof...(Rs)>
        linear_combination(
          QState<Rs...> &r,
          Ca a, const QState<As...> &sa,
          Cb b, const QState<Bs...> &sb )
    {
      using R = type_by_index<I, Rs...>;
      r.template get<R>() = a * sa.template get<R>() + b * sb.template get<R>();
      linear_combination<I + 1>(r, a, sa, b, sb);
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
      s.template get<I>() = static_cast<Q>(value);
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
      return (l.template get<L>() == r.template get<L>()) && equal<I + 1>(l, r);
    }

  } // namespace details
} // namespace Quantities

#endif // QDETAILS_H_INCLUDED
