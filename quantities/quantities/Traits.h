#ifndef QUANTITIES_TRAITS_H_INCLUDED
#define QUANTITIES_TRAITS_H_INCLUDED

/*!
  \file Traits.h
  \author gennadiy
  \brief Quantity's traits, definition, documentation and tests.
*/

#include "details.h"
#include "math/Type.h"

namespace Quantities
{
  template<Math::Type T, int Dim, details::Name name> struct Traits
  {
    using type = T;
    static constexpr int dim = Dim;
    static constexpr int ncomps = details::ncomps_of<T>();
    static constexpr auto id = name;
  };

  namespace details
  {
    template<class> struct is_traits : std::false_type {};
    template<Math::Type T, int Dim, Name name> struct is_traits<Traits<T, Dim, name>> : std::true_type {};
  }

  template<class T> concept IsTraits = details::is_traits<T>::value;
} // namespace Quantities

/*---------------------------------------------------------------------------------------*/
/*--------------------------------------- tests -----------------------------------------*/
/*---------------------------------------------------------------------------------------*/

namespace Quantities::tests
{
  using rho_t = Traits<int, 11, "rho">;
  static_assert(std::is_same_v<rho_t::type, int>);
  static_assert(rho_t::dim == 11);
  static_assert(rho_t::ncomps == 1);
  static_assert(std::string_view(rho_t::id) == "rho");

  struct HD
  {
    double rho, P, w;
    static constexpr int ncomps = 3 * sizeof(double);
    constexpr bool operator==(const HD &) const = default;
    constexpr HD& operator+=(const HD &o) { rho += o.rho; P += o.P; w += o.w; return *this; }
    constexpr HD& operator-=(const HD &o) { rho -= o.rho; P -= o.P; w -= o.w; return *this; }
    constexpr HD& operator*=(const HD &o) { rho *= o.rho; P *= o.P; w *= o.w; return *this; }
    constexpr HD& operator/=(const HD &o) { rho /= o.rho; P /= o.P; w /= o.w; return *this; }
  };
  static_assert(Math::Type<HD>);

  using HD_t = Traits<HD, 0, "HD">;
  static_assert(std::is_same_v<HD_t::type, HD>);
  static_assert(HD_t::dim == 0);
  static_assert(HD_t::ncomps == HD::ncomps);
  static_assert(std::string_view(HD_t::id) == "HD");

} // namespace Quantities::tests

/*---------------------------------------------------------------------------------------*/
/*------------------------------------ documentation ------------------------------------*/
/*---------------------------------------------------------------------------------------*/

/*!
  \interface Quantities::Type
  \brief Alias for Math::Type, constrains types eligible for use in Quantities::Traits.
  \see Math::Type
*/

/*!
  \class Quantities::Traits
  \tparam T Type of the quantity's data, see Math::Type.
  \tparam Dim Dimension of the mesh elements where its data are defined.
  \tparam Name String ID of the quantity.
  \brief Type traits of a quantity.

  Quantity's traits type is its main identifier in the system. It is used to create and
  access the values of a state. It is also used to implement all kinds of additional utilities
  (transport, (de)serialization, vector of state) to make them user-friendly.

  Apart from user-defined traits the class contains an additional one, four in all:
  * type (user-defined): type of the quantitiy, usually double, Vector3D or even State
  * dim (user-defined): dimension of mesh elements where data are defined
  * id (user-defined): string ID of the quantity
  * ncomps (auto): number of quantity's components

  Traits::ncomps is a number of components of the quantity. For a simple quantity it's 1,
  while for a compound one it's greater than 1. It's defined based on whether the field
  of the same name is presented in Type (then it equals to it) or not (and then it's 1).

  Quantity's traits defines as follows:
  \code
  using w_t = Traits<Vector3D, 2, "w">; // ncomps == 3
  using rho_t = Traits<double, 3, "rho">; // ncomps == 1

  constexpr w_t w;
  constexpr rho_t rho;
  \endcode
  In the example above vector of velocity ("w") vector is defined on faces (Dim=2)
  and scalar density ("rho") is defined in cells (Dim=3).
  \c w_t and \c rho_t are now so-called type-names of the corresponding quantities.
  It's also recommended to define variable-names of the quantities \c w and \c rho
  which can be used to access a state' component in user-code.
  \code
  State<rho_t, w_t> s;
  s.get<rho_t>() = 1;
  s[rho] = 2;
  \endcode
  Variable-names should be used in end-user code like solvers,
  while type-names are intended to be used in generic code like approximations.
  Defined quantity's type-names and variable-names must be visible to all users
  in order to organize collaborative work between different computational objects.

  \see Quantities::State
*/

#endif // QUANTITIES_TRAITS_H_INCLUDED
