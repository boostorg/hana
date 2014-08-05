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
    //! [main]
    BOOST_HANA_CONSTEXPR_LAMBDA auto double_ = [](auto x) {
        return x * 2;
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto one_over = [](auto x) {
        return 1 / x;
    };

    BOOST_HANA_CONSTEXPR_ASSERT(eval(fmap(double_, lazy(one_over)(4))) == 1 / 2);

    fmap(double_, lazy(one_over)(0)); // never evaluated
    //! [main]
}
