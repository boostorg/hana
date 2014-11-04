/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/fusion/sequence.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/foreign.hpp>
#include <boost/hana/integral.hpp>

#include "helper.hpp"
#include <test/injection.hpp>

#include <vector>
using namespace boost::hana;


int main() {
    with_nonassociative_forward_sequences([=](auto container) {
        // count
        {
            BOOST_HANA_CONSTEXPR_LAMBDA auto is_even = [](auto x) {
                return x % 2 == 0;
            };

            BOOST_HANA_RUNTIME_ASSERT(count(container(), is_even) == 0);
            BOOST_HANA_RUNTIME_ASSERT(count(container(1), is_even) == 0);
            BOOST_HANA_RUNTIME_ASSERT(count(container(2), is_even) == 1);
            BOOST_HANA_RUNTIME_ASSERT(count(container(1, 2), is_even) == 1);
            BOOST_HANA_RUNTIME_ASSERT(count(container(1, 2, 3), is_even) == 1);
            BOOST_HANA_RUNTIME_ASSERT(count(container(1, 2, 3, 4), is_even) == 2);
        }

        // foldl
        {
            BOOST_HANA_CONSTEXPR_LAMBDA auto f = test::injection([]{});
            constexpr auto s = 0;
            BOOST_HANA_RUNTIME_ASSERT(equal(foldl(container(), s, f), s));
            BOOST_HANA_RUNTIME_ASSERT(equal(foldl(container(1), s, f), f(s, 1)));
            BOOST_HANA_RUNTIME_ASSERT(equal(foldl(container(1, '2'), s, f), f(f(s, 1), '2')));
            BOOST_HANA_RUNTIME_ASSERT(equal(foldl(container(1, '2', 3.3), s, f), f(f(f(s, 1), '2'), 3.3)));
            BOOST_HANA_RUNTIME_ASSERT(equal(foldl(container(1, '2', 3.3, 4.4f), s, f), f(f(f(f(s, 1), '2'), 3.3), 4.4f)));
        }

        // foldr
        {
            BOOST_HANA_CONSTEXPR_LAMBDA auto f = test::injection([]{});
            constexpr auto s = 0;
            BOOST_HANA_RUNTIME_ASSERT(equal(foldr(container(), s, f), s));
            BOOST_HANA_RUNTIME_ASSERT(equal(foldr(container(1), s, f), f(1, s)));
            BOOST_HANA_RUNTIME_ASSERT(equal(foldr(container(1, '2'), s, f), f(1, f('2', s))));
            BOOST_HANA_RUNTIME_ASSERT(equal(foldr(container(1, '2', 3.3), s, f), f(1, f('2', f(3.3, s)))));
            BOOST_HANA_RUNTIME_ASSERT(equal(foldr(container(1, '2', 3.3, 4.4f), s, f), f(1, f('2', f(3.3, f(4.4f, s))))));
        }

        // for_each
        {
            auto check_with = [=](auto ...xs) {
                std::vector<int> seen{};
                for_each(container(xs...), [&](int x) {
                    seen.push_back(x);
                });
                BOOST_HANA_RUNTIME_ASSERT(seen == std::vector<int>{xs...});
            };
            check_with();
            check_with(0);
            check_with(0, 1);
            check_with(0, 1, 2);
            check_with(0, 1, 2, 3);
            check_with(0, 1, 2, 3, 4);
        }

        // length
        {
            BOOST_HANA_CONSTANT_ASSERT(length(container()) == size_t<0>);
            BOOST_HANA_CONSTANT_ASSERT(length(container(1)) == size_t<1>);
            BOOST_HANA_CONSTANT_ASSERT(length(container(1, '2')) == size_t<2>);
            BOOST_HANA_CONSTANT_ASSERT(length(container(1, '2', 3.3)) == size_t<3>);
        }
    });
}
