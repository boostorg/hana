/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/maybe.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/identity/applicative.hpp>
#include <boost/hana/detail/injection.hpp>
#include <boost/hana/detail/number/comparable.hpp>
#include <boost/hana/functional.hpp>
using namespace boost::hana;


template <int i>
constexpr auto x = detail::number<>(i);

int main() {
    constexpr auto applicative = detail::identity<>;
    using A = detail::Identity<>;

    BOOST_HANA_CONSTEXPR_LAMBDA auto f = detail::injection([]{});


    // traverse
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            traverse<A>(compose(applicative, f), just(x<0>)),
            applicative(just(f(x<0>)))
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            traverse<A>(compose(applicative, f), nothing),
            applicative(nothing)
        ));
    }
}
