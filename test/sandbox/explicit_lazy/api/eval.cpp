/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/sandbox/explicit_lazy.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/list.hpp>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto x) { return x + 1; };

int main() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto fmap = curry<2>(boost::hana::fmap);
    BOOST_HANA_STATIC_ASSERT(eval(lazy(f)(1)) == 2);
    BOOST_HANA_STATIC_ASSERT(eval(fmap(fmap(f), lazy(list)(1, 2, 3))) == list(2, 3, 4));
}
