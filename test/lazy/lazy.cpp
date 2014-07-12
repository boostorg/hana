/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/lazy.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto x) {
    return x + 1;
};

BOOST_HANA_CONSTEXPR_LAMBDA auto invalid = [](auto x) {
    return x.this_function_must_not_be_instantiated;
};

int main() {
    BOOST_HANA_STATIC_ASSERT(eval(lazy(f)(1)) == 1 + 1);
    BOOST_HANA_STATIC_ASSERT(eval(lazy(f)(2.2)) == 2.2 + 1);

    lazy(invalid)(1); // The function must not be applied.
}
