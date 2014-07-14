/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/lazy.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/functional.hpp>

#include "../comparable.hpp"
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto x) {
    return x + 1;
};

int main() {
    BOOST_HANA_STATIC_ASSERT(fmap(f, lazy(1)) == lazy(f(1)));
    BOOST_HANA_STATIC_ASSERT(fmap(f, lazy(2.2)) == lazy(f(2.2)));

    BOOST_HANA_STATIC_ASSERT(fmap(id, lazy(1)) == lazy(1));
    BOOST_HANA_STATIC_ASSERT(fmap(id, lazy('2')) == lazy('2'));
    BOOST_HANA_STATIC_ASSERT(fmap(id, lazy(3.3)) == lazy(3.3));
}
