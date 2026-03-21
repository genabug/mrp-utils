#ifndef IO_MODE_H_INCLUDED
#define IO_MODE_H_INCLUDED

/*!
  \file IOMode.h
  \authors boldar, gennadiy
  \brief IO manipulators for array-like types (Vector, Tensor, etc.)
*/

#include <cctype>
#include <locale>
#include <istream>
#include <ostream>

namespace IO
{
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
  namespace details
  {
    template<class T>
      std::istream& read_values(std::istream &in, T *data, size_t count, char open, char close)
    {
      in >> std::ws;

      auto ch = in.peek();
      if (ch == std::char_traits<char>::eof())
      {
        in.setstate(std::ios::failbit);
        return in;
      }

      auto c = static_cast<char>(ch);
      bool bracketed = (c == open);

      if (bracketed)
        in.get();
      else if (!std::isdigit(c) && c != '-' && c != '+' && c != '.')
      {
        in.setstate(std::ios::failbit);
        return in;
      }

      for (size_t i = 0; i < count; ++i)
      {
        if (bracketed && i > 0)
        {
          in >> std::ws;
          if (in.peek() == ',')
            in.get();
          else
          {
            in.setstate(std::ios::failbit);
            return in;
          }
        }
        if (!(in >> data[i]))
          return in;
      }

      if (bracketed)
      {
        in >> std::ws;
        if (in.peek() == close)
          in.get();
        else
          in.setstate(std::ios::failbit);
      }

      return in;
    }
  } // namespace details
} // namespace IO

#endif // IO_MODE_H_INCLUDED
