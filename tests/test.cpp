#include <cppan/all.hpp>
#include <cppan/support/boost_hash.hpp>
#include <cppan/support/boost_serialization.hpp>

#if defined(CPPAN_TEST_YAS)
#  include <cppan/support/yas.hpp>
#  include <yas/text_iarchive.hpp>
#  include <yas/text_oarchive.hpp>
#endif

#include <boost/mpl/assert.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/adapted/mpl.hpp>
#include <boost/fusion/sequence/comparison.hpp>
#include <boost/serialization/string.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <boost/test/minimal.hpp>

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

void test_io()
{
    // serialize
    const char* src = "(42 Hello 15 20 10)";
    istringstream iss(src);

    D d;
    iss >> d;

    ostringstream oss;
    oss << d;

    BOOST_CHECK(oss.str() == src);
}

struct dump_members
{
    template<typename T>
    void operator()(T& member) const
    {
        cout << typeid(T).name() << ": Member value: " << member.value_ << endl;
        cout << "\tHas no_hash annotation: " << has_no_hash<typename T::annotations_type>::value << endl;
    }
};

void test_basics()
{
    B1 b;

    b.int_field_ = 10;
    b.string_field_ = "test";
    b.no_ann_field_ = 20.20;

    B1::annotations_for_int_field_ ann1;
    B1::annotations_for_string_field_ ann2;
    B1::annotations_for_no_ann_field_ ann3;

    cout << "Value of int_annotation for A::int_field_ " << ann1.int_annotation << endl;    

    cout << "has_annotations<A> = " << cppan::has_annotations<B1>::value << endl;
    cout << "has_annotations<int> = " << cppan::has_annotations<int>::value << endl;

    boost::fusion::for_each(b, dump_members());
    
    const B1& b1 = b;
    boost::fusion::for_each(b1, dump_members());
}

void test_boost_hash()
{
    B1 b;

    b.int_field_ = 10;
    b.string_field_ = "test";
    b.no_ann_field_ = 20.20;

    BOOST_CHECK(boost::hash_value(b) == 0xe095b1b6);

    // string_field_ has no_hash and must not be used
    b.string_field_ = "test1";
    BOOST_CHECK(boost::hash_value(b) == 0xe095b1b6);

    b.int_field_ = 11;
    BOOST_CHECK(boost::hash_value(b) != 0xe095b1b6);
}

void test_boost_serialization()
{
    using namespace boost::fusion; // for comparisons operators

    B1 b;

    b.int_field_ = 10;
    b.string_field_ = "test";
    b.no_ann_field_ = 20.20;

    std::ostringstream oss;
    boost::archive::text_oarchive oa(oss);
    oa << const_cast<const B1&>(b);

    B1 b_restored;
    std::istringstream iss(oss.str());
    boost::archive::text_iarchive ia(iss);
    ia >> b_restored;

    BOOST_CHECK(b != b_restored);
    BOOST_CHECK(b.int_field_ == b_restored.int_field_);
    BOOST_CHECK(b.no_ann_field_ == b_restored.no_ann_field_);
}

void test_yas()
{
#if defined(CPPAN_TEST_YAS)
    using namespace boost::fusion; // for comparisons operators

    B1 b;

    b.int_field_ = 10;
    b.string_field_ = "test";
    b.no_ann_field_ = 20.20;

    // Serialize, deserialize using yas
    ostringstream oss;
    yas::text_file_oarchive oa(oss);
    oa & b;

    B1 b_restored;
    istringstream iss(oss.str());
    yas::text_file_iarchive oi(iss);
    oi & b_restored;

    BOOST_CHECK(b != b_restored);
#endif
}

int test_main(int argc, char* argv[])
{
    test_basics();
    test_io();
    test_boost_hash();
    test_boost_serialization();
    test_yas();

    return 0;
}
