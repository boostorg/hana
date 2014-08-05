/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/mpl/integral_c.hpp>

#include <boost/hana/constant/laws.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/list/instance.hpp>

#include <boost/mpl/integral_c.hpp>
using namespace boost::hana;


template <typename T>
void test() {
    namespace mpl = boost::mpl;
    BOOST_HANA_CONSTANT_ASSERT(Constant::laws::check(
        list(
            mpl::integral_c<T, 0>{},
            mpl::integral_c<T, 1>{},
            mpl::integral_c<T, 2>{},
            mpl::integral_c<T, 3>{}
        )
    ));
}

int main() {
    test<int>();
    test<long>();
}
