#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE types::string
#include <boost/test/unit_test.hpp>

#include "../src/types/string.h"

BOOST_AUTO_TEST_CASE(construct_from_value)
{
	phpext::types::value  v1("test string");
	phpext::types::string s1(v1);
}
