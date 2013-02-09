#include <cppan/cppan.hpp>
#include <cppan/support/boost_hash.hpp>
#include <cppan/support/boost_serialization.hpp>

#include <boost/mpl/assert.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/serialization/string.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <iostream>
#include <sstream>
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
        ((double, no_ann_field_, CPPAN_NIL_SEQ))
      )
};
//
//struct B : A
//{
//    CPPAN_DECLARE_AND_ANNOTATE_WITH_BASE(
//        (A),
//        ((double, d1, CPPAN_NIL_SEQ))
//      )
//};

//struct B : A
//{
//    typedef boost::mpl::vector< A   > base_types; 
//    
//    double d1;   
//    
//    struct annotations_for_d1 
//    { 
//        annotations_for_d1 ()   {}   
//    };   
//    
//    typedef boost::fusion::vector< 
//        ::cppan::annotated_member< double, annotations_for_d1>  
//    > annotated_tuple_type; 
//    
//    annotated_tuple_type annotated_tuple() 
//    { 
//        return annotated_tuple_type(annotated_tuple_type::types::item0(d1)); 
//    } 
//    
//    typedef boost::fusion::vector< 
//        ::cppan::annotated_member<const  double, annotations_for_d1>  
//    > const_annotated_tuple_type; 
//    
//    const_annotated_tuple_type annotated_tuple() const 
//    { 
//        return const_annotated_tuple_type( d1  ); 
//    } 
//    
//    friend class boost::serialization::access; 
//    
//    template<typename Archive> void serialize(Archive& ar, const unsigned int version) 
//    { 
//        ::cppan::serialize(ar, annotated_tuple(), version); 
//    }
//};

CPPAN_DEFINE_MEMBER_DETECTOR(no_hash);

BOOST_MPL_ASSERT((has_no_hash<A::annotations_for_string_field_>));
BOOST_MPL_ASSERT((has_no_hash<cppan::annotated_member< std::string, A::annotations_for_string_field_> >));

BOOST_MPL_ASSERT_NOT((has_no_hash<A::annotations_for_int_field_>));
BOOST_MPL_ASSERT_NOT((has_no_hash<cppan::annotated_member< int, A::annotations_for_int_field_> >));

BOOST_MPL_ASSERT_NOT((has_no_hash<int>));

BOOST_MPL_ASSERT((cppan::has_annotations<A>));
BOOST_MPL_ASSERT_NOT((cppan::has_annotations<int>));

struct dump_members
{
    template<typename MemberType, typename AnnotationsType>
    void operator()(const cppan::annotated_member<MemberType, AnnotationsType>& member) const
    {
        cout << "Member value: " << member.value_ << endl;
        cout << "\tHas no_hash annotation: " << has_no_hash<AnnotationsType>::value << endl;
    }
};

int main(int argc, char* argv[])
{
    A a;

    a.int_field_ = 100;
    a.string_field_ = "string field1";
    a.no_ann_field_ = 22.;

    A::annotations_for_int_field_ a1;
    A::annotations_for_string_field_ a2;
    A::annotations_for_no_ann_field_ a3;

    cout << "Value of int_annotation for A::int_field_" << a1.int_annotation << endl;    

    cout << "has_annotations<A> = " << cppan::has_annotations<A>::value << endl;
    cout << "has_annotations<int> = " << cppan::has_annotations<int>::value << endl;

    boost::fusion::for_each(a.annotated_tuple(), dump_members());
    
    // Calculate hash
    size_t hash_value = boost::hash_value(a);
    cout << "Hash: " << hash_value << endl;

    // Serialize, deserialize object
    string data;
    {
        std::ostringstream oss;
        boost::archive::text_oarchive oa(oss);
        oa << const_cast<const A&>(a);
        data = oss.str();
    }

    A a_restored;
    {
        std::istringstream iss(data);
        boost::archive::text_iarchive ia(iss);
        ia >> a_restored;
    }

	return 0;
}

