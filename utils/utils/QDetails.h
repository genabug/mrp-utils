#ifndef QDETAILS_H_INCLUDED
#define QDETAILS_H_INCLUDED

/*!
  \file QDetails.h
  \author gennadiy
  \brief A bunch of helpers to implement Quantities::QState.
*/

#include "QFwds.h"

#include <tuple>
#include <ostream>
#include <istream>

namespace Quantities::details
{
  template<size_t N, class Q, class... Qs>
    struct index_of_impl
    {
      static constexpr size_t value = N;
    };

  template<size_t I, class Q, class Qi, class... Qs>
    struct index_of_impl<I, Q, Qi, Qs...>
    {
      static constexpr size_t value =
        std::is_same_v<Q, Qi>? I : index_of_impl<I + 1, Q, Qs...>::value;
    };

  template<class Q, class... Qs>
    constexpr size_t index_of = index_of_impl<0, Q, Qs...>::value;

  template<size_t I, class... Qs>
    using type_of = std::tuple_element_t<I, std::tuple<Qs...>>;

  template<class Q, class... Qs>
    constexpr bool has = sizeof...(Qs) > index_of<Q, Qs...>;

  template<class... Qs>
    constexpr std::initializer_list<const char *> qnames = {Qs::id...};

/*---------------------------------------------------------------------------------------*/

  // pretty printer
  template<size_t I = 0, State S>
    void print_state(std::ostream &out, const S &state)
  {
    if constexpr (I == S::ncomps)
    {
      out << '}';
      return;
    }

    using Q = S::template type_of<I>;
    out << (I? ", " : "{") << Q::id << ": " << state.template get<I>();
    print_state<I + 1>(out, state);
  }

/*---------------------------------------------------------------------------------------*/

  template<size_t I = 0, State S>
    void write_state(std::ostream &out, const S &state)
  {
    if constexpr (I != S::ncomps)
    {
      out << (I? " " : "") << state.template get<I>();
      write_state<I + 1>(out, state);
    }
  }

/*---------------------------------------------------------------------------------------*/

  template<size_t I = 0, State S>
    void read_state(std::istream &in, S &state)
  {
    if constexpr (I != S::ncomps)
    {
      in >> state.template get<I>();
      read_state<I + 1>(in, state);
    }
  }

/*---------------------------------------------------------------------------------------*/

  template<size_t I = 0, State L, State R>
    void add_to(L &lstate, const R &rstate) noexcept
  {
    if constexpr (I != L::ncomps)
    {
      using T = L::template type_of<I>;
      static_assert(R::template has<T>, "right-hand state doesn't have a quantity");
      lstate.template get<T>() += rstate.template get<T>();
      add_to<I + 1>(lstate, rstate);
    }
  }

/*---------------------------------------------------------------------------------------*/

  template<size_t I = 0, State L, State R>
    void sub_from(L &lstate, const R &rstate) noexcept
  {
    if constexpr (I != L::ncomps)
    {
      using T = L::template type_of<I>;
      static_assert(R::template has<T>, "right-hand state doesn't have a quantity");
      lstate.template get<T>() -= rstate.template get<T>();
      sub_from<I + 1>(lstate, rstate);
    }
  }

/*---------------------------------------------------------------------------------------*/

  template<size_t I = 0, State S, class T> requires(!is_state_v<T>)
    void mult_by(S &state, T value) noexcept
  {
    if constexpr (I != S::ncomps)
    {
      state.template get<I>() *= value;
      mult_by<I + 1>(state, value);
    }
  }

/*---------------------------------------------------------------------------------------*/

  template<size_t I = 0, State S, class T> requires(!is_state_v<T>)
    void div_by(S &state, T value) noexcept
  {
    if constexpr (I != S::ncomps)
    {
      state.template get<I>() /= value;
      div_by<I + 1>(state, value);
    }
  }

/*---------------------------------------------------------------------------------------*/

  template<size_t I = 0, State S, class T> requires(!is_state_v<T>)
    void set_to(S &state, T value) noexcept
  {
    if constexpr (I != S::ncomps)
    {
      using Q_t = S::template type_of<I>::type;
      state.template get<I>() = static_cast<Q_t>(value);
      set_to<I + 1>(state, value);
    }
  }

  template<size_t I = 0, State L, State R>
    void set_to(L &lstate, const R &rstate) noexcept
  {
    if constexpr (I != L::ncomps)
    {
      using T = L::template type_of<I>;
      static_assert(R::template has<T>, "right-hand state doesn't have a quantity");
      lstate.template get<T>() = rstate.template get<T>();
      set_to<I + 1>(lstate, rstate);
    }
  }

/*---------------------------------------------------------------------------------------*/

  template<size_t I = 0, State L, State R>
    bool equal(const L &lstate, const R &rstate) noexcept
  {
    if constexpr (I != L::ncomps)
    {
      using T = L::template type_of<I>;
      static_assert(R::template has<T>, "right-hand state doesn't have a quantity");
      return (lstate.template get<T>() == rstate.template get<T>()) && equal<I + 1>(lstate, rstate);
    }
    else
      return true;
  }

} // namespace Quantities::details

#endif // QDETAILS_H_INCLUDED
