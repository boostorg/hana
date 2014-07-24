/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/monad/bind_mcd.hpp>
#include <boost/hana/monad/flatten_mcd.hpp>

#include <boost/hana/detail/minimal/comparable.hpp>
#include <boost/hana/detail/minimal/monad.hpp>
#include <boost/hana/detail/static_assert.hpp>
using namespace boost::hana;


template <typename mcd>
void test() {
    constexpr auto monad = detail::minimal::monad<mcd>;
    constexpr auto comparable = detail::minimal::comparable<>;
    struct invalid { };

    BOOST_HANA_STATIC_ASSERT(
        then(monad(invalid{}), monad(comparable(2))) == monad(comparable(2))
    );
}

int main() {
    test<Monad::flatten_mcd>();
    test<Monad::bind_mcd>();
}
