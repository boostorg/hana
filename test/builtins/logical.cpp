/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/logical/logical.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/number/comparable.hpp>
using namespace boost::hana;


struct logical {
    bool value;
    constexpr operator bool() const { return value; }
};

int main() {
    constexpr auto comparable = detail::number<>;
    BOOST_HANA_CONSTEXPR_LAMBDA auto t = [=](auto) { return comparable(0); };
    BOOST_HANA_CONSTEXPR_LAMBDA auto e = [=](auto) { return comparable(1); };

    // eval_if
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            eval_if(logical{true}, t, e),
            comparable(0)
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            eval_if(logical{false}, t, e),
            comparable(1)
        ));
    }

    // not_
    {
        BOOST_HANA_CONSTEXPR_ASSERT(not_(logical{true}) == false);
        BOOST_HANA_CONSTEXPR_ASSERT(not_(logical{false}) == true);
    }
}
