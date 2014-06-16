/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/functional.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
using namespace boost::hana;


struct g_was_here { };
BOOST_HANA_CONSTEXPR_LAMBDA auto g = [](auto x) {
    return g_was_here{};
};

BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](g_was_here, auto ...x) {
    return sizeof...(x);
};

struct nonpod { virtual ~nonpod() { } };

int main() {
    BOOST_HANA_STATIC_ASSERT(compose(f, g)(1) == f(g(1)));
    BOOST_HANA_STATIC_ASSERT(compose(f, g)(1, '2') == f(g(1), '2'));
    BOOST_HANA_STATIC_ASSERT(compose(f, g)(1, '2', 3.3) == f(g(1), '2', 3.3));
    BOOST_HANA_STATIC_ASSERT(compose(f, g)(1, '2', 3.3, 4.4f) == f(g(1), '2', 3.3, 4.4f));
    BOOST_HANA_STATIC_ASSERT(compose(f, g)(1, '2', 3.3, 4.4f, nonpod{}) == f(g(1), '2', 3.3, 4.4f, nonpod{}));
}
