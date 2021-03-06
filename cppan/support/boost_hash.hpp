#if !defined(CPPAN_SUPPORT_BOOST_HASH_INCLUDED)
#define CPPAN_SUPPORT_BOOST_HASH_INCLUDED

#include <cppan/all.hpp>

#include <boost/functional/hash.hpp>
#include <boost/fusion/view/filter_view.hpp>
#include <boost/fusion/algorithm/iteration/accumulate.hpp>

#if !defined(CPPAN_DOXYGEN)

namespace cppan { 

CPPAN_DEFINE_MEMBER_DETECTOR(no_hash);

namespace detail {

struct hash_value_visitor
{
    typedef size_t result_type;

    template<typename T>
    size_t operator()(size_t value, T& member) const
    {
        boost::hash_combine(value, member.value_);
        return value;
    }
};

}}

#endif // !defined(CPPAN_DOXYGEN)

namespace boost {

/// Override hash_value in boost namespace.
template<typename T>
std::size_t hash_value(T& obj, typename boost::enable_if< ::cppan::has_annotations<T> >::type* = 0)
{
    using namespace boost::fusion;
    using namespace cppan::detail;
    using boost::mpl::_;
    using boost::mpl::not_;

    return accumulate(
        filter_view<T, not_< has_no_hash<_> > >(obj)
      , size_t(0)
      , hash_value_visitor()
      );
}

}

#endif
