#pragma once

#include <cppan/metafunctions.hpp>

#include <boost/fusion/support/tag_of_fwd.hpp>
#include <boost/fusion/support/category_of.hpp>
#include <boost/fusion/support/is_view.hpp>
#include <boost/fusion/sequence/intrinsic.hpp>
#include <boost/fusion/iterator.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/at.hpp>

namespace cppan { namespace detail {

    template<typename T> struct sequence_tag { typedef T type; };
    template<typename T> struct sequence_iterator_tag { typedef T type; };

    template<typename Struct, int Pos>
    struct sequence_iterator
        : boost::fusion::iterator_base<sequence_iterator<Struct, Pos> >
    {
        typedef Struct struct_type;
        typedef boost::mpl::int_<Pos> index;
        typedef boost::fusion::random_access_traversal_tag category;

        sequence_iterator(Struct& str) : struct_(str) {}

        Struct& struct_;
    };

}} // namespace cppan, detail

namespace boost { namespace fusion {     

    namespace traits 
    {
        template<typename CppanType>
        struct tag_of<CppanType, typename boost::enable_if< ::cppan::has_annotations<CppanType> >::type>
        {
            typedef ::cppan::detail::sequence_tag<CppanType> type;
        };

        template<typename CppanType, int Pos>
        struct tag_of<::cppan::detail::sequence_iterator<CppanType, Pos>, typename boost::enable_if< ::cppan::has_annotations<CppanType> >::type>
        {
            typedef ::cppan::detail::sequence_iterator_tag<CppanType> type;
        };
    } // namespace traits

    namespace extension 
    {
        template<typename CppanType>
        struct is_sequence_impl< ::cppan::detail::sequence_tag<CppanType> >
        {
            template<typename T>
            struct apply : mpl::true_ {};
        };

        template<typename CppanType>
        struct is_view_impl< ::cppan::detail::sequence_tag<CppanType> >
        {
            template<typename T>
            struct apply : mpl::false_ {};
        };

        template<typename CppanType>
        struct category_of_impl< ::cppan::detail::sequence_tag<CppanType> >
        {
            template<typename Sequence>
            struct apply
            {
                struct type : random_access_traversal_tag {};
            };
        };

        template<typename CppanType>
        struct begin_impl< ::cppan::detail::sequence_tag<CppanType> >
        {
            template<typename Sequence>
            struct apply
            {
                typedef ::cppan::detail::sequence_iterator<Sequence, 0> type;

                static type
                call(Sequence& seq)
                {
                    return type(seq);
                }
            };
        };

        template<typename CppanType>
        struct end_impl< ::cppan::detail::sequence_tag<CppanType> >
        {
            template<typename Sequence>
            struct apply
            {
                typedef ::cppan::detail::sequence_iterator<
                    Sequence
                  , boost::mpl::size<
                        typename ::cppan::member_shortcut_types<Sequence>::type
                      >::type::value
                  > type;

                static type
                call(Sequence& seq)
                {
                    return type(seq);
                }
            };
        };

        template<typename CppanType>
        struct size_impl< ::cppan::detail::sequence_tag<CppanType> >
        {
            template<typename Sequence>
            struct apply
                : boost::mpl::size<
                    typename ::cppan::member_shortcut_types<Sequence>::type
                  >::type
            {};
        };

        template<typename CppanType>
        struct next_impl< ::cppan::detail::sequence_iterator_tag<CppanType> >
        {
            template<typename Iterator>
            struct apply
            {
                typedef typename Iterator::struct_type struct_type;
                typedef typename Iterator::index index;
                typedef ::cppan::detail::sequence_iterator<struct_type, index::value + 1> type;

                static type
                call(Iterator const& i)
                {
                    return type(i.struct_);
                }
            };
        };

        template<typename CppanType>
        struct advance_impl< ::cppan::detail::sequence_iterator_tag<CppanType> >
        {
            template<typename Iterator, typename N>
            struct apply
            {
                typedef typename Iterator::struct_type struct_type;
                typedef typename Iterator::index index;
                typedef ::cppan::detail::sequence_iterator<struct_type, index::value + N::value> type;

                static type
                call(Iterator const& it)
                {
                    return type(it.struct_);
                }
            };
        };

        template<typename CppanType>
        struct equal_to_impl< ::cppan::detail::sequence_iterator_tag<CppanType> >
        {
            template<typename It1, typename It2>
            struct apply
                : boost::mpl::equal_to<
                typename It1::index,
                typename It2::index>
            {};
        };

        template<typename CppanType>
        struct value_of_impl< ::cppan::detail::sequence_iterator_tag<CppanType> >
        {
            template<typename Iterator>
            struct apply;

            template<typename Sequence, int Pos>
            struct apply< ::cppan::detail::sequence_iterator<Sequence, Pos> >
            {
                typedef typename boost::mpl::at_c<
                    typename cppan::member_shortcut_types<Sequence>::type
                  , Pos
                  >::type::deref_type type;
            };
        };

        template<typename CppanType>
        struct deref_impl< ::cppan::detail::sequence_iterator_tag<CppanType> >
        {
            template<typename Iterator>
            struct apply;

            template<int Pos>
            struct apply< ::cppan::detail::sequence_iterator<CppanType, Pos> >
            {
                typedef typename boost::mpl::at_c<
                    typename cppan::member_shortcut_types<CppanType>::type
                  , Pos
                  >::type shortcut_type;
        
                typedef typename shortcut_type::deref_type type;

                static type
                call(const ::cppan::detail::sequence_iterator<CppanType, Pos>& it)
                {
                    return shortcut_type::deref(it.struct_);
                }
            };
        };
    } // namespace extension
}}
