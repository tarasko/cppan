#if !defined(CPPAN_DETAIL_DECLARE_AND_ANNOTATE_INCLUDED)
#define CPPAN_DETAIL_DECLARE_AND_ANNOTATE_INCLUDED

#include <cppan/detail/fusion_extension.hpp>

#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/seq/transform.hpp>
#include <boost/preprocessor/comma_if.hpp>
#include <boost/preprocessor/identity.hpp>
#include <boost/preprocessor/empty.hpp>

#include <boost/mpl/vector.hpp>

#if defined(CPPAN_USE_TUPLE_FOR_ANNOTATIONS)
#  include <boost/fusion/adapted/struct/define_struct_inline.hpp>
#endif

#if !defined(CPPAN_ENABLE_BOOST_SERIALIZATION)
#  define CPPAN_ENABLE_BOOST_SERIALIZATION
#endif

/// Declare and annotate members with specified types. Accept Boost.Preprocessor sequence of member field declaration.
/// Each element in sequence must be Boost.Preprocessor tuple with 3 elements (member_type, member_name, sequence_of_annotations).
/// \code
/// sequence_of_annotations := sequence_of_annotations | CPPAN_NIL_SEQ
/// \endcode
/// \c CPPAN_NIL_SEQ can be used if member doesn`t have annotations.
/// \c sequence_of_annotations - is Boost.Preprocessor sequence of tuples with 2 elements (annotation_name, annotation_runtime_value).
/// @code
///struct D : B1, B2
///{
///    CPPAN_DECLARE_AND_ANNOTATE_WITH_BASE(
///        D,
///        (B1)(B2),
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
#define CPPAN_DECLARE_AND_ANNOTATE_WITH_BASE(Self, BasesSeq, MembersTuple) \
    typedef boost::mpl::vector<BOOST_PP_SEQ_FOR_EACH_I(CPPAN_DETAIL_REPEAT_SEQ, _, BasesSeq)> base_types; \
    BOOST_PP_SEQ_FOR_EACH_R(1, CPPAN_DETAIL_DECLARE_MEMBER, _, MembersTuple) \
    BOOST_PP_SEQ_FOR_EACH_R(1, CPPAN_DETAIL_DECLARE_ANNOTATION_STRUCT, _, MembersTuple) \
    CPPAN_DETAIL_DECLARE_MEMBER_SHORTCUTS(Self, MembersTuple) \
    CPPAN_ENABLE_BOOST_SERIALIZATION

/// Declare and annotate members with specified types. Accept Boost.Preprocessor sequence of member field declaration.
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
///        A,
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
#define CPPAN_DECLARE_AND_ANNOTATE(Self, MembersSeq) CPPAN_DECLARE_AND_ANNOTATE_WITH_BASE(Self, BOOST_PP_SEQ_NIL, MembersSeq)

/// Define empty annotations sequence
#define CPPAN_NIL_SEQ BOOST_PP_SEQ_NIL

#if !defined(CPPAN_DOXYGEN)

// Define type that will be used for tuple. Better to use C++11 template aliasing instead of macro
#  define CPPAN_DETAIL_TUPLE_TYPE boost::fusion::vector

#  define CPPAN_DETAIL_REPEAT_SEQ(r, data, i, elem) BOOST_PP_COMMA_IF(i) elem

// Accessors for top level tuple
#  define CPPAN_DETAIL_L1_TYPE(tup) BOOST_PP_TUPLE_ELEM(3, 0, tup)
#  define CPPAN_DETAIL_L1_MEMBER(tup) BOOST_PP_TUPLE_ELEM(3, 1, tup)
#  define CPPAN_DETAIL_L1_ANN_SEQ(tup) BOOST_PP_TUPLE_ELEM(3, 2, tup)

// Accessort for annotations level tuple
#  define CPPAN_DETAIL_L2_NAME(tup) BOOST_PP_TUPLE_ELEM(2, 0, tup)
#  define CPPAN_DETAIL_L2_VALUE(tup) BOOST_PP_TUPLE_ELEM(2, 1, tup)

// Produce name for annotation structure
#  define CPPAN_DETAIL_ANN_NAME(tup) BOOST_PP_CAT(annotations_for_, CPPAN_DETAIL_L1_MEMBER(tup))

// Declare member using top level tuple
#  define CPPAN_DETAIL_DECLARE_MEMBER(r, data, tup) CPPAN_DETAIL_L1_TYPE(tup) CPPAN_DETAIL_L1_MEMBER(tup);

#  if !defined(CPPAN_USE_TUPLE_FOR_ANNOTATIONS)

// Produce colon on false and comma on true
#    define CPPAN_DETAIL_COLON_OR_COMMA(cond) BOOST_PP_IF(cond, BOOST_PP_COMMA, BOOST_PP_IDENTITY(:))()

// Produce annotation initializer list for annotation structure constructor
#    define CPPAN_DETAIL_INITIALIZER_LIST(r, data, i, tup) CPPAN_DETAIL_COLON_OR_COMMA(i) CPPAN_DETAIL_L2_NAME(tup)(CPPAN_DETAIL_L2_VALUE(tup))

// Produce declaration of annotation member
#    define CPPAN_DETAIL_ANNOTATIONS_LIST(r, data, i, tup) decltype(CPPAN_DETAIL_L2_VALUE(tup)) CPPAN_DETAIL_L2_NAME(tup);

// Declare annotation struct using tuple
#    define CPPAN_DETAIL_DECLARE_ANNOTATION_STRUCT(r, data, tup) \
    struct CPPAN_DETAIL_ANN_NAME(tup) { \
        CPPAN_DETAIL_ANN_NAME(tup) () \
            BOOST_PP_SEQ_FOR_EACH_I_R(r, CPPAN_DETAIL_INITIALIZER_LIST, _, CPPAN_DETAIL_L1_ANN_SEQ(tup)) {} \
        BOOST_PP_SEQ_FOR_EACH_I_R(r, CPPAN_DETAIL_ANNOTATIONS_LIST, _, CPPAN_DETAIL_L1_ANN_SEQ(tup)) \
    };

#  else

// TODO: Initialize annotations with provided values

#    define CPPAN_DETAIL_TRANSFORM_TO_FUSION_SEQ(d, data, tup) decltype(CPPAN_DETAIL_L2_VALUE(tup)), CPPAN_DETAIL_L2_NAME(tup)

#    define CPPAN_DETAIL_DECLARE_ANNOTATION_STRUCT(r, data, tup) \
    BOOST_FUSION_DEFINE_STRUCT_INLINE(CPPAN_DETAIL_ANN_NAME(tup), BOOST_PP_SEQ_TRANSFORM(CPPAN_DETAIL_TRANSFORM_TO_FUSION_SEQ, _, CPPAN_DETAIL_L1_ANN_SEQ(tup)))

#  endif

// Produce ", member" or "member" if i is 0
#  define CPPAN_DETAIL_SHORTCUT(r, self, i, tup) BOOST_PP_COMMA_IF(i) ::cppan::member_shortcut<self, CPPAN_DETAIL_L1_TYPE(tup), &self::CPPAN_DETAIL_L1_MEMBER(tup), CPPAN_DETAIL_ANN_NAME(tup)>

#  define CPPAN_DETAIL_DECLARE_MEMBER_SHORTCUTS(Self, X) \
    template<typename T> \
    struct get_member_shortcuts \
    { \
        typedef boost::mpl::vector<BOOST_PP_SEQ_FOR_EACH_I(CPPAN_DETAIL_SHORTCUT, T, X)> type; \
    };

#endif // !defined(CPPAN_DOXYGEN)

#endif // !defined(CPPAN_DETAIL_DECLARE_AND_ANNOTATE_INCLUDED)
