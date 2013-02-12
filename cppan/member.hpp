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

}

#endif