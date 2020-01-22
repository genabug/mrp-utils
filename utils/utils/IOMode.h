#ifndef IO_MODE_H_INCLUDED
#define IO_MODE_H_INCLUDED

/*!
  \file IOMode.h
  \authors A.S.Boldarev, gennadiy (gennadiy3.14@gmail.com)
  \brief ...
*/

#include <locale>

template<class User>
  class IO_mode final : public std::locale::facet
{
  mutable bool use;

public:
  ~IO_mode() = default;
  static std::locale::id id;

  static const IO_mode& get_mode(std::ios_base &stream)
  {
    std::locale loc = stream.getloc();
    if (!std::has_facet<IO_mode<User>>(loc))
      stream.imbue(std::locale(loc, new IO_mode<User>));
    return std::use_facet<IO_mode<User>>(stream.getloc());
  }

  bool use_brackets() const { return use; }
  void use_brackets(bool flag) const { use = flag; }
};

template<class User> std::locale::id IO_mode<User>::id;


template<class User> class Manipulators
{
public:
  static std::ios_base& bareComponents(std::ios_base &stream)
  {
    IO_mode<User>::get_mode(stream).use_brackets(false);
    return stream;
  }

  static std::ios_base& inBrackets(std::ios_base &stream)
  {
    IO_mode<User>::get_mode(stream).use_brackets(true);
    return stream;
  }
};

/*---------------------------------------------------------------------------------------*/
/*---------------------------------- documentation ---------------------------------------*/
/*---------------------------------------------------------------------------------------*/

/*!
  \class IO_mode
  \tparam User Arbitraty type in order to apply IO mode to a specific user(s) only.
  \brief Facet for localization which defines the mode of user input/output:
    in brackets or bare components.
  \see Vector
  \see Tensor
*/

/*!
  \fn const IO_mode& IO_mode::get_mode(std::ios_base &stream)
  \brief Create new IO_mode if necessary and it in the current locale
    of the stream "stream", and return the pointer to IO_mode stored in the current locale.
*/

/*!
  \class Manipulators
  \tparam User Arbitraty type in order to apply IO mode to a specific user(s) only.
  \brief Set of IO manipulators for the specified user(s) of IO_mode.

  The goal is to reduce code duplication for the different users. The implementation
  is the same for anyone -- one is used to setup the flag, another discards it.
  (In fact, it's entirely up to you how to interpret the flag, historically it was
  used, and still used, to change input/output of two types -- vector and tensor.)
  That's why manipulators are placed in one common class and have specific names.
  Another goal is to simplify usage for templated types. For example, printint vectors
  with any type/size, the corresponding manipulator should be called only once:
  \code
  Vector<2> v2;
  Vector<3> v3;
  Vector<3, int> v3i;
  std::cout << Vectors::bareComponents << v2 << "; " << v3 << "; " << v3i << std::endl;
  \endcode

  The usage is quite simple -- just inherit your User from Manipulators using itself
  as template argument (similar to CRTP pattern):
  \code
  class User : public Manipulators<User> {};
  \endcode
  And then use it in the IO operators overloads of your User, see examples in \see Vector
  and \see Tensor types.
*/

/*!
  \fn std::ios_base& Manipulators::bareComponents(std::ios_base &stream)
  \brief Manipulator for the output stream - bare components.
*/

/*!
  \fn std::ios_base& Manipulators::inBrackets(std::ios_base &stream)
  \brief Manipulator for the output stream - brackets form.
*/

#endif // IO_MODE_H_INCLUDED
