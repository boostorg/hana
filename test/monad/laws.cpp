/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/monad/bind_mcd.hpp>
#include <boost/hana/monad/flatten_mcd.hpp>
#include <boost/hana/monad/laws.hpp>

#include <boost/hana/detail/minimal/monad.hpp>
#include <boost/hana/detail/static_assert.hpp>
using namespace boost::hana;


template <typename mcd>
void test() {
    constexpr auto monad = detail::minimal::monad<mcd>;
    BOOST_HANA_STATIC_ASSERT(Monad::laws::check(
        monad(1),
        2,
        [=](auto x) { return monad(x + 1); },
        [=](auto x) { return monad(x * 2); }
    ));
}

int main() {
    test<Monad::flatten_mcd>();
    test<Monad::bind_mcd>();
}
