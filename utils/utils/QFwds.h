#ifndef Q_FORWARD_DECLARATIONS_H_INCLUDED
#define Q_FORWARD_DECLARATIONS_H_INCLUDED

#include <type_traits>

namespace Quantities
{
  template<class Type, int Dim, char... Name> struct QTraits;

  namespace details
  {
    template<class>
      struct is_traits : std::false_type {};

    template<class Type, int Dim, char... Name>
      struct is_traits<QTraits<Type, Dim, Name...>> : std::true_type {};
  }

  template<class T> constexpr bool is_traits_v = details::is_traits<T>::value;
  template<class T> concept Traits = is_traits_v<T>;

  template<Traits...> class QState;

  namespace details
  {
    template<class>
      struct is_state : std::false_type {};

    template<class... Qs>
      struct is_state<QState<Qs...>> : std::true_type {};
  }

  template<class S> constexpr bool is_state_v = details::is_state<S>::value;
  template<class S> concept State = is_state_v<S>;
}

#endif // Q_FORWARD_DECLARATIONS_H_INCLUDED