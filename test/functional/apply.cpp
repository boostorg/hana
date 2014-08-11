/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/functional.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/injection.hpp>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto f = detail::injection([]{});

struct nonpod {
    virtual ~nonpod() { }
    friend bool operator==(nonpod, nonpod) { return true; }
};

int main() {
    BOOST_HANA_CONSTANT_ASSERT(equal(apply(f), f()));
    BOOST_HANA_CONSTEXPR_ASSERT(equal(apply(f, 1), f(1)));
    BOOST_HANA_CONSTEXPR_ASSERT(equal(apply(f, 1, '2'), f(1, '2')));
    BOOST_HANA_CONSTEXPR_ASSERT(equal(apply(f, 1, '2', 3.3), f(1, '2', 3.3)));
    BOOST_HANA_CONSTEXPR_ASSERT(equal(apply(f, 1, '2', 3.3, nullptr), f(1, '2', 3.3, nullptr)));
    BOOST_HANA_RUNTIME_ASSERT(equal(apply(f, 1, '2', 3.3, nullptr, nonpod{}), f(1, '2', 3.3, nullptr, nonpod{})));
}
