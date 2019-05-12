    /// \file vector.h
    /// \author Boldarev A. S.
    /// \brief Vectors (vector values) and handling the vectors.

#ifndef vectorincluded

#include <iostream>
#include <cctype>
#include <cmath>
#include <cassert>
#include <locale>
#include <new>

/// Vector value.
/** Vector value of dimensionality N, "component_type" is the type of
    the cartesian components.
    
    The vector values may have various physical nature and various behavior,
    such as sliding vectors, localized vectors etc. Moreover, the class
    "vector" can be also used for storage of sets of N numbers which,
    actually, are not the vectors... For developing functions which have
    different functionality for different subtypes of vectors, the parameter
    "subtype" is added: for instance, void f(vector<2, double, t1>) and
    void f(vector<2, double, t2>) will be different functions. Since this
    parameter is a class, not an integer, it is possible to group different
    subtypes of vectors by introducing the template specializations:
    template <class t1>
      void f(vector<2, double, t1>);
    handles the vector of any subtype, while
    template <class t2> class subtype;
    template <class t2>
      void f(vector<2, double, subtype<t2> >)
    defines the alternative behavior for some subtypes of vector.
*/
template <int N, class component_type = double, class subtype = int>
  class vector
    {component_type components[N];   // Cartesian components.

     // Making all components zero.
     vector<N, component_type, subtype> &makezero()
       {for (int i = 0; i < N; i++)
           components[i] = 0.;
        return *this;
       }

     template <class subtype1>
       vector<N, component_type, subtype> &copy
                   (const vector<N, component_type, subtype1> &source)
         {for (int i = 0; i < N; i++)
            components[i] = source.components[i];
          return *this;
         }
     
     // Facet for localization which defines the mode of output
     // (in brackets or only components).
     class IO_mode        :     public std::locale::facet
     {
       mutable bool bracketsFormFlag;
       
       public:
       
       static std::locale::id id;
       
       ~IO_mode()   {}
       
       // Create new IO_mode if necessary and it in the current
       // locale of the stream "aStream", and return the pointer to
       // IO_mode stored in the current locale.
       static const IO_mode &getIO_mode(std::ios_base &aStream)
       {
         const std::locale::id *dummy = &id;
            /* This is necessary only because compilers are not obligated to
               instantiate the members of class templates which are not used
               explicitly, and often they do not. As a result, we must
               explicitly reference the static member "id", although we do
               not need it, because otherwise, may be, it will not be
               instantiated, and the linker will tell us that IO_mode::id is
               unresolved external. This problem was detected with Microsoft
               Visual C++ 2005 compiler.                                   */
         std::locale loc = aStream.getloc();
         if (!std::has_facet<IO_mode>(loc))
           aStream.imbue(std::locale(loc, new IO_mode));
         return std::use_facet<IO_mode>(aStream.getloc());
       }
       
       bool isBracketsForm() const
       {
         return bracketsFormFlag;
       }
       
       void setBracketsFormFlag(bool theFlag) const
       {
         bracketsFormFlag = theFlag;
       }
     };
     
     public:

     /// Dimensionality (available if the type is a template parameter).
     enum {dimensionality = N};
     
     /// Component type.
     typedef component_type typeOfComponent;
     
     /// Subtype.
     typedef subtype subtypeOfVector;
     
     /// Names of Cartesian components of vectors (for dimensions <= 3).
     enum vectorindex
       {X = 0, Y = 1, Z = 2};

     // The following functions are for creation of vectors by their Cartesian
     // components. They are not constructors (so, the class "vector<N>"
     // has no constructors and may be used in unions), but the static
     // functions which return the objects of class, so, the calls of these
     // functions may be considered as the literals and used in expressions.
     
     /// One-dimensional vector.
     /** Also assigns the value "a" to all the Cartesian components, thus
         this function is suitable also for obtaining the zero vector of any
         dimensionality.
     */
     static vector<N, component_type, subtype> comps(component_type a)
       {vector<N, component_type, subtype> v;
        for (int i = 0; i < N; i++)
           v.components[i] = a;
        return v;
       }

     /// Two-dimensional vector.
     static vector<N, component_type, subtype> comps(component_type a,
                                                     component_type b)
       {assert(N == 2);
        vector<N, component_type, subtype> v;
        v.components[0] = a;
        v.components[(N > 1) ? 1 : N-1] = b;
        return v;
       }

     /// Three-dimensional vector.
     static vector<N, component_type, subtype> comps (component_type a,
                                                      component_type b,
                                                      component_type c)
       {assert(N == 3);
        vector<N, component_type, subtype> v;
        v.components[0] = a;
        v.components[(N > 1) ? 1 : N-1] = b;
        v.components[(N > 2) ? 2 : N-1] = c;
        return v;
       }
       
     /// Four-dimensional vector.
     static vector<N, component_type, subtype> comps(component_type a,
                                                     component_type b,
                                                     component_type c,
                                                     component_type d)
       {assert(N == 4);
        vector<N, component_type, subtype> v;
        v.components[0] = a;
        v.components[(N > 1) ? 1 : N-1] = b;
        v.components[(N > 2) ? 2 : N-1] = c;
        v.components[(N > 3) ? 3 : N-1] = d;
        return v;
       }
       
     /// Five-dimensional vector.
     static vector<N, component_type, subtype> comps(component_type a,
                                                     component_type b,
                                                     component_type c,
                                                     component_type d,
                                                     component_type e)
       {assert(N == 5);
        vector<N, component_type, subtype> v;
        v.components[0] = a;
        v.components[(N > 1) ? 1 : N-1] = b;
        v.components[(N > 2) ? 2 : N-1] = c;
        v.components[(N > 3) ? 3 : N-1] = d;
        v.components[(N > 4) ? 4 : N-1] = e;
        return v;
       }
       
     /// Six-dimensional vector.
     static vector<N, component_type, subtype> comps(component_type a,
                                                     component_type b,
                                                     component_type c,
                                                     component_type d,
                                                     component_type e,
                                                     component_type f)
       {assert(N == 6);
        vector<N, component_type, subtype> v;
        v.components[0] = a;
        v.components[(N > 1) ? 1 : N-1] = b;
        v.components[(N > 2) ? 2 : N-1] = c;
        v.components[(N > 3) ? 3 : N-1] = d;
        v.components[(N > 4) ? 4 : N-1] = e;
        v.components[(N > 5) ? 5 : N-1] = f;
        return v;
       }
       
     /// Seven-dimensional vector.
     static vector<N, component_type, subtype> comps(component_type a,
                                                     component_type b,
                                                     component_type c,
                                                     component_type d,
                                                     component_type e,
                                                     component_type f,
                                                     component_type g)
       {assert(N == 7);
        vector<N, component_type, subtype> v;
        v.components[0] = a;
        v.components[(N > 1) ? 1 : N-1] = b;
        v.components[(N > 2) ? 2 : N-1] = c;
        v.components[(N > 3) ? 3 : N-1] = d;
        v.components[(N > 4) ? 4 : N-1] = e;
        v.components[(N > 5) ? 5 : N-1] = f;
        v.components[(N > 6) ? 6 : N-1] = g;
        return v;
       }
       
     /// Eight-dimensional vector.
     static vector<N, component_type, subtype> comps(component_type a,
                                                     component_type b,
                                                     component_type c,
                                                     component_type d,
                                                     component_type e,
                                                     component_type f,
                                                     component_type g,
                                                     component_type h)
       {assert(N == 8);
        vector<N, component_type, subtype> v;
        v.components[0] = a;
        v.components[(N > 1) ? 1 : N-1] = b;
        v.components[(N > 2) ? 2 : N-1] = c;
        v.components[(N > 3) ? 3 : N-1] = d;
        v.components[(N > 4) ? 4 : N-1] = e;
        v.components[(N > 5) ? 5 : N-1] = f;
        v.components[(N > 6) ? 6 : N-1] = g;
        v.components[(N > 7) ? 7 : N-1] = h;
        return v;
       }
       
     /// Nine-dimensional vector.
     static vector<N, component_type, subtype> comps(component_type a,
                                                     component_type b,
                                                     component_type c,
                                                     component_type d,
                                                     component_type e,
                                                     component_type f,
                                                     component_type g,
                                                     component_type h,
                                                     component_type i)
       {assert(N == 9);
        vector<N, component_type, subtype> v;
        v.components[0] = a;
        v.components[(N > 1) ? 1 : N-1] = b;
        v.components[(N > 2) ? 2 : N-1] = c;
        v.components[(N > 3) ? 3 : N-1] = d;
        v.components[(N > 4) ? 4 : N-1] = e;
        v.components[(N > 5) ? 5 : N-1] = f;
        v.components[(N > 6) ? 6 : N-1] = g;
        v.components[(N > 7) ? 7 : N-1] = h;
        v.components[(N > 8) ? 8 : N-1] = i;
        return v;
       }
       
     /// Ten-dimensional vector.
     static vector<N, component_type, subtype> comps(component_type a,
                                                     component_type b,
                                                     component_type c,
                                                     component_type d,
                                                     component_type e,
                                                     component_type f,
                                                     component_type g,
                                                     component_type h,
                                                     component_type i,
                                                     component_type j)
       {assert(N == 10);
        vector<N, component_type, subtype> v;
        v.components[0] = a;
        v.components[(N > 1) ? 1 : N-1] = b;
        v.components[(N > 2) ? 2 : N-1] = c;
        v.components[(N > 3) ? 3 : N-1] = d;
        v.components[(N > 4) ? 4 : N-1] = e;
        v.components[(N > 5) ? 5 : N-1] = f;
        v.components[(N > 6) ? 6 : N-1] = g;
        v.components[(N > 7) ? 7 : N-1] = h;
        v.components[(N > 8) ? 8 : N-1] = i;
        v.components[(N > 9) ? 9 : N-1] = j;
        return v;
       }
     
     /// Assignment of a scalar to vector.
     /** Makes all components of vector equal to "value".
         Dangerous, but necessary for some specific purposes.
     */
     const vector<N, component_type, subtype> &operator=(component_type value)
       {for (int i = 0; i < N; i++)
           components[i] = value;
        return *this;
       }
       
     // 1. Unary + & -.

     /// Unary -.
     vector <N, component_type, subtype> operator-() const
       {int i;
        vector<N, component_type, subtype> rezult;

        for (i = 0; i < N; i++)
           rezult.components[i] = - components[i];
        return(rezult);
       }

     /// Unary +.
     vector <N, component_type, subtype> operator+() const
       {return(*this);
       }

     // 2. Assignment operators.

     ///
     template <class subtype1>
       vector<N, component_type, subtype> & operator=
                               (const vector<N, component_type, subtype1> &x)
         {int i;

          for (i = 0; i < N; i++)
             components[i] = x[i];
          return(*this);
         }
     
     ///
     vector<N, component_type, subtype> & operator+=
                               (const vector<N, component_type, subtype> &x)
       {int i;

        for (i = 0; i < N; i++)
           components[i] += x.components[i];
        return(*this);
       }

     ///
     vector<N, component_type, subtype> & operator-=
                               (const vector<N, component_type, subtype> &x)
       {int i;

        for (i = 0; i < N; i++)
           components[i] -= x.components[i];
        return(*this);
       }

     /// Assignment with multiplying by a scalar.
     vector<N, component_type, subtype> & operator*=(const component_type x)
       {int i;

        for (i = 0; i < N; i++)
           components[i] *= x;
        return(*this);
       }

     /// Assignment with dividing by a scalar.
     vector<N, component_type, subtype> & operator/=(const component_type x)
       {int i;

        for (i = 0; i < N; i++)
           components[i] /= x;
        return(*this);
       }

     // 3. Multiplicative operators.

     /// Scalar product.
     inline friend component_type operator*
                       (const vector<N, component_type, subtype> &a,
                        const vector<N, component_type, subtype> &b)
         {int i;
	 component_type tttmp(a.components[0] * b.components[0]);
          for (i = 1; i < N; i++)
             tttmp += a.components[i] * b.components[i];
          return(tttmp);
         }

     /// Multiplying by a scalar.
     inline friend vector<N, component_type, subtype> operator*
                               (const component_type c,
                                const vector<N, component_type, subtype> &a)
        {int i;
         vector<N, component_type, subtype> rezult;

         for (i = 0; i < N; i++)
            rezult.components[i] = a.components[i] * c;
         return(rezult);
        }

     /// Multiplying by a scalar.
     inline friend vector<N, component_type, subtype> operator*
                               (const vector<N, component_type, subtype> &a,
                                const component_type c)
        {int i;
         vector<N, component_type, subtype> rezult;

         for (i = 0; i < N; i++)
            rezult.components[i] = a.components[i] * c;
         return(rezult);
        }

     /// Dividing by a scalar.
     inline friend vector<N, component_type, subtype> operator/
                              (const vector<N, component_type, subtype> &a,
                               const component_type c)
        {int i;
         vector<N, component_type, subtype> rezult;

         for (i = 0; i < N; i++)
            rezult.components[i] = a.components[i] / c;
         return(rezult);
        }

     // 4. Addition and subtraction.

     /// Addition.
     inline friend vector<N, component_type, subtype> operator+
                                (const vector<N, component_type, subtype> &a,
                                 const vector<N, component_type, subtype> &b)
        {int i;
         vector<N, component_type, subtype> rezult;

         for (i = 0; i < N; i++)
            rezult.components[i] = a.components[i] + b.components[i];
         return(rezult);
        }

     /// Subtraction.
     inline friend vector<N, component_type, subtype> operator-
                                (const vector<N, component_type, subtype> &a,
                                 const vector<N, component_type, subtype> &b)
        {int i;
         vector<N, component_type, subtype> rezult;

         for (i = 0; i < N; i++)
            rezult.components[i] = a.components[i] - b.components[i];
         return(rezult);
        }

     // 5. Comparison of two vectors.

     /// Are they equal?
     /** The comparison is component-wise, so, as in the case of comparison
         of two real numbers, it is possible the negative result caused by
         small errors of rounding. As in the case of real numbers, more
         reliable way of comparison is fabs(a - b) < eps with "eps"
         positive and small enough.
     */
     inline friend int operator==(const vector<N, component_type, subtype> &a,
                                  const vector<N, component_type, subtype> &b)
        {int i;

         for (i = 0; i < N; i++)
            if (a.components[i] != b.components[i])
              return(0);
         return(1);
        }

     /// Are they inequal?
     /** The comparison is component-wise, so, as in the case of comparison
         of two real numbers, it is possible the negative result caused by
         small errors of rounding. As in the case of real numbers, more
         reliable way of comparison is fabs(a - b) >= eps with "eps"
         positive and small enough.
     */
     inline friend int operator!=(const vector<N, component_type, subtype> &a,
                                  const vector<N, component_type, subtype> &b)
        {int i;

         for (i = 0; i < N; i++)
            if (a.components[i] != b.components[i])
              return(1);
         return(0);
        }

     // 6. Input/output.

     /// Reading of a vector from a stream.
     /** The stream should contain either N numbers (Cartesian components)
         separated by any blank characters, or the construction
         (x, y), (x, y, z) etc. (e. g. the N Cartesian components should be
         separated by commas and enclosed into the round brackets).
     */
     inline friend std::istream &operator>>
               (std::istream &stream, vector<N, component_type, subtype> &a)
        {int i;

         // In both cases (e. g. when the stream contains "x y z ..." and
         // when it contains "(x,y,z,...)") the vector must be normally
         // reconstructed from the input stream.
         
         char c;
         bool in_brackets = true;
         while (stream.get(c) && c != '(')
           if (!isspace(c))
             {stream.putback(c);
              in_brackets = false;
              break;
             }

         for (i = 0; i < N; i++)
            {while (stream.get(c) && c != ',')
               if (!isspace(c))
                 {stream.putback(c);
                  break;
                 }

             stream >> a.components[i];
            }

         while (in_brackets && stream.get(c) && c != ')')
           if (!isspace(c))
             {stream.putback(c);
              break;
             }

         return(stream);
        }

     /// Manipulator for the output stream - bare components.
     /** aStream << vector<N>::bareComponents results in that all output
         of the vectors in that stream will be performed in the form of N
         numbers (Cartesian components) separated by spaces.
     */
     static std::ios_base &bareComponents(std::ios_base &aStream)
     {
       IO_mode::getIO_mode(aStream).setBracketsFormFlag(false);
       return aStream;
     }
     
     /// Manipulator for the output stream - brackets form.
     /** aStream << vector<N>::inBrackets results in that all output
         of the vectors in that stream will be performed in the form of N
         numbers (Cartesian components) separated by commas and enclosed
         into the round brackets, (x, y, z, ...). This regime is default.
     */
     static std::ios_base &inBrackets(std::ios_base &aStream)
     {
       IO_mode::getIO_mode(aStream).setBracketsFormFlag(true);
       return aStream;
     }
     
     /// Writing of the vector into the output stream.
     /** The output is performed in the form defined by manipulators
         "bareComponents" and "inBrackets" (the last is default).
     */ 
     inline friend std::ostream &operator<<
           (std::ostream &stream, const vector<N, component_type, subtype> &a)
        {const std::locale::id *dummy = &IO_mode::id;
            /* This is necessary only because compilers are not obligated to
               instantiate the members of class templates which are not used
               explicitly, and often they do not. As a result, we must
               explicitly reference the static member "id", although we do
               not need it, because otherwise, may be, it will not be
               instantiated, and the linker will tell us that IO_mode::id is
               unresolved external. This problem was detected with Microsoft
               Visual C++ 2005 compiler.                                   */
         int i;
         const std::locale &loc = stream.getloc();
         const bool bracketsRegime =
                      std::has_facet<IO_mode>(loc) ?
                      std::use_facet<IO_mode>(loc).isBracketsForm() : true;

         if (bracketsRegime)
           stream << '(';
         
         static const char *separator[] = {" ", ", "},
                           *terminator[] = {"", ")"};

         for (i = 0; i < N; i++)
            stream << a.components[i]
                   << ((i < N-1) ? separator[bracketsRegime] :
                                   terminator[bracketsRegime]);
         return(stream);
        }

     /// Access to the Cartesian components of constant vector.
     const component_type &operator[](int i) const
       {return components[i];
       }

     /// Access to the Cartesian components of vector.
     component_type &operator[](int i)
       {return components[i];
       }
    };


// 7. Some useful functions of vectors.

/// Square of vector's length.
template <int N, class component_type, class subtype>
  inline component_type sqs(const vector<N, component_type, subtype> &a)
     {return(a * a);
     }

/// Length (absolute value).
template <int N, class subtype>
  inline double fabs(const vector<N, double, subtype> &a)
     {return(sqrt(a * a));
     }

/// Cosine of the angle between the vectors.
template <int N, class subtype>
  inline double cos(const vector<N, double, subtype> &a,
                    const vector<N, double, subtype> &b)
     {return(a * b / (fabs(a) * fabs(b)));
     }

/// Sine of the angle between the vectors.
/** Returns the positive value of angle without regard the
    sequence of parameters, sin(a, b) == sin(b, a).
*/
template <int N, class subtype>
  inline double sin(const vector<N, double, subtype> &a,
                    const vector<N, double, subtype> &b)
     {double x;
      x = cos(a, b);
      x *= x;
      return(sqrt(1. - x));
     }

// To introduce the vector product, we must consider the
// 2D and 3D vectors separately.

template class vector<2>;
template class vector<3>;
  // Explicit instantiation of the template may resolve a problem with
  // some compilers (for example, it was detected for some versions of
  // gnu c++), when the overloadad operator cannot be found if the
  // parametrized type is the parameter of some other template.

/// Two-dimensional vector.
typedef vector<2> vector2D;

/// Vector product of two 2D vectors (a scalar value).
/** We use operator % for vector product, while * denotes
    the scalar product.
*/
template <class component_type, class subtype>
  inline double operator%(const vector<2, component_type, subtype> &a,
                          const vector<2, component_type, subtype> &b)
    {return(a[vector<2, component_type, subtype>::X] *
            b[vector<2, component_type, subtype>::Y]
          - a[vector<2, component_type, subtype>::Y] *
            b[vector<2, component_type, subtype>::X]);
    }

/// Rotation by 90 degrees in the positive (counterclockwise) direction.
template <class component_type, class subtype>
  inline vector<2, component_type, subtype> operator~
                               (const vector<2, component_type, subtype> &r)
    {vector<2, component_type, subtype> rezult;

     rezult[vector<2, component_type, subtype>::X] =
                              - r[vector<2, component_type, subtype>::Y];
     rezult[vector<2, component_type, subtype>::Y] =
                                r[vector<2, component_type, subtype>::X];
     return(rezult);
    }

/// Three-dimensional vector.
typedef vector<3> vector3D;

/// Vector product of two 3D vectors (a vector value too).
/** We use operator % for vector product, while * denotes
    the scalar product.
*/
template <class component_type, class subtype>
  inline vector<3, component_type, subtype> operator%
                               (const vector<3, component_type, subtype> &a,
                                const vector<3, component_type, subtype> &b)
    {vector<3, component_type, subtype> rezult;

     rezult[vector<3, component_type, subtype>::X] =
          a[vector<3, component_type, subtype>::Y] *
          b[vector<3, component_type, subtype>::Z] -
          b[vector<3, component_type, subtype>::Y] *
          a[vector<3, component_type, subtype>::Z];

     rezult[vector<3, component_type, subtype>::Y] =
          a[vector<3, component_type, subtype>::Z] *
          b[vector<3, component_type, subtype>::X] -
          b[vector<3, component_type, subtype>::Z] *
          a[vector<3, component_type, subtype>::X];

     rezult[vector<3, component_type, subtype>::Z] =
          a[vector<3, component_type, subtype>::X] *
          b[vector<3, component_type, subtype>::Y] -
          b[vector<3, component_type, subtype>::X] *
          a[vector<3, component_type, subtype>::Y];

     return(rezult);
    }

// Definition of static object.
template <int N, class component_type, class subtype>
  std::locale::id vector<N, component_type, subtype>::IO_mode::id;

#define vectorincluded

#endif
