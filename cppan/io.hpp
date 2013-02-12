#if !defined(CPPAN_IO_INCLUDED)
#define CPPAN_IO_INCLUDED

#include <cppan/has_annotations.hpp>

#include <boost/fusion/sequence/io/in.hpp>
#include <boost/fusion/sequence/io/out.hpp>

namespace std {

template<typename Char, typename Traits, typename CppanType>
typename boost::enable_if< ::cppan::has_annotations<CppanType>,  std::basic_ostream<Char, Traits>& >::type 
operator<<(std::basic_ostream<Char, Traits>& os, CppanType& value)
{
    return boost::fusion::out(os, value);
}

template<typename Char, typename Traits, typename CppanType>
typename boost::enable_if< ::cppan::has_annotations<CppanType>,  std::basic_istream<Char, Traits>& >::type 
operator<<(std::basic_istream<Char, Traits>& os, CppanType& value)
{
    return boost::fusion::in(os, value);
}

}

namespace cppan {

template<typename MemberType, typename AnnotationsType, typename Char, typename Traits>
std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<Char, Traits>& os, const member<MemberType, AnnotationsType>& m)
{
    os << m.value_;
    return os;
}

}

#endif // !defined(CPPAN_IO_INCLUDED)
