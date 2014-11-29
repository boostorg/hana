/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/detail/while.hpp>
#include <boost/hana/foreign.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/functional/curry.hpp>

#include <test/injection.hpp>
#include <vector>
using namespace boost::hana;


int main() {
    auto equal_to = curry<2>(equal);

    // compile-time condition
    {
        struct { } invalid{};
        auto f = test::injection([]{});
        using test::x;

        BOOST_HANA_CONSTANT_CHECK(equal(
            detail::until(equal_to(x<0>), x<0>, invalid),
            x<0>
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            detail::until(equal_to(f(x<0>)), x<0>, f),
            f(x<0>)
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            detail::until(equal_to(f(f(x<0>))), x<0>, f),
            f(f(x<0>))
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            detail::until(equal_to(f(f(f(x<0>)))), x<0>, f),
            f(f(f(x<0>)))
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            detail::until(equal_to(f(f(f(f(x<0>))))), x<0>, f),
            f(f(f(f(x<0>))))
        ));

        // Make sure it can be called with an lvalue state:
        auto state = x<0>;
        BOOST_HANA_CONSTANT_CHECK(equal(
            detail::until(equal_to(f(f(f(f(x<0>))))), state, f),
            f(f(f(f(x<0>))))
        ));
    }

    // runtime condition
    {
        auto has_length = [](auto n) {
            return [n](auto v) { return v.size() == n; };
        };
        auto f = [](auto v) {
            v.push_back(v.size());
            return v;
        };

        BOOST_HANA_RUNTIME_CHECK(equal(
            detail::until(has_length(0u), std::vector<int>{}, f),
            std::vector<int>{}
        ));

        BOOST_HANA_RUNTIME_CHECK(equal(
            detail::until(has_length(1u), std::vector<int>{}, f),
            std::vector<int>{0}
        ));

        BOOST_HANA_RUNTIME_CHECK(equal(
            detail::until(has_length(2u), std::vector<int>{}, f),
            std::vector<int>{0, 1}
        ));

        BOOST_HANA_RUNTIME_CHECK(equal(
            detail::until(has_length(3u), std::vector<int>{}, f),
            std::vector<int>{0, 1, 2}
        ));

        BOOST_HANA_RUNTIME_CHECK(equal(
            detail::until(has_length(4u), std::vector<int>{}, f),
            std::vector<int>{0, 1, 2, 3}
        ));

        // Make sure it can be called with an lvalue state:
        std::vector<int> v{};
        BOOST_HANA_RUNTIME_CHECK(equal(
            detail::until(has_length(4u), v, f),
            std::vector<int>{0, 1, 2, 3}
        ));
    }
}
