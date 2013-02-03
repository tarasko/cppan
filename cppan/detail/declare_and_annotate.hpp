#if !defined(CPPAN_DETAIL_DECLARE_AND_ANNOTATE_INCLUDED)
#define CPPAN_DETAIL_DECLARE_AND_ANNOTATE_INCLUDED

#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/seq/transform.hpp>
#include <boost/preprocessor/comma_if.hpp>
#include <boost/preprocessor/identity.hpp>
#include <boost/preprocessor/empty.hpp>

#if defined(CPPAN_USE_TUPLE_FOR_ANNOTATIONS)
#  include <boost/fusion/adapted/struct/define_struct_inline.hpp>
#endif

#if !defined(CPPAN_ENABLE_BOOST_SERIALIZATION)
#  define CPPAN_ENABLE_BOOST_SERIALIZATION
#endif

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

#endif
