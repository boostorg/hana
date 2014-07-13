/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/functor.hpp>

#include <boost/hana/detail/minimal/functor.hpp>
#include <boost/hana/detail/static_assert.hpp>
using namespace boost::hana;


template <typename mcd>
void test() {
    constexpr auto functor = detail::minimal::functor<mcd>;
    constexpr auto x = 1;
    constexpr auto y = 2.2;
    BOOST_HANA_STATIC_ASSERT(replace([](auto) { return true; }, y, functor(x)) == functor(y));
    BOOST_HANA_STATIC_ASSERT(replace([](auto) { return false; }, y, functor(x)) == functor(x));
}

int main() {
    test<Functor::fmap_mcd>();
    test<Functor::adjust_mcd>();
}
