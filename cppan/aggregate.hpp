#if !defined(CPPAN_AGGREGATE_INCLUDED)
#define CPPAN_AGGREGATE_INCLUDED

#include <cppan/metafunctions.hpp>
#include <cppan/detail/fusion_extension.hpp>

#include <boost/fusion/view/transform_view.hpp>
#include <boost/fusion/view/joint_view.hpp>
#include <boost/fusion/sequence/io.hpp>

namespace cppan {

template<typename T>
struct tuple_extractor
{
    template<typename Signature>
    struct result;

    template<typename Base>
    struct result< tuple_extractor<T>(Base) >
    {
        typedef Base& type;
    };

    template<typename Base>
    typename Base& operator()(Base) const
    {
        return (Base&)obj_;
    }

    tuple_extractor(T& obj) : obj_(obj) {}
    T& obj_;
};

struct join
{
    template<typename Signature>
    struct result;

    template<typename S, typename T>
    struct result< join(S, T) >
    {
        typedef boost::fusion::joint_view<S, T> type;
    };

    template<typename S, typename T>
    boost::fusion::joint_view<S, T> operator()(S& s, T& t) const
    {
        return boost::fusion::joint_view<S, T>(s, t);
    }
};

struct tester
{
    template<typename T>
    void operator()(T& v) const
    {
        std::cout << typeid(T&).name() << std::endl;
        std::cout << v << std::endl;
    }
};

/// Return tuple of members and annotations aggregated over all base types and class itself.
template<typename T>
void aggregate(T& obj, typename boost::enable_if< has_annotations<T> >::type* = 0)
{
    typedef typename base_types<T>::type base_types;

    std::cout << obj;
    // Declare transform view that will return view with annotated_tuples for base types
    boost::fusion::transform_view< base_types, tuple_extractor<T> > base_tuples_view((base_types()), tuple_extractor<T>(obj));
    //boost::fusion::for_each(base_tuples_view, tester());

    // std::cout << base_tuples_view << std::endl;

    // Accumulate annotated tuples for base types
    //auto res = boost::fusion::accumulate(base_tuples_view, obj, join()); 
}

}

#endif // !defined(CPPAN_AGGREGATE_INCLUDED)