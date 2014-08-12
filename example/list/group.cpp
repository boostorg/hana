/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTANT_ASSERT(
        group(list(int_<1>, long_<1>, type<int>, char_<'x'>, char_<'x'>))
        ==
        list(
            list(int_<1>, long_<1>),
            list(type<int>),
            list(char_<'x'>, char_<'x'>)
        )
    );
    //! [main]
}
