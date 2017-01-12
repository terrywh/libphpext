#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE types::string
#include <boost/test/unit_test.hpp>

#include "../src/types/array.h"

BOOST_AUTO_TEST_CASE(construct_from_value)
{
	phpext::types::value v1(nullptr);
	phpext::types::array a1(v1); // this actually triggers and assert
}
