#if !defined(CPPAN_HAS_ANNOTATIONS_INCLUDED)
#define CPPAN_HAS_ANNOTATIONS_INCLUDED

#include <boost/mpl/has_xxx.hpp>

namespace cppan {

#if defined(CPPAN_DOXYGEN)
    /// Metafunction that become true if members of T were declared using CPPAN_DECLARE_AND_ANNOTATE_WITH_BASE
    template<typename T> struct has_annotations;
#else
    BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(has_annotations, base_types, false);
#endif 

}

#endif
