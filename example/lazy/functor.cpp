/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/lazy.hpp>
using namespace boost::hana;


int main() {
    //! [fmap]
    BOOST_HANA_CONSTEXPR_LAMBDA auto double_ = [](auto x) {
        return x * 2;
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto one_over = [](auto x) {
        return 1 / x;
    };

    BOOST_HANA_CONSTEXPR_ASSERT(eval(fmap(lazy(one_over)(4), double_)) == 1 / 2);

    fmap(lazy(one_over)(0), double_); // never evaluated
    //! [fmap]
}
