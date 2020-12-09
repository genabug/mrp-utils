#ifndef QTRAITS_H_INCLUDED
#define QTRAITS_H_INCLUDED

/*!
  \file QTraits.h
  \author gennadiy
  \brief Quantity traits class definition and documentation.
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

  template<class Type, int Dim, char... Name> struct QTraits
  {
    using type = Type;
    static constexpr int dim = Dim;
    static constexpr char id[] = {Name..., '\0'}; // + terminal '\0' (c-string)
    static constexpr int ncomps = details::ncomps_v<Type>;
    static constexpr int size = sizeof(Type)/sizeof(double);
  };

  // a static member must be defined outside of the struct
  template<class Type, int Dim, char... Name>
    constexpr char QTraits<Type, Dim, Name...>::id[];

} // namespace Quantities

/*---------------------------------------------------------------------------------------*/
/*------------------------------------ documentation ------------------------------------*/
/*---------------------------------------------------------------------------------------*/

/*!
  \class Quantities::QTraits
  \tparam Type Type of the quantity's data (double, vector3D, ...).
  \tparam Dim Dimension of the mesh elements where its data are defined.
  \tparam Name String ID of the quantity.
  \brief Type traits of a quantity.

  Quantity's traits type is its main identifier in the system. It is used to create,
  access and work with the data in a user-code. It is also used to implement all kinds
  of additional utilities (transport, (de)serialization, vector of state) to make them
  user-friendly.

  Apart from user-defined traits the class contains two additional ones, five in all:
  * type (user-defined): type of the quantitiy, usually double or vector3D
  * dim (user-defined): dimension of mesh elements where data are defined
  * id (user-defined): string ID of the quantity
  * ncomps (auto): number of quantity's component(s)
  * size (auto): size of the quantity in doubles

  ncomps trait to distinguish simple quantities from the compound ones on a type-level.
  Number of a simple quantity components is 1, while for a compound one it's more than 1.
  A user is not allowed to manually set a value of the property, it's defined automatically
  based on next assumption: if given type Type containes ncomps member itself
  (i.e. Type::ncomps is valid code), then number of quantity's components is set equal
  to its value. Otherwise it is set to 1. The assumption is debatable though.

  size trait defines quantity's size in size of doubles (see definition). It's used for
  (de)serialization of a quantity i.e. read/write from/to a stream. Also debatable
  assumption to use double as a base value.

  Quantity's traits defines as follows:
  \code
  using w_t = QTraits<vector3D, 2, 'w'>; // ncomps == 1, size == 3
  using rho_t = QTraits<double, 3, 'r','h','o'>; // ncomps == 1, size == 1

  constexpr w_t w;
  constexpr rho_t rho;
  \endcode
  In the example above velocity (w) vector (vector3D) is defined on faces (2)
  and density (rho) scalar (double) is defined in cells (3). w_t and rho_t are now
  so-called type-names of the corresponding quantities. It's also recommended to define
  variable-names of the quantities as shown above. Due to template argument deduction
  mechanism they allow to use much shorter methods (if presented) in the utility classes.
  Indeed, compare access to a quantity from a state:
  \code
  QState<rho_t, w_t> s;
  s.get<rho_t>() = 1;
  s[rho] = 2;
  \endcode
  Variable-names should be used in end-user code like solvers, type-names are intended
  to be used in generic code like approximations.
  Defined quantity's type-names and variable-names must be visible to all their users
  in order to organize collaborative work between different computational objects.

  \see Quantities::QState
*/

#endif // QTRAITS_H_INCLUDED
