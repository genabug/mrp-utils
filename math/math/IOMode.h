#ifndef IO_MODE_H_INCLUDED
#define IO_MODE_H_INCLUDED

/*!
  \file IOMode.h
  \authors boldar, gennadiy
  \brief IO manipulators for array-like types (Vector, Tensor, etc.)
*/

#include <locale>
#include <ostream>

namespace IO {
  class Mode final : public std::locale::facet
  {
    mutable bool brackets = true;

  public:
    static std::locale::id id;

    bool use_brackets() const { return brackets; }
    void use_brackets(bool flag) const { brackets = flag; }
  };

  inline std::locale::id Mode::id;

  inline bool use_brackets(std::ostream &out)
  {
    const std::locale &loc = out.getloc();
    return !std::has_facet<Mode>(loc) || std::use_facet<Mode>(loc).use_brackets();
  }

  inline std::ios_base& bareComps(std::ios_base &stream)
  {
    std::locale loc = stream.getloc();
    if (!std::has_facet<Mode>(loc))
      stream.imbue(std::locale(loc, new Mode));
    std::use_facet<Mode>(stream.getloc()).use_brackets(false);
    return stream;
  }

  inline std::ios_base& inBrackets(std::ios_base &stream)
  {
    std::locale loc = stream.getloc();
    if (!std::has_facet<Mode>(loc))
      stream.imbue(std::locale(loc, new Mode));
    std::use_facet<Mode>(stream.getloc()).use_brackets(true);
    return stream;
  }
} // namespace IO

#endif // IO_MODE_H_INCLUDED
