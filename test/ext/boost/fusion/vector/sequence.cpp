/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#define BOOST_HANA_TEST_SEQUENCE_KNOWN_PARTS 3
#if BOOST_HANA_TEST_PART == 1
#   define BOOST_HANA_TEST_SEQUENCE_PART 1
#elif BOOST_HANA_TEST_PART == 2
#   define BOOST_HANA_TEST_SEQUENCE_PART 2
#elif BOOST_HANA_TEST_PART == 3
#   define BOOST_HANA_TEST_SEQUENCE_PART 3
#endif

#define FUSION_MAX_VECTOR_SIZE 50
#include <boost/hana/ext/boost/fusion/vector.hpp>

#include <laws/sequence.hpp>

#include <boost/fusion/container/vector.hpp>
using namespace boost::hana;


int main() {
    test::TestSequence<ext::boost::fusion::Vector>{};
}
