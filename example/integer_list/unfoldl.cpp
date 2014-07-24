/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integer_list.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/pair/instance.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto x) {
        return if_(x == int_<0>, nothing, just(pair(x - int_<1>, x)));
    };

    BOOST_HANA_STATIC_ASSERT(
        unfoldl<IntegerList>(f, int_<10>) == integer_list<int, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10>
    );
    //! [main]
}
