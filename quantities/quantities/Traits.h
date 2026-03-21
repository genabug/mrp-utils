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
    static constexpr auto id = name;
    static constexpr int size = details::size_of<T>();
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
  using rho_t = Traits<int, 111, "rho">;
  static_assert(std::is_same_v<rho_t::type, int>);
  static_assert(rho_t::dim == 111);
  static_assert(std::string_view(rho_t::id) == "rho");
  static_assert(rho_t::size == 1);

  struct HD
  {
    double rho, P, w;
    static constexpr int size = 3 * sizeof(double);
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
  static_assert(std::string_view(HD_t::id) == "HD");
  static_assert(HD_t::size == HD::size);

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

  Apart from user-defined traits the class contains two additional ones, five in all:
  * type (user-defined): type of the quantitiy, usually double, Vector3D or even State
  * dim (user-defined): dimension of mesh elements where data are defined
  * id (user-defined): string ID of the quantity
  * size (auto): number of quantity's components

  Traits::size is a number of components of the quantity. Size of a simple quantity is 1,
  while for a compound one it's greater than 1. It's defined automatically
  based on whether the field of the same name is presented in Type (then it equals to it)
  or not (and then it equals to 1).

  Quantity's traits defines as follows:
  \code
  using w_t = Traits<Vector3D, 2, "w">; // ncomps == 1, size == 3
  using rho_t = Traits<double, 3, "rho">; // ncomps == 1, size == 1

  constexpr w_t w;
  constexpr rho_t rho;
  \endcode
  In the example above velocity (w) vector (Vector3D) is defined on faces (2)
  and density (rho) scalar (double) is defined in cells (3). w_t and rho_t are now
  so-called type-names of the corresponding quantities. It's also recommended to define
  variable-names of the quantities as shown above. Due to template argument deduction
  mechanism they are allowed to be used in the much shorter methods in the utility classes:
  \code
  State<rho_t, w_t> s;
  s.get<rho_t>() = 1;
  s[rho] = 2;
  \endcode
  Variable-names should be used in end-user code like solvers,
  while type-names are intended to be used in generic code like approximations.
  Defined quantity's type-names and variable-names must be visible to all their users
  in order to organize collaborative work between different computational objects.

  \see Quantities::State
*/

#endif // QUANTITIES_TRAITS_H_INCLUDED
