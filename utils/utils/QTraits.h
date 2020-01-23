#ifndef QTRAITS_H_INCLUDED
#define QTRAITS_H_INCLUDED

/*!
  \file QTraits.h
  \author gennadiy
  \brief Quantity traits class definition.
  ...
*/

namespace Quantities
{

  namespace details
  {
    // helper structure to get number of components of quantity Q
    // if Q has field ncomps then get its value else return 1
    template<class Q, class = int> struct ncomps_s {
      static constexpr int value = 1; };
    template<class Q> struct ncomps_s<Q, decltype((void) Q::ncomps, 0)> {
      static constexpr int value = Q::ncomps; };

    template<class Q> constexpr int ncomps_v = ncomps_s<Q>::value;
  }

  template<class Q, int Dim, char... Name> struct QTraits
  {
    using type = Q;
    static constexpr int dim = Dim;
    static constexpr char id[] = {Name..., '\0'}; // + terminal '\0' (c-string)
    static constexpr int ncomps = details::ncomps_v<Q>;
    static constexpr int size = sizeof(Q)/sizeof(double);
  };

  // a static member must be defined outside of the struct
  template<class Type, int Dim, char... Name>
    constexpr char QTraits<Type, Dim, Name...>::id[];

} // namespace Quantities

#endif // QTRAITS_H_INCLUDED
