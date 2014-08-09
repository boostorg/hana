/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/injection.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/number/comparable.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/logical/logical.hpp>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto implies = infix([](auto p, auto q) {
    return or_(not_(p), q);
});

int main() {
    constexpr auto c = detail::number<>;
    BOOST_HANA_CONSTEXPR_LAMBDA auto f = detail::injection([]{});
    BOOST_HANA_CONSTEXPR_LAMBDA auto g = detail::injection([]{});

    // injectivity
    {
        BOOST_HANA_CONSTEXPR_LAMBDA auto is_injection = [=](auto x, auto y) {
            return equal(f(x), f(y)) ^implies^ equal(x, y);
        };

        BOOST_HANA_CONSTEXPR_ASSERT(all_of(
            ap(list(is_injection),
                list(c(0), c(1), c(2), c(3)),
                list(c(0), c(1), c(2), c(3)))
        ));
    }

    // various number of arguments
    {
        BOOST_HANA_CONSTANT_ASSERT(not_equal(
            f(), g()
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            f(), f()
        ));

        BOOST_HANA_CONSTANT_ASSERT(not_equal(
            f(c(0)),
            g(c(0))
        ));

        BOOST_HANA_CONSTANT_ASSERT(not_equal(
            f(c(0), c(1)),
            f(c(0))
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            f(c(0), c(1)),
            f(c(0), c(1))
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(not_equal(
            f(c(0), c(1)),
            f(c(1), c(0))
        ));
    }
}
