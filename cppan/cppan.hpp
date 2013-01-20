#if !defined(CPPAN_INCLUDED)
#define CPPAN_INCLUDED

#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>

#if defined(CPPAN_USE_TUPLE_FOR_ANNOTATIONS)
#  include <boost/fusion/adapted/struct/define_struct_inline.hpp>
#endif

#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/seq/transform.hpp>
#include <boost/preprocessor/comma_if.hpp>
#include <boost/preprocessor/identity.hpp>
#include <boost/preprocessor/empty.hpp>

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

//--------------------- USER LEVEL MACROSES --------------------------------------------

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


/// Define metafunction that become true if type member were defined using CPPAN_DECLARE_AND_ANNOTATE
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

//--------------------- IMPLEMENTATION LEVEL MACROSES --------------------------------------------

// Define type that will be used for tuple. Better to use C++11 template aliasing instead of macro
#define CPPAN_DETAIL_TUPLE_TYPE boost::fusion::vector

// Accessors for top level tuple
#define CPPAN_DETAIL_L1_TYPE(tup) BOOST_PP_TUPLE_ELEM(3, 0, tup)
#define CPPAN_DETAIL_L1_MEMBER(tup) BOOST_PP_TUPLE_ELEM(3, 1, tup)
#define CPPAN_DETAIL_L1_ANN_SEQ(tup) BOOST_PP_TUPLE_ELEM(3, 2, tup)

// Accessort for annotations level tuple
#define CPPAN_DETAIL_L2_NAME(tup) BOOST_PP_TUPLE_ELEM(2, 0, tup)
#define CPPAN_DETAIL_L2_VALUE(tup) BOOST_PP_TUPLE_ELEM(2, 1, tup)

// Produce name for annotation structure
#define CPPAN_DETAIL_ANN_NAME(tup) BOOST_PP_CAT(annotations_for_, CPPAN_DETAIL_L1_MEMBER(tup))

// Declare member using top level tuple
#define CPPAN_DETAIL_DECLARE_MEMBER(r, data, tup) CPPAN_DETAIL_L1_TYPE(tup) CPPAN_DETAIL_L1_MEMBER(tup);

#if !defined(CPPAN_USE_TUPLE_FOR_ANNOTATIONS)

// Produce colon on false and comma on true
#define CPPAN_DETAIL_COLON_OR_COMMA(cond) BOOST_PP_IF(cond, BOOST_PP_COMMA, BOOST_PP_IDENTITY(:))()

// Produce annotation initializer list for annotation structure constructor
#define CPPAN_DETAIL_INITIALIZER_LIST(r, data, i, tup) CPPAN_DETAIL_COLON_OR_COMMA(i) CPPAN_DETAIL_L2_NAME(tup)(CPPAN_DETAIL_L2_VALUE(tup))

// Produce declaration of annotation member
#define CPPAN_DETAIL_ANNOTATIONS_LIST(r, data, i, tup) decltype(CPPAN_DETAIL_L2_VALUE(tup)) CPPAN_DETAIL_L2_NAME(tup);

// Declare annotation struct using tuple
#define CPPAN_DETAIL_DECLARE_ANNOTATION_STRUCT(r, data, tup) \
    struct CPPAN_DETAIL_ANN_NAME(tup) { \
        CPPAN_DETAIL_ANN_NAME(tup) () \
            BOOST_PP_SEQ_FOR_EACH_I_R(r, CPPAN_DETAIL_INITIALIZER_LIST, _, CPPAN_DETAIL_L1_ANN_SEQ(tup)) {} \
        BOOST_PP_SEQ_FOR_EACH_I_R(r, CPPAN_DETAIL_ANNOTATIONS_LIST, _, CPPAN_DETAIL_L1_ANN_SEQ(tup)) \
    };

#else

// TODO: Initialize annotations with provided values

#define CPPAN_DETAIL_TRANSFORM_TO_FUSION_SEQ(d, data, tup) decltype(CPPAN_DETAIL_L2_VALUE(tup)), CPPAN_DETAIL_L2_NAME(tup)

#define CPPAN_DETAIL_DECLARE_ANNOTATION_STRUCT(r, data, tup) \
    BOOST_FUSION_DEFINE_STRUCT_INLINE(CPPAN_DETAIL_ANN_NAME(tup), BOOST_PP_SEQ_TRANSFORM(CPPAN_DETAIL_TRANSFORM_TO_FUSION_SEQ, _, CPPAN_DETAIL_L1_ANN_SEQ(tup)))

#endif

// Produce ", member" or "member" if i is 0
#define CPPAN_DETAIL_ENUM_MEMBERS(r, data, i, tup) BOOST_PP_COMMA_IF(i) CPPAN_DETAIL_L1_MEMBER(tup)

#define CPPAN_DETAIL_ANNOTATED_MEMBER(r, const_or_empty, i, tup) BOOST_PP_COMMA_IF(i) ::cppan::annotated_member<const_or_empty() CPPAN_DETAIL_L1_TYPE(tup), CPPAN_DETAIL_ANN_NAME(tup)>

#define CPPAN_DETAIL_DECLARE_MUTABLE_TUPLE_TYPE(X) \
    typedef CPPAN_DETAIL_TUPLE_TYPE<BOOST_PP_SEQ_FOR_EACH_I(CPPAN_DETAIL_ANNOTATED_MEMBER, BOOST_PP_EMPTY, X)> annotated_tuple_type; \
    annotated_tuple_type annotated_tuple() { return annotated_tuple_type(BOOST_PP_SEQ_FOR_EACH_I(CPPAN_DETAIL_ENUM_MEMBERS, _, X)); }

#define CPPAN_DETAIL_DECLARE_CONST_TUPLE_TYPE(X) \
    typedef CPPAN_DETAIL_TUPLE_TYPE<BOOST_PP_SEQ_FOR_EACH_I(CPPAN_DETAIL_ANNOTATED_MEMBER, BOOST_PP_IDENTITY(const), X)> const_annotated_tuple_type; \
    const_annotated_tuple_type annotated_tuple() const { return const_annotated_tuple_type(BOOST_PP_SEQ_FOR_EACH_I(CPPAN_DETAIL_ENUM_MEMBERS, _, X)); }

}									// namespace cppan

#endif								// !defined(CPPAN_INCLUDED)