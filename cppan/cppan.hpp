#if !defined(CPPAN_INCLUDED)
#define CPPAN_INCLUDED

#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>

#if defined(CPPAN_USE_TUPLE_FOR_ANNOTATIONS)
#  include <boost/fusion/adapted/struct/define_struct_inline.hpp>
#endif

#include <boost/mpl/has_xxx.hpp>

namespace cppan {

/// Bound together reference to annotated member and annotations type
template<typename MemberType, typename AnnotationsType>
struct annotated_member
{
    typedef AnnotationsType annotations_type;
    typedef MemberType member_type;

    MemberType& value_;

    annotated_member(MemberType& value) : value_(value) {}
};

/// Declare and annotate member with specified types
/// @code
///struct A
///{
///    CPPAN_DECLARE_AND_ANNOTATE(
///        ((int, int_field_,
///            ((int_annotation, 24.0))
///            ((string_annotation, "Privet"))
///        ))
///        ((std::string, string_field_,
///            ((no_serialization, std::true_type()))
///            ((no_hash, std::true_type()))
///        ))
///        ((double, no_ann_field_, BOOST_PP_SEQ_NIL))
///      )
//};
/// @endcode
#define CPPAN_DECLARE_AND_ANNOTATE(X) \
    typedef void annotated_tag; \
    BOOST_PP_SEQ_FOR_EACH_R(1, CPPAN_DETAIL_DECLARE_MEMBER, _, X) \
    BOOST_PP_SEQ_FOR_EACH_R(1, CPPAN_DETAIL_DECLARE_ANNOTATION_STRUCT, _, X) \
    CPPAN_DETAIL_DECLARE_MUTABLE_TUPLE_TYPE(X) \
    CPPAN_DETAIL_DECLARE_CONST_TUPLE_TYPE(X) 

#define CPPAN_NIL_SEQ BOOST_PP_SEQ_NIL

/// \class has_annotations
/// \brief Check presence of annotated_tag typedef in supplied type

BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(has_annotations, annotated_tag, false);

/// Stolen from http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Member_Detector
/// Define metafunction that detect presence of some member in structure
#define CPPAN_DEFINE_MEMBER_DETECTOR(X)                                               \
template<typename T> class has_##X {                                                \
    struct Fallback { int X; };                                                     \
    struct Derived : T, Fallback { };                                               \
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
};


}									// namespace cppan

#endif								// !defined(CPPAN_INCLUDED)