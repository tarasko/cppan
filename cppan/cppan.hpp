#if !defined(CPPAN_INCLUDED)
#define CPPAN_INCLUDED

#include <cppan/detail/declare_and_annotate.hpp>

#include <boost/fusion/container/vector.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_class.hpp>

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

/// Depending on constness of type T, return either T::annotated_tuple_type or T::const_annotated_tuple_type.
/// This can be helpfull for those who don`t have C++11 auto and decltype keywords.
template<typename T>
struct annotated_tuple_type
{
    typedef typename T::annotated_tuple_type type;
};

template<typename T>
struct annotated_tuple_type<T const>
{
    typedef typename T::const_annotated_tuple_type type;
};

/// Declare and annotate member with specified types. Accept Boost.Preprocessor sequence of member field declaration.
/// Each element in sequence must be Boost.Preprocessor tuple with 3 elements (member_type, member_name, sequence_of_annotations).
/// \code
/// sequence_of_annotations := sequence_of_annotations | CPPAN_NIL_SEQ
/// \endcode
/// \c CPPAN_NIL_SEQ can be used if member doesn`t have annotations.
/// \c sequence_of_annotations - is Boost.Preprocessor sequence of tuples with 2 elements (annotation_name, annotation_runtime_value).
/// @code
///struct A
///{
///    CPPAN_DECLARE_AND_ANNOTATE(
///        ((int, int_field_,
///            ((int_annotation, 24))
///            ((string_annotation, "Privet"))
///        ))
///        ((std::string, string_field_,
///            ((no_serialization, std::true_type()))
///            ((no_hash, std::true_type()))
///        ))
///        ((double, no_ann_field_, CPPAN_NIL_SEQ))
///      )
///};
/// @endcode
#define CPPAN_DECLARE_AND_ANNOTATE(X) \
    typedef void annotated_tag; \
    BOOST_PP_SEQ_FOR_EACH_R(1, CPPAN_DETAIL_DECLARE_MEMBER, _, X) \
    BOOST_PP_SEQ_FOR_EACH_R(1, CPPAN_DETAIL_DECLARE_ANNOTATION_STRUCT, _, X) \
    CPPAN_DETAIL_DECLARE_MUTABLE_TUPLE_TYPE(X) \
    CPPAN_DETAIL_DECLARE_CONST_TUPLE_TYPE(X) \
    CPPAN_ENABLE_BOOST_SERIALIZATION

/// Define empty annotations sequence
#define CPPAN_NIL_SEQ BOOST_PP_SEQ_NIL

#if defined(CPPAN_DOXYGEN)
/// Metafunction that become true if annotated_tag typedef exists in T
template<typename T> struct has_annotations;
#else
BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(has_annotations, annotated_tag, false);
#endif 

/// Stolen from http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Member_Detector
/// Define metafunction that detect presence of some member in structure.
/// Has specialization for cppan::annotated_member< MemberType, AnnotationsType >, which step 
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
class has_##X<::cppan::annotated_member<MemberType, AnnotationsType> >              \
    : public has_##X<AnnotationsType>                                               \
{};

}									// namespace cppan

#endif								// !defined(CPPAN_INCLUDED)
