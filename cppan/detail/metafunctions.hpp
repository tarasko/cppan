#pragma once

#include <cppan/member.hpp>

#include <boost/mpl/transform.hpp>
#include <boost/type_traits/add_const.hpp>

namespace cppan { namespace detail {

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

/// Depending on constness of type T, return either T::base_types or T::base_types with applied add_const transformation.
template<typename T>
struct base_types
{
    typedef typename T::base_types type;
};

template<typename T>
struct base_types<const T> : boost::mpl::transform< typename T::base_types, boost::add_const<boost::mpl::_> >
{    
};

template<typename T>
struct member_shortcut_types
{
    typedef typename T::template apply<T>::type type;
};

template<typename T>
struct member_shortcut_types<T const> : boost::mpl::transform< typename T::template apply<T>::type, detail::make_constant_shortcut >
{
};

}}
