#pragma once

#include <cppan/member.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/empty_base.hpp>
#include <boost/type_traits/is_class.hpp>

/// Stolen from http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Member_Detector
/// Define metafunction that detect presence of some member in structure.
/// Has specialization for cppan::member< MemberType, AnnotationsType >, which step 
/// into AnnotationsType to check annotation presence.
#define CPPAN_DEFINE_MEMBER_DETECTOR(X)                                             \
template<typename T> class has_##X {                                                \
    struct Fallback { int X; };                                                     \
    struct Derived                                                                  \
      : ::boost::mpl::if_<                                                          \
            ::boost::is_class<T>                                                    \
          , T                                                                       \
          , ::boost::mpl::empty_base                                                \
          >::type                                                                   \
      , Fallback { };                                                               \
                                                                                    \
    template<typename U, U> struct Check;                                           \
                                                                                    \
    typedef char ArrayOfOne[1];                                                     \
    typedef char ArrayOfTwo[2];                                                     \
                                                                                    \
    template<typename U> static ArrayOfOne & func(Check<int Fallback::*, &U::X> *); \
    template<typename U> static ArrayOfTwo & func(...);                             \
  public:                                                                           \
    typedef has_##X type;                                                           \
    enum { value = sizeof(func<Derived>(0)) == 2 };                                 \
};                                                                                  \
template<typename MemberType, typename AnnotationsType>                             \
class has_##X<::cppan::member<MemberType, AnnotationsType> >                        \
    : public has_##X<AnnotationsType>                                               \
{};
