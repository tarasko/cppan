#include <cppan/all.hpp>
#include <cppan/support/boost_hash.hpp>
#include <cppan/support/boost_serialization.hpp>

#include <boost/mpl/assert.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/adapted/mpl.hpp>
#include <boost/serialization/string.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <iostream>
#include <sstream>
#include <typeinfo>
#include <string>

using namespace std;

struct B1
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

// Check that
struct B2
{
    CPPAN_DECLARE_AND_ANNOTATE(
        ((int, b2_mem, CPPAN_NIL_SEQ))
      )
};

struct D : B1, B2
{
    CPPAN_DECLARE_AND_ANNOTATE_WITH_BASE(
        (B1)(B2),
        ((double, d_mem, CPPAN_NIL_SEQ))
      )
};

CPPAN_DEFINE_MEMBER_DETECTOR(no_hash);

BOOST_MPL_ASSERT((has_no_hash<B1::annotations_for_string_field_>));
BOOST_MPL_ASSERT((has_no_hash<cppan::member< std::string, B1::annotations_for_string_field_> >));

BOOST_MPL_ASSERT_NOT((has_no_hash<B1::annotations_for_int_field_>));
BOOST_MPL_ASSERT_NOT((has_no_hash<cppan::member< int, B1::annotations_for_int_field_> >));

BOOST_MPL_ASSERT_NOT((has_no_hash<int>));

BOOST_MPL_ASSERT((cppan::has_annotations<B1>));
BOOST_MPL_ASSERT_NOT((cppan::has_annotations<int>));

struct dump_members
{
    template<typename T>
    void operator()(T& member) const
    {
        cout << typeid(T).name() << ": Member value: " << member.value_ << endl;
        cout << "\tHas no_hash annotation: " << has_no_hash<typename T::annotations_type>::value << endl;
    }
};

int main(int argc, char* argv[])
{
    B1 b;

    b.int_field_ = 100;
    b.string_field_ = "string field1";
    b.no_ann_field_ = 22.;

    B1::annotations_for_int_field_ ann1;
    B1::annotations_for_string_field_ ann2;
    B1::annotations_for_no_ann_field_ ann3;

    cout << "Value of int_annotation for A::int_field_ " << ann1.int_annotation << endl;    

    cout << "has_annotations<A> = " << cppan::has_annotations<B1>::value << endl;
    cout << "has_annotations<int> = " << cppan::has_annotations<int>::value << endl;

    boost::fusion::for_each(b, dump_members());
    
    const B1& b1 = b;
    boost::fusion::for_each(b, dump_members());

    // Calculate hash
    size_t hash_value = boost::hash_value(b);
    cout << "Hash: " << hash_value << endl;

    // Serialize, deserialize object
    string data;
    {
        std::ostringstream oss;
        boost::archive::text_oarchive oa(oss);
        oa << const_cast<const B1&>(b);
        data = oss.str();
    }

    B1 b_restored;
    {
        std::istringstream iss(data);
        boost::archive::text_iarchive ia(iss);
        ia >> b_restored;
    }

    D d;
    d.int_field_ = 42;
    d.no_ann_field_ = 15.;
    d.string_field_ = "fuck";
    d.b2_mem = 20;
    d.d_mem = 10.;

    cout << "D = " << d << endl;
    cout << "D const = " << (const D&)d << endl;


	return 0;
}

