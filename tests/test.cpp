#include <cppan/cppan.hpp>
#include <cppan/support/boost_hash.hpp>

#include <boost/mpl/assert.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>

#include <iostream>
#include <typeinfo>
#include <string>

using namespace std;

struct A
{
    CPPAN_DECLARE_AND_ANNOTATE(
        ((int, int_field_,
            ((int_annotation, 24))
            ((string_annotation, "Hello world"))
        ))
        ((std::string, string_field_,
            ((no_serialization, 0))
            ((no_hash, 0))
        ))
        ((double, no_ann_field_, BOOST_PP_SEQ_NIL))
      )
};

CPPAN_DEFINE_MEMBER_DETECTOR(no_hash);

BOOST_MPL_ASSERT((has_no_hash<A::annotations_for_string_field_>));
BOOST_MPL_ASSERT_NOT((has_no_hash<A::annotations_for_int_field_>));
BOOST_MPL_ASSERT_NOT((has_no_hash<int>));

BOOST_MPL_ASSERT((cppan::has_annotations<A>));
BOOST_MPL_ASSERT_NOT((cppan::has_annotations<int>));

// case 1: we know field and want explicitly its annotations
// typedef A::annotation_for_int_field annotations_type;
// annotations_type a;
// int value_of_int_annotation = a.int_annotation;
//
// case 2: we want to traverse over fields with annotations.
// A a; // Construct a
// boost::fusion::for_each(a.as_annotated_tuple(), visitor());
// 
// struct visitor
// {
//     template<typename MemberType, typename Annotations>
//     void operator(annotated_member<MemberType, Annotations> v)
//     {
//          auto = v.value_; // Access member value
//          Annotations a;
//          auto = a.int_annotation; // Access annotation value
//     }
// };


struct dump_members
{
    template<typename MemberType, typename AnnotationsType>
    void operator()(const cppan::annotated_member<MemberType, AnnotationsType>& member) const
    {
        cout << "Member value: " << member.value_ << endl;

        // We can make compile time check here whether member has specific annotation
        cout << "\tHas no_hash annotation: " << has_no_hash<AnnotationsType>::value << endl;
    }
};

int main(int argc, char* argv[])
{
    A a;

    // Use case 1:
    // явно обращаемс€ к члену и к типу содержащему его аннотации
    a.int_field_ = 100;
    a.string_field_ = "string field1";
    a.no_ann_field_ = 22.;

    A::annotations_for_int_field_ a1;
    A::annotations_for_string_field_ a2;
    A::annotations_for_no_ann_field_ a3;

    cout << "Value of int_annotation for A::int_field_" << a1.int_annotation << endl;    

    // Use case 2:
    // ћетафункци€ провер€юща€ €вл€ютс€ ли пол€ в типе аннотированными
    cout << "has_annotations<A> = " << cppan::has_annotations<A>::value << endl;
    cout << "has_annotations<int> = " << cppan::has_annotations<int>::value << endl;

    // Use case 3:
    // ѕолучение членов структуры и св€занных с ними аннотаций в виде кортежа, 
    boost::fusion::for_each(a.annotated_tuple(), dump_members());
    
    // Calculate hash
    size_t hash_value = boost::hash_value(a);
    cout << "Hash: " << hash_value << endl;

	return 0;
}

