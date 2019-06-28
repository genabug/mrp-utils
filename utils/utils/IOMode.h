#ifndef IO_MODE_H_INCLUDED
#define IO_MODE_H_INCLUDED

/*!
  \file IOMode.h
  \author gennadiy (gennadiy3.14@gmail.com), A.S.Boldarev
  \brief ...
*/

#include <locale>

namespace details
{
  template<class Dummy = void> struct id_init_helper { static std::locale::id id; };
  template<class Dummy> std::locale::id id_init_helper<Dummy>::id;
}

class IO_mode final : public details::id_init_helper<>, public std::locale::facet
{
  mutable bool use;

public:
  ~IO_mode() = default;

  static const IO_mode& get_mode(std::ios_base &stream)
  {
    std::locale loc = stream.getloc();
    if (!std::has_facet<IO_mode>(loc))
      stream.imbue(std::locale(loc, new IO_mode));
    return std::use_facet<IO_mode>(stream.getloc());
  }

  bool use_brackets() const { return use; }
  void use_brackets(bool flag) const { use = flag; }
};

static std::ios_base& bareComponents(std::ios_base &stream)
{
  IO_mode::get_mode(stream).use_brackets(false);
  return stream;
}

static std::ios_base& inBrackets(std::ios_base &stream)
{
  IO_mode::get_mode(stream).use_brackets(true);
  return stream;
}

/*---------------------------------------------------------------------------------------*/
/*---------------------------------- documentation ---------------------------------------*/
/*---------------------------------------------------------------------------------------*/

/*!
  \class IO_Mode
  \author A.S.Boldarev
  \brief Facet for localization which defines the mode of output:
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
  \fn std::ios_base& bareComponents(std::ios_base &stream)
  \brief Manipulator for the output stream - bare components.
    stream << bareComponents results in that all output of the vectors/tensors
    in that stream will be performed in the form of N numbers separated by spaces.
*/

/*!
  \fn std::ios_base& inBrackets(std::ios_base &stream)
  \brief Manipulator for the output stream - brackets form.
    stream << inBrackets results in that all output of the vectors/tensors
    in that stream will be performed in the form of N numbers separated by commas
    and enclosed into the round brackets, (x, y, z, ...). This regime is default.
*/

#endif // IO_MODE_H_INCLUDED
