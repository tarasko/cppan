#pragma once

#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/type_traits/add_const.hpp>

namespace cppan {

    namespace detail 
    {
        template<typename From, typename To>
        struct propagate_constness
        {
            typedef To type;
        };

        template<typename From, typename To>
        struct propagate_constness<From const, To>
        {
            typedef typename boost::add_const<To>::type type;
        };
    } // namespace detail

    /// Bound together reference to annotated member and annotations type
    template<typename MemberType, typename AnnotationsType>
    struct member
    {
        typedef AnnotationsType annotations_type;
        typedef MemberType member_type;

        MemberType& value_;

        member(MemberType& value) : value_(value) {}
    };

    template<typename T, typename Member, Member T::*MemberPtr, typename Annotations>
    struct member_shortcut
    {
        typedef Annotations annotations_type;
        typedef Member member_type;

        typedef member<typename detail::propagate_constness<T, member_type>::type, annotations_type> deref_type;

        static deref_type deref(T& obj)
        {
            return deref_type(obj.*MemberPtr);
        }
    };

    namespace detail 
    {
        struct make_constant_shortcut
        {
            template<typename Shortcut>
            struct apply;

            template<typename T, typename Member, Member T::*MemberPtr, typename Annotation>
            struct apply< member_shortcut<T, Member, MemberPtr, Annotation> >
            {
                typedef member_shortcut<
                    typename boost::add_const<T>::type
                  , Member
                  , MemberPtr       // Will this implicitly cast?
                  , Annotation 
                  > type;
            };
        };
    } // namespace detail

    /// Depending on constness of type T, return either T::base_types or T::base_types with applied add_const transformation.
    template<typename T>
    struct base_types
    {
        typedef typename T::base_types type;
    };

    template<typename T>
    struct base_types<T const> : boost::mpl::transform< typename T::base_types, boost::add_const<boost::mpl::_> >
    {    
    };

    template<typename T>
    struct member_shortcut_types
    {
        typedef typename T::get_member_shortcuts<T>::type type;
    };

    template<typename T>
    struct member_shortcut_types<T const> : boost::mpl::transform< typename T::get_member_shortcuts<T>::type, detail::make_constant_shortcut >
    {
    };

#if defined(CPPAN_DOXYGEN)
    /// Metafunction that become true if members of T were declared using CPPAN_DECLARE_AND_ANNOTATE_WITH_BASE
    template<typename T> struct has_annotations;
#else
    BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(has_annotations, base_types, false);
#endif 

}
