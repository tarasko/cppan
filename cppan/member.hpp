#if !defined(CPPAN_MEMBER_INCLUDED)
#define CPPAN_MEMBER_INCLUDED

namespace cppan {

/// Bound together reference to annotated member and annotations type
template<typename MemberType, typename AnnotationsType>
struct member
{
    typedef AnnotationsType annotations_type;
    typedef MemberType member_type;

    MemberType& value_;

    member(MemberType& value) : value_(value) {}
};

// TODO: Ad-hoc to compare sequences with boost fusion comparisons. (require using boost::fusion)
// Make own comparison operators for all type satisfying has_annotations.

template<typename MemberType1, typename AnnotationsType1, typename MemberType2, typename AnnotationsType2>
bool operator==(const member<MemberType1, AnnotationsType1>& m1, const member<MemberType2, AnnotationsType2>& m2)
{
    return m1.value_ == m2.value_;
}

template<typename MemberType1, typename AnnotationsType1, typename MemberType2, typename AnnotationsType2>
bool operator!=(const member<MemberType1, AnnotationsType1>& m1, const member<MemberType2, AnnotationsType2>& m2)
{
    return m1.value_ != m2.value_;
}

template<typename MemberType1, typename AnnotationsType1, typename MemberType2, typename AnnotationsType2>
bool operator<(const member<MemberType1, AnnotationsType1>& m1, const member<MemberType2, AnnotationsType2>& m2)
{
    return m1.value_ < m2.value_;
}

template<typename MemberType1, typename AnnotationsType1, typename MemberType2, typename AnnotationsType2>
bool operator<=(const member<MemberType1, AnnotationsType1>& m1, const member<MemberType2, AnnotationsType2>& m2)
{
    return m1.value_ <= m2.value_;
}

template<typename MemberType1, typename AnnotationsType1, typename MemberType2, typename AnnotationsType2>
bool operator>(const member<MemberType1, AnnotationsType1>& m1, const member<MemberType2, AnnotationsType2>& m2)
{
    return m1.value_ > m2.value_;
}

template<typename MemberType1, typename AnnotationsType1, typename MemberType2, typename AnnotationsType2>
bool operator>=(const member<MemberType1, AnnotationsType1>& m1, const member<MemberType2, AnnotationsType2>& m2)
{
    return m1.value_ >= m2.value_;
}

}

#endif