/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/mpl/vector.hpp>

#include <boost/hana/detail/assert.hpp>

#include <boost/mpl/vector.hpp>
using namespace boost::hana;
namespace mpl = boost::mpl;


int main() {
    BOOST_HANA_CONSTANT_ASSERT(is_empty(mpl::vector<>{}));
    BOOST_HANA_CONSTANT_ASSERT(is_empty(mpl::vector0<>{}));

    BOOST_HANA_CONSTANT_ASSERT(!is_empty(mpl::vector<int>{}));
    BOOST_HANA_CONSTANT_ASSERT(!is_empty(mpl::vector1<int>{}));

    BOOST_HANA_CONSTANT_ASSERT(!is_empty(mpl::vector<int, char>{}));
    BOOST_HANA_CONSTANT_ASSERT(!is_empty(mpl::vector2<int, char>{}));
}
